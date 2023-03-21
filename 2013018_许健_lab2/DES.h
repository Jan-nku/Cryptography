#ifndef DES_H
#define DES_H
#include<iostream>
using namespace std;

typedef unsigned char u_char;
u_char Plaintext[8], Ciphertext[8], Key[8];
typedef enum { encipher, decipher }OPTION;
int bit_get(const u_char* bits, int byte) {
    return ((bits[byte / 8] & (0x80 >> (byte % 8))) != 0);
}
void bit_set(u_char* bits, int byte, int num) {
	if (num == 1)
	{
		bits[byte / 8] |= (0x80 >> (byte % 8));
	}
	else {
		bits[byte / 8] &= ~(0x80 >> (byte % 8));
	}
}
void print_bits(u_char* bits, int byte) {
    for (int i = 0; i < byte; i++) {
        cout << bit_get(bits, i);
    }
    cout << endl;
}
static u_char subkeys[16][7];
static const int IP[64] =
{
    58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
    57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7
};
static const int IP_1[64] =
{
    40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26,33,1,41,9, 49,17,57,25
};
static const int LS[16] =
{
    1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};
static const int PC1[56] =
{
    57,49,41,33,25,17,9,1,58,50,42,34,26,18,
    10,2,59,51,43,35,27,19,11,3,60,52,44,36,
    63,55,47,39,31,23,15,7,62,54,46,38,30,22,
    14,6,61,53,45,37,29,21,13,5,28,20,12,4
};
static const int PC2[48] =
{
    14,17,11,24,1,5,3,28,15,6,21,10,
    23,19,12,4,26,8,16,7,27,20,13,2,
    41,52,31,37,47,55,30,40,51,45,33,48,
    44,49,39,56,34,53,46,42,50,36,29,32
};
static const int Ebox[48] =
{
    32,1,2,3,4,5,4,5,6,7,8,9,
    8,9,10,11,12,13,12,13,14,15,16,17,
    16,17,18,19,20,21,20,21,22,23,24,25,
    24,25,26,27,28,29,28,29,30,31,32,  1
};
static const int Sbox[8][4][16] =
{
    {
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},
    },
    {
        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},
    },
    {
        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},
    },
    {
        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},
    },
    {
        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},
    },
    {
        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},
    },
    {
        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},
    },
    {
        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11},
    }
};
static const int Pbox[32] =
{
    16,7,20,21,29,12,28,17,
    1,15,23,26,5,18,31,10,
    2,8,24,14,32,27,3,9,
    19,13,30,6,22,11,4,25
};
static struct des_test_case {
    int num, mode; // mode 1 = encrypt, 0 = decrypt
    unsigned char key[8], in[8], out[8];
} cases[] = {
    { 1, 1,     { 0x10, 0x31, 0x6E, 0x02, 0x8C, 0x8F, 0x3B, 0x4A },
                { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x82, 0xDC, 0xBA, 0xFB, 0xDE, 0xAB, 0x66, 0x02 } },
    { 2, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x95, 0xF8, 0xA5, 0xE5, 0xDD, 0x31, 0xD9, 0x00 },
                { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 3, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0xDD, 0x7F, 0x12, 0x1C, 0xA5, 0x01, 0x56, 0x19 },
                { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 4, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x2E, 0x86, 0x53, 0x10, 0x4F, 0x38, 0x34, 0xEA },
                { 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 5, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x4B, 0xD3, 0x88, 0xFF, 0x6C, 0xD8, 0x1D, 0x4F },
                { 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 6, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x20, 0xB9, 0xE7, 0x67, 0xB2, 0xFB, 0x14, 0x56 },
                { 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 7, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x55, 0x57, 0x93, 0x80, 0xD7, 0x71, 0x38, 0xEF },
                { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 8, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x6C, 0xC5, 0xDE, 0xFA, 0xAF, 0x04, 0x51, 0x2F },
                { 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 9, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x0D, 0x9F, 0x27, 0x9B, 0xA5, 0xD8, 0x72, 0x60 },
                { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    {10, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0xD9, 0x03, 0x1B, 0x02, 0x71, 0xBD, 0x5A, 0x0A },
                { 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

    { 1, 0,     { 0x10, 0x31, 0x6E, 0x02, 0x8C, 0x8F, 0x3B, 0x4A },
                { 0x82, 0xDC, 0xBA, 0xFB, 0xDE, 0xAB, 0x66, 0x02 },
                { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 2, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x95, 0xF8, 0xA5, 0xE5, 0xDD, 0x31, 0xD9, 0x00 } },
    { 3, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0xDD, 0x7F, 0x12, 0x1C, 0xA5, 0x01, 0x56, 0x19 } },
    { 4, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x2E, 0x86, 0x53, 0x10, 0x4F, 0x38, 0x34, 0xEA } },
    { 5, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x4B, 0xD3, 0x88, 0xFF, 0x6C, 0xD8, 0x1D, 0x4F } },
    { 6, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x20, 0xB9, 0xE7, 0x67, 0xB2, 0xFB, 0x14, 0x56 } },
    { 7, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x55, 0x57, 0x93, 0x80, 0xD7, 0x71, 0x38, 0xEF } },
    { 8, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x6C, 0xC5, 0xDE, 0xFA, 0xAF, 0x04, 0x51, 0x2F } },
    { 9, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x0D, 0x9F, 0x27, 0x9B, 0xA5, 0xD8, 0x72, 0x60 } },
    {10, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0xD9, 0x03, 0x1B, 0x02, 0x71, 0xBD, 0x5A, 0x0A } }
};

