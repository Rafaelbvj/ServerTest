#include <iostream>
#include <windows.h>
#include <list>
#include "ServerCom.h"
#include "HTTPCom.h"
using namespace std;
void InitProtocolHTTP(HTTPPage * h){
    SetHttpTitle(h,1.1,200,"OK");
    AddHttpParam(h,"Server", "Rafael");
    AddHttpParam(h,"Connection","Keep-Alive");
    AddHttpParam(h,"Accept-Ranges","bytes");
    AddHttpParam(h,"Content-Type","text/html");
}
int main()
{
    HTTPPage hp;
    char *data;
    u_long datasize;
    ServerInfo si;
    OpenPort(&si,5200);
    ListenPort(&si,50,MODE_BLOCK);
    fd_set fdr;
    vector <SOCKET> connected;
    FD_ZERO(&fdr);
    FD_SET(si.sock,&fdr);
    cout<<"Esperando conexoes..."<<endl;
    ZeroMemory(&hp,sizeof(HTTPPage));
    InitProtocolHTTP(&hp);
    AttachHTML(&hp,"page.html");
    int ncon =0;
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
        if(FD_ISSET(connected.at(e),&fdr)){                    //Se o cliente interagir ele vem pra c�
            ioctlsocket(connected.at(e),FIONREAD,&datasize);
            if(datasize <=0){
            cout<<"Requisicao do cliente!"<<endl;
            FD_CLR(connected.at(e),&fdr);
            connected.erase(connected.begin()+e);
            continue;
            }
            cout<<"Cliente "<<e<<" enviou"<<endl;
            data = (char*)malloc(datasize);

            recv(connected.at(e),data,datasize,0);         //Request
            if(data[0] == 'P'){
                cout<<"Post"<<endl;
                cout<<data<<endl;
            }
            if(data[0]== 'G'){
            cout<<"Get"<<endl;
            SendHTTPPage(&hp,&si,connected.at(e));
            RemoveHttpParam(&hp,"Content-Type");
            SetHttpTitle(&hp,1.1,404,"NOT FOUND");
            DetachHTML(&hp);
            }
            free(data);

        }

    }


    }
    cout<<WSAGetLastError()<<endl;
    //WSACleanup();
    return 0;
}
