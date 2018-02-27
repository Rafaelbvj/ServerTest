#include "HTTPCom.h"
int DetachHTML(HTTPPage* hp){
    RemoveHttpParam(hp,"Content-Length");
    if(hp == NULL){
        return -1;
    }
    free(hp->op.httpparams);
    hp->op.data = NULL;
    hp->op.httpparams = NULL;
    return 0;
}
char *GetPostData(HTTPPage *hp,char* data,unsigned long sizedata){

    for(unsigned long i=0;i<sizedata||data[i] == '\0';i++){
        if(!strncmp(&data[i],"\r\n\r\n\0",4)){

            hp->op.data = (char*)malloc(sizeof(char)*(sizedata-i-4));

            memcpy(hp->op.data,&data[i+4],sizedata-i-4);

            hp->op.data[sizedata-i-4] = '\0';
            return hp->op.data;
        }
    }

    return 0;
}


int  ClearHttpPage(HTTPPage *hp){
DetachHTML(hp);
hp->params.clear();
hp->values.clear();
hp->title.clear();
if(hp->op.httpparams != NULL){
    free(hp->op.httpparams);
    hp->op.httpparams = NULL;
}
ZeroMemory(&hp->op,sizeof(HTTPOperation));
return 0;
}
int  RemoveHttpParam(HTTPPage *hp,const char *param){
        for(unsigned int i=0;i<hp->params.size();i++){
            if(!strcmp(hp->params.at(i).c_str(),param)){
                hp->params.erase(hp->params.begin()+i);
                hp->values.erase(hp->values.begin()+i);
                return 0;
            }
        }
    return -1;
}
char *GetHTTPParam(HTTPOperation *ht,const char *data,unsigned long sizedata, const char *param){

ht->data = strstr(data,param);
for(int index=0, addr = (int)ht->data;index<(int)&data[sizedata]-addr;index++){

    if(ht->data[index]==':'){
        for(index++;ht->data[index]==' ';index++){}
        for(int i=index;i<(int)&data[sizedata]-(int)&ht->data[index];i++){
            if(ht->data[i] == 0xD||ht->data[i] == 0xA){

            ht->data = (char*)malloc((i-index)*sizeof(char));
            ht->data[i-index] = '\0';
            memcpy(ht->data,(void*)(index+addr),i-index);
            return ht->data;
            }

        }

    }
}
return NULL;
}
int SetHttpTitle(HTTPPage *ho, float version, int code,const char *msg){
if(ho == NULL){return -1;}
ho->op.data = (char*)malloc(100+strlen(msg));
sprintf(ho->op.data,"HTTP/%.1f %d %s\r\n",version,code,msg);
ho->title = string(ho->op.data);
free(ho->op.data);
return 0;

}
int AddHttpParam(HTTPPage *ho,const char *param,const char *value){
if(param == NULL||value == NULL){return -1;}
ho->params.push_back(string(param));
ho->values.push_back(string(value));
return 0;
}
int AttachHTML(HTTPPage *hp,const char *path){
hp->htmlfile = fopen(path,"rb");
if(hp ->htmlfile == NULL){return -1;}
fseek(hp->htmlfile,0,SEEK_END);
hp->op.sizedata = ftell(hp->htmlfile);
rewind(hp->htmlfile);
hp->op.httpparams = (char*)malloc(hp->op.sizedata*sizeof(char));
fread(hp->op.httpparams,sizeof(char),hp->op.sizedata,hp->htmlfile);
hp->op.data =(char*)malloc(sizeof(char)*100);
sprintf(hp->op.data,"%d",hp->op.sizedata);
AddHttpParam(hp,"Content-Length",hp->op.data);
free(hp->op.data);
fclose(hp->htmlfile);
return 0;
}
int  SendHTTPPage(HTTPPage *hp, ServerInfo *sf, SOCKET s){

int addr=hp->title.size();

if(addr == 0){
    return -5;
}
if(hp->params.empty()){
return send(s,hp->title.c_str(),hp->title.size(),0);
}
for(unsigned int i=0;i<hp->params.size();i++){
     addr+=hp->params.at(i).size()+hp->values.at(i).size()+4; //+4 são :+space+\r+\n

}


hp->op.data =(char*)malloc(addr+hp->op.sizedata+3);
sprintf(hp->op.data,"%s",hp->title.c_str());
for(unsigned int i=0;i<hp->params.size();i++){
    sprintf(hp->op.data,"%s%s: %s\r\n",hp->op.data,hp->params.at(i).c_str(),hp->values.at(i).c_str());
}
sprintf(hp->op.data,"%s\r\n",hp->op.data);

if(hp->op.httpparams == NULL){
    sprintf(hp->op.data,"%s\0",hp->op.data);
    send(s,hp->op.data,addr,0);
    free(hp->op.data);
    return 2;
}

sprintf(hp->op.data,"%s%s\0",hp->op.data,hp->op.httpparams);
send(s,hp->op.data,strlen(hp->op.data),0);
free(hp->op.data);
return 1;
}
