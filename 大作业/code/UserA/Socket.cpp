#include"Socket.h"
#include"BigInt.h"
#include"RSA.h"
#include<sstream>
#include"AES.h"
extern Rsa UserA, UserB;
WSADATA wsaData;
SOCKET ClientSocket;
SOCKADDR_IN ServerAddr;
char ID[8];
//存储通讯使用的AESKey、AESIV
u_char AESKey[16], AESIV[16];
bool change = false;
#define MAX_TIME 5000000

//AES Key可以指定，也可以随机初始化
const u_char Plaintext1[16] = { 0x00,0x01,0x00,0x01,0x01,0xa1,0x98,0xaf,0xda,0x78,0x17,0x34,0x86,0x15,0x35,0x66 };
const u_char Plaintext2[16] = { 0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34 };
const u_char Ciphertext1[16] = { 0x6c,0xdd,0x59,0x6b,0x8f,0x56,0x42,0xcb,0xd2,0x3b,0x47,0x98,0x1a,0x65,0x42,0x2a };
const u_char Ciphertext2[16] = { 0x39,0x25,0x84,0x1d,0x02,0xdc,0x09,0xfb,0xdc,0x11,0x85,0x97,0x19,0x6a,0x0b,0x32 };
const u_char AES_IV[16] = { '0','1','0','2','0','3','0','4','0','5','0','6','0','7','0','8' };
const u_char AES_Key1[16] = { 0x00,0x01,0x20,0x01,0x71,0x01,0x98,0xae,0xda,0x79,0x17,0x14,0x60,0x15,0x35,0x94 };
const u_char AES_Key2[16] = { 0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c };

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
        cout << "UserB发送消息：" << result << endl;
    }
    closesocket(recvSocket);
    return 0;
}

//RSA公钥交换
void RSAshare() {
    char sendBuf[500];
    char recvBuf[500];
    string tmp;
    //交换N
    memset(sendBuf, 0, sizeof(sendBuf));
    memcpy(sendBuf, BigIntToString(UserA.N).c_str(), BigIntToString(UserA.N).size());
    send(ClientSocket, sendBuf, sizeof(sendBuf), 0);
    memset(recvBuf, 0, sizeof(recvBuf));
    recv(ClientSocket, recvBuf, sizeof(recvBuf), 0);
    tmp = recvBuf;
    UserB.N = tmp;
    //交换e
    memset(sendBuf, 0, sizeof(sendBuf));
    memcpy(sendBuf, BigIntToString(UserA.e).c_str(), BigIntToString(UserA.e).size());
    send(ClientSocket, sendBuf, sizeof(sendBuf), 0);
    memset(recvBuf, 0, sizeof(recvBuf));
    recv(ClientSocket, recvBuf, sizeof(recvBuf), 0);
    tmp = recvBuf;
    UserB.e = tmp;
    //输出UserA的公钥
    cout << "UserB.e：" << UserB.e << endl;
    cout << "UserB.N：" << UserB.N << endl;
}

//创建一个n位随机数
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

//伪随机数生成：RSA产生器
BigInt createRandomRSA() {
    Rsa rsa;
    init(rsa, 256);
    //获取伪随机数序列的X[index]号元素
    //这里使用rand()函数初始化index、X0
    int index = rand();
    BigInt X0 = rand();
    BigInt X1;
    for (int i = 0; i < index; i++) {
        X1 = rsa.decryption(X0);
        X0 = X1;
    }
    return X1;
}

string RSA_Encrypt_SingleString(Rsa& rsa, string m, bool reverse = false) {
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

    BigInt a;
    if (reverse) {
        a = rsa.inv_encryption(BigInt(tmp));
    }
    else {
        a = rsa.encryption(BigInt(tmp));
    }
    
    return BigIntToString(a);
}

