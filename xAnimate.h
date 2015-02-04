
#ifndef __X_ANIMATE__
#define __X_ANIMATE__

#include <math.h>

#include "xTables.h"
#include "xCallbacks.h"

/* xAnimate API :
 *
 *	void	anim_init	( void )
 *	bool	anim_ready	( void )
 *	xAnim*	anim_var	( double )
 *
 *	void	anim_set			( xAnim*, double )
 *	void 	anim_set_continue	( xAnim*, double )
 *
 *	bool	anim_start	( xAnim*, double, uint32_t, eAnimMode, xAnimCallback )
 *	bool	anim_queue	( xAnim*, double, uint32_t, eAnimMode, xAnimCallback )
 *
 *	void	anim_free			( xAnim* )
 *	void	anim_free_module	( void )
 *
 */


//================================================================
// Definitions
//================================================================

#define EASE_IN_FACTOR 1.8
#define EASE_OUT_FACTOR 0.5

#ifndef PI
	#define PI 3.14159265359
#endif
#ifndef PI_DIV_2
	#define PI_DIV_2 1.57079632679
#endif

bool XANIMATE_READY = false;

typedef struct _xAnim xAnim;
typedef struct _xAnimator xAnimator;

typedef double (*xAnimStepFunc)(double);

typedef void (*xAnimCallback)(xAnim*);

struct _xAnim
{
	double val;
	int ival;
	xAnimator* anim;
	xArray* queue;
};
XTABLE_GENERATE_FUNCTIONS(xAnim, xAnim*);

struct _xAnimator
{
	xAnim* var;
	double start;
	double target;
	uint32_t total_dur;
	uint32_t duration;
	xAnimStepFunc step_func;
	xAnimCallback callback;
};
XTABLE_GENERATE_FUNCTIONS(xAnimator, xAnimator*);

xArray* animations = NULL;

void XANIMATE_tick(uint32_t);
int XANIMATE_round(double);


//================================================================
// Public API
//================================================================

//==================== init ====================

void anim_init(void)
{
	if ( !xCallbacks_ready() )
	{
		fprintf(stderr, "anim_start: xCallbacks not initialized\n");
		return;
	}
	animations = arr_create();
	xCallbacks_add_tick(XANIMATE_tick);
	XANIMATE_READY = true;
}

bool anim_ready(void)
{
	return XANIMATE_READY;
}

xAnim* anim_var(double val)
{
	xAnim* var;

	var = malloc(sizeof(xAnim));
	if ( var != NULL )
	{
		var->val = val;
		var->ival = XANIMATE_round(val);
		var->anim = NULL;
		var->queue = arr_create();
	}
	return var;
}


//==================== set ====================

void anim_set(xAnim* var, double val)
{
	int i;
	if ( var->anim != NULL )
	{
		for ( i=0; i<animations->size; i++ )
		{
			if ( arr_get_xAnimator(animations, i) == var->anim )
			{
				arr_remove(animations, i);
				break;
			}
		}
		free(var->anim);
		var->anim = NULL;
	}
	if ( var->queue->size > 0 )
	{
		for ( i=0; i<var->queue->size; i++ )
			free(arr_get_xAnimator(var->queue, i));
		arr_empty(var->queue);
	}

	var->val = val;
	var->ival = XANIMATE_round(val);
}

void anim_set_continue(xAnim* var, double val)
{
	var->val = val;
	var->ival = XANIMATE_round(val);

	if ( var->anim != NULL )
		var->anim->start = var->val;
}


//==================== animate ====================

bool anim_start(xAnim* var, double target, uint32_t duration, xAnimStepFunc step_func, xAnimCallback callback)
{
	int i;

	if ( var->queue->size > 0 )
	{
		for ( i=0; i<var->queue->size; i++ )
			free(arr_get_xAnimator(var->queue, i));
		arr_empty(var->queue);
	}

	if ( var->anim == NULL )
	{
		var->anim = malloc(sizeof(xAnimator));
		if ( var->anim == NULL ) return false;
		arr_add_xAnimator(animations, var->anim);
	}

	var->anim->var = var;
	var->anim->start = var->val;
	var->anim->target = target;
	var->anim->total_dur = duration;
	var->anim->duration = duration;
	var->anim->step_func = step_func;
	var->anim->callback = callback;
	return true;
}

bool anim_queue(xAnim* var, double target, double duration, xAnimStepFunc step_func, xAnimCallback callback)
{
	xAnimator* next;

	next = malloc(sizeof(xAnimator));
	if ( next == NULL ) return false;

	next->var = var;
	next->target = target;
	next->total_dur = duration;
	next->duration = duration;
	next->step_func = step_func;
	next->callback = callback;

	if ( var->anim == NULL )
	{
		var->anim = next;
		var->anim->start = var->val;
		arr_add_xAnimator(animations, var->anim);
	}
	else
		arr_add_xAnimator(var->queue, next);

	return true;
}


//==================== free ====================

void anim_free(xAnim* var)
{
	int i;
	if ( var != NULL )
	{
		if ( var->anim != NULL ) free(var->anim);
		for ( i=0; i<var->queue->size; i++ )
			free(arr_get_xAnimator(var->queue, i));
		arr_free(var->queue);
		free(var);
	}
}

void anim_free_module(void)
{
	int i;
	for ( i=0; i<animations->size; i++ )
		free(arr_get_xAnimator(animations, i));
	arr_free(animations);
	animations = NULL;
	xCallbacks_remove_tick(XANIMATE_tick);
	XANIMATE_READY = false;
}


//==================== builtin step functions ====================

double XANIM_LINEAR(double pct_time)
{
	return pct_time;
}

double XANIM_EASE_IN(double pct_time)
{
	return sin(pct_time*PI_DIV_2);
}

double XANIM_EASE_OUT(double pct_time)
{
	return (1.0 - cos(pct_time*PI_DIV_2));
}

double XANIM_EASE_IN_OUT(double pct_time)
{
	return pct_time*XANIM_EASE_IN(pct_time) + (1.0-pct_time)*XANIM_EASE_OUT(pct_time);
}

double XANIM_WAIT(double pct_time)
{
	return 0.0;
}


//================================================================
// "Private" Functions
//================================================================

void XANIMATE_tick(uint32_t dt)
{
	xAnim* var;
	xAnimator* anim;
	int i;

	for ( i=0; i<animations->size; i++ )
	{
		anim = arr_get_xAnimator(animations, i);
		var = anim->var;

		if ( dt >= anim->duration )
		{
			arr_remove(animations, i);
			i--;
			var->anim = NULL;

			var->val = anim->start + anim->step_func(1.0)*(anim->target - anim->start);
			var->ival = XANIMATE_round(var->val);
			if ( anim->callback != NULL ) anim->callback(var);

			free(anim);

			if ( var->anim == NULL && var->queue->size > 0 )
			{
				anim = arr_get_xAnimator(var->queue, 0);
				arr_remove(var->queue, 0);

				var->anim = anim;
				var->anim->start = var->val;
				arr_add_xAnimator(animations, var->anim);
			}
			return;
		}

		anim->var->val = anim->start + (anim->target - anim->start) * anim->step_func(1.0 - ((double)anim->duration / (double)anim->total_dur));
		anim->var->ival = XANIMATE_round(anim->var->val);
		anim->duration -= dt;
	}
}

int XANIMATE_round(double v)
{
	double d = v - floor(v);
	if ( d > 0.5 ) return (int)ceil(v);
	return (int)floor(v);
}

#endif
