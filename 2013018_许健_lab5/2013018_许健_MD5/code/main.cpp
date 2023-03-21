#include"MD5.h"
#include<iostream>
#include<cstdlib>
using namespace std;

//int num[4];
//int compare(int* num1, int*num2 ) {
//    int num = 0;
//    for (int i = 0; i < 4; i++) {
//        for (int j = 0; j < 32; j++) {
//            int a = num1[i] & (1 << j);
//            int b = num2[i] & (1 << j);
//            if (a != b) {
//                num++;
//            }
//        }
//    }
//    return num;
//}

int main() {
    MD5 test;
    while (1) {
        cout << "Text: ";
        string a = "";
        getline(cin, a);
        cout << "MD5: " << test.encode(a) << endl;
    }
}

