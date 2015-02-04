
#include <stdio.h>

#include "xTables.h"

void tpprint(xPair* p)
{
	switch (p->type)
	{
		case VAL_PTR:		printf("%s => *p (%x)\n", p->key, p->value);			break;
		case VAL_MAP:
		{
			char buff[1024];
			map_tostring(p->value, buff, 1024);
			printf("%s => %s", p->key, buff);
			break;
		}
		case VAL_STRING:	printf("%s => \"%s\"\n", p->key, (char*)(p->value));	break;
		case VAL_INT:		printf("%s => %i\n", p->key, *((int*)p->value));		break;
		case VAL_DOUBLE:	printf("%s => %f\n", p->key, *((double*)p->value));		break;
	}
}

int main(void)
{
	xMap* map;
	xMap* other;
	char buff[1024];
	xPair* p;
	xArray* arr;
	int i;

	printf("\n-- creating map with random stuff\n");
	map = map_create();
	map_seti(map, "quarante-deux", 42);
	map_sets(map, "string blabla", "blabla");
	map_setd(map, "pi", 3.1415926);
	map_setp(map, "null", NULL);
	map_setmap(map, "myself", map);
	map_tostring(map, buff, 1024) ? printf(buff) : printf("error: map_tostring\n");

	printf("\n-- deleting 'null'\n");
	map_del(map, "null");
	map_tostring(map, buff, 1024) ? printf(buff) : printf("error: map_tostring\n");

	printf("\n-- deleting 'quarante-deux'\n");
	map_del(map, "quarante-deux");
	map_tostring(map, buff, 1024) ? printf(buff) : printf("error: map_tostring\n");

	printf("\n-- deleting 'test'\n");
	map_del(map, "test");

	printf("\n-- getting string 'string blabla'\n");
	map_gets(map, "string blabla", buff, 1024) ? printf("%s\n", buff) : printf("failed!\n");

	printf("\n-- creating other map with numbers\n");
	other = map_create();
	map_seti(other, "twelve", 12);
	map_seti(other, "thirteen", 13);
	map_seti(other, "fourteen", 14);
	map_seti(other, "fifteen", 15);
	map_tostring(other, buff, 1024) ? printf(buff) : printf("error: map_tostring\n");

	printf("\n-- setting numbers into first map\n");
	map_setmap(map, "numbers", other);
	map_tostring(map, buff, 1024) ? printf(buff) : printf("error: map_tostring\n");

	printf("\n-- iterating with while...\n");
	map_start(map);
	while ( map_hasnext(map) )
	{
		xPair* p = map_next(map);
		tpprint(p);
	}

	printf("\n-- deleting 'myself'\n");
	map_del(map, "myself");

	printf("\n-- iterating with for...\n");
	for ( p=map_first(map); p!=NULL; p=map_next(map) )
		tpprint(p);

	map_free(map);
	map_free(other);

	printf("\n-- creating array with random stuff\n");
	arr = arr_create();
	for ( i=0; i<6; i++ )
		arr_addi(arr, (i < 3) ? 3-i : i-3);
	arr_tostring(arr, buff, 1024) ? printf(buff) : printf("error: arr_tostring\n");

	printf("\n-- adding doubles on 10,12\n");
	arr_setd(arr, 10, 6.28);
	arr_setd(arr, 12, 3.14);
	arr_tostring(arr, buff, 1024) ? printf(buff) : printf("error: arr_tostring\n");

	printf("\n-- sorting ints...\n");
	printf("found %i ints\n", arr_sorti(arr));
	arr_tostring(arr, buff, 1024) ? printf(buff) : printf("error: arr_tostring\n");
	
	printf("\n-- sorting doubles...\n");
	printf("found %i doubles\n", arr_sortd(arr));
	arr_tostring(arr, buff, 1024) ? printf(buff) : printf("error: arr_tostring\n");

	printf("\n-- adding strings\n");
	arr_sets(arr, 2, "blabla");
	arr_sets(arr, 4, "pouet");
	arr_sets(arr, 6, "gron");
	arr_sets(arr, 7, "mouaif");
	arr_sets(arr, 8, "hurr");
	arr_tostring(arr, buff, 1024) ? printf(buff) : printf("error: arr_tostring\n");
	
	printf("\n-- sorting strings\n");
	printf("found %i strings\n", arr_sorts(arr));
	arr_tostring(arr, buff, 1024) ? printf(buff) : printf("error: arr_tostring\n");

	printf("\n-- insert 42 on 1\n");
	arr_inserti(arr, 42, 1);
	arr_tostring(arr, buff, 1024) ? printf(buff) : printf("error: arr_tostring\n");

	return 0;
}
