#include "ServerCom.h"
int OpenPort(ServerInfo *si, unsigned int p){
memset(si,0,sizeof(ServerInfo));
#if __WIN32__
WSAStartup(MAKEWORD(2,2),&si->wsa);
#endif // __WIN32__
HOSTENT *ht = gethostbyname("localhost");
si->lia.s_addr = *(u_long*)ht->h_addr_list[0];
strcpy(si->ip,inet_ntoa(si->lia));
si->sock = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
si->sconf.sin_addr = si->lia;
si->sconf.sin_port = htons(p);
si->sconf.sin_family = AF_INET;
si->port = p;

return bind(si->sock,(struct sockaddr*)&si->sconf,sizeof(si->sconf));

}
int ListenPort(ServerInfo *si,unsigned int nconnect,u_long mode){
if(si == NULL||nconnect == 0){
    return -1;
}

si->nlisten = nconnect;
si->mode = mode;
#if __UNIX__
ioctl(si->sock,FIONBIO,&mode);
#endif // __UNIX__
#if __WIN32__
ioctlsocket(si->sock,FIONBIO,&mode);
#endif
if(listen(si->sock,nconnect)==SOCKET_ERROR){return -2;}

return 0;
}
int AcceptPort(ServerInfo *si){

if(si == NULL){
    return -1;
}
return accept(si->sock,(SOCKADDR*)&si->sconf,0);

}
