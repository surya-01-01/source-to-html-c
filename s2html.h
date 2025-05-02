#ifndef S2html_H
#define S2html_H

/* constants */

#define HTML_OPEN	1
#define HTML_CLOSE	0
#define PEVENT_DATA_SIZE	1024
#define USER_HEADER_FILE		1
#define STD_HEADER_FILE			2
#define RES_KEYWORD_DATA		3
#define RES_KEYWORD_NON_DATA	4

/********** function prototypes **********/




typedef enum
{
	PEVENT_NULL,
	PEVENT_PREPROCESSOR_DIRECTIVE,
	PEVENT_RESERVE_KEYWORD,
	PEVENT_NUMERIC_CONSTANT,
	PEVENT_STRING,
	PEVENT_HEADER_FILE,
	PEVENT_REGULAR_EXP,
	PEVENT_SINGLE_LINE_COMMENT,
	PEVENT_MULTI_LINE_COMMENT,
	PEVENT_ASCII_CHAR,
	PEVENT_EOF
}pevent_e;

typedef struct
{
	pevent_e type; // event type
	int property; // property associated with data
	int length; // data length
	char data[PEVENT_DATA_SIZE]; // cwparsed string
}pevent_t;

int delim(char ch);
int isOperator(const char ch);
int isKeyword(const char* str);
int isIdentifier(const char* str);
int isConstant(const char* str);
int isSpecialCharacter(char ch);

void html_begin(FILE* dest_fp, int type); /* type => not used, but can be used to add differnet HTML tags */
void html_end(FILE* dest_fp, int type); /* type => not used, but can be used to add differnet HTML tags */
void source_to_html(FILE* fp,pevent_t *event);
pevent_t *get_parser_event(FILE *fp);

#endif