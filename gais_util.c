#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "gais_util.h"
void get_field(char *rec_start, char *rec_end, char *field, char *field_temp)
{
	char pattern[1000];
	char *field_start,*field_end;
	memset(pattern,0,1000);
	pattern[0] = '\n';
	pattern[1] = '@';
	strcat(pattern,field);
	strcat(pattern,":");
	field_start = memmem(rec_start,rec_end-rec_start,pattern,strlen(pattern))+strlen(pattern);
	if(field_start == NULL)
	{
		field_temp[0] = '\0';
		return;
	}
	field_end = memmem(field_start,rec_end-field_start,"\n@",2)+1;
	strncpy(field_temp,field_start,field_end-field_start);
	field_temp[field_end-field_start] = '\0';
}

