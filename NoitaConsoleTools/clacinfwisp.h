//将infwisp程序中的代码封装成一个类来使用
#ifndef CLACINFWISP_H_
#define CLACINFWISP_H_
#include <thread>
#include "cfgClass.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

#include "Functions.h"
using namespace std;
struct clacPar
{
    int startNum;
    int endNum;
    int addLT;
    int pinLT;
    int helLT;
    int arcLT;
    int decLT;
    int splLT;
    int nulLT;
};

void threadCalc(int mod, int startNum, int endNum, int isSaveOrNo, int addLT, int pinLT, int decLT, int helLT, int arcLT, int splLT, int ID, int nulLT);

class clacinfwisp {
    vector<thread> T;//动态存储线程
    vector<int> TNum;//动态存储分配后的参数
public:
    void clac(const cfgClass& cfg);
    void clacNoInput(const cfgClass& cfg, int startNum, int endNum, int SiwtchNum, int addLT, int pinLT, int decLT, int helLT, int arcLT, int splLT, int nulLT, string filePath);
};

#endif