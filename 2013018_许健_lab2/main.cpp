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
	cout << "----------------验证DES加密----------------" << endl;
	for (int i = 0; i < 10; i++) {
		cout << "明文分组" << i << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", cases[i].in[j]);
		}
		cout << endl;
		DES_Encryption(cases[i].in, Ciphertext, cases[i].key); 
		cout << "加密后的密文分组" << i << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", Ciphertext[j]);
		}
		cout << endl;
	}
	cout << "----------------验证DES解密----------------" << endl;
	for (int i = 10; i < 20; i++) {
		cout << "密文分组" << (i - 10) << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", cases[i].in[j]);
		}
		cout << endl;
		DES_Decryption(Plaintext, cases[i].in, cases[i].key);
		cout << "解密后的明文分组" << (i - 10) << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", Plaintext[j]);
		}
		cout << endl;
	}
	int num1[10][8] = { 0 };
	int num1_total[10] = { 0 };
	int num2[10][8] = { 0 };
	int num2_total[10] = { 0 };
	cout << "----------------检测雪崩效应----------------" << endl;
	cout << "----------------固定密钥，改变明文----------------" << endl;
	for (int i = 0; i < 10; i++) {
		cout << "原始明文分组" << i << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", cases[i].in[j]);
		}
		cout << endl;
		cout << "原始密文分组" << i << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", cases[i].out[j]);
		}
		cout << endl;
		for (int j = 0; j < 8; j++) {
			cout << "改变明文分组的第" << 8 * j << "位后不一致的位数:  ";
			memset(Plaintext, 0, 8);
			memcpy(Plaintext, cases[i].in, 8);
			bit_set(Plaintext, 8 * j, (1 - bit_get(Plaintext, 8 * j)));
			DES_Encryption(Plaintext, Ciphertext, cases[i].key);
			cout << (num1[i][j] = compare(Ciphertext, cases[i].out)) << endl;
			num1_total[i] += num1[i][j];
		}
		cout << "密文分组平均改变的位数为:  " << (float(num1_total[i]) / 8) << endl;
	}
	for (int i = 0; i < 10; i++) {
		cout << "密文分组" << i << "平均改变的位数为:  " << (float(num1_total[i]) / 8) << endl;
	}

	cout << "----------------固定明文，改变密钥----------------" << endl;
	for (int i = 0; i < 10; i++) {
		cout << "原始密钥分组" << i << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", cases[i].key[j]);
		}
		cout << endl;
		cout << "原始密文分组" << i << ":  ";
		for (int j = 0; j < 8; j++) {
			printf("%02x ", cases[i].out[j]);
		}
		cout << endl;
		for (int j = 0; j < 8; j++) {
			cout << "改变密钥分组的第" << 8 * j << "位后不一致的位数:  ";
			memset(Key, 0, 8);
			memcpy(Key, cases[i].key, 8);
			bit_set(Key, 8 * j, (1 - bit_get(Key, 8 * j)));
			DES_Encryption(Plaintext, Ciphertext, Key);
			cout << (num2[i][j] = compare(Ciphertext, cases[i].out)) << endl;
			num2_total[i] += num2[i][j];
		}
		cout << "密文分组平均改变的位数为:  " << (float(num1_total[i]) / 8) << endl;
	}
	for (int i = 0; i < 10; i++) {
		cout << "密文分组" << i << "平均改变的位数为:  " << (float(num1_total[i]) / 8) << endl;
	}



}
