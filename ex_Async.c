
#include <stdio.h>

#include "xCallbacks.h"
#include "xAsync.h"

void tick(uint32_t dt)
{
	printf(". %i ms\n", dt);
}

xMap* task(xMap* args)
{
	char buff[255];

	printf("TASK! id=%i\n", map_geti(args, "id"));
	printf(">>> TYPE A LINE <<<\n");

	fgets(buff, 254, stdin);
	if ( !feof(stdin) )
		buff[strlen(buff)-1] = '\0';

	map_sets(args, "res", buff);
	return args;
}

void cb(xMap* res)
{
	char buff[255];
	map_gets(res, "res", buff, 255);
	printf("CALLBACK! id=%i res='%s'\n", map_geti(res, "id"), buff);
	map_free(res);
}

void pop1(uint32_t dt, xMap* args)
{
	printf("pop1 !\n");
	async_task(task, map_quick("id", VAL_INT, 42), cb);
}

int main(void)
{
	xCallbacks_init();
	xCallbacks_set_dt(100);
	xCallbacks_eat_cpu(false);
	xCallbacks_add_tick(tick);

	async_init();

	printf("-- waiting 1s...\n");
	xCallbacks_add(pop1, NULL, 1000);

	xCallbacks_run();

	xCallbacks_free();
	return 0;
}
