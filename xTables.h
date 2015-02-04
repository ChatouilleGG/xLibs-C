
#ifndef __X_TABLES__
#define __X_TABLES__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

/* xMap API :
 *
 *	bool	map_init	( xMap* )
 *	xMap*	map_create	( void )
 *
 *	xMap* 	map_quick	( char* key1,  eValType type1,  value1,  ... )
 *
 *	bool	map_seti	( xMap*,  char*,  int		)
 *	bool	map_setd	( xMap*,  char*,  double	)
 *	bool	map_setp	( xMap*,  char*,  void*		)
 *	bool	map_sets	( xMap*,  char*,  char*		)
 *	bool	map_setmap	( xMap*,  char*,  xMap*		)
 *	bool	map_setarr	( xMap*,  char*,  xArray*	)	
 *
 *	xPair*	map_get		( xMap*,  char* )
 *	int		map_geti	( xMap*,  char* )
 *	double	map_getd	( xMap*,  char* )
 *	void*	map_getp	( xMap*,  char* )
 *	bool	map_gets	( xMap*,  char*,  char* buff,  size_t buffsize )
 *	xMap*	map_getmap	( xMap*,  char* )
 *	xArray*	map_getarr	( xMap*,  char* )
 *
 *	bool	map_del		( xMap*,  char* )
 *	void	map_empty	( xMap* )
 *	void	map_free	( xMap* )
 *
 *	void	map_start	( xMap* )
 *	xPair*	map_first	( xMap* )
 *	bool	map_hasnext	( xMap* )
 *	xPair*	map_next	( xMap* )
 *
 *	bool	map_tostring	( xMap*,  char* buff,  size_t buffsize )
 */
 
 
 /* xArray API :
 *
 *	bool	arr_init	( xArray* )
 *	xArray*	arr_create	( void )
 *
 *	xArray* arr_quick	( eValType type1,  value1,  eValType type2,  value2,  ... )
 *
 *	bool	arr_seti	( xArray*,  uint32_t,  int		)
 *	bool	arr_setd	( xArray*,  uint32_t,  double	)
 *	bool	arr_setp	( xArray*,  uint32_t,  void*	)
 *	bool	arr_sets	( xArray*,  uint32_t,  char*	)
 *	bool	arr_setmap	( xArray*,  uint32_t,  xMap*	)
 *	bool	arr_setarr	( xArray*,  uint32_t,  xArray*	)
 *
 *	xElem*	arr_get		( xArray*,  uint32_t )
 *	int		arr_geti	( xArray*,  uint32_t )
 *	double	arr_getd	( xArray*,  uint32_t )
 *	void*	arr_getp	( xArray*,  uint32_t )
 *	bool	arr_gets	( xArray*,  uint32_t,  char* buff,  int buffsize )
 *	xMap*	arr_getmap	( xArray*,  uint32_t )
 *	xArray*	arr_getarr	( xArray*,  uint32_t )
 *
 *	bool	arr_addi	( xArray*,  int		)
 *	bool	arr_addd	( xArray*,  double	)
 *	bool	arr_addp	( xArray*,  void*	)
 *	bool	arr_adds	( xArray*,  char*	)
 *	bool	arr_addmap	( xArray*,  xMap*	)
 *	bool	arr_addarr	( xArray*,  xArray*	)
 *
 *	bool	arr_inserti		( xArray*,  int,	  uint32_t )
 *	bool	arr_insertd		( xArray*,  double,	  uint32_t )
 *	bool	arr_insertp		( xArray*,  void*,	  uint32_t )
 *	bool	arr_inserts		( xArray*,  char*,	  uint32_t )
 *	bool	arr_insertmap	( xArray*,  xMap*,	  uint32_t )
 *	bool	arr_insertarr	( xArray*,  xArray*,  uint32_t )
 *
 *	bool	arr_remove	( xArray*,  uint32_t )
 *	void	arr_empty	( xArray* )
 *	void	arr_free	( xArray* )
 *
 *	uint32_t	arr_size	( xArray* )
 *
 *	uint32_t	arr_sort	( xArray*,  int (*comp)(const xElem**, const xElem**) )
 *	uint32_t	arr_sorti	( xArray* )
 *	uint32_t	arr_sortd	( xArray* )
 *	uint32_t	arr_sorts 	( xArray* )
 *
 *	bool	arr_tostring	( xArray*,  char* buff,  size_t buffsize )
 */
 
 
 /* Macros :
 *
 *	XTABLE_GENERATE_FUNCTIONS(name, ptr_type)
 *		Generates map_get, map_set, arr_get, arr_set, arr_add, arr_insert for a user-defined type
 *		eg: arr_get_FooStruct(17)
 *		Generated functions will work with pointers and will not duplicate values
 *		Elements will be of type VAL_PTR
 *		Cast checks will NOT be performed when using map_get and arr_get for custom types
 */

