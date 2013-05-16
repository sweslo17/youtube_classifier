#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "youtube_util.h"
void sep_init()
{
	strcpy(quot_start,"\'\"{[(");
	strcpy(quot_end,"\'\"}])");
	strcpy(sep,":-|");
}
int issep(char input)
{
	int i;
	for(i=0;i<strlen(sep);i++)
	{
		if(input == sep[i])
		{
			return 1;
		}
	}
	return 0;
}
int isquot_start(char input)
{
	int i;
	for(i=0;i<strlen(quot_start);i++)
	{
		if(input == quot_start[i])
		{
			return i;
		}
	}
	return -1;
}
int isquot_end(char input)
{
	int i;
	for(i=0;i<strlen(quot_end);i++)
	{
		if(input == quot_end[i])
		{
			return i;
		}
	}
	return -1;
}
int iscut(char input)
{
	if(isspace(input)!=0||isquot_start(input)!=-1||isquot_end(input)!=-1||issep(input)!=0)
	{
		return 1;
	}
	return 0;
}

