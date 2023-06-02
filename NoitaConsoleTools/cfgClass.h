#ifndef CFGCLASS_H_
#define CFGCLASS_H_
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
using namespace std;
class cfgClass {
public:
	cfgClass();
	string getParameter(string) const;
private:
	vector<string> cfgStr;//定义一个用于存储参数的字符串向量
	vector<string> cfgNumInt1;
	vector<int> cfgNumInt2;//累加重复次数的
};

#endif