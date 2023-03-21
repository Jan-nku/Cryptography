#include <iostream>
#include <winsock.h>
#include <string>
#include <cstring>
#include <ctime>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define IP "127.0.0.1"
#define Port 6666

extern char ID[8];
DWORD WINAPI handler_Recv(LPVOID lparam);
int Connect();
void RSAshare();
void AESshare();
void communicate();

