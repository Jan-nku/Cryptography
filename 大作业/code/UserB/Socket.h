#include <iostream>
#include <winsock.h>
#include <string>
#include <cstring>
#include <ctime>
#include"BigInt.h"
#include"RSA.h"
#include"AES.h"
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define IP "127.0.0.1"
#define Port 6666

extern WSADATA wsaData;
extern SOCKADDR_IN ServerAddr, ClientAddr;
extern SOCKET ServerSocket, ClientSocket;
extern u_char AESKey[16], AESIV[16];
DWORD WINAPI handler_Recv(LPVOID lparam);
int Connect();
void RSAshare();
void AESshare();
void communicate();