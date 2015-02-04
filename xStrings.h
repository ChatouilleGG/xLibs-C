
#ifndef __X_STRINGS__
#define __X_STRINGS__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/* xString API :
 *
 *	bool		str_init	( xString*,  char* )
 *	xString*	str_create	( char* )
 *
 *	xString*	str_format		( char* fmt,  ... )
 *	xString*	str_vformat		( char* fmt,  va_list )
 *	bool		str_reformat	( xString*,  char* fmt,  ... )
 *
 *	int			str_cmp		( xString*,  xString* )
 *	int			str_icmp	( xString*,  xString* )
 *	bool		str_eq		( xString*,  xString* )
 *	bool		str_ieq		( xString*,  xString* )
 *
 *	xString*	str_sub		( xString*,  int begin,  int end )
 *	xString*	str_left	( xString*,  int end )
 *	xString*	str_tail	( xString*,  int begin )
 *	xString*	str_right	( xString*,  int offset )
 *
 *	xString*	quick_str		( char* )
 *	xString*	quick_format	( char* fmt,  ... )
 *	xString*	quick_vformat	( char* fmt,  va_list )
 *	xString*	quick_sub		( xString*,  int begin,  int end )
 *	xString*	quick_left		( xString*,  int end )
 *	xString*	quick_tail		( xString*,  int begin )
 *	xString*	quick_right		( xString*,  int offset	)
 *
 *	int		str_findc		( xString*,  char* )
 *	int		str_finds		( xString*,  char* )
 *	int		str_findstr		( xString*,  xString* )
 *
 *	void	str_free		( xString* )
 *	void	XSTRING_CLEANUP	( void )
 *
 */


//================================================================
// Definitions
//================================================================

#define STR_MIN_MEMSIZE 6
#define STR_MEM_FILL 1.5
#define STR_SHRINK_AT 0.16

#ifndef _BOOL
	#define _BOOL
	typedef enum { false, true } bool;
#endif

typedef struct _xString xString;
struct _xString
{
	char* chars;
	int len;

	int memsize;
};


typedef struct _xTrashElem xTrashElem;
struct _xTrashElem
{
	xString* str;
	xTrashElem* next;
};
xTrashElem* str_trash = NULL;


void XSTRING_TRASH(xString*);
void XSTRING_resize(xString*, int);


//================================================================
// Public API
//================================================================

//==================== init ====================

bool str_init(xString* str, char* chars)
{
	if ( str != NULL )
	{
		if ( chars == NULL ) chars = "";

		str->len = strlen(chars);
		str->memsize = max( floor( (float)(str->len+1) * STR_MEM_FILL ), STR_MIN_MEMSIZE);
		str->chars = malloc(str->memsize * sizeof(char));
		if ( str->chars != NULL )
		{
			str->chars[0] = '\0';
			strcat(str->chars, chars);
			return true;
		}
		str->len = -1;
	}
	return false;
}

xString* str_create(char* chars)
{
	xString* str;
	str = malloc(sizeof(xString));
	if ( str != NULL )
	{
		if ( str_init(str, chars) )
			return str;
		free(str);
	}
	return NULL;
}


//==================== format ====================

xString* str_vformat(char* format, va_list args)
{
	xString* str;
	int newlen;

	if ( format == NULL ) return NULL;

	str = str_create("");
	if ( str != NULL )
	{
		newlen = c99_vsnprintf(str->chars, str->memsize, format, args);
		if ( newlen >= str->memsize )
		{
			XSTRING_resize(str, floor((float)(newlen+1) * STR_MEM_FILL) );
			vsprintf(str->chars, format, args);
		}
	}
	return str;
}

xString* str_format(char* format, ...)
{
	xString* res;
	va_list args;
	va_start(args, format);
	res = str_vformat(format, args);
	va_end(args);
	return res;
}

bool str_reformat(xString* str, char* format, ...)
{
	va_list args;
	xString* res;

	if ( str == NULL ) return false;

	va_start(args, format);
	res = str_vformat(format, args);
	va_end(args);

	free(str->chars);
	str->chars = res->chars;
	str->len = res->len;
	str->memsize = res->memsize;
	free(res);

	return true;
}


//==================== equality ====================

int str_cmp(xString* s1, xString* s2)
{
	if ( s1 != NULL && s2 != NULL )
		return strcmp(s1->chars, s2->chars);
	return s1-s2;
}

int str_icmp(xString* s1, xString* s2)
{
	char* a;
	char* b;
	if ( s1 != NULL && s2 != NULL )
	{
		int d;
		a = s1->chars;
		b = s2->chars;
		for (;; a++, b++)
		{
			d = (int)tolower(*a) - (int)tolower(*b);
			if ( d != 0 || !*a )
				return d;
		}
	}
	return s1-s2;
}

