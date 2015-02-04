
#include <stdio.h>

#include "xCallbacks.h"

void tick(uint32_t dt)
{
	//printf("%i\n", dt);
}

char buff[1000];

void pop1(uint32_t dt, xMap* args)
{
	printf("pop1 after %ims\n", dt);
	if ( map_tostring(args, buff, 1000) ) printf("%s\n", buff);
	if ( args != NULL ) map_free(args);
}

void pop2(uint32_t dt, xMap* args)
{
	printf("pop2 after %ims\n", dt);
	if ( map_tostring(args, buff, 1000) ) printf("%s\n", buff);
	if ( args != NULL ) map_free(args);
}

void pop3(uint32_t dt, xMap* args)
{
	printf("pop3 after %ims\n", dt);
	if ( map_tostring(args, buff, 1000) ) printf("%s\n", buff);
	if ( args != NULL ) map_free(args);
}

void pop4(uint32_t dt, xMap* args)
{
	printf("pop4 after %ims\n", dt);
	if ( map_tostring(args, buff, 1000) ) printf("%s\n", buff);
	if ( args != NULL ) map_free(args);
}

void printEvents()
{
	int i;
	xEvent* e;
	long now;

	now = XCALLBACKS_currentTimeMillis();
	printf("== EVENTS ==\n");
	for ( i=0; i<events->size; i++ )
	{
		e = arr_get_xEvent(events, i);
		printf("  %i => %u\t(%ims from now)\n", i, e->date, e->date - now);
	}
}

int main(void)
{
	xMap* map;
	xArray* arr;

	xCallbacks_init();
	xCallbacks_set_dt(2);
	xCallbacks_add_tick(tick);

	printEvents();
	xCallbacks_add(pop1, NULL, 1000);
	printEvents();
	xCallbacks_add(pop2, map_quick("bleh", VAL_INT, 42), 2000);
	xCallbacks_add(pop3, map_quick(
		"brouf", VAL_STRING, "pouet",
		"array", VAL_ARRAY, arr_quick(
			VAL_INT, 10,
			VAL_INT, 20,
			VAL_INT, 30,
			VAL_INT, 40,
			VAL_STRING, "okok"
		)
	), 500);
	xCallbacks_add(pop4, map_create(), 1500);
	printEvents();

	xCallbacks_run();

	return 0;
}
