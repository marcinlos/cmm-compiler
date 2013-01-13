
%{
#include <stdio.h>
#include "ast.h"
#include "parser.tab.h"

#define MAX_STRING 4096
#define BUFFER_SIZE 256

void print_error(const char* msg) 
{
    fprintf(stderr, "(%d, %d): %s\n", yylloc.first_line, 
        yylloc.first_column, msg);
}

char string_buffer[MAX_STRING];

void append_char(char c)
{
    char* b = string_buffer;
    while (*b) ++ b;
    *b = c;
}

char escape_seq(const char* c)
{
    char next;
    switch (*c)
    {
    case '\n': next = '\n'; break;
    case '\t': next = '\t'; break;
    case '\r': next = '\r'; break;
    case '0': next = '\0' ; break;
    case '"': next = '\"' ; break;
    case '\\': next = '\\'; break;
    default:
        {   
            char msg[BUFFER_SIZE];
            sprintf(msg, "Unknown escape sequence: `\\%s'", c);
            print_error(msg);
            return -1;
        }
    }
}

void append_escaped(const char* c)
{
    char next = escape_seq(c);
    if (next != -1)
        append_char(next);   
}

int nested_comments = 0;

static void update_loc(const char* text)
{
    yylloc.first_line = yylloc.last_line;
    yylloc.first_column = yylloc.last_column;
    char c;
    while (c = *text++)
    {
        if (c == '\n') 
        {
            ++ yylloc.last_line;
            yylloc.last_column = 0;
        }
        ++ yylloc.last_column;
    }
}

#define YY_USER_ACTION update_loc(yytext);

%}

%option noyywrap
%option stack

DIGIT           [0-9]

%x S_COMMENT S_STR

%%


"char"                  yylval.type = TYPE_CHAR; return CHAR_K;
"int"                   yylval.type = TYPE_INT; return INT_K;
"void"                  yylval.type = TYPE_VOID; return VOID_K;

"if"                    return IF_K;
"else"                  return ELSE_K;
"for"                   return FOR_K;
"while"                 return WHILE_K;
"extern"                return EXTERN_K;
"return"                return RETURN_K;


"=="                    return OP_EQ;
"!="                    return OP_NEQ;
"<="                    return OP_LTE;
">="                    return OP_GTE;
"&&"                    return OP_AND;
"||"                    return OP_OR;

[\[\](){};/\-+*<>,!=]     return yytext[0];

<*>"/*"     {
                if (++ nested_comments == 1)
                yy_push_state(S_COMMENT);
            }


<S_COMMENT>([^*\/]|\*[^/]|\/[^*])* ;

<S_COMMENT>"*/"     {
                        if (-- nested_comments == 0) 
                            yy_pop_state();
                    }

<S_COMMENT><<EOF>>  {
                        print_error("Unterminated comment");
                        return 0;
                    }

[a-zA-Z_][a-zA-Z0-9_]*      yylval.string = strdup(yytext); return ID;

{DIGIT}+                    yylval.integer = atoi(yytext); return NUM;

\'[[:print:]]\'     {
                        yylval.character = *yytext; 
                        return CHAR;
                    }
                    
\'\\[n0]\'          {
                        yylval.character = escape_seq(yytext + 1); 
                        return CHAR;
                    }
                    
\"              yy_push_state(S_STR);

<S_STR>\"   {
                yy_pop_state();
                yylval.string = strdup(string_buffer);
                strcpy(string_buffer, "");
                return STRING;
            }

<S_STR>\\.      append_escaped(yytext + 1);

<S_STR>.        append_char(*yytext);

<S_STR>\n   {
                print_error("Unexpected EOL inside string literal");
                yy_pop_state();
            }

<S_STR><<EOF>>  {
                    print_error("Unexpected EOF inside string");
                    return 0;
                }


[ \t\n] ;


<*>.    { 
            char msg[BUFFER_SIZE];
            sprintf(msg, "Unexpected character: `%s'", yytext);
            print_error(msg);
        }


%%


