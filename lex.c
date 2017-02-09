#include <ctype.h>
#include <string.h>

#include "utils/defs.h"
#include "utils/ip.c"
#include "utils/utils.c"

char lexeme[MAX_LEXEME] ;
int lexeme_length = 0 ;

bool is_id(char* s)
{
	if(s == NULL || (!isalpha(*s) &&  *s != '_'))
		return false ;
	while(*s != 0)
	{
		if(!isalpha(*s) && !isdigit(*s) && *s != '_')
			return false ;
		s++ ;
	}
	return true ;
}

bool is_num(char* s)
{
	if(s == NULL || (!isdigit(*s) && *s != '.'))
		return false ;
	while(*s != 0 && *s != '.')
	{
		if(!isdigit(*s))
			return false ;
		s++ ;
	}
	if(*s == '.')
		s++ ;
	while(*s != 0)
	{
		if(!isdigit(*s))
			return false ;
		s++ ;
	}
	return true ;
}

int is_keyword(char* s)
{
	int i = 0 ;
	for(i = 0 ; i < NUM_KEYWORDS ; i++)
	{
		if(strcmp_i(s , keyword_values[i]))
			return i ;
	}
	return -1 ;
}

token* create_token(int type)
{
	token* r = (token*) malloc(sizeof (token)) ;
	r -> type = type ;
	if(type < NUM_KEYWORDS)
		r -> value = keyword_values[type] ;
	else
		r -> value = clone(lexeme , lexeme_length) ;
	return r ;
}

bool blank(char c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r' ;
}

token* scan()
{
	static int a = 1 ;
	a++ ;
	lexeme_length = 0 ;
	while(blank(cur))
	{
		if(cur == '\n')
		{
			line_no++ ;
			ch_read = 0 ;
		}
		read_char() ;
	}
	char temp ;
	switch(cur)
	{
		case EOF :
			return NULL ;
		case '+' :
		case '*' :
		case '/' :
		case ',' :
		case ';' :
		case '(' :
		case ')' :		
		case '-' :
		case '\'' :
			lexeme[0] = cur ;
			lexeme[1] = '\0' ;
			lexeme_length = 1 ;
			read_char() ;
			break ;
		case '<' :
		case '>' :
		case '=' :
		case '!' :
			temp = cur ;
			lexeme[0] = temp ;
			if(read_this('='))
			{
				lexeme[1] = '=' ;
				lexeme[2] = '\0' ;
				lexeme_length = 2 ;
				read_char() ;
			}
			else
			{
				lexeme[1] = '\0' ;
				lexeme_length = 1 ;
				read_char() ;
			}
			break ;
		case '.' :
			lexeme[0] = cur ;
			lexeme[1] = '\0' ;
			lexeme_length = 1 ;
			read_char() ;
			if(!isdigit(cur))
				break ;
		default :
			if(isdigit(cur) || cur == '.')
			{
				while(isalpha(cur) || isdigit(cur) || cur == '.')
				{
					lexeme[lexeme_length++] = cur ;
					read_char() ;
				}
			}
			else
			{
				if(isalpha(cur) || cur == '_')
				{
					while(isalpha(cur) || isdigit(cur) || cur == '_')
					{
						lexeme[lexeme_length++] = cur ;
						read_char() ;
					}
				}
			}
	}
	lexeme[lexeme_length] = 0 ;
	// printf("%s\n", lexeme);
	int ret = is_keyword(lexeme) ;
	if(ret != -1)
	{
		return create_token(ret) ;
	}
	if(is_id(lexeme))
	{
		return create_token(ID) ;
	}
	else if(is_num(lexeme))
	{
		return create_token(NUM) ;
	}
	else
	{
		if(lexeme_length != 0)
			printf("Unrecognized token '%s' on line no %d and colno %d \n", lexeme , line_no , ch_read - lexeme_length);
		else
			printf("Unrecognized token '%c' on line no %d and colno %d \n", cur , line_no , ch_read - 1);
		return NULL ;
	}
}

char* get_type(int type)
{
	if(type < NUM_OPERATORS)
		return operator_names[type] ;
	else if(type >= NUM_KEYWORDS)
		return keyword_values[type] ;
	else
		return "Keyword" ;
}

void print_token(token* i)
{
	printf("%s %s\n", get_type(i -> type) , i -> value);
}