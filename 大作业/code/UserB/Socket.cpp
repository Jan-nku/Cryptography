#include"Socket.h"
#include"BigInt.h"
#include"RSA.h"
#include"AES.h"
#include<sstream>
WSADATA wsaData;
SOCKADDR_IN ServerAddr, ClientAddr;
SOCKET ServerSocket, ClientSocket;
extern Rsa UserA, UserB;
char ID[8];
//�洢ͨѶʹ�õ�AESKey��AESIV
u_char AESKey[16], AESIV[16];

string AES_Encryption_CBC(string m, const u_char* Key, const u_char* IV) {
	int size = m.size();
	u_char iv[16], cipher[16];
	memset(iv, 0, 16);
	memcpy(iv, IV, 16);
	int off = 16 - size % 16;
	if (off == 16) {
		off = 0;
	}
	for (int i = 0; i < off; i++) {
		m += '\0';
	}
	size = m.size();
	string result = "";
	u_char plain[16];
	for (int i = 0; i < size / 16; i++) {
		memcpy(plain, m.substr(i * 16, 16).c_str(), 16);
		for (int j = 0; j < 16; j++) {
			plain[j] ^= iv[j];
		}
		memset(cipher, 0, 16);
		AES_Encryption(plain, cipher, Key);
		for (int j = 0; j < 16; j++) {
			result += cipher[j];
		}
		memcpy(iv, cipher, 16);
	}
	return result;
}

string AES_Decryption_CBC(string c, const u_char* Key, const u_char* IV) {
	int size = c.size();
	int off = 16 - size % 16;
	if (off == 16) {
		off = 0;
	}
	for (int i = 0; i < off; i++) {
		c += '\0';
	}
	size = c.size();
	string result = "";
	u_char iv[16], cipher[16], plain[16];
	memset(iv, 0, 16);
	memcpy(iv, IV, 16);
	for (int i = 0; i < size / 16; i++) {
		memcpy(cipher, c.substr(i * 16, 16).c_str(), 16);
		AES_Decryption(plain, cipher, Key);
		for (int j = 0; j < 16; j++) {
			plain[j] ^= iv[j];
			result += plain[j];
		}
		memcpy(iv, cipher, 16);
	}
	return result;
}

DWORD WINAPI handler_Recv(LPVOID lparam) {
	SOCKET recvSocket = (SOCKET)(LPVOID)lparam;
	char recvBuf[500];
	while (1) {
		memset(recvBuf, 0, sizeof(recvBuf));
		int len = recv(recvSocket, recvBuf, sizeof(recvBuf), 0);
		string tmp = recvBuf;
		string result = AES_Decryption_CBC(tmp, AESKey, AESIV);
		cout << "UserA������Ϣ��" << result << endl;
	}
	closesocket(recvSocket);
	return 0;
}


//RSA��Կ����
void RSAshare() {
	char sendBuf[500];
	char recvBuf[500];
	string tmp;
	//����N
	memset(sendBuf, 0, sizeof(sendBuf));
	memcpy(sendBuf, BigIntToString(UserB.N).c_str(), BigIntToString(UserB.N).size());
	send(ClientSocket, sendBuf, sizeof(sendBuf), 0);
	memset(recvBuf, 0, sizeof(recvBuf));
	recv(ClientSocket, recvBuf, sizeof(recvBuf), 0);
	tmp = recvBuf;
	UserA.N = tmp;
	//����e
	memset(sendBuf, 0, sizeof(sendBuf));
	memcpy(sendBuf, BigIntToString(UserB.e).c_str(), BigIntToString(UserB.e).size());
	send(ClientSocket, sendBuf, sizeof(sendBuf), 0);
	memset(recvBuf, 0, sizeof(recvBuf));
	recv(ClientSocket, recvBuf, sizeof(recvBuf), 0);
	tmp = recvBuf;
	UserA.e = tmp;
	//���UserA�Ĺ�Կ
	cout << "UserA.e��" << UserA.e << endl;
	cout << "UserA.N��" << UserA.N << endl;
}

