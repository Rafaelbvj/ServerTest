#pragma once
#include <string.h>
#include <malloc.h>
#include <vector>
#include "ServerCom.h"

using namespace std;
/*              Author: Rafael C.





*/
typedef struct httpop{
char *data;
unsigned int sizedata;
char *httpparams;
}HTTPOperation;
typedef struct httppage{
string title;           //Opcional
vector <string> params;
vector <string> values;
HTTPOperation op;
FILE *htmlfile;
}HTTPPage;
char *GetFileParam(HTTPOperation*, const char*, unsigned long);
char *GetHTTPParam(HTTPOperation*,const char *,unsigned long ,const char *);
int  AddHttpParam(HTTPPage*,const char *,const char*);
int  RemoveHttpParam(HTTPPage*,const char*);
int  SetHttpTitle(HTTPPage *,float,int,const char*);
int  AttachHTML(HTTPPage*,const char*);
int  DetachHTML(HTTPPage*);
int  ClearHttpPage(HTTPPage *);
int  SendHTTPPage(HTTPPage*,ServerInfo*,SOCKET s);
char *GetPostData(HTTPPage*,char*,unsigned long);
