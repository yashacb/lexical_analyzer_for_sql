#include <stdio.h>
#include <stdlib.h>

#include "lex.c"

int main()
{
	while(1)
	{
		token* t = scan() ;
		if(t == NULL)
			break ;
		print_token(t) ;
	}
	return 0 ;
}