bool str_eq(xString* s1, xString* s2)
{
	return (s1 != NULL && s2 != NULL && strcmp(s1->chars,s2->chars) == 0);
}

bool str_ieq(xString* s1, xString* s2)
{
	return (s1 != NULL && s2 != NULL && str_icmp(s1,s2) == 0);
}


//==================== substring ====================

xString* str_sub(xString* str, int begin, int end)
{
	xString* res;

	res = NULL;
	if ( str != NULL && begin >= 0 && end >= begin )
	{
		if ( begin >= str->len )
			begin = str->len;
		if ( end >= str->len )
			end = str->len-1;

		res = malloc(sizeof(xString));
		if ( res != NULL )
		{
			res->len = (end-begin)+1;
			res->memsize = max( floor( (float)(res->len+1) * STR_MEM_FILL ), STR_MIN_MEMSIZE);
			res->chars = malloc(res->memsize * sizeof(char));
			if ( res->chars != NULL )
			{
				if ( res->len > 0 )
					strncpy(res->chars, str->chars+begin, res->len);
				res->chars[res->len] = '\0';
			}
			else
			{
				free(res);
				res = NULL;
			}
		}
	}
	return res;
}

xString* str_left(xString* str, int end)
{
	return str_sub(str, 0, end);
}

xString* str_tail(xString* str, int begin)
{
	return str_sub(str, begin, str->len-1);
}

xString* str_right(xString* str, int offset)
{
	return str_sub(str, str->len - offset, str->len-1);
}


//==================== quick-use ====================

xString* quick_str(char* chars)
{
	xString* res = str_create(chars);
	XSTRING_TRASH(res);
	return res;
}

xString* quick_vformat(char* format, va_list args)
{
	xString* res;
	res = str_vformat(format, args);
	XSTRING_TRASH(res);
	return res;
}

xString* quick_format(char* format, ...)
{
	xString* res;
	va_list args;
	va_start(args, format);
	res = str_vformat(format, args);
	va_end(args);
	XSTRING_TRASH(res);
	return res;
}

xString* quick_sub(xString* str, int begin, int end)
{
	xString* res = str_sub(str, begin, end);
	XSTRING_TRASH(res);
	return res;
}

xString* quick_left(xString* str, int end)
{
	xString* res = str_left(str, end);
	XSTRING_TRASH(res);
	return res;
}

xString* quick_tail(xString* str, int begin)
{
	xString* res = str_tail(str, begin);
	XSTRING_TRASH(res);
	return res;
}

xString* quick_right(xString* str, int offset)
{
	xString* res = str_right(str, offset);
	XSTRING_TRASH(res);
	return res;
}


//==================== find ====================

int str_findc(xString* str1, char* chars)
{
	int span;
	if ( str1 != NULL && chars != NULL )
	{
		span = strcspn(str1->chars, chars);
		if ( span < str1->len )
			return span;
	}
	return -1;
}

int str_finds(xString* str1, char* str2)
{
	if ( str1 != NULL && str2 != NULL )
	{
		char* p = strstr(str1->chars, str2);
		if ( p != NULL )
			return (int)((long)p - (long)str1->chars);
	}
	return -1;
}

int str_findstr(xString* str1, xString* str2)
{
	return (str2 != NULL) ? str_finds(str1, str2->chars) : -1;
}


//==================== free ====================

void str_free(xString* str)
{
	if ( str != NULL )
	{
		free(str->chars);
		free(str);
	}
}

void XSTRING_CLEANUP(void)
{
	xTrashElem* elem;
	while ( str_trash != NULL )
	{
		elem = str_trash->next;
		free(str_trash->str);
		free(str_trash);
		str_trash = elem;
	}
}


//==================== private ====================

void XSTRING_TRASH(xString* str)
{
	xTrashElem* elem;
	elem = malloc(sizeof(xTrashElem));
	if ( elem == NULL ) return;
	elem->str = str;
	elem->next = str_trash;
	str_trash = elem;
}

void XSTRING_resize(xString* str, int newsize)
{
	char* newmem;

	if ( str == NULL ) return;
	if ( str->len >= newsize )
		newsize = floor( (float)(str->len+1) * STR_MEM_FILL );

	newmem = malloc(newsize * sizeof(char));
	if ( newmem == NULL ) return;

	newmem[0] = '\0';
	strcat(newmem, str->chars);
	free(str->chars);
	str->chars = newmem;
}

int c99_vsnprintf(char* str, size_t size, const char* format, va_list ap)
{
    int count = -1;

    if (size != 0)
        count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
    if (count == -1)
        count = _vscprintf(format, ap);

    return count;
}


#endif
