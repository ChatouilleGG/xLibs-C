
#include <stdio.h>

#include "xStrings.h"

int main(void)
{
	xString* str;
	int i,j;

	str = str_create("bonjour");
	printf("str: %s\n\n", str->chars);

	str_reformat(str, "%s %s %s", "gauche", str->chars, "droite");
	printf("str: %s\n\n", str->chars);

	printf(quick_format("freeing str, bye %p\n\n", str)->chars);
	str_free(str);

	str = str_create("pouet(brouf, zor);");
	printf("str: %s\n\n", str->chars);

	i = str_findc(str, "(");
	printf("index of '(': %i\n", i);
	printf("left: %s\n", quick_left(str, i)->chars);
	printf("right: %s\n\n", quick_tail(str, i)->chars);

	j = str_findc(str, ")");
	printf("index of ')': %i\n", j);
	printf("left: %s\n", quick_left(str, j)->chars);
	printf("right: %s\n\n", quick_tail(str, j)->chars);

	printf("grab: %s\n\n", quick_sub(str, i+1, j-1)->chars);

	XSTRING_CLEANUP();

	return 0;
}
