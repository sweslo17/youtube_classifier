#ifndef _YOUTUBE_H
#define _YOUTUBE_H
#define TITLE_RATE 1
#define KEYWORD_RATE 0.7
#define CONTENT_RATE 0.5
#define CONTENT_LEN 100
char quot_start[100];
char quot_end[100];
char sep[100];
void init();
int issep(char);
int isquot_start(char);
int isquot_end(char);
int iscut(char);


#endif
