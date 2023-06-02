#ifndef TRANSLATIONSLOAD_H_
#define TRANSLATIONSLOAD_H_
#include <array>
#include <fstream>
#include <string>
#include <iostream>
#include <map>
using namespace std;
class translationsLoad {
public:
	translationsLoad();
	string getTran(string Par, string lau) const;
private:
	map<string, array<string,11>>tranMap;//翻译表
	map<string, int>laug;//语言文本对应数组下标值
};

#endif