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
int sizedata;
char *httpparams;
}HTTPOperation;
typedef struct httppage{
string title;           //Opcional
char *method;
vector <string> params;
vector <string> values;
HTTPOperation op;
FILE *htmlfile;
}HTTPPage;
char *GetHTTPParam(HTTPOperation*,const char *,unsigned int ,const char *);
int  SetHttpMethod(HTTPOperation*,const char *);
int  AddHttpParam(HTTPPage*,const char *,const char*);
int  RemoveHttpParam(HTTPPage*,const char*);
int  SetHttpTitle(HTTPPage *,float,int,const char*);
int  AttachHTML(HTTPPage*,const char*);
int  DetachHTML(HTTPPage*);
int  ClearHttpPage(HTTPPage *);
int  SendHTTPPage(HTTPPage*,ServerInfo*,SOCKET s);

