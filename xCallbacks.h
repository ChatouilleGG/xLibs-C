
#ifndef __X_CALLBACKS__
#define __X_CALLBACKS__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <math.h>
#include <sys/timeb.h>

// IFDEF WIN
#include <windows.h>

#include "xTables.h"

/* xCallbacks API :
 *
 *	void	xCallbacks_init		( void )
 *	bool	xCallbacks_ready	( void )
 *
 *	void	xCallbacks_set_dt		( uint32_t ms )
 *	void	xCallbacks_add_tick		( xTickHandler )
 *	void	xCallbacks_remove_tick	( xTickHandler )
 *
 *	void	xCallbacks_add		( xCallback, xMap*, uint32_t )
 *	void	xCallbacks_run		( void )
 *	void	xCallbacks_stop		( void )
 *	
 *	void	xCallbacks_free		( void )
 *
 */


//================================================================
// Definitions
//================================================================

#ifndef _BOOL
	#define _BOOL
	typedef enum { false, true } bool;
#endif

bool XCALLBACKS_READY = false;

bool XCALLBACKS_STOP = false;

uint32_t target_dt = 1;

typedef void (*xTickHandler)(uint32_t);
XTABLE_GENERATE_FUNCTIONS(xTickHandler, xTickHandler);
xArray* tick_handlers = NULL;

typedef void (*xCallback)(uint32_t, xMap*);

typedef struct _xEvent xEvent;
struct _xEvent
{
	long date;
	xCallback func;
	xMap* args;
	long from;
};
XTABLE_GENERATE_FUNCTIONS(xEvent, xEvent*);

xArray* events = NULL;
xArray* events_to_add = NULL;

void XCALLBACKS_SLEEP(long);
long XCALLBACKS_currentTimeMillis(void);
void XCALLBACKS_INSERT(xEvent*, xArray*, int, int);


//================================================================
// Public API
//================================================================

//==================== init ====================

void xCallbacks_init(void)
{
	tick_handlers = arr_create();
	events = arr_create();
	events_to_add = arr_create();
	XCALLBACKS_READY = true;
}

bool xCallbacks_ready(void)
{
	return XCALLBACKS_READY;
}


//==================== config ====================

void xCallbacks_set_dt(uint32_t ms)
{
	target_dt = ms;
}

void xCallbacks_add_tick(xTickHandler func)
{
	arr_add_xTickHandler(tick_handlers, func);
}

void xCallbacks_remove_tick(xTickHandler func)
{
	int i;
	for ( i=0; i<tick_handlers->size; i++ )
	{
		if ( arr_get_xTickHandler(tick_handlers, i) == func )
		{
			arr_remove(tick_handlers, i);
			break;
		}
	}
}


//==================== run ====================

bool xCallbacks_add(xCallback func, xMap* args, uint32_t delay)
{
	xEvent* event;
	long now;

	now = XCALLBACKS_currentTimeMillis();

	event = malloc(sizeof(xEvent));
	if ( event == NULL )
		return false;

	event->func = func;
	event->args = args;
	event->from = now;
	event->date = now+delay;
	arr_add_xEvent(events_to_add, event);
	return true;
}

void xCallbacks_run(void)
{
	long timestamp, now;
	uint32_t dt;
	xEvent* event;
	int i;

	if ( !XCALLBACKS_READY ) return;

	timestamp = XCALLBACKS_currentTimeMillis();
	XCALLBACKS_SLEEP(target_dt);

	while (true)
	{
		now = XCALLBACKS_currentTimeMillis();
		dt = now - timestamp;
		timestamp = now;

		for ( i=0; i<tick_handlers->size; i++ )
		{
			((xTickHandler)arr_getp(tick_handlers, i)) (dt);
			if ( XCALLBACKS_STOP )
			{
				XCALLBACKS_STOP = false;
				return;
			}
		}

		event = arr_get_xEvent(events, 0);
		now = XCALLBACKS_currentTimeMillis();
		while ( event != NULL && event->date <= now )
		{
			event->func(now - event->from, event->args);
			if ( XCALLBACKS_STOP )
			{
				XCALLBACKS_STOP = false;
				return;
			}

			arr_remove(events, 0);
			free(event);

			event = arr_get_xEvent(events, 0);
			now = XCALLBACKS_currentTimeMillis();
		}

		for ( i=0; i<events_to_add->size; i++ )
			XCALLBACKS_INSERT(arr_get_xEvent(events_to_add, i), events, 0, events->size-1);
		arr_empty(events_to_add);

		XCALLBACKS_SLEEP(timestamp+target_dt-now);
	}
}

void xCallbacks_stop(void)
{
	XCALLBACKS_STOP = true;
}


//==================== free ====================

void xCallbacks_free(void)
{
	arr_free(tick_handlers);
	arr_free(events);
	arr_free(events_to_add);
	events = NULL;
	XCALLBACKS_READY = false;
}


//================================================================
// "Private" Functions
//================================================================

LARGE_INTEGER counts_per_sec;
bool perfcounter_init = false;
long XCALLBACKS_currentTimeMillis(void)
{
	static LARGE_INTEGER counts;

	if ( !perfcounter_init )
	{
		QueryPerformanceFrequency(&counts_per_sec);
		perfcounter_init = true;
	}
	
	QueryPerformanceCounter(&counts);

	return (counts.QuadPart*1000)/counts_per_sec.QuadPart;
}

/* precise sleep taken from SFML */
#pragma comment(lib, "Winmm.lib")
void XCALLBACKS_SLEEP(long ms)
{
	if ( ms <= 0 ) return;

	// Get the supported timer resolutions on this system
	TIMECAPS tc;
	timeGetDevCaps(&tc, sizeof(TIMECAPS));

	// Set the timer resolution to the minimum for the Sleep call
	timeBeginPeriod(tc.wPeriodMin);

	// Wait...
	Sleep(ms);

	// Reset the timer resolution back to the system default
	timeEndPeriod(tc.wPeriodMin);
}

void XCALLBACKS_INSERT(xEvent* e, xArray* arr, int lb, int hb)
{
	if ( hb-lb < 0 )
		arr_add_xEvent(arr, e);
	else if ( hb-lb == 0 )
	{
		if ( e->date <= arr_get_xEvent(arr, lb)->date )
			arr_insert_xEvent(arr, e, lb);
		else
			arr_insert_xEvent(arr, e, hb+1);
	}
	else
	{
		xEvent* m = arr_get_xEvent(arr, floor((lb+hb)/2));
		if ( e->date <= m->date )
			XCALLBACKS_INSERT(e, arr, lb, floor((lb+hb)/2));
		else
			XCALLBACKS_INSERT(e, arr, floor((lb+hb)/2)+1, hb);
	}
}

#endif
