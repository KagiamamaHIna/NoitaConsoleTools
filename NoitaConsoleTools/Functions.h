#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <string>
#include <iostream>
using namespace std;

bool isNumber(const string& str);//判断是否为纯数字
bool isZero(string stringNum);//判断0的
int getTest(string& bufA);//判断数字的，不合法返回-1
int getNumber(const char* speaky);//询问获取一个数字
string getChar(const char* speaky);//询问获取一个字符串
int getModMax(const char* speaky, int modMax, int outInt = 0);//询问返回两个特定的数字
string slashSwap(string str);//转换\/
string toLower(string par);//转成小写
string toUpper(string par);//转成大写
string getFileName(string Path);//从绝对路径里面获得文件名称
vector<string> getCommond(const char* ask);
string getFilePath(string Path);//把字符串中的系统变量转换成对应的字符串
string delFirst(string par);//删除最前面的字符
bool LineGetOut(string ask);//输入y或n返回假或真
string Stamp2Time(long long timestamp);//将时间戳转换成字符串
void getSubdirs(string path, vector<struct _finddata_t>& files);//获得某一目录下所有文件夹
string GetExePath(void);//获得当前程序运行路径
int returnIntOrDef(string par, string num);//字符串转数字
string getPar(string str);//将""里面的数据取出

#endif