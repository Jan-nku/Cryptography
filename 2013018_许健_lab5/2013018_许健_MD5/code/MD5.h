#ifndef _MD5_H
#define _MD5_H
#include<iostream>
#include<vector>
#include<string>
using namespace std;

typedef unsigned int u_int;
#define MD5_A 0x67452301
#define MD5_B 0xefcdab89
#define MD5_C 0x98badcfe
#define MD5_D 0x10325476

const static u_int T[64] = {
    0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
    0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
    0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
    0x6b901122,0xfd987193,0xa679438e,0x49b40821,
    0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,
    0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
    0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,
    0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
    0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
    0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
    0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
    0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
    0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,
    0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
    0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
    0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
};

const static u_int Lshift[] = {
    7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
    5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
    4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
    6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21
};

class MD5 {
private:
    u_int A, B, C, D, Round;
    u_int F(u_int b, u_int c, u_int d) {
        return (b & c) | ((~b) & d);
    }
    u_int G(u_int b, u_int c, u_int d) {
        return (b & d) | (c & (~d));
    }
    u_int H(u_int b, u_int c, u_int d) {
        return b ^ c ^ d;
    }
    u_int I(u_int b, u_int c, u_int d) {
        return c ^ (b | (~d));
    }
    u_int shift(u_int a, u_int n) {
        return (a << n) | (a >> (32 - n));
    }
    vector<u_int> padding(string src) {
        Round = ((src.length() + 8) / 64) + 1;
        vector<u_int> rec(Round * 16);
        for (u_int i = 0; i < src.length(); i++) {
            rec[i >> 2] |= (int)(src[i]) << ((i % 4) * 8);
        }
        rec[src.length() >> 2] |= (0x80 << ((src.length() % 4) * 8));
        rec[rec.size() - 2] = (src.length() << 3);
        return rec;
    }
    string format(u_int num) {
        string res = "";
        const char str[] = "0123456789abcdef";
        u_int base = 1 << 8;
        for (int i = 0; i < 4; i++) {
            string tmp = "";
            u_int b = (num >> (i * 8)) % base & 0xff;
            for (int j = 0; j < 2; j++) {
                tmp = str[b % 16] + tmp;
                b /= 16;
            }
            res += tmp;
        }
        return res;
    }
    void iterateFunc(u_int* X) {
        u_int a = A, b = B, c = C, d = D;
        u_int g, k, tmp;
        for (int i = 0; i < 64; i++) {
            if (i < 16) {
                g = F(b, c, d);
                k = i;
            }
            else if (i < 32) {
                g = G(b, c, d);
                k = (1 + 5 * i) % 16;
            }
            else if (i < 48) {
                g = H(b, c, d);
                k = (5 + 3 * i) % 16;
            }
            else {
                g = I(b, c, d);
                k = (7 * i) % 16;
            }
            tmp = d;
            d = c;
            c = b;
            b = b + shift(a + g + X[k] + T[i], Lshift[i]);
            a = tmp;
        }
        A += a;
        B += b;
        C += c;
        D += d;
    }

public:
    string encode(string src) {
        A = MD5_A;
        B = MD5_B;
        C = MD5_C;
        D = MD5_D;
        vector<u_int> rec = padding(src);
        for (u_int i = 0; i < Round; i++) {
            u_int num[16];
            for (int j = 0; j < 16; j++) {
                num[j] = rec[i * 16 + j];
            }
            iterateFunc(num);
        }
        return format(A) + format(B) + format(C) + format(D);
    }
};



#endif // !_MD5_H
