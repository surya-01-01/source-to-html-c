#include"s2html.h"
#include<stdio.h>
#include<string.h>
#include<ctype.h>

pevent_t pevent_data;

static const char operators[] = {'/', '+', '*', '-', '%', '=', '<', '>', '~', '&', ',', '!', '^', '|'};
static char symbols[] = {'(', ')', '{', '[', ':'};

static char* res_kwords_data[] = {"const", "volatile", "extern", "auto", "register",
    "static", "signed", "unsigned", "short", "long", 
 "double", "char", "int", "float", "struct", 
 "union", "enum", "void", "typedef", ""
};
static char* res_kwords_non_data[] = {"goto", "return", "continue", "break", 
    "if", "else", "for", "while", "do", 
    "switch", "case", "default","sizeof", ""
   };
/* start_or_end_conv function definitation */
void html_begin(FILE* dest_fp, int type) /* type => not used, but can be used to add differnet HTML tags */
{
	/* Add HTML begining tags into destination file */
	fprintf(dest_fp, "<!DOCTYPE html>\n");
	fprintf(dest_fp, "<html lang=\"en-US\">\n");
	fprintf(dest_fp, "<head>\n");
	fprintf(dest_fp, "<title>%s</title>\n", "sode2html");
	fprintf(dest_fp, "<meta charset=\"UTF-8\">\n");
	fprintf(dest_fp, "<link rel=\"stylesheet\" href=\"styles.css\">\n");
	fprintf(dest_fp, "</head>\n");
	fprintf(dest_fp, "<body style=\"background-color:lightgrey;\">\n");
	fprintf(dest_fp, "<pre>\n");
}
void html_end(FILE* dest_fp, int type) /* type => not used, but can be used to add differnet HTML tags */
{
	/* Add HTML closing tags into destination file */
	fprintf(dest_fp, "</pre>\n");
	fprintf(dest_fp, "</body>\n");
	fprintf(dest_fp, "</html>\n");
}


/* sourc_to_html function definitation */
void source_to_html(FILE* fp, pevent_t *event)
{
#ifdef DEBUG
	printf("%s", event->data);
#endif
	switch(event->type)
	{
		case PEVENT_PREPROCESSOR_DIRECTIVE:
			fprintf(fp,"<span class=\"preprocess_dir\">%s</span>",event->data);
			break;
		case PEVENT_MULTI_LINE_COMMENT:
		case PEVENT_SINGLE_LINE_COMMENT:
			fprintf(fp,"<span class=\"comment\">%s</span>",event->data);
			break;
		case PEVENT_STRING:
			fprintf(fp,"<span class=\"string\">%s</span>",event->data);
			break;
		case PEVENT_HEADER_FILE:
			/* write to html */
			if(event->property == USER_HEADER_FILE)
				fprintf(fp,"<span class=\"header_file\">%s</span>",event->data);
			else
				fprintf(fp,"<span class=\"header_file\">&lt;%s&gt;</span>",event->data);
			break;
		case PEVENT_REGULAR_EXP:
		case PEVENT_EOF :
			fprintf(fp,"%s",event->data);
			break;
		case PEVENT_NUMERIC_CONSTANT:
			fprintf(fp,"<span class=\"numeric_constant\">%s</span>",event->data);
			break;
		case PEVENT_RESERVE_KEYWORD:
			if(event->property == RES_KEYWORD_DATA)
			{
				fprintf(fp,"<span class=\"reserved_key1\">%s</span>",event->data);
			}
			else
			{
				fprintf(fp,"<span class=\"reserved_key2\">%s</span>",event->data);
			}
			break;
		case PEVENT_ASCII_CHAR:
			fprintf(fp,"<span class=\"ascii_char\">%s</span>",event->data);
			break;
		default :
			printf("Unknown event\n");
			break;
	}
}

