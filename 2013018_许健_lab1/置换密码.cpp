#include <iostream>
#include <string>
#include<vector>
#include<ctime>
#include<iomanip>
#include <algorithm>
#include<map>
using namespace std;
//本次实验仅使用大写字母A-Z
string Plaintext;//明文
string Ciphertext;//密文
//Unknown字符串是要攻击的密文
string Unknown = "SIC GCBSPNA XPMHACQ JB GPYXSMEPNXIY JR SINS MF SPNBRQJSSJBE JBFMPQNSJMB FPMQ N XMJBS N SM N XMJBS H HY QCNBR MF N XMRRJHAY JBRCGZPC GINBBCA JB RZGI N VNY SINS SIC MPJEJBNA QCRRNEC GNB MBAY HC PCGMTCPCD HY SIC PJEISFZA PCGJXJCBSR SIC XNPSJGJXNBSR JB SIC SPNBRNGSJMB NPC NAJGC SIC MPJEJBNSMP MF SIC QCRRNEC HMH SIC PCGCJTCP NBD MRGNP N XMRRJHAC MXXMBCBS VIM VJRICR SM ENJB ZBNZSIMPJOCD GMBSPMA MF SIC QCRRNEC";
vector<char> PermutationTable(26);//置换表
//频率统计信息fre
char fre[26] = { 'e','t','o','i','a','n','s','r','h','l','d','u','c','m','p','y','f','g','w','b','v','k','x','j','q','z' };
//未知字符串的置换表
char UnknownTable[26] = { 'e','t','o','i','a','n','s','r','h','l','d','u','c','m','p','y','f','g','w','b','v','k','x','j','q','z' };
map<string, float>Unknown_map;
void init(vector<char>& PermutationTable) {
	vector<int>Array;
	for (int i = 0; i < 26; i++) {
		Array.push_back(i);
	}
	for (int i = 26; i > 0; i--)
	{
		srand(unsigned(time(NULL)));
		int index = rand() % i;
		PermutationTable[i - 1] = Array[index] + 65;
		Array.erase(Array.begin() + index);
	}
}
string Encryption(string plaintext) {
	for (auto& i : plaintext) {
		if (i >= 'A' && i <= 'Z') {
			i = PermutationTable[i - 65];
		}
	}
	return plaintext;
}
string Decryption(string ciphertext) {
	for (auto& i : ciphertext) {
		if (i >= 'A' && i <= 'Z') {
			for (int j = 0; j < 26; j++) {
				if (i == PermutationTable[j]) {
					i = 65 + j;
					break;
				}
			}
		}
	}
	return ciphertext;
}
static bool cmp(const pair<char, int>& a, const pair<char, int>& b)
{
	// 以pair对的第2个数的大小从大往小排序 
	return a.second > b.second;
}
void WordFrequency(string Unknown){
	string word;
	float word_count = 0;
	for (auto i : Unknown) {
		if (i >= 'A' && i <= 'Z') {
			word += i;
		}
		else {
			if (Unknown_map.find(word) == Unknown_map.end()) {
				Unknown_map.insert(pair<string, int>(word, 1));
			}
			else {
				word_count++;
				Unknown_map[word]++;
			}
			word = "";
		}
	}
	cout << "单词频率" << endl;
	auto iter = Unknown_map.begin();
	cout << "单词出现总次数 : " << word_count << endl;
	while (iter != Unknown_map.end()) {
		//if (iter->second != 1) {
		cout << iter->first << " : " << iter->second / word_count * 100 << endl;
		//}
		iter++;
	}
}

void CharFrequency(string Unknown) {
	float Count = 0;
	vector<pair<char, float>>frequency(26, { 'A',0 });
	for (int i = 0; i < 26; i++)
	{
		frequency[i].first += i;
	}
	for (auto i : Unknown) {
		if (i >= 'A' && i <= 'Z') {
			frequency[i - 65].second++;
		}
	}
	for (int i = 0; i < 26; i++) {
		Count += frequency[i].second;
	}
	cout << "单字母频率" << endl;
	sort(frequency.begin(), frequency.end(), cmp);
	cout << setiosflags(ios::fixed) << setprecision(2);
	for (int i = 0; i < 26; i++) {
		frequency[i].second = frequency[i].second / Count * 100;
		cout << frequency[i].first << ": " << frequency[i].second << "  ";
		if (i == 12 || i == 25) { cout << endl; }
	}
	for (auto& i : Unknown) {
		if (i >= 'A' && i <= 'Z') {
			for (int j = 0; j < 26; j++)
			{
				if (i == frequency[j].first) {
					i = fre[j];
					break;
				}
			}
		}
	}
	cout << Unknown << endl;
}

void frequencyCount(string Unknown) {
	CharFrequency(Unknown);
	WordFrequency(Unknown);
	
}

void test(string Unknown) {
	cout << Unknown << endl;
	for (auto& i : Unknown) {
		if (i == 'A')i = 'l';
		if (i == 'B')i = 'n';
		if (i == 'C')i = 'e';
		if (i == 'D')i = 'd';
		if (i == 'E')i = 'g';
		if (i == 'F')i = 'f';
		if (i == 'G')i = 'c';
		if (i == 'H')i = 'b';
		if (i == 'I')i = 'h';
		if (i == 'J')i = 'i';
		if (i == 'K')i = '?';
		if (i == 'L')i = '?';
		if (i == 'M')i = 'o';
		if (i == 'N')i = 'a';
		if (i == 'O')i = 'z';
		if (i == 'P')i = 'r';
		if (i == 'Q')i = 'm';
		if (i == 'R')i = 's';
		if (i == 'S')i = 't';
		if (i == 'T')i = 'v';
		if (i == 'U')i = '?';
		if (i == 'V')i = 'w';
		if (i == 'W')i = '?';
		if (i == 'X')i = 'p';
		if (i == 'Y')i = 'y';
		if (i == 'Z')i = 'u';
	}
	cout << Unknown<<endl;
}

int main()
{
	cout << "初始置换表" << endl;
	init(PermutationTable);
	for (int i = 0; i < 26; i++) {
		cout << char(65 + i) << ":" << PermutationTable[i] << "	";
		if (i == 12 || i == 25) { cout << endl; }
	}
	int flag;
	while (1) {
		cout << "欢迎使用置换密码系统，功能如下：" << endl;
		cout << "	1: 加密解密" << endl;
		cout << "	2: 攻击密文" << endl;
		cout << "	3: 重置置换表" << endl;
		cout << "	4: 退出" << endl;
		cout << "请输入要进行的操作:";
		(cin >> flag).get();
		switch (flag) {
		case 1:
			cout << "请输入要加密的字符串:";
			getline(cin, Plaintext);
			cout << "明文:" << Plaintext << endl;
			Ciphertext = Encryption(Plaintext);
			cout << "加密后:" << Ciphertext << endl;
			cout << "解密后:" << Decryption(Ciphertext) << endl;
			break;
		case 2:
			//frequencyCount(Unknown);
			test(Unknown);
			break;
		case 3:
			init(PermutationTable);
			cout << "重置置换表" << endl;
			for (int i = 0; i < 26; i++) {
				cout << char(65 + i) << ":" << PermutationTable[i] << "	";
				if (i == 12 || i == 25) { cout << endl; }
			}
			break;
		case 4:
			goto L;
			break;
		default:
			cerr << "输入非法，退出！" << endl;
			goto L;

		}
	}
L:
	return 0;
}