void permute(u_char* bits, const int* mapping, int byte)
{
    int n = byte / 8 + (byte % 8 != 0);
    u_char* tmp = new u_char[n];
    memset(tmp, 0, n);
    for (int i = 0; i < byte; i++) {
        bit_set(tmp, i, bit_get(bits, mapping[i] - 1));
    }
    memcpy(bits, tmp, n);
    return;
}
void bit_shift(u_char* bits, int byte, int num) {
    int n = byte / 8 + (byte % 8 != 0);
    int* table = new int[byte];
    for (int i = 0; i < byte; i++) {
        if (i < byte - num) {
            table[i] = i + num;
        }
        else {
            table[i] = i + num - byte;
        }
    }
    u_char* tmp = new u_char[n];
    memset(tmp, 0, n);
    for (int i = 0; i < byte; i++) {
        bit_set(tmp, i, bit_get(bits, table[i]));
    }
    for (int i = 0; i < byte; i++) {
        bit_set(bits, i, bit_get(tmp, i));
    }
    return;
}
void GetSubkeys(const u_char* key) {
    u_char temp[8];
    memset(temp, 0, 8);
    memcpy(temp, key, 8);
    permute(temp, PC1, 64);
    u_char lkey[4], rkey[4];
    memset(lkey, 0, 4);
    memset(rkey, 0, 4);
    for (int i = 0; i < 28; i++) {
        bit_set(lkey, i, bit_get(temp, i));
        bit_set(rkey, i, bit_get(temp, i + 28));
    }
    for (int i = 0; i < 16; i++) {
        bit_shift(lkey, 28, LS[i]);
        bit_shift(rkey, 28, LS[i]);
        for (int j = 0; j < 28; j++) {
            bit_set(subkeys[i], j, bit_get(lkey, j));
            bit_set(subkeys[i], j + 28, bit_get(rkey, j));
        }            
        permute(subkeys[i], PC2, 48);
    }
}
void DES_main(const u_char* source, u_char* target, const u_char* key, OPTION option) {
    GetSubkeys(key);
    u_char L[17][4] = { 0 }, R[17][4] = { 0 };
    u_char temp[8];
    memset(temp, 0, 8);
    memcpy(temp, source, 8);
    permute(temp, IP, 64);
    memcpy(L[0], temp, 4);
    memcpy(R[0], temp + 4, 4);
    for (int i = 1; i <= 16; i++) {
        memcpy(L[i], R[i-1], 4);
        u_char MR[6];
        memset(MR, 0, 6);
        memcpy(MR, R[i - 1], 4);
        permute(MR, Ebox, 48);
        u_char tmp1[6];
        memset(tmp1, 0, 6);
        int k;
        if (option == encipher) {
            k = i - 1;
        }
        else {
            k = 16 - i;
        }
        for (int j = 0; j < 48; j++) {
            int bit = 1;
            if (bit_get(MR, j) == bit_get(subkeys[k], j)) {
                bit = 0;
            }
            bit_set(tmp1, j, bit);
        }
        u_char func[4];
        for (int j = 0; j < 48; j += 6) {
            int x = bit_get(tmp1, j);
            x = x * 2 + bit_get(tmp1, j + 5);
            int y = bit_get(tmp1, j + 1);
            y = y * 2 + bit_get(tmp1, j + 2);
            y = y * 2 + bit_get(tmp1, j + 3);
            y = y * 2 + bit_get(tmp1, j + 4);
            int num = Sbox[j / 6][x][y];
            bit_set(func, (j / 6) * 4, ((num & 0x08) != 0));
            bit_set(func, (j / 6) * 4 + 1, ((num & 0x04) != 0));
            bit_set(func, (j / 6) * 4 + 2, ((num & 0x02) != 0));
            bit_set(func, (j / 6) * 4 + 3, ((num & 0x01) != 0));
        }
        permute(func, Pbox, 32);
        for (int j = 0; j < 32; j++) {
            int bit = 1;
            if (bit_get(func, j) == bit_get(L[i-1], j)) {
                bit = 0;
            }
            bit_set(R[i], j, bit);
        }
    }
    memcpy(target, R[16], 4);
    memcpy(target + 4, L[16], 4);
    permute(target, IP_1, 64);
}
void DES_Encryption(const u_char* plaintext, u_char* ciphertext, const u_char* key) {
    DES_main(plaintext, ciphertext, key, encipher);
    return;
}
void DES_Decryption(u_char* plaintext, const u_char* ciphertext, const u_char* key) {
    DES_main(ciphertext, plaintext, key, decipher);
    return;
}

#endif
