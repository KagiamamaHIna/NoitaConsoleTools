#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <string>
#include <iostream>
using namespace std;

bool isNumber(const string& str);
bool isZero(string stringNum);
int getTest(string& bufA);
int getNumber(const char* speaky);
string getChar(const char* speaky);
int getModMax(const char* speaky, int modMax, int outInt = 0);
string slashSwap(string str);
string toLower(string par);
string toUpper(string par);
string getFileName(string Path);
vector<string> getCommond(const char* ask);
string getFilePath(string Path);
string delFirst(string par);
bool LineGetOut(string ask);
string Stamp2Time(long long timestamp);
void getSubdirs(string path, vector<struct _finddata_t>& files);
string GetExePath(void);
int returnIntOrDef(string par, string num);
string getPar(string str);

#endif