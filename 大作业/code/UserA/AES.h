#ifndef AES_H
#define AES_H
#include<iostream>

typedef unsigned char u_char;
struct word {
	u_char byte[4];
};

//¥¢¥Ê√˜Œƒ°¢√‹Œƒ°¢√‹‘ø
extern u_char* Key;
extern word* Plaintext;
extern word* Ciphertext;


extern int Nb, Nk, Nr;

void WordtoByte(const word* state, u_char* result, int N);
void BytetoWord(word* state, const u_char* result, int N);
void printWord(word* state, int N);
void initRcon();
word RotWord(word w);
word SubWord(word w);
word WordXOR(word w1, word w2);
void KeyExpansion(const u_char* key, word* w);
u_char GFMultiplyByte(u_char L, u_char R);
void ByteSub(word* state);
void InvByteSub(word* state);
void ShiftRow(word* state);
void InvShiftRow(word* state);
void MixColumn(word* state);
void InvMixColumn(word* state);
void AddRoundKey(word* state, int round);

void AES_Encryption(const u_char* plaintext, u_char* ciphertext, const u_char* key);
void AES_Decryption(u_char* plaintext, const u_char* ciphertext, const u_char* key);

#endif // !AES_H