//����һ��nλ�����
string createRandom(int n = 64) {
	n = n / 4;
	unsigned char hex_table[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	ostringstream oss;
	int k;
	do {
		k = rand() % 16;
	} while (k == 0);
	oss << hex_table[k];
	for (size_t i = 1; i < n; ++i) {
		oss << hex_table[rand() % 16];
	}
	string str(oss.str());
	return str;
}

//α��������ɣ�RSA������
BigInt createRandomRSA() {
	Rsa rsa;
	init(rsa, 256);
	//��ȡα��������е�X[index]��Ԫ��
	//����ʹ��rand()������ʼ��index��X0
	int index = rand();
	BigInt X0 = rand();
	BigInt X1;
	for (int i = 0; i < index; i++) {
		X1 = rsa.decryption(X0);
		X0 = X1;
	}
	return X1;
}

string RSA_Encrypt_SingleString(Rsa& rsa, string m) {
	char code[38] = {
		'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F','G','H','I','J',
		'K','L','M','N','O','P','Q','R','S','T',
		'U','V','W','X','Y','Z',' ', '*'
	};
	string tmp;
	for (int i = 0; i < m.size(); i++) {
		int num = 0;
		for (int j = 0; j < 38; j++) {
			if (m[i] == code[j] || m[i] == code[j] + (97 - 65)) {
				num = j;
				break;
			}
		}
		if (num < 10) {
			tmp += '0';
		}
		tmp += to_string(num);
	}
	BigInt a = rsa.encryption(BigInt(tmp));
	return BigIntToString(a);
}

string RSA_Decrypt_SingleString(Rsa& rsa, string c) {
	BigInt a = c;
	BigInt b = rsa.decryption(a);
	string tmp = BigIntToString(b);
	cout << "���ܺ������" << endl;

	char code[38] = {
		'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F','G','H','I','J',
		'K','L','M','N','O','P','Q','R','S','T',
		'U','V','W','X','Y','Z',' ', '*'
	};
	string content = "";
	bool isZero = true;
	for (int i = 0; i < tmp.size(); i++) {
		if (isZero && tmp[i] == '0') {
			continue;
		}
		isZero = false;
		content += tmp[i];
	}
	if (content.size() % 2 != 0) {
		content = '0' + content;
	}
	string text;
	cout << content << endl;
	for (int i = 0; i < content.size()/2; i++) {
		text += code[stoi(content.substr(2 * i, 2))];
	}
	return text;
}

string RSA_Encrypt_SingleString_nocode(Rsa& rsa, string m, bool reverse = false) {
	BigInt a;
	if (reverse) {
		a = rsa.inv_encryption(BigInt(m));
	}
	else {
		a = rsa.encryption(BigInt(m));
	}

	return BigIntToString(a);
}


string RSA_Decrypt_SingleString_nocode(Rsa& rsa, string c, bool reverse = false) {
	BigInt a = c;
	BigInt b;
	if (reverse) {
		b = rsa.inv_decryption(a);
	}
	else {
		b = rsa.decryption(a);
	}
	string tmp = BigIntToString(b);
	return tmp;
}

int ctoi(char c) {
	if (c <= '9' && c >= '0') {
		return (c -= 48);
	}
	else if (c <= 'z' && c >= 'a') {
		return (c = c - 97 + 10);
	}
	else if (c <= 'Z' && c >= 'A') {
		return (c = c - 65 + 10);
	}
	else {
		return -1;
	}
}

void AESshare() {
	//����UserA�������n1��ID
	char recvBuf[500];
	int len = recv(ClientSocket, recvBuf, sizeof(recvBuf), 0);
	string tmp = "";
	for (int i = 0; i < len; i++) {
		tmp += recvBuf[i];
	}
	string text = RSA_Decrypt_SingleString(UserB, tmp);
	string n1 = text.substr(0, 16);
	memset(ID, 0, sizeof(ID));
	memcpy(ID, text.c_str() + n1.size(), text.size() - n1.size());
	cout << "���յ�UserA�������n1��" << n1 << endl;
	cout << "���յ�UserA��ID��" << ID << endl;

	//UserB����n1��n2
	char sendBuf[500];
	memset(sendBuf, 0, sizeof(sendBuf));
	string n_2 = createRandom();
	tmp = RSA_Encrypt_SingleString(UserA, n1 + n_2);
	memcpy(sendBuf, tmp.c_str(), tmp.size());
	send(ClientSocket, sendBuf, tmp.size(), 0);
	cout << "���͸�UserA�������n1��" << n1 << endl;
	cout << "���͸�UserA�������n2��" << n_2 << endl;

	//����n2
	memset(recvBuf, 0, sizeof(recvBuf));
	len = recv(ClientSocket, recvBuf, sizeof(recvBuf), 0);
	tmp = "";
	for (int i = 0; i < len; i++) {
		tmp += recvBuf[i];
	}
	text = RSA_Decrypt_SingleString(UserB, tmp);
	string n2 = text.substr(0, 16);
	cout << "���յ�UserA�������n2��" << n2 << endl;
	if (n2 == n_2) {
		cout << "�����Աȣ�UserA��������n2�������Ƿ��͵������n2" << endl;
	}
	else {
		cout << "�����Աȣ�UserA��������n2�����Ƿ��͵������n2��һ��" << endl;
	}

	//����AES��Կ����ʼ��IV
	char hex[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	memset(recvBuf, 0, sizeof(recvBuf));
	len = recv(ClientSocket, recvBuf, sizeof(recvBuf), 0);
	tmp = "";
	for (int i = 0; i < len; i++) {
		tmp += recvBuf[i];
	}
	string tmp1 = RSA_Decrypt_SingleString_nocode(UserB, tmp);
	text = RSA_Encrypt_SingleString_nocode(UserA, tmp1);
	string AES_Key = text.substr(0, 32);
	string AES_IV = text.substr(32, 32);
	cout << "UserA���͵�AES��Կ��" << AES_Key << endl;
	cout << "UserA���͵�AES���ܺ��IV��" << AES_IV << endl;
	for (int i = 0; i < 16; i++) {
		AESIV[i] = (ctoi(AES_IV[2 * i]) << 4) | ctoi(AES_IV[2 * i + 1]);
		AESKey[i] = (ctoi(AES_Key[2 * i]) << 4) | ctoi(AES_Key[2 * i + 1]);
	}
	u_char true_IV[16];
	AES_Decryption(true_IV, AESIV, AESKey);
	string temp = "";
	for (int i = 0; i < 16; i++) {
		temp = temp + hex[(true_IV[i] >> 4)] + hex[true_IV[i] & 0xf];
	}
	cout << "UserA���͵�AES���ܺ��IV��" << temp << endl;
	memset(AESIV, 0, 16);
	memcpy(AESIV, true_IV, 16);
}


int Connect() {
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSADATA��ʼ��ʧ�ܣ������� : %d", WSAGetLastError());
		return 1;
	}
	printf("WSAStartup Complete!\n");

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(Port);
	ServerAddr.sin_addr.S_un.S_addr = inet_addr(IP);

	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET) {
		printf("Socket����ʧ�ܣ������� : %d", WSAGetLastError());
	}
	cout << "Socket created!\n";

	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		cout << "failed to bind!" << endl;
		return -1;
	}
	cout << "Bind Success!" << endl;

	int ClientAddrlen = sizeof(ClientAddr);
	cout << "Start listening..." << endl;
	int l = listen(ServerSocket, 1);
	if (l != 0) {
		cout << "error" << GetLastError() << l << endl;
	}

	cout << "Starting accepting..." << endl;
	ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClientAddrlen);
	cout << "��UserA��������" << endl;

}

void communicate(){
	HANDLE hThread_recv = CreateThread(NULL, NULL, &handler_Recv, LPVOID(ClientSocket), 0, NULL);
	string sendBuf;
	while (1) {
		getline(cin, sendBuf);
		string tmp = AES_Encryption_CBC(sendBuf, AESKey, AESIV);
		send(ClientSocket, tmp.c_str(), tmp.size(), 0);
		cout << "���͸�UserA��" << sendBuf << endl;
	}

	CloseHandle(hThread_recv);
	closesocket(ServerSocket);
	WSACleanup();
	//return 0;
}