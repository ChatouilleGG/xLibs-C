
#include <stdio.h>

#include "xCallbacks.h"
#include "xAnimate.h"

xAnim* var1 = NULL;
double prevval = 67.0;
int track = 1;

void done1(xAnim* var)
{
	printf("animation 1 done!\n");
}

void done2(xAnim* var)
{
	printf("animation 2 done!\n");
}

void pop1(uint32_t dt, xMap* args)
{
	printf("pop1 after %ims\n", dt);
	printf("-- creating var1 = 67\n");
	var1 = anim_var(67.0);
	printf("-- animating var1 to 112 (1s)\n");
	anim_start(var1, 112.0, 1000, AM_EASE_IN, done1);
	printf("-- queueing animation var1 to -12 (0.6s)\n");
	anim_queue(var1, -12.0, 500, AM_EASE_OUT, done2);
}

void track_val(uint32_t dt, xMap* args)
{
	if ( !track ) return;

	printf("var1 val = %i (%f) (diff=%f)\n", var1->ival, var1->val, var1->val - prevval);
	prevval = var1->val;
	xCallbacks_add(track_val, NULL, 10);
}

void stop_track_val(uint32_t dt, xMap* args)
{
	track = 0;
	printf("stop!\n");
	xCallbacks_stop();
}

int main(void)
{
	xCallbacks_init();
	xCallbacks_set_dt(10);
	xCallbacks_eat_cpu(true);

	anim_init();

	printf("-- waiting 1s...\n");
	xCallbacks_add(pop1, NULL, 1000);
	xCallbacks_add(track_val, NULL, 1001);
	xCallbacks_add(stop_track_val, NULL, 2600);

	xCallbacks_run();

	xCallbacks_free();
	return 0;
}
