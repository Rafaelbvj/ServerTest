#pragma once
#include <stdio.h>
#include <iostream>
#define MODE_BLOCK    0
#define MODE_NONBLOCK 1
/*              Author: Rafael C.





*/
using namespace std;

#if __UNIX__
#include <winsock2.h>
typedef u_int	SOCKET;
typedef struct hostent HOSTENT;
#endif
#if __WIN32__
#include <windows.h>
#endif // __WIN32__
typedef struct serverinfo{
char ip[11];                //localhost ip
int port;                   //port
SOCKET sock;
struct sockaddr_in sconf;
IN_ADDR lia;
unsigned int nlisten;
int mode;
#if __WIN32__
WSAData wsa;
#endif
}ServerInfo;

int OpenPort(ServerInfo *,unsigned int);
int ListenPort(ServerInfo *,unsigned int,u_long);
int AcceptPort(ServerInfo*);