string RSA_Decrypt_SingleString(Rsa& rsa, string c, bool reverse = false) {
    BigInt a = c;
    BigInt b;
    if (reverse) {
        b = rsa.inv_decryption(a);
    }
    else{
        b = rsa.decryption(a);
    }
    string tmp = BigIntToString(b);

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
    for (int i = 0; i < content.size() / 2; i++) {
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


void AESshare() {
    //发送ID和随机数n1
    char sendBuf[500];
    memset(sendBuf, 0, sizeof(sendBuf));
    string n1 = createRandom();
    string tmp = RSA_Encrypt_SingleString(UserB, n1 + ID);
    memcpy(sendBuf, tmp.c_str(), tmp.size());
    send(ClientSocket, sendBuf, tmp.size(), 0);
    cout << "发送给UserB的随机数n1：" << n1 << endl;
    cout << "发送给UserB的ID：" << ID << endl;

    //接收n1和n2
    char recvBuf[500];
    memset(recvBuf, 0, sizeof(recvBuf));
    int len = recv(ClientSocket, recvBuf, sizeof(recvBuf), 0);
    tmp = "";
    for (int i = 0; i < len; i++) {
        tmp += recvBuf[i];
    }
    string text = RSA_Decrypt_SingleString(UserA, tmp);
    string n_1 = text.substr(0, 16);
    string n_2 = text.substr(16, 16);
    cout << "接收到UserB的随机数n1：" << n_1 << endl;
    cout << "接收到UserB的随机数n2：" << n_2 << endl;
    if (n1 == n_1) {
        cout << "经过对比，UserB传过来的n1正是我们发送的随机数n1" << endl;
    }
    else {
        cout << "经过对比，UserB传过来的n1与我们发送的随机数n1不一致" << endl;
    }

    //发送n2
    memset(sendBuf, 0, sizeof(sendBuf));
    tmp = RSA_Encrypt_SingleString(UserB, n_2);
    memcpy(sendBuf, tmp.c_str(), tmp.size());
    send(ClientSocket, sendBuf, tmp.size(), 0);
    cout << "发送给UserB的随机数n2：" << n_2 << endl;
    
    //程序有一点问题就是AES_Key的首字节不能是0x0?
    //发送AES密钥、初始化IV
    char hex[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
    tmp = "";
    for (int i = 0; i < 16; i++) {
        tmp = tmp + hex[(AES_Key2[i] >> 4)] + hex[(AES_Key2[i] & 0xf)];
    }
    u_char cipher_IV[16];
    AES_Encryption(AES_IV, cipher_IV, AES_Key2);
    memset(AESKey, 0, 16);
    memcpy(AESKey, AES_Key2, 16);
    memset(AESIV, 0, 16);
    memcpy(AESIV, AES_IV, 16);

    for (int i = 0; i < 16; i++) {
        tmp = tmp + hex[(cipher_IV[i] >> 4)] + hex[(cipher_IV[i] & 0xf)];
    }
    string tmp1 = RSA_Encrypt_SingleString_nocode(UserA, tmp, true);
    string tmp2 = RSA_Encrypt_SingleString_nocode(UserB, tmp1);
    memset(sendBuf, 0, sizeof(sendBuf));
    memcpy(sendBuf, tmp2.c_str(), tmp2.size());
    send(ClientSocket, sendBuf, tmp2.size(), 0);
    string temp = "";
    for (int i = 0; i < 16; i++) {
        temp = temp + hex[(AES_IV[i] >> 4)] + hex[(AES_IV[i] & 0xf)];
    }
    cout << "发送给UserB的AES密钥：" << tmp.substr(0, 32) << endl;
    cout << "发送给UserB的AES加密前的IV：" << temp << endl;
    cout << "发送给UserB的AES加密后的IV：" << tmp.substr(32, 32) << endl;

}

int Connect() {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSADATA初始化失败，错误码 : %d", WSAGetLastError());
        return 1;
    }
    cout << "WSAStartup Complete!\n";

    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET) {
        printf("Socket创建失败，错误码 : %d", WSAGetLastError());
    }
    cout << "Socket created!\n";

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(Port);
    ServerAddr.sin_addr.S_un.S_addr = inet_addr(IP);

    cout << "请输入身份ID:";
    cin.getline(ID, 16);
    cout << "等待连接..." << endl;
    if (connect(ClientSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) < 0) {
        cout << "连接失败" << endl;
        return -1;
    }
    cout << "与UserB连接成功" << endl;
}

//timer线程函数，隔一段时间更换AES密钥
//更换指标也可以替换成A、B发送消息次数
DWORD WINAPI timer(LPVOID lparam) {
    clock_t start = clock();
    while (true) {
        //是否需要更换AES密钥
        if (change) {
            start = clock();
            change = false;
        }
        //密钥使用超过一定时间，不安全需要更换
        if (clock() - start > MAX_TIME) {
            change = true;
            start = clock();
        }
    }
    return 0;
}
    
void communicate() {
    HANDLE hThread_recv = CreateThread(NULL, NULL, &handler_Recv, LPVOID(ClientSocket), 0, NULL);
    string sendBuf;
    while (1) {
        getline(cin, sendBuf);
        string tmp = AES_Encryption_CBC(sendBuf, AESKey, AESIV);
        send(ClientSocket, tmp.c_str(), tmp.size(), 0);
        cout << "发送给UserB：" << sendBuf << endl;
    }

    CloseHandle(hThread_recv);
    closesocket(ClientSocket);
    WSACleanup();
}