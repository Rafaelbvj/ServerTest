#include <iostream>
#include <windows.h>
#include <list>
#include "ServerCom.h"
#include "HTTPCom.h"
using namespace std;
void InitProtocolHTTP(HTTPPage * h){
    SetHttpTitle(h,1.1,200,"OK");
    AddHttpParam(h,"Server", "Rafael");
    AddHttpParam(h,"Connection","close");
    AddHttpParam(h,"Accept-Ranges","bytes");
    AddHttpParam(h,"Content-Type","text/html");
}
int main()
{

    HTTPPage hp;
    char *data;
    u_long datasize;
    fd_set fdr;
    vector <SOCKET> connected;
    ServerInfo si;
    OpenPort(&si,5200);
    ListenPort(&si,50,MODE_BLOCK);
    FD_ZERO(&fdr);
    FD_SET(si.sock,&fdr);
    //FreeConsole();
    cout<<"Esperando conexoes..."<<endl;
    int ncon =0;
    char *fp;
    for(int num =0; (num = select(0,&fdr,0,0,0)) != SOCKET_ERROR;){

    if(FD_ISSET(si.sock,&fdr)){                             //Se conectou-se ao servidor
        connected.push_back(AcceptPort(&si));
        if(connected.back()== INVALID_SOCKET){
        connected.erase(connected.end());
        continue;}

        cout<<"Cliente "<<ncon<<" conectou"<<endl;
        FD_SET(connected.back(),&fdr);                       //Coloca o cliente na fila



    }

    for(int e=0;e<connected.size();e++){
        if(FD_ISSET(connected.at(e),&fdr)){                    //Se o cliente interagir ele vem pra cá
            ioctlsocket(connected.at(e),FIONREAD,&datasize);
            if(datasize <=0){
            cout<<"Requisicao do cliente!"<<endl;
            }
            else{
            cout<<"Cliente "<<e<<" enviou"<<endl;
            data = (char*)malloc(sizeof(char)*datasize);
            recv(connected.at(e),data,datasize,0);         //Request
            data[datasize]=  '\0';
            fp = GetFileParam(&hp.op,data,datasize);
            if(data[0] == 'P'){
                cout<<"Post"<<endl;
                InitProtocolHTTP(&hp);
                RemoveHttpParam(&hp,"Content-Type");
                AddHttpParam(&hp,"Content-Type","audio/mpeg");
                AttachHTML(&hp,&fp[1]);
                GetPostData(&hp,data, datasize);
            }
            if(data[0]== 'G'){
            cout<<"Get"<<endl;
            if(strncmp(fp,"/favicon.ico",12)==0){
            SetHttpTitle(&hp,1.1,200,"OK");
            }
            else{
            InitProtocolHTTP(&hp);
            AttachHTML(&hp,"page.html");
            }
            }
            SendHTTPPage(&hp,&si,connected.at(e));
            ClearHttpPage(&hp);

            }
            free(fp);
            shutdown(connected.at(e),SD_BOTH);
            FD_CLR(connected.at(e),&fdr);
            connected.erase(connected.begin()+e);


        }

    }


    }
    cout<<WSAGetLastError()<<endl;
    //WSACleanup();
    return 0;
}