pevent_t *get_parser_event(FILE *fp)
{
    int i=0;
    char ch;
    while (fscanf(fp, "%c", &ch) == 1)
    {
        if(ch=='#')
        {
            pevent_data.type=PEVENT_PREPROCESSOR_DIRECTIVE;
            while(ch!='\n')
            {
                pevent_data.data[i++]=ch;
                fscanf(fp, "%c", &ch);
            }
            pevent_data.data[i]='\0';
            pevent_data.length=i;
            i=0;
            return &pevent_data;
        }
        else if(ch=='/')
        {
            char t_ch;
            fscanf(fp, "%c", &t_ch);
            if (t_ch == '/')
            {
                pevent_data.type=PEVENT_SINGLE_LINE_COMMENT;
                pevent_data.data[i++] = '/';
                pevent_data.data[i++] ='/';
                while (ch != '\n')
                {
                    pevent_data.data[i++] = ch;
                    fscanf(fp, "%c", &ch);   
                }
                pevent_data.data[i]='\0';
                printf("SINGLE LINE COMMENTS-->%s\n",pevent_data.data);
                pevent_data.length=i;
                i=0;
                return &pevent_data;
            }
            else if(t_ch=='*')
            {
                pevent_data.data[i++] = '/';
                pevent_data.data[i++] = '*';
                ch=t_ch;
                while (1) {
                    ch = fgetc(fp);
                    pevent_data.data[i++] = ch;
                    if (ch == '*' && (ch = fgetc(fp)) == '/') {
                        pevent_data.data[i++] = ch;
                        break;
                    } else {
                        ungetc(ch, fp);
                    }
                }
                while (ch != '*' && ((ch = fgetc(fp)) == '/')) 
                {
                    pevent_data.data[i++] = ch;
                    fscanf(fp, "%c", &ch);   
                }
                pevent_data.data[i++] = '/';
                pevent_data.data[i]='\0';
                pevent_data.type=PEVENT_MULTI_LINE_COMMENT;
                printf("MULTI LINE COMMENTS-->%s\n",pevent_data.data);
                i=0;
                return &pevent_data;
            }
            else
            {
                fseek(fp,-1,SEEK_CUR);
            }
        }

        if(delim(ch))
        {
            if(i!=0)
            {
                pevent_data.data[i]='\0';
                if(isKeyword(pevent_data.data))
                {
                    printf("KEYWORD-->%s\n",pevent_data.data);
                    pevent_data.type=PEVENT_RESERVE_KEYWORD;
                    return &pevent_data;
                }
                else if(isIdentifier(pevent_data.data))
                {
                    printf("IDENTIFIER-->%s\n",pevent_data.data);
                    pevent_data.type=PEVENT_REGULAR_EXP;
                    return &pevent_data;
                }
                else if(isConstant(pevent_data.data))
                {
                    printf("CONSTANT-->%s\n",pevent_data.data);
                    pevent_data.type=PEVENT_NUMERIC_CONSTANT;
                    return &pevent_data;
                }
                else
                {
                    printf("UNIDENTIFIED->%s\n",pevent_data.data);
                    pevent_data.type=PEVENT_REGULAR_EXP;
                    return &pevent_data;
                }
                
            }
            if(isOperator(ch))
            {
                printf("OPERATOR-->%c\n",ch);
                pevent_data.type=PEVENT_REGULAR_EXP;
                pevent_data.data[0]=ch;
                pevent_data.data[1]='\0';
                    return &pevent_data;
            }
            else if(isSpecialCharacter(ch))
            {
                printf("SPECIAL CHARACTER-->%c\n",ch);
                pevent_data.type=PEVENT_REGULAR_EXP;
                pevent_data.data[0]=ch;
                pevent_data.data[1]='\0';
                    return &pevent_data;
            }
            else if(ch==' ' || ch=='\n')
            {
                pevent_data.type=PEVENT_REGULAR_EXP;
                pevent_data.data[0]=ch;
                pevent_data.data[1]='\0';
                return &pevent_data;
            }
            i=0;
        }
        else
        {
            pevent_data.data[i++]=ch;
        }
    }
    pevent_data.type=PEVENT_EOF;
    return &pevent_data;
}

int delim(char ch)
{
    if(ch == ' ' || ch == '+' || ch == '-'|| ch == '*' || ch == '/' || ch == ','|| ch=='"'
        || ch == ';' || ch == '%' || ch == '>'|| ch == '<' || ch == '=' || ch == '('
        || ch == ')' || ch == '[' || ch == ']'|| ch == '{' || ch == '}' || ch=='\n' || ch=='&')
        {
            return 1;
        }
        else
        { 
            return 0;
        }
}

int isOperator(const char ch)
{
    int i=0;
    while(operators[i])
    {
        if(ch==operators[i])
        {
            return 1;
        }
        i++;
    }
    return 0;
}

int isKeyword(const char* str)
{
    for (int i = 0;i < sizeof(res_kwords_data) / sizeof(res_kwords_data[0]); i++) {
       if (strcmp(str, res_kwords_data[i]) == 0) {
           return 1;
       }
   }
   return 0;
}

int isIdentifier(const char* str)
{
    if(str[0]>='0' && str[0]<='9')
        return 0;
    return 1;
}

int isConstant(const char* str)
{
    int i=0;
    while (str[i])
    {
        if(isdigit(str[i]))
            i++;
        else
            return 0;
    }
    return 1;
}

int isSpecialCharacter(char ch)
{
    int i=0;
    while(symbols[i])
    {
        if(ch==symbols[i])
        {
            return 1;
        }
        i++;
    }
    return 0;
}

