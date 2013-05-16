/**
 * ./classify class_trans category.map rec_file
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "gais_util.h"
#include "youtube_util.h"
#ifdef DEBUG
#define DEBUGP(x, args ...) fprintf(stderr, " [%s(), %s:%u]\n" \
		x, __FUNCTION__, __FILE__,__LINE__, ## args)
#else
#define DEBUGP(x, args ...)
#endif
struct map
{
	char *term;
	int key;
};
struct cat_map
{
	char *term;
	int key[100];
};
int score_field(char *,char*,struct map *,int,float *);
int map_cmp(const void *,const void *);
//char *get_class()
int main(int argc, char *argv[])
{
	FILE *class_file,*rec_file,*output_file,*cat_file;

	int buffer_size = 100000000;
	char *temp = (char *)malloc(sizeof(char)*buffer_size+3);
	char class[1000][100];
	char field_temp[100000];
	char *class_str;
	int i=0,j=0,k=0,class_index=0,length=0,rec_left=0;
	int map_size=0,match_flag=0;
	float score_tbl[1000];
	float score=0;
	char *rec_start=NULL,*rec_end=NULL;
	struct map *map = (struct map *)malloc(10000*sizeof(struct map)+1);
	struct cat_map *cat_map = (struct cat_map *)malloc(10000*sizeof(struct cat_map)+1);
	class_file = fopen(argv[1],"rb");
	cat_file = fopen(argv[2],"rb");
	rec_file = fopen(argv[3],"rb");
	/*output_file = fopen(argv[3],"w");*/
	i=0;
	j=0;
	/*while(fgets(temp,100000,cat_file)!=NULL)
	{
		if(temp[strlen(temp)-1] == '\n')
		{
			temp[strlen(temp)-1] = '\0';
		}
		if(temp[0] != '\t')
		{
			cat_map[i-1].key[j] = -1;
			cat_map[i].term = (char *)malloc(sizeof(char)*strlen(temp)+1);
			strcpy(cat_map[i].term,temp);
			i++;
			j=0;
		}
		else
		{
			cat_map[i-1].key[j] = atoi(&temp[1]);
			j++;
		}
	}*/
	while(fgets(temp,100000,class_file)!=NULL)
	{
		if(temp[strlen(temp)-1] == '\n')
		{
			temp[strlen(temp)-1] = '\0';
		}
		if(temp[0] != '\t')
		{
			strcpy(class[class_index],temp);
			class_index++;
		}
		else
		{
			map[i].term = (char *)malloc(sizeof(char)*strlen(temp)+1);
			strcpy(map[i].term,&temp[1]);
				map[i].key = class_index-1;
			i++;
		}
	}
	qsort(map,i,sizeof(struct map),map_cmp);
	map_size = i;
	rec_start = temp;
	rec_left = 0;
	sep_init();
	while((length = fread(temp+rec_left,sizeof(char),buffer_size-rec_left,rec_file))!=0)
	{
		rec_start = temp;
		while(rec_start<temp+length+rec_left)
		{
			rec_end = memmem(rec_start+2,buffer_size-(rec_start-temp),"\n@\n@id:",7);
			//find term
			if(rec_end!=NULL)
			{
				rec_end++;
			}
			else
			{
				if(length == buffer_size-rec_left)
				{
					//file not end
					//k+=3000;
					rec_left = buffer_size-(rec_start-temp);
					//rec_left = strlen(rec_start);
					memcpy(temp,rec_start,rec_left);
					//rec_left = strlen(rec_start);
					break;
				}
				else
				{
					//file reach end
					//k+=length;
					//printf("error:%d\t%d\n",feof(rec_file),k);
					rec_end = temp+length+rec_left;
				}
			}
			//count score start
			memset(score_tbl,0,sizeof(score_tbl));
			match_flag = 0;
			class_str = NULL;
			get_field(rec_start,rec_end,"title",field_temp);
			match_flag += score_field(field_temp,"title",map,map_size,score_tbl);
			get_field(rec_start,rec_end,"keyword",field_temp);
			match_flag += score_field(field_temp,"keyword",map,map_size,score_tbl);
			get_field(rec_start,rec_end,"content",field_temp);
			match_flag += score_field(field_temp,"content",map,map_size,score_tbl);
			/*if(match_flag != 0)
			{
				for(i=0;rec_start+i<rec_end;i++)
				{
					putchar(*(rec_start+i));
				}
				class_str = get_class(class,score_tbl);
				if(class_str!=NULL)
				{
					printf("@class:%s\n",class_str);
				}
			}*/
			if(match_flag != 0)
			{
				for(i=0;rec_start+i<rec_end;i++)
				{
					putchar(*(rec_start+i));
				}
				printf("@class:");
				for(i=0;i<1000;i++)
				{
					if(score_tbl[i]>0)
					{
						printf("%s:%f,",class[i],score_tbl[i]);
					}
				}
				printf("\n");
			}
			//printf("%s",field_temp);
			rec_start = rec_end;
		}
		//memset(temp+rec_left,0,100);
	}
	return 0;
}
int map_cmp(const void *a,const void *b)
{
	return strcasecmp(((struct map *)a)->term,((struct map *)b)->term);
}
int score_field(char *field_temp,char *field_name,struct map *map,int map_size,float *score_tbl)
{
	int i,j=0,quot_flag=-1,in_quot=0,last=0,map_key,flag=0,content_last=0;
	char temp[100000];
	int count_tbl[1000];
	float temp_score;
	memset(count_tbl,0,sizeof(count_tbl));
	if(strcmp(field_name,"title")==0)
	{
		for(i=0;i<strlen(field_temp);i++)
		{
			if(issep(field_temp[i]))
			{
				for(;iscut(field_temp[i])&&i>0;i--);
				for(j=0;iscut(field_temp[j])&&j<i;j++);
				strncpy(temp,&field_temp[j],i-j+1);
				temp[i-j+1] = '\0';
				if((map_key = binary_search(temp,map,map_size))!=-1)
				{
					score_tbl[map_key] += 1*TITLE_RATE;
					flag=1;
					DEBUGP("in title issep:%s\n",temp);
				}
				break;
			}
		}
		if(i == strlen(field_temp))
			i=0;
		else
			i++;
		last = strlen(field_temp)-1;
		while(i<strlen(field_temp))
		{
			quot_flag = -1;
			for(;iscut(field_temp[i])&&i<strlen(field_temp);i++)
			{
				quot_flag = isquot_start(field_temp[i]);
			}
			last = strlen(field_temp)-1;
			while(i<last)
			{
				/*for(;iscut(last);last--);		*/
				in_quot = 0;
				for(;iscut(field_temp[last])&&i<last;last--)
				{
					if(quot_flag!=-1)
					{
						if(quot_flag == isquot_end(field_temp[last]))
						{
							in_quot = 1;
						}
					}
				}
				strncpy(temp,&field_temp[i],last-i+1);
				temp[last-i+1]='\0';
				if((map_key = binary_search(temp,map,map_size))!=-1)
				{
					if(in_quot!=0)
					{
						DEBUGP("in title:%s\n",temp);
						score_tbl[map_key] += 1*TITLE_RATE;
					}
					else
					{
						DEBUGP("in title inquot:%s\n",temp);
						count_tbl[map_key]++;
					}
					flag=1;
					break;
				}
				for(;!iscut(field_temp[last])&&i<last;last--);
			}
			for(;!iscut(field_temp[i])&&i<strlen(field_temp);i++);
		}
		for(i=0;i<1000;i++)
		{
			if(count_tbl[i]!=0)
			{
				temp_score = 1;
				for(j=0;j<count_tbl[i];j++)
				{
					temp_score *= 0.5;
				}
				score_tbl[i] += ((1-temp_score)/0.5)*TITLE_RATE;
			}
		}
	}
	else if(strcmp(field_name,"keyword")==0)
	{
		/*if(field_temp[strlen(field_temp)-1] == '\n')
		{
			field_temp[strlen(field_temp)-1] = '\0';
		}*/
		//memset(count_tbl,0,sizeof(count_tbl));
		for(i=0;i<strlen(field_temp);i++)
		{
			if(field_temp[i]!=','&&field_temp[i]!='\n')
			{
				temp[j] = field_temp[i];
				j++;
			}
			else
			{
				temp[j] = '\0';
				if(strlen(temp)!=0)
				{
					map_key = binary_search(temp,map,map_size);
					if(map_key!=-1)
					{
						DEBUGP("in keyword:%s\n",temp);
						count_tbl[map_key] += 1;
					}
				}
				j=0;
				temp[0] = '\0';
				i += 1;
			}
		}
		for(i=0;i<1000;i++)
		{
			if(count_tbl[i]!=0)
			{
				temp_score = 1;
				for(j=0;j<count_tbl[i];j++)
				{
					temp_score *= 0.5;
				}
				score_tbl[i] += ((1-temp_score)/0.5)*KEYWORD_RATE;
				flag = 1;
			}
		}
	}
	else if(strcmp(field_name,"content")==0)
	{
		i=0;
		int quot_start_flag=0,quot_end_flag=0;
		content_last = i + CONTENT_LEN;
		if(content_last>strlen(field_temp))
		{
			content_last = strlen(field_temp)-1;
		}
		/*for(;iscut(field_temp[content_last])&&content_last>0;content_last--)
		{
			quot_end_flag = isquot_end(field_temp[content_last]);
		}*/
		while(i<strlen(field_temp)-1)//buggy
		{
			if(!iscut(field_temp[content_last]))
			{
				for(;!iscut(field_temp[content_last])&&content_last>i;content_last--);
			}
			for(;iscut(field_temp[content_last])&&content_last>i;content_last--);
			while(i<content_last+1)
			{
				quot_flag = -1;
				for(;iscut(field_temp[i])&&i<strlen(field_temp);i++)
				{
					quot_flag = isquot_start(field_temp[i]);
				}
				last = content_last;
				while(i<last)
				{
					/*for(;iscut(last);last--);		*/
					in_quot = 0;
					for(;iscut(field_temp[last])&&i<last;last--)
					{
						if(quot_flag!=-1)
						{
							if(quot_flag == isquot_end(field_temp[last]))
							{
								in_quot = 1;
							}
						}
					}
					strncpy(temp,&field_temp[i],last-i+1);
					temp[last-i+1]='\0';
					if((map_key = binary_search(temp,map,map_size))!=-1)
					{
						//printf("***%s***\n",temp);
						if(in_quot!=0)
						{
							DEBUGP("in content:%s\n",temp);
							score_tbl[map_key] += 1*CONTENT_RATE;
						}
						else
						{
							DEBUGP("in content in quot:%s\n",temp);
							count_tbl[map_key]++;
						}
						flag=1;
						break;
					}
					for(;!iscut(field_temp[last])&&i<last;last--);
				}
				for(;!iscut(field_temp[i])&&i<strlen(field_temp);i++);
			}
			i = content_last+1;
			content_last += CONTENT_LEN;
			if(content_last>strlen(field_temp)-1)
			{
				content_last = strlen(field_temp)-1;
			}
			for(;iscut(field_temp[i])&&i<content_last;i++);
		}
		for(i=0;i<1000;i++)
		{
			if(count_tbl[i]!=0)
			{
				temp_score = 1;
				for(j=0;j<count_tbl[i];j++)
				{
					temp_score *= 0.5;
				}
				score_tbl[i] += ((1-temp_score)/0.5)*CONTENT_RATE;
				flag = 1;
			}
		}
	}
	return flag;
}

int binary_search(char *find,struct map *map,int map_size)
{
	int low=0,high=map_size-1,mid;
	while(low<=high)
	{
		mid = (low+high)/2;
		if(strcasecmp(find,map[mid].term)>0)
		{
			low = mid+1;
		}
		else if(strcasecmp(find,map[mid].term)<0)
		{
			high = mid-1;
		}
		else
		{
			return map[mid].key;
		}
	}
	return -1;
}

