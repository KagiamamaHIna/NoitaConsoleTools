#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <io.h>
#include <windows.h>

using namespace std;

bool isNumber(const string& str) //判断你输入的字符串是否由纯数字组成
{
    for (char const& c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

bool isZero(string stringNum) {
    string cmpStr = "0";//初始化
    string ThisStr = "0";//初始化
    int count = 0;
    for (int i = 0; i <= stringNum.length(); i++) {//遍历字符
        cmpStr = stringNum.substr(i, 1);//截取字符
        if (cmpStr != "0" && !count) {
            count = 1;//当检测到第一个有效数字时更改这个变量来进行字符串存储
        }
        if (count && cmpStr != "0") {
            ThisStr = ThisStr + cmpStr;
        }
    }
    if (ThisStr.length() < 11) return true; else return false;//大小判断
}
int getTest(string& bufA) {
    if (bufA.length() < 10 && isNumber(bufA) || isZero(bufA)) //数字长度判断和零的判断，避免异常
    {
        if (isNumber(bufA)) {
            return std::stoi(bufA);//获取正确的整数
        }
        printf("参数不为纯数字，请重新输入！\n");
    }
    else if (!isNumber(bufA)) {//字符串过长不显示数字过大
        printf("参数不为纯数字，请重新输入！\n");
    }
    else {
        printf("数字过大(>999999999)，请重新输入！\n");
    }
    return -1;
}
int getNumber(const char* speaky) { //自定义一个询问并获取数字的函数
    string bufA;
    int reNum = -1;
    while (reNum == -1)
    {
        printf(speaky);
        getline(cin, bufA);
        reNum = getTest(bufA);
    }
    return reNum;
}
string getChar(const char* speaky) { //自定义一个询问并获取字符串的函数
    string bufA;
    printf(speaky);
    getline(cin, bufA);
    return bufA;
}

int getModMax(const char* speaky, int modMax, int outInt = 0) { //自定义一个询问并返回特定数字的函数
    string bufA;
    while (true)
    {
        printf(speaky);
        getline(cin, bufA);
        if (bufA == "y") {
            return modMax;
        }
        else if (bufA == "n") {
            return outInt;
        }
        else {
            printf("你需要输入的是y或n，而不是其他参数！\n");
        }
    }
}

string slashSwap(string str) { // 把\转换成/
    string buf = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] != '\\') {
            buf += str[i];
        }
        else {
            buf += '\\';
        }
    }
    return buf;
}

string toLower(string par) {
    transform(par.begin(), par.end(), par.begin(), ::tolower);
    return par;
}

string toUpper(string par) {
    transform(par.begin(), par.end(), par.begin(), ::toupper);
    return par;
}

string getFileName(string Path) {//从绝对路径里面获得文件名称
    int pos = Path.rfind("\\");
    if (pos == -1) {
        pos = Path.rfind("/");
    }
    string buf = "";
    for (int i = pos + 1; i < Path.length(); i++) {
        buf += Path[i];
    }
    return buf;
}

vector<string> getCommond(const char* ask) {//返回命令数组
    printf("%s", ask);
    string commondStr;
    getline(cin, commondStr);
    vector<string> comm;
    string bufStr = "";
    for (int i = 0; i < commondStr.length(); i++) {//分割参数
        if (commondStr[i] != ' ') {
            bufStr = bufStr + commondStr[i];
        }
        else {
            comm.push_back(bufStr);
            bufStr = "";
        }
    }
    comm.push_back(bufStr);//压入最后一个参数
    return comm;
}

string getFilePath(string Path) {//把字符串中的系统变量转换成对应的字符串
    string sysBuf = "";
    string ReturnStr = "";
    int i = 0;
    while (Path[i]) {//分割参数
        if (Path[i] != '%') {
            ReturnStr += Path[i];
        }
        else if (Path[i] == '%') {
            i++;
            while (Path[i] != '%') {
                sysBuf += Path[i];
                i++;
                if (i > Path.length() - 1) {
                    cerr << "错误:某字符串中的系统变量只有开始符没有结束符" << endl;
                    break;
                }
            }
            if (getenv(sysBuf.c_str())) {
                sysBuf = getenv(sysBuf.c_str());
            }
            ReturnStr += sysBuf;
            sysBuf = "";
        }
        i++;
    }
    return ReturnStr;
}

string delFirst(string par) {
    string ReturnStr = "";
    for (int i = 1; i < par.length(); i++) {//舍去开头
        ReturnStr += par[i];
    }
    return ReturnStr;
}

bool LineGetOut(string ask) {
    string bufA;
    while (true) {
        printf("%s", ask.c_str());
        getline(cin, bufA);
        if (bufA == "y") {
            return false;
        }
        else if (bufA == "n") {
            return true;
        }
        else {
            printf("你输入的不是y或者n，请重新输入\n");
        }
    }
}

string Stamp2Time(long long timestamp)//文件的时间戳转换为时间
{
    time_t tick = (time_t)(timestamp);
    struct tm tm;
    char s[40];
    localtime_s(&tm, &tick);
    strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
    return s;
}

void getSubdirs(string path, vector<struct _finddata_t>& files)//获得某一目录下的所有文件夹
{
    long long hFile = 0;
    struct _finddata_t fileinfo;
    std::string p;
    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    files.push_back(fileinfo);
            }
            else
            {
                ;
            }
        } while (_findnext(hFile, &fileinfo) == 0);

        _findclose(hFile);
    }
}

string GetExePath(void)
{
    char szFilePath[MAX_PATH + 1] = { 0 };
    GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
    (strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串  
    string path = szFilePath;
    return path;
}

int returnIntOrDef(string par, string num) {
    if (isNumber(num)) {
        return stoi(num);
    }
    printf("配置文件中的参数:%s，不为纯数字，返回默认值0", par.c_str());
    return 0;
}

string getPar(string str) {//获取参数
    int pos = str.find('"');
    string buf = "";
    for (int i = pos + 1; i < str.length(); i++) {
        if (str[i] != '"') {
            buf += str[i];
        }
        else if (i > str.length() - 1) return "error";
        else {
            return buf;//判断到直接返回值
        }
    }
}