#define XTABLE_GENERATE_FUNCTIONS(NAME,PTRTYPE)\
bool map_set_##NAME(xMap* map, char* key, ##PTRTYPE ptr)\
{\
	return XMAP_set(map, key, ptr, VAL_PTR);\
}\
bool arr_set_##NAME(xArray* arr, uint32_t index, ##PTRTYPE ptr)\
{\
	return XARRAY_set(arr, index, ptr, VAL_PTR);\
}\
##PTRTYPE map_get_##NAME(xMap* map, char* key)\
{\
	xPair* p = map_get(map, key);\
	return (p != NULL && p->type == VAL_PTR) ? (##PTRTYPE)p->value : NULL;\
}\
##PTRTYPE arr_get_##NAME(xArray* arr, uint32_t index)\
{\
	xElem* v = arr_get(arr, index);\
	return (v != NULL && v->type == VAL_PTR) ? (##PTRTYPE)v->value : NULL;\
}\
bool arr_insert_##NAME(xArray* arr, ##PTRTYPE p, uint32_t index)\
{\
	if ( XARRAY_insert(arr, index) )\
		return arr_set_##NAME(arr, index, p);\
	return false;\
}\
bool arr_add_##NAME(xArray* arr, ##PTRTYPE p)\
{\
	return arr_insert_##NAME(arr, p, arr->size);\
}\

#define ARR_END -1
#define arr_quick(type, value, ...) arr_from_values(type, (void*)value, __VA_ARGS__, ARR_END)

#define MAP_END NULL
#define map_quick(key, type, value, ...) map_from_values(key, type, (void*)value, __VA_ARGS__, MAP_END)

 
//================================================================
// Definitions
//================================================================

#define DEFAULT_MAP_SIZE 10
#define MAP_ENLARGE_AT 75
#define MAP_SHRINK_AT 15
#define MAP_RESIZE_MULT 2
#define MAP_RESIZE_DIV 2

#define DEFAULT_ARR_SIZE 10
#define ARR_ENLARGE_AT 80
#define ARR_SHRINK_AT 25
#define ARR_RESIZE_MULT 2
#define ARR_RESIZE_DIV 2

#ifndef _BOOL
	#define _BOOL
	typedef enum { false, true } bool;
#endif

typedef enum {
	VAL_PTR,		// unknown pointer		NOT duplicated
	VAL_MAP,		// pointer to xMap		NOT duplicated
	VAL_ARRAY,		// pointer to xArray	NOT duplicated
	VAL_STRING,		// pointer to string	duplicated
	VAL_INT,		// int					duplicated
	VAL_DOUBLE		// double				duplicated
} eValType;

// eValType <= VAL_STRING are all pointers
// eValType >= VAL_STRING are all duplicated


//==================== xMap ====================

typedef struct _xPair xPair;
struct _xPair
{
	char* key;
	void* value;
	eValType type;

	xPair* next;
};

typedef struct _xMap xMap;
struct _xMap
{
	xPair** table;

	uint32_t size;
	uint32_t fill;

	uint32_t cur_index;
	xPair* cur_p;
};

bool XMAP_set(xMap*, char*, void*, eValType);
void XMAP_insert(xMap*, xPair*);
void XMAP_resize(xMap*, uint32_t);
uint32_t XMAP_hash(unsigned char*);
bool XMAP_tostring(xMap*, char*, size_t, uint32_t);


//==================== xArray ====================

typedef struct _xElem xElem;
struct _xElem
{
	eValType type;
	void* value;
};

typedef struct _xArray xArray;
struct _xArray
{
	xElem** table;
	uint32_t size;

	uint32_t max_size;
};

bool XARRAY_set(xArray*, uint32_t, void*, eValType);
bool XARRAY_insert(xArray*, uint32_t);
bool XARRAY_resize(xArray*, uint32_t);
uint32_t XARRAY_sorttype(xArray* arr, eValType type);
int XARRAY_comp_int(const xElem**, const xElem**);
int XARRAY_comp_double(const xElem**, const xElem**);
int XARRAY_comp_string(const xElem**, const xElem**);


//================================================================
// Public API
//================================================================

//==================== init ====================

bool map_init(xMap* map)
{
	if ( map != NULL )
	{
		map->table = calloc(DEFAULT_MAP_SIZE, sizeof(xPair*));
		if ( map->table != NULL )
		{
			map->size = DEFAULT_MAP_SIZE;
			map->fill = 0;
			return true;
		}
	}
	return false;
}

xMap* map_create(void)
{
	xMap* map;

	map = malloc(sizeof(xMap));
	if ( map != NULL )
		map_init(map);

	return map;
}

bool arr_init(xArray* arr)
{
	if ( arr != NULL )
	{
		arr->table = calloc(DEFAULT_ARR_SIZE, sizeof(xElem*));
		if ( arr->table != NULL )
		{
			arr->max_size = DEFAULT_ARR_SIZE;
			arr->size = 0;
			return true;
		}
	}
	return false;
}

xArray* arr_create(void)
{
	xArray* arr;

	arr = malloc(sizeof(xArray));
	if ( arr != NULL )
		arr_init(arr);

	return arr;
}


//==================== set ====================

bool map_seti(xMap* map, char* key, int i)
{
	int* value = malloc(sizeof(int));
	if ( value == NULL )
		return false;
	*value = i;
	if ( !XMAP_set(map, key, value, VAL_INT) )
	{
		free(value);
		return false;
	}
	return true;
}

bool map_setd(xMap* map, char* key, double d)
{
	double* value = malloc(sizeof(double));
	if ( value == NULL )
		return false;
	*value = d;
	if ( !XMAP_set(map, key, value, VAL_DOUBLE) )
	{
		free(value);
		return false;
	}
	return true;
}

bool map_setp(xMap* map, char* key, void* ptr)
{
	return XMAP_set(map, key, ptr, VAL_PTR);
}

bool map_sets(xMap* map, char* key, char* s)
{
	char* value = malloc((strlen(s)+1)*sizeof(char));
	if ( value == NULL )
		return false;
	strcpy(value, s);
	if ( !XMAP_set(map, key, value, VAL_STRING) )
	{
		free(value);
		return false;
	}
	return true;
}

bool map_setmap(xMap* map, char* key, xMap* m)
{
	return XMAP_set(map, key, m, VAL_MAP);
}

bool map_setarr(xMap* map, char* key, xArray* t)
{
	return XMAP_set(map, key, t, VAL_ARRAY);
}


bool arr_seti(xArray* arr, uint32_t index, int i)
{
	int* value = malloc(sizeof(int));
	if ( value == NULL )
		return false;
	*value = i;
	if ( !XARRAY_set(arr, index, value, VAL_INT) )
	{
		free(value);
		return false;
	}
	return true;
}

bool arr_setd(xArray* arr, uint32_t index, double d)
{
	double* value = malloc(sizeof(double));
	if ( value == NULL )
		return false;
	*value = d;
	if ( !XARRAY_set(arr, index, value, VAL_DOUBLE) )
	{
		free(value);
		return false;
	}
	return true;
}

bool arr_setp(xArray* arr, uint32_t index, void* ptr)
{
	return XARRAY_set(arr, index, ptr, VAL_PTR);
}

bool arr_sets(xArray* arr, uint32_t index, char* s)
{
	char* value = malloc((strlen(s)+1)*sizeof(char));
	if ( value == NULL )
		return false;
	strcpy(value, s);
	if ( !XARRAY_set(arr, index, value, VAL_STRING) )
	{
		free(value);
		return false;
	}
	return true;
}

bool arr_setmap(xArray* arr, uint32_t index, xMap* m)
{
	return XARRAY_set(arr, index, m, VAL_MAP);
}

bool arr_setarr(xArray* arr, uint32_t index, xArray* t)
{
	return XARRAY_set(arr, index, t, VAL_ARRAY);
}


//==================== get ====================

xPair* map_get(xMap* map, char* key)
{
	if ( map != NULL && key != NULL && strlen(key) > 0 )
	{
		uint32_t index;
		xPair* p;

		index = XMAP_hash((unsigned char*)key) % map->size;
		p = map->table[index];
		while ( p != NULL )
		{
			if ( strcmp(p->key, key) == 0 )
				return p;
			p = p->next;
		}
	}
	return NULL;
}

int map_geti(xMap* map, char* key)
{
	xPair* p = map_get(map, key);
	return (p != NULL && p->type == VAL_INT) ? *((int*)(p->value)) : 0;
}

double map_getd(xMap* map, char* key)
{
	xPair* p = map_get(map, key);
	return (p != NULL && p->type == VAL_DOUBLE) ? *((double*)(p->value)) : 0.0;
}

void* map_getp(xMap* map, char* key)
{
	xPair* p = map_get(map, key);
	return (p != NULL && p->type <= VAL_STRING) ? p->value : NULL;
}

bool map_gets(xMap* map, char* key, char* dest, uint32_t destsize)
{
	if ( dest != NULL && destsize > 0 )
	{
		xPair* p = map_get(map, key);
		if ( p != NULL && p->type == VAL_STRING )
		{
			strncpy(dest, (char*)p->value, destsize);
			return true;
		}
	}
	return false;
}

xMap* map_getmap(xMap* map, char* key)
{
	xPair* p = map_get(map, key);
	return (p != NULL && (p->type == VAL_MAP || p->type == VAL_PTR)) ? (xMap*)p->value : NULL;
}

xArray* map_getarr(xMap* map, char* key)
{
	xPair* p = map_get(map, key);
	return (p != NULL && (p->type == VAL_ARRAY || p->type == VAL_PTR)) ? (xArray*)p->value : NULL;
}


xElem* arr_get(xArray* arr, uint32_t index)
{
	if ( arr != NULL && index < arr->size )
		return arr->table[index];

	return NULL;
}

int arr_geti(xArray* arr, uint32_t index)
{
	xElem* v = arr_get(arr, index);
	return (v != NULL && v->type == VAL_INT) ? *((int*)(v->value)) : 0;
}

double arr_getd(xArray* arr, uint32_t index)
{
	xElem* v = arr_get(arr, index);
	return (v != NULL && v->type == VAL_DOUBLE) ? *((double*)(v->value)) : 0.0;
}

void* arr_getp(xArray* arr, uint32_t index)
{
	xElem* v = arr_get(arr, index);
	return (v != NULL && v->type <= VAL_STRING) ? v->value : NULL;
}

bool arr_gets(xArray* arr, uint32_t index, char* dest, uint32_t destsize)
{
	if ( dest != NULL && destsize > 0 )
	{
		xElem* v = arr_get(arr, index);
		if ( v != NULL && v->type == VAL_STRING )
		{
			strncpy(dest, (char*)v->value, destsize);
			return true;
		}
	}
	return false;
}

xMap* arr_getmap(xArray* arr, uint32_t index)
{
	xElem* v = arr_get(arr, index);
	return (v != NULL && (v->type == VAL_MAP || v->type == VAL_PTR)) ? (xMap*)v->value : 0;
}

xArray* arr_getarr(xArray* arr, uint32_t index)
{
	xElem* v = arr_get(arr, index);
	return (v != NULL && (v->type == VAL_ARRAY || v->type == VAL_PTR)) ? (xArray*)v->value : 0;
}


//==================== insert ====================

bool arr_inserti(xArray* arr, int i, uint32_t index)
{
	if ( XARRAY_insert(arr, index) )
		return arr_seti(arr, index, i);
	return false;
}

bool arr_insertd(xArray* arr, double d, uint32_t index)
{
	if ( XARRAY_insert(arr, index) )
		return arr_setd(arr, index, d);
	return false;
}

bool arr_insertp(xArray* arr, void* p, uint32_t index)
{
	if ( XARRAY_insert(arr, index) )
		return arr_setp(arr, index, p);
	return false;
}

bool arr_inserts(xArray* arr, char* s, uint32_t index)
{
	if ( XARRAY_insert(arr, index) )
		return arr_sets(arr, index, s);
	return false;
}

bool arr_insertmap(xArray* arr, xMap* m, uint32_t index)
{
	if ( XARRAY_insert(arr, index) )
		return arr_setmap(arr, index, m);
	return false;
}

bool arr_insertarr(xArray* arr, xArray* t, uint32_t index)
{
	if ( XARRAY_insert(arr, index) )
		return arr_setarr(arr, index, t);
	return false;
}


//==================== append ====================

bool arr_addi(xArray* arr, int i)
{
	return arr_inserti(arr, i, arr->size);
}

bool arr_addd(xArray* arr, double d)
{
	return arr_insertd(arr, d, arr->size);
}

bool arr_addp(xArray* arr, void* p)
{
	return arr_insertp(arr, p, arr->size);
}

bool arr_adds(xArray* arr, char* s)
{
	return arr_inserts(arr, s, arr->size);
}

bool arr_addmap(xArray* arr, xMap* m)
{
	return arr_insertmap(arr, m, arr->size);
}

bool arr_addarr(xArray* arr, xArray* t)
{
	return arr_insertarr(arr, t, arr->size);
}


//==================== sort ====================

uint32_t arr_sort(xArray* arr, int (*comp)(const xElem**, const xElem**) )
{
	if ( arr != NULL && comp != NULL )
	{
		uint32_t i;
		uint32_t j = 0;
		uint32_t num = 0;
		for ( i=0; i<arr->size; i++ )
		{
			if ( arr->table[i] == NULL )
			{
				if ( i+1 > j )
					j = i+1;
				for ( ; j<arr->size; j++ )
				{
					if ( arr->table[j] != NULL )
					{
						arr->table[i] = arr->table[j];
						arr->table[j] = NULL;
						num = i+1;
						break;
					}
				}
				if ( num < i+1 )	// no more items
					break;
			}
			else
				num = i+1;
		}
		if ( num > 0 )
		{
			qsort(arr->table, num, sizeof(xElem*), comp);
			return num;
		}
	}
	return 0;
}

uint32_t arr_sorti(xArray* arr)
{
	return XARRAY_sorttype(arr, VAL_INT);
}

uint32_t arr_sortd(xArray* arr)
{
	return XARRAY_sorttype(arr, VAL_DOUBLE);
}

uint32_t arr_sorts(xArray* arr)
{
	return XARRAY_sorttype(arr, VAL_STRING);
}


//==================== del ====================

bool map_del(xMap* map, char* key)
{
	if ( map != NULL && key != NULL && strlen(key) > 0 )
	{
		uint32_t index;
		xPair* p;

		index = XMAP_hash((unsigned char*)key) % map->size;
		if ( map->table[index] == NULL )
			return false;

		p = map->table[index];
		if ( strcmp(p->key, key) == 0 )
		{
			free(p->key);
			if ( p->type >= VAL_STRING )
				free(p->value);
			map->table[index] = p->next;
			free(p);

			if ( map->table[index] == NULL )
			{
				map->fill --;
				if ( 100.0f*(float)map->fill/(float)map->size < MAP_SHRINK_AT )
					XMAP_resize(map, map->size / MAP_RESIZE_DIV);
			}
			return true;
		}
		else
		{
			while ( p->next != NULL )
			{
				if ( strcmp(p->next->key, key) == 0 )
				{
					xPair* p2 = p->next;
					free(p2->key);
					if ( p2->type >= VAL_STRING )
						free(p2->value);
					p->next = p2->next;
					free(p2);
					return true;
				}
				p = p->next;
			}
		}
	}
	return false;
}

bool arr_remove(xArray* arr, uint32_t index)
{
	if ( arr != NULL && index < arr->size )
	{
		if ( arr->table[index] != NULL )
		{
			if ( arr->table[index]->type >= VAL_STRING )
				free(arr->table[index]->value);
			free(arr->table[index]);
		}

		if ( index < arr->size-1 )
		{
			memmove(arr->table+index, arr->table+index+1, (arr->size - (index+1))*sizeof(xElem*));
			arr->table[arr->size-1] = NULL;
		}

		arr->size --;
		if ( 100.0f*(float)arr->size/(float)arr->max_size < ARR_SHRINK_AT )
			XARRAY_resize(arr, arr->max_size / ARR_RESIZE_DIV);

		return true;
	}
	return false;
}

void map_empty(xMap* map)
{
	if ( map != NULL )
	{
		if ( map->table != NULL )
		{
			uint32_t i;
			for ( i=0; i<map->size; i++ )
			{
				while ( map->table[i] != NULL )
				{
					xPair* p = map->table[i];
					free(p->key);
					if ( p->type >= VAL_STRING )
						free(p->value);
					map->table[i] = p->next;
					free(p);
				}
			}
			free(map->table);
		}
		map_init(map);
	}
}

void map_free(xMap* map)
{
	if ( map != NULL )
	{
		map_empty(map);
		free(map->table);
		free(map);
	}
}

void arr_empty(xArray* arr)
{
	if ( arr != NULL )
	{
		if ( arr->table != NULL )
		{
			uint32_t i;
			for ( i=0; i<arr->size; i++ )
			{
				if ( arr->table[i] != NULL )
				{
					if ( arr->table[i]->type >= VAL_STRING )
						free(arr->table[i]->value);
					free(arr->table[i]);
				}
			}
			free(arr->table);
		}
		arr_init(arr);
	}
}

void arr_free(xArray* arr)
{
	if ( arr != NULL )
	{
		arr_empty(arr);
		free(arr->table);
		free(arr);
	}	
}


//==================== iterator ====================

void map_start(xMap* map)
{
	if ( map != NULL )
	{
		uint32_t i;
		for ( i=0; i<map->size; i++ )
		{
			if ( map->table[i] != NULL )
			{
				map->cur_index = i;
				map->cur_p = map->table[i];
				return;
			}
		}
		map->cur_p = NULL;
	}
}

bool map_hasnext(xMap* map)
{
	return (map->cur_p != NULL);
}

xPair* map_next(xMap* map)
{
	xPair* p = map->cur_p;
	if ( p != NULL )
	{
		if ( p->next != NULL )
			map->cur_p = p->next;
		else
		{
			uint32_t i;
			map->cur_p = NULL;
			for ( i=map->cur_index+1; i<map->size; i++ )
			{
				if ( map->table[i] != NULL )
				{
					map->cur_index = i;
					map->cur_p = map->table[i];
					break;
				}
			}
		}
	}
	return p;
}

xPair* map_first(xMap* map)
{
	map_start(map);
	return map_next(map);
}

uint32_t arr_size(xArray* arr)
{
	return (arr != NULL) ? arr->size : 0;
}


//==================== quick constructors ====================

xArray* arr_from_values(eValType type, void* value, ...)
{
	va_list args;
	xArray* arr;
	int curtype;

	arr = arr_create();

	curtype = type;
	va_start(args, type);
	while (curtype >= 0)
	{
		switch (curtype)
		{
			case VAL_PTR:		arr_addp	(arr, va_arg(args, void*));		break;
			case VAL_MAP:		arr_addmap	(arr, va_arg(args, xMap*));		break;
			case VAL_ARRAY:		arr_addarr	(arr, va_arg(args, xArray*));	break;
			case VAL_STRING:	arr_adds	(arr, va_arg(args, char*));		break;
			case VAL_INT:		arr_addi	(arr, va_arg(args, int));		break;
			case VAL_DOUBLE:	arr_addd	(arr, va_arg(args, double));	break;
		}
		curtype = va_arg(args, int);
	}
	va_end(args);
	return arr;
}

xMap* map_from_values(char* key, eValType type, void* value, ...)
{
	va_list args;
	xMap* map;
	char* curkey;
	eValType curtype;

	map = map_create();

	curkey = key;
	va_start(args, key);
	while ( curkey != NULL )
	{
		curtype = va_arg(args, eValType);
		switch (curtype)
		{
			case VAL_PTR:		map_setp	(map, curkey, va_arg(args, void*));		break;
			case VAL_MAP:		map_setmap	(map, curkey, va_arg(args, xMap*));		break;
			case VAL_ARRAY:		map_setarr	(map, curkey, va_arg(args, xArray*));	break;
			case VAL_STRING:	map_sets	(map, curkey, va_arg(args, char*));		break;
			case VAL_INT:		map_seti	(map, curkey, va_arg(args, int));		break;
			case VAL_DOUBLE:	map_setd	(map, curkey, va_arg(args, double));	break;
		}
		curkey = va_arg(args, char*);
	}
	va_end(args);
	return map;
}


//==================== tostring ====================

bool map_tostring(xMap* map, char* buff, size_t buffsize)
{
	if ( map != NULL && buff != NULL && buffsize > 0 )
	{
		uint32_t old_index = map->cur_index;
		xPair* old_p = map->cur_p;
		bool result;

		buff[0] = '\0';
		result = XMAP_tostring(map, buff, buffsize, 0);
		
		map->cur_index = old_index;
		map->cur_p = old_p;
		return result;
	}
	return false;
}

bool arr_tostring(xArray* arr, char* buff, size_t buffsize)
{
	if ( arr != NULL && buff != NULL && buffsize > 0 )
	{
		buff[0] = '\0';
		return XARRAY_tostring(arr, buff, buffsize, 0);
	}
	return false;
}

//================================================================
// "Private" Functions
//================================================================

bool XMAP_set(xMap* map, char* key, void* value, eValType type)
{	
	if ( map != NULL && key != NULL && strlen(key) > 0 )
	{
		xPair* p = malloc(sizeof(xPair));
		if ( p == NULL )
			return false;

		p->key = malloc((strlen(key)+1)*sizeof(char));
		if ( p->key == NULL )
		{
			free(p);
			return false;
		}
		strcpy(p->key, key);

		p->value = value;
		p->type = type;

		XMAP_insert(map, p);
		return true;
	}
	return false;
}

void XMAP_insert(xMap* map, xPair* p)
{
	uint32_t index = XMAP_hash((unsigned char*)p->key) % map->size;
	if ( map->table[index] == NULL )
	{
		p->next = NULL;
		map->table[index] = p;

		map->fill ++;
		if ( 100.0f*(float)map->fill/(float)map->size > MAP_ENLARGE_AT )
			XMAP_resize(map, map->size * MAP_RESIZE_MULT);
	}
	else
	{
		map_del(map, p->key);
		p->next = map->table[index];
		map->table[index] = p;
	}
}

bool XARRAY_set(xArray* arr, uint32_t index, void* value, eValType type)
{
	if ( arr != NULL )
	{
		if ( index >= arr->max_size )
		{
			uint32_t new_size = arr->max_size * ARR_RESIZE_MULT;
			while ( 100.0f*(float)index/(float)new_size > ARR_ENLARGE_AT )
				new_size *= ARR_RESIZE_MULT;

			if ( !XARRAY_resize(arr, new_size) )
				return false;
		}

		if ( arr->table[index] == NULL )
		{
			arr->table[index] = malloc(sizeof(xElem));
			if ( arr->table[index] == NULL )
				return false;
		}
		else if ( arr->table[index]->type >= VAL_STRING )
			free(arr->table[index]->value);

		arr->table[index]->type = type;
		arr->table[index]->value = value;

		if ( index >= arr->size )
			arr->size = index+1;

		return true;
	}
	return false;
}

bool XARRAY_insert(xArray* arr, uint32_t index)
{
	if ( arr != NULL && index <= arr->size )
	{
		if ( index < arr->size )
		{
			memmove(arr->table+index+1, arr->table+index, (arr->size - index)*sizeof(xElem*));
			arr->table[index] = NULL;
		}

		arr->size ++;
		if ( 100.0f*(float)arr->size/(float)arr->max_size > ARR_ENLARGE_AT )
			XARRAY_resize(arr, arr->max_size * ARR_RESIZE_MULT);

		return true;
	}
	return false;
}

void XMAP_resize(xMap* map, uint32_t new_size)
{
	xMap new_map;
	uint32_t i;

	if ( new_size < DEFAULT_MAP_SIZE )
		new_size = DEFAULT_MAP_SIZE;
	if ( map->size == new_size )
		return;

	new_map.table = calloc(new_size, sizeof(xPair*));
	if ( new_map.table == NULL )
		return;

	new_map.size = new_size;
	new_map.fill = 0;
	for ( i=0; i<map->size; i++ )
	{
		xPair* p = map->table[i];
		while ( p != NULL )
		{
			xPair* np = p->next;
			XMAP_insert(&new_map, p);
			p = np;
		}
	}

	free(map->table);
	map->table = new_map.table;
	map->size = new_map.size;
	map->fill = new_map.fill;
}

bool XARRAY_resize(xArray* arr, uint32_t new_size)
{
	xElem** new_table;

	if ( new_size < DEFAULT_ARR_SIZE )
		new_size = DEFAULT_ARR_SIZE;
	if ( arr->size == new_size )
		return true;

	new_table = calloc(new_size, sizeof(xElem*));
	if ( new_table == NULL )
		return false;

	memcpy(new_table, arr->table, arr->size*sizeof(xElem*));
	free(arr->table);
	arr->table = new_table;
	arr->max_size = new_size;
	return true;
}

uint32_t XMAP_hash(unsigned char *str)
{
	uint32_t hash = 5381;
	if ( str != NULL )
	{
		uint32_t i = 0;
		while ( str[i] != '\0' )
		{
			// hash *= 33;
			// hash += (char)str[i];
			hash = hash + (hash << 5) + (char)str[i];
			i++;
		}
	}
	return hash;
}

uint32_t XARRAY_sorttype(xArray* arr, eValType type)
{
	if ( arr != NULL && (type == VAL_INT || type == VAL_DOUBLE || type == VAL_STRING) )
	{
		uint32_t i;
		uint32_t j = 0;
		uint32_t num = 0;
		for ( i=0; i<arr->size; i++ )
		{
			if ( arr->table[i] == NULL || arr->table[i]->type != type )
			{
				if ( i+1 > j )
					j = i+1;
				for ( ; j<arr->size; j++ )
				{
					if ( arr->table[j] != NULL && arr->table[j]->type == type )
					{
						xElem* v = arr->table[i];
						arr->table[i] = arr->table[j];
						arr->table[j] = v;
						num = i+1;
						break;
					}
				}
				if ( num < i+1 )	// no more items
					break;
			}
			else
				num = i+1;
		}
		if ( num > 0 )
		{
			switch (type)
			{
				case VAL_INT:		qsort(arr->table, num, sizeof(xElem*), XARRAY_comp_int);		break;
				case VAL_DOUBLE:	qsort(arr->table, num, sizeof(xElem*), XARRAY_comp_double);	break;
				case VAL_STRING:	qsort(arr->table, num, sizeof(xElem*), XARRAY_comp_string);	break;
				default:	break;
			}
			return num;
		}
	}
	return 0;
}

int XARRAY_comp_int(const xElem** a, const xElem** b)
{
	return ( *(int*)( (*a)->value ) - *(int*)( (*b)->value ) );
}

int XARRAY_comp_double(const xElem** a, const xElem** b)
{
	return ( *(double*)( (*a)->value ) - *(double*)( (*b)->value ) );
}

int XARRAY_comp_string(const xElem** a, const xElem** b)
{
	return strcmp( (char*)( (*a)->value ), (char*)( (*b)->value ) );
}

bool XMAP_tostring(xMap* map, char* buff, size_t buffsize, uint32_t indent)
{
	if ( map != NULL && buff != NULL && buffsize >= 3 )
	{
		uint32_t i;
		xPair* p;

		strcat(buff, "{");
		for ( p=map_first(map); p!=NULL; p=map_next(map) )
		{
			static char tmp[255];

			if ( strlen(buff) + indent + strlen(p->key) + 7 > buffsize )
				return false;

			strcat(buff, "\n");
			for ( i=0; i<indent; i++ )
				strcat(buff, "\t");
			strcat(buff, "\t\"");
			strcat(buff, p->key);
			strcat(buff, "\" : ");

			switch ( p->type )
			{
				case VAL_PTR:		sprintf(tmp, "*p (%p)", p->value);			break;
				case VAL_MAP:
					if ( p->value == map )	// direct recursion
						sprintf(tmp, "*this");
					else if ( !XMAP_tostring((xMap*)p->value, buff+strlen(buff), buffsize-strlen(buff), indent+1) )
						return false;
					else
						tmp[0] = '\0';
					break;
				case VAL_ARRAY:
					if ( !XARRAY_tostring((xArray*)p->value, buff+strlen(buff), buffsize-strlen(buff), indent+1) )
						return false;
					tmp[0] = '\0';
					break;
				case VAL_STRING:	sprintf(tmp, "\"%s\"", (char*)(p->value));	break;
				case VAL_INT:		sprintf(tmp, "%i", *((int*)p->value));		break;
				case VAL_DOUBLE:	sprintf(tmp, "%f", *((double*)p->value));	break;
			}

			if ( strlen(buff) + strlen(tmp) + 1 > buffsize )
				return false;
			strcat(buff, tmp);
			strcat(buff, ",");
		}
		if ( strlen(buff) > 1 )
		{
			if ( strlen(buff) + 1 + indent > buffsize )
				return false;
			strcat(buff, "\n");
			for ( i=0; i<indent; i++ )
				strcat(buff, "\t");
		}
		if ( strlen(buff) + 1 > buffsize )
			return false;
		strcat(buff, "}");
		return true;
	}
	return false;
}

bool XARRAY_tostring(xArray* arr, char* buff, size_t buffsize, uint32_t indent)
{
	if ( arr != NULL && buff != NULL && buffsize > 3 )
	{
		uint32_t i;
		uint32_t j;
		xElem* v;

		strcat(buff, "[");
		for ( j=0; j<arr->size; j++ )
		{
			static char tmp[255];

			if ( strlen(buff) + indent + 2 > buffsize )
				return false;
			strcat(buff, "\n");
			for ( i=0; i<indent; i++ )
				strcat(buff, "\t");
			strcat(buff, "\t");

			v = arr->table[j];
			if ( v == NULL )
				strcpy(tmp, "N/A");
			else
			{
				switch ( v->type )
				{
					case VAL_PTR:		sprintf(tmp, "*p (%p)", v->value);			break;
					case VAL_MAP:
						if ( !XMAP_tostring((xMap*)v->value, buff+strlen(buff), buffsize-strlen(buff), indent+1) )
							return false;
						tmp[0] = '\0';
						break;
					case VAL_ARRAY:
						if ( v->value == arr )	// direct recursion
							sprintf(tmp, "*this");
						else if ( !XARRAY_tostring((xArray*)v->value, buff+strlen(buff), buffsize-strlen(buff), indent+1) )
							return false;
						else
							tmp[0] = '0';
						break;
					case VAL_STRING:	sprintf(tmp, "\"%s\"", (char*)(v->value));	break;
					case VAL_INT:		sprintf(tmp, "%i", *((int*)v->value));		break;
					case VAL_DOUBLE:	sprintf(tmp, "%f", *((double*)v->value));	break;
				}
			}
			if ( strlen(buff) + strlen(tmp) + 3 + indent > buffsize )
				return false;
			strcat(buff, tmp);
			strcat(buff, ",");
		}
		if ( strlen(buff) > 1 )
		{
			if ( strlen(buff) + 1 + indent > buffsize )
				return false;
			strcat(buff, "\n");
			for ( i=0; i<indent; i++ )
				strcat(buff, "\t");
		}
		if ( strlen(buff) + 1 > buffsize )
			return false;
		strcat(buff, "]");
		return true;
	}
	return false;
}

#endif

