#include"DES.h"
using namespace std;
int compare(u_char before[8], u_char after[8]) {
	int num = 0;
	for (int i = 0; i < 63; i++) {
		if (bit_get(before, i) != bit_get(after, i)) {
			num++;
		}
	}
	return num;
}
int main() {
	cout << "----------------��֤DES����----------------" << endl;
	for (int i = 0; i < 10; i++) {
		cout << "���ķ���" << i << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", cases[i].in[j]);
		}
		cout << endl;
		DES_Encryption(cases[i].in, Ciphertext, cases[i].key); 
		cout << "���ܺ�����ķ���" << i << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", Ciphertext[j]);
		}
		cout << endl;
	}
	cout << "----------------��֤DES����----------------" << endl;
	for (int i = 10; i < 20; i++) {
		cout << "���ķ���" << (i - 10) << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", cases[i].in[j]);
		}
		cout << endl;
		DES_Decryption(Plaintext, cases[i].in, cases[i].key);
		cout << "���ܺ�����ķ���" << (i - 10) << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", Plaintext[j]);
		}
		cout << endl;
	}
	int num1[10][8] = { 0 };
	int num1_total[10] = { 0 };
	int num2[10][8] = { 0 };
	int num2_total[10] = { 0 };
	cout << "----------------���ѩ��ЧӦ----------------" << endl;
	cout << "----------------�̶���Կ���ı�����----------------" << endl;
	for (int i = 0; i < 10; i++) {
		cout << "ԭʼ���ķ���" << i << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", cases[i].in[j]);
		}
		cout << endl;
		cout << "ԭʼ���ķ���" << i << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", cases[i].out[j]);
		}
		cout << endl;
		for (int j = 0; j < 8; j++) {
			cout << "�ı����ķ���ĵ�" << 8 * j << "λ��һ�µ�λ��:  ";
			memset(Plaintext, 0, 8);
			memcpy(Plaintext, cases[i].in, 8);
			bit_set(Plaintext, 8 * j, (1 - bit_get(Plaintext, 8 * j)));
			DES_Encryption(Plaintext, Ciphertext, cases[i].key);
			cout << (num1[i][j] = compare(Ciphertext, cases[i].out)) << endl;
			num1_total[i] += num1[i][j];
		}
		cout << "���ķ���ƽ���ı��λ��Ϊ:  " << (float(num1_total[i]) / 8) << endl;
	}
	for (int i = 0; i < 10; i++) {
		cout << "���ķ���" << i << "ƽ���ı��λ��Ϊ:  " << (float(num1_total[i]) / 8) << endl;
	}

	cout << "----------------�̶����ģ��ı���Կ----------------" << endl;
	for (int i = 0; i < 10; i++) {
		cout << "ԭʼ��Կ����" << i << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", cases[i].key[j]);
		}
		cout << endl;
		cout << "ԭʼ���ķ���" << i << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", cases[i].out[j]);
		}
		cout << endl;
		for (int j = 0; j < 8; j++) {
			cout << "�ı���Կ����ĵ�" << 8 * j << "λ��һ�µ�λ��:  ";
			memset(Key, 0, 8);
			memcpy(Key, cases[i].key, 8);
			bit_set(Key, 8 * j, (1 - bit_get(Key, 8 * j)));
			DES_Encryption(Plaintext, Ciphertext, Key);
			cout << (num2[i][j] = compare(Ciphertext, cases[i].out)) << endl;
			num2_total[i] += num2[i][j];
		}
		cout << "���ķ���ƽ���ı��λ��Ϊ:  " << (float(num1_total[i]) / 8) << endl;
	}
	for (int i = 0; i < 10; i++) {
		cout << "���ķ���" << i << "ƽ���ı��λ��Ϊ:  " << (float(num1_total[i]) / 8) << endl;
	}



}
