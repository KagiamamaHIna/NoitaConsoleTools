#include <thread>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <array>

#include "cfgClass.h"
#include "Functions.h"
#include "clacinfwisp.h"

mutex mtx;

clock_t ClockStart, ClockEnd;
fstream file;
vector<fstream> fileCace;
const char* File = "infwispList.txt";
atomic<int>HasAnw = 0;

vector<vector<vector<int>>> infwispCache;

void threadCalc(int mod, int startNum, int endNum, int isSaveOrNo, int addLT, int pinLT, int decLT, int helLT, int arcLT, int splLT, int ID, int nulLT) {
    int out = 1;
    int YouNeedNum;
    int Count = 0;
    int test;
    int add2 = 0;
    int pin2 = 0;
    int hel2 = 0;
    int arc2 = 0;
    int nul2 = 0;
    if (pinLT != 0 && addLT == 0) {
        pin2 = mod;
    }
    else if (helLT != 0 && addLT == 0 && pinLT == 0) {
        hel2 = mod;
    }
    else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
        arc2 = mod;
    }
    else if (nulLT != 0 && arcLT == 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
        nul2 = mod;
    }
    else {
        add2 = mod;
    }
    if (splLT != 0 && decLT != 0) {
        //int PreNumI = -decLT * 42 - splLT * 30 + endNum;//预计算，减少性能消耗
        for (int add = add2; add <= addLT; add++)
        {
            for (int pin = pin2; pin <= pinLT; pin++)
            {
                for (int hel = hel2; hel <= helLT; hel++)
                {
                    for (int arc = arc2; arc <= arcLT; arc++)
                    {
                        for (int nul = nul2; nul <= nulLT; nul++)
                        {
                            for (int dec = 0; dec <= decLT; dec++)
                            {
                                if (Count > 1) { out = 1; }//重置out，避免优化算法永远不起作用
                                Count = 0;
                                for (int spl = 0; spl <= splLT; spl++)
                                {
                                    YouNeedNum = -(75 * add + 25 * pin + 50 * hel + 80 * arc - dec * 42 - spl * 30 + nul * 280);
                                    if (YouNeedNum < 0 && out) {
                                        test = startNum / 30;
                                        if (test == 1) { out = 0; }
                                        if (startNum % 30 == 0) { out = 0; }//如果不加这两个判断在特定条件下会陷入死循环
                                        if (Count > 1) { out = 0; }//卡死判断，即此处连续执行超过两次就退出
                                        spl = test - 1;
                                        Count++;
                                        continue;
                                    }
                                    if (YouNeedNum >= startNum + 1 && YouNeedNum <= endNum + 1) {//符合条件就是可以永久化的，+1是为了排除一些不合条件的选项
                                        HasAnw++;//如果有了可以永久化的结果自增
                                        if (isSaveOrNo) {
                                            fileCace[ID] << "蓝表数量:" << add << ",乒乓数量:" << pin << ",螺旋魔弹数量:" << hel << ",相位弧度数量:" << arc << ",红表数量:" << dec << ",连锁法术数量:" << spl << ",无害射击数量:" << nul << endl;
                                        }
                                        else {
                                            mtx.lock();
                                            printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d ,无害射击数量:%d \n", add, pin, hel, arc, dec, spl, nul);
                                            mtx.unlock();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (splLT == 0 && decLT != 0) {//方案2 只计算减去时间修正中的红表，计算红表的最小值，然后直接赋值跳过无用循环
        //int PreNumI = -decLT * 42 + endNum;//预计算，减少性能消耗
        for (int add = add2; add <= addLT; add++)
        {
            for (int pin = pin2; pin <= pinLT; pin++)
            {
                for (int hel = hel2; hel <= helLT; hel++)
                {
                    for (int arc = arc2; arc <= arcLT; arc++)
                    {
                        for (int nul = nul2; nul <= nulLT; nul++)
                        {
                            if (Count > 1) { out = 1; }
                            Count = 0;
                            for (int dec = 0; dec <= decLT; dec++)
                            {
                                YouNeedNum = -(75 * add + 25 * pin + 50 * hel + 80 * arc - dec * 42 + nul * 280);
                                if (YouNeedNum < 0 && out) {
                                    test = startNum / 42;
                                    if (test == 1) { out = 0; }
                                    if (startNum % 42 == 0) { out = 0; }
                                    if (Count > 1) { out = 0; }
                                    dec = test - 1;
                                    Count++;
                                    continue;
                                }
                                if (YouNeedNum >= startNum + 1 && YouNeedNum <= endNum + 1) {//符合条件就是可以永久化的，+1是为了排除一些不合条件的选项
                                    HasAnw++;//如果有了可以永久化的结果自增
                                    if (isSaveOrNo) {
                                        fileCace[ID] << "蓝表数量:" << add << ",乒乓数量:" << pin << ",螺旋魔弹数量:" << hel << ",相位弧度数量:" << arc << ",红表数量:" << dec << ",连锁法术数量:" << splLT << ",无害射击数量:" << nul << endl;
                                    }
                                    else {
                                        mtx.lock();
                                        printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d ,无害射击数量:%d \n", add, pin, hel, arc, dec, splLT, nul);
                                        mtx.unlock();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (splLT != 0 && decLT == 0) {//方案3 只计算减去时间修正中的连锁，计算连锁的最小值，然后直接赋值跳过无用循环
        //int PreNumI = -decLT * 30 + endNum;//预计算，减少性能消耗
        for (int add = add2; add <= addLT; add++)
        {
            for (int pin = pin2; pin <= pinLT; pin++)
            {
                for (int hel = hel2; hel <= helLT; hel++)
                {
                    for (int arc = arc2; arc <= arcLT; arc++)
                    {
                        for (int nul = nul2; nul <= nulLT; nul++)
                        {
                            if (Count > 1) { out = 1; }
                            Count = 0;
                            for (int spl = 0; spl <= splLT; spl++)
                            {
                                YouNeedNum = -(75 * add + 25 * pin + 50 * hel + 80 * arc - spl * 30 + nul * 280);
                                if (YouNeedNum < 0 && out) {
                                    test = startNum / 30;
                                    if (test == 1) { out = 0; }
                                    if (startNum % 30 == 0) { out = 0; }
                                    if (Count > 1) { out = 0; }
                                    spl = test - 1;
                                    Count++;
                                    continue;
                                }
                                if (YouNeedNum >= startNum + 1 && YouNeedNum <= endNum + 1) {//符合条件就是可以永久化的，+1是为了排除一些不合条件的选项
                                    HasAnw++;//如果有了可以永久化的结果自增
                                    if (isSaveOrNo) {
                                        fileCace[ID] << "蓝表数量:" << add << ",乒乓数量:" << pin << ",螺旋魔弹数量:" << hel << ",相位弧度数量:" << arc << ",红表数量:" << decLT << ",连锁法术数量:" << spl << ",无害射击数量:" << nul << endl;
                                    }
                                    else {
                                        mtx.lock();
                                        printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d ,无害射击数量:%d \n", add, pin, hel, arc, decLT, spl, nul);
                                        mtx.unlock();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}

    void clacinfwisp::clac(const cfgClass& cfg) {//addLT蓝表,pinLT乒乓,decLT红表，helLT螺旋魔弹,arcLT相位弧度,splLT连锁法术
        printf("注:乒乓回弹和盘旋魔弹影响的存在时间数值一样、相位传送和自身环绕(True Orbit)同理\n\n");
        int startNum, endNum, modMax, addLT, pinLT, decLT, helLT, arcLT, splLT, YouNeedNum, isSaveOrNo, isFileCustOrNo = 0;
        int closeNum = 1, test = 1, nulLT = 0;
        int Count = 0;
        int threadNum = 1;
        string testInt = cfg.getParameter("threadNum");
        if (testInt != "无参数") {
            int testNum = getTest(testInt);
            if (testNum != -1) {
                threadNum = testNum;
            }
            else {
                printf("上条是指threadNum参数出问题，接下来将使用默认值1\n");
            }
        }
        //为了减少性能消耗，获取一次赋值给一个变量
        string File2 = File;
        fileCace = vector<fstream>(threadNum - 1);//初始化
        while (true)
        {
            int threadNum2[2] = { 0,0 };//第一个存总数，第二个存余数
            int out = 1;
            HasAnw = 0;
            int isYes = 0;
            int threadCaceID = -1;
            int YouShouldNum;
            startNum = getNumber("输入投射物存在时间范围的起始值:");
            while (true) {
                endNum = getNumber("输入投射物存在时间范围的终止值:");
                if (startNum <= endNum) { //判断终止值不能小于起始值
                    break;
                }
                printf("终止值不能小于起始值。\n");
            }
            modMax = getNumber("输入所有影响存在时间修正的上限数:");
            isSaveOrNo = getModMax("如果不需要将数值存储到文件中输入n,需要则输入y:", 1, 0);
            if (isSaveOrNo) {
                if (closeNum == 0 && isFileCustOrNo == 1) { //判断是否自定义过路径并且已经运行过一遍的代码
                    isFileCustOrNo = getModMax("如果不需要更改文件路径输入n,需要则输入y:", 1, 0);
                }
                else {
                    isFileCustOrNo = getModMax("如果不需要自定义文件路径输入n,需要则输入y:", 1, 0);
                }
                if (isFileCustOrNo) {
                    File2 = getChar("请输入文件路径:");
                    File = File2.c_str();
                }
                printf("tips:接下来将不会在控制台内打印数据，而是将数据保存入根路径/%s 文件中\n", File);
            }
            addLT = getModMax("如果不需要计算蓝表输入n,需要计算输入y:", modMax);//根据你所输入的改变可循环的最大值，以此实现无需计算
            pinLT = getModMax("如果不需要计算乒乓输入n,需要计算输入y:", modMax);
            decLT = getModMax("如果不需要计算红表输入n,需要计算输入y:", modMax);
            helLT = getModMax("如果不需要计算螺旋魔弹输入n,需要计算输入y:", modMax);
            arcLT = getModMax("如果不需要计算相位输入n,需要计算输入y:", modMax);
            nulLT = getModMax("如果不需要计算无害射击(NULL SHOT)输入n,需要计算输入y:", modMax);//280
            splLT = getModMax("如果不需要计算连锁输入n,需要计算输入y:", modMax);
            if (isSaveOrNo) {
                file.open(File, ios::out | ios::app);
                file << "本次输入的存在时间范围为:" << startNum << "到" << endNum << ",总修正上限为" << modMax << endl;
                file << "蓝表上限为:" << addLT << endl;
                file << "乒乓上限为:" << pinLT << endl;
                file << "螺旋魔弹上限为:" << helLT << endl;
                file << "相位上限为:" << arcLT << endl;
                file << "红表上限为:" << decLT << endl;
                file << "连锁上限为:" << splLT << endl;
                file << "无害射击上限为:" << nulLT << endl;
            }
            //穷举计算 一共四种方案
            //方案1 两种减去时间的修正都计算，计算连锁的最小值，然后直接赋值跳过无用循环
            if (splLT != 0 && decLT != 0) {
                YouShouldNum = startNum / 42;
                if (decLT < YouShouldNum) {
                    isYes = 1;
                    goto calcEnd;
                }
                int LT;
                int PreNumI = -decLT * 42 - splLT * 30 + endNum;//预计算，减少性能消耗
                if (pinLT != 0 && addLT == 0) {
                    LT = ceil(-((float(PreNumI) - 25) / 25));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;//计算线程数和需要分配的参数
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    pinLT = TNum[0];
                }
                else if (helLT != 0 && addLT == 0 && pinLT == 0) {
                    LT = ceil(-((float(PreNumI) - 50) / 50));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    helLT = TNum[0];
                }
                else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                    LT = ceil(-((float(PreNumI) - 80) / 80));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    arcLT = TNum[0];
                }
                else if (nulLT != 0 && arcLT == 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                    LT = ceil(-((float(PreNumI) - 280) / 280));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    nulLT = TNum[0];
                }
                else {
                    LT = ceil(-((float(PreNumI) - 75) / 75));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    addLT = TNum[0];
                }
                if (threadNum > 1) {
                    T = vector<thread>(threadNum - 1); //初始化线程数量，主线程也算一个，所以-1
                    int count2[2] = { TNum[0],0 };
                    for (int i = 0; i < T.size(); i++) {//9 9 8 8(34)
                        threadCaceID++;
                        string FileID = File + to_string(threadCaceID);
                        if (isSaveOrNo) fileCace[threadCaceID].open(FileID, ios::out | ios::app);
                        count2[1] = count2[0] + 1;
                        count2[0] = count2[0] + TNum[i + 1];//算应该分配的范围
                        if (pinLT != 0 && addLT == 0) {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, count2[0], decLT, helLT, arcLT, splLT, threadCaceID, nulLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                        }
                        else if (helLT != 0 && addLT == 0 && pinLT == 0) {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, count2[0], arcLT, splLT, threadCaceID, nulLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                        }
                        else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, helLT, count2[0], splLT, threadCaceID, nulLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                        }
                        else if (nulLT != 0 && arcLT == 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, helLT, arcLT, splLT, threadCaceID, count2[0]);
                        }
                        else {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, count2[0], pinLT, decLT, helLT, arcLT, splLT, threadCaceID, nulLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                        }
                        //cout << count2[1] << "/0:" << count2[0] << endl;//debug用
                    }
                }
                ClockStart = clock();
                for (int add = 0; add <= addLT; add++)
                {/*
                    if (add * 75 + PreNumI >= 75) {//如果条件满足，即增加时间修正过大的话，就直接跳过下列所有结果直接输出，因为往下计算均是无效结果，>=75是为了减少一次减法运算带来的性能消耗
                        goto calcEnd;
                    }*/
                    for (int pin = 0; pin <= pinLT; pin++)
                    {/*
                        if (addLT == 0 && pin * 25 + PreNumI >= 25) {
                            goto calcEnd;
                        }*/
                        for (int hel = 0; hel <= helLT; hel++)
                        {/*
                            if (addLT == 0 && pinLT == 0 && hel * 50 + PreNumI >= 50) {
                                goto calcEnd;
                            }*/
                            for (int arc = 0; arc <= arcLT; arc++)
                            {
                                for (int nul = 0; nul <= nulLT; nul++)
                                {
                                    for (int dec = 0; dec <= decLT; dec++)
                                    {
                                        if (Count > 1) { out = 1; }//重置out，避免优化算法永远不起作用
                                        Count = 0;
                                        for (int spl = 0; spl <= splLT; spl++)
                                        {
                                            YouNeedNum = -(75 * add + 25 * pin + 50 * hel + 80 * arc - dec * 42 - spl * 30 + nul * 280);
                                            if (YouNeedNum < 0 && out) {
                                                test = startNum / 30;
                                                if (test == 1) { out = 0; }
                                                if (startNum % 30 == 0) { out = 0; }//如果不加这两个判断在特定条件下会陷入死循环
                                                if (Count > 1) { out = 0; }//卡死判断，即此处连续执行超过两次就退出
                                                spl = test - 1;
                                                Count++;
                                                continue;
                                            }
                                            if (YouNeedNum >= startNum + 1 && YouNeedNum <= endNum + 1) {//符合条件就是可以永久化的，+1是为了排除一些不合条件的选项
                                                HasAnw++;//如果有了可以永久化的结果自增
                                                if (isSaveOrNo) {
                                                    file << "蓝表数量:" << add << ",乒乓数量:" << pin << ",螺旋魔弹数量:" << hel << ",相位弧度数量:" << arc << ",红表数量:" << dec << ",连锁法术数量:" << spl << ",无害射击数量:" << nul << endl;
                                                }
                                                else {
                                                    mtx.lock();
                                                    printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d ,无害射击数量:%d \n", add, pin, hel, arc, dec, spl, nul);
                                                    mtx.unlock();
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if (splLT == 0 && decLT != 0) {//方案2 只计算减去时间修正中的红表，计算红表的最小值，然后直接赋值跳过无用循环
                YouShouldNum = startNum / 42;
                if (decLT < YouShouldNum) {
                    isYes = 1;
                    goto calcEnd;
                }
                int PreNumI = -decLT * 42 + endNum;//预计算，减少性能消耗
                int LT;
                if (pinLT != 0 && addLT == 0) {
                    LT = ceil(-((float(PreNumI) - 25) / 25));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;//计算线程数和需要分配的参数
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    pinLT = TNum[0];
                }
                else if (helLT != 0 && addLT == 0 && pinLT == 0) {
                    LT = ceil(-((float(PreNumI) - 50) / 50));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    helLT = TNum[0];
                }
                else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                    LT = ceil(-((float(PreNumI) - 80) / 80));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    arcLT = TNum[0];
                }
                else if (nulLT != 0 && arcLT == 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                    LT = ceil(-((float(PreNumI) - 280) / 280));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    nulLT = TNum[0];
                }
                else {
                    LT = ceil(-((float(PreNumI) - 75) / 75));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    addLT = TNum[0];
                }
                if (threadNum > 1) {
                    T = vector<thread>(threadNum - 1); //初始化线程数量，主线程也算一个，所以-1
                    int count2[2] = { TNum[0],0 };
                    for (int i = 0; i < T.size(); i++) {//9 9 8 8(34)
                        threadCaceID++;
                        string FileID = File + to_string(threadCaceID);
                        if (isSaveOrNo) fileCace[threadCaceID].open(FileID, ios::out | ios::app);
                        count2[1] = count2[0] + 1;
                        count2[0] = count2[0] + TNum[i + 1];//算应该分配的范围
                        if (pinLT != 0 && addLT == 0) {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, count2[0], decLT, helLT, arcLT, splLT, threadCaceID, nulLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                        }
                        else if (helLT != 0 && addLT == 0 && pinLT == 0) {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, count2[0], arcLT, splLT, threadCaceID, nulLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                        }
                        else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, helLT, count2[0], splLT, threadCaceID, nulLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                        }
                        else if (nulLT != 0 && arcLT == 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, helLT, arcLT, splLT, threadCaceID, count2[0]);
                        }
                        else {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, count2[0], pinLT, decLT, helLT, arcLT, splLT, threadCaceID, nulLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                        }
                    }
                }
                ClockStart = clock();
                for (int add = 0; add <= addLT; add++)
                {
                    for (int pin = 0; pin <= pinLT; pin++)
                    {
                        for (int hel = 0; hel <= helLT; hel++)
                        {
                            for (int arc = 0; arc <= arcLT; arc++)
                            {
                                for (int nul = 0; nul <= nulLT; nul++)
                                {
                                    if (Count > 1) { out = 1; }
                                    Count = 0;
                                    for (int dec = 0; dec <= decLT; dec++)
                                    {
                                        YouNeedNum = -(75 * add + 25 * pin + 50 * hel + 80 * arc - dec * 42 + nul * 280);
                                        if (YouNeedNum < 0 && out) {
                                            test = startNum / 42;
                                            if (test == 1) { out = 0; }
                                            if (startNum % 42 == 0) { out = 0; }
                                            if (Count > 1) { out = 0; }
                                            dec = test - 1;
                                            Count++;
                                            continue;
                                        }
                                        if (YouNeedNum >= startNum + 1 && YouNeedNum <= endNum + 1) {//符合条件就是可以永久化的，+1是为了排除一些不合条件的选项
                                            HasAnw++;//如果有了可以永久化的结果自增
                                            if (isSaveOrNo) {
                                                file << "蓝表数量:" << add << ",乒乓数量:" << pin << ",螺旋魔弹数量:" << hel << ",相位弧度数量:" << arc << ",红表数量:" << dec << ",连锁法术数量:" << splLT << ",无害射击数量:" << nul << endl;
                                            }
                                            else {
                                                mtx.lock();
                                                printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d ,无害射击数量:%d \n", add, pin, hel, arc, dec, splLT, nul);
                                                mtx.unlock();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if (splLT != 0 && decLT == 0) {//方案3 只计算减去时间修正中的连锁，计算连锁的最小值，然后直接赋值跳过无用循环
                YouShouldNum = startNum / 30;
                if (splLT < YouShouldNum) {
                    isYes = 1;
                    goto calcEnd;
                }
                int PreNumI = -decLT * 30 + endNum;//预计算，减少性能消耗
                int LT;
                if (pinLT != 0 && addLT == 0) {
                    LT = ceil(-((float(PreNumI) - 25) / 25));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;//计算线程数和需要分配的参数
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    pinLT = TNum[0];
                }
                else if (helLT != 0 && addLT == 0 && pinLT == 0) {
                    LT = ceil(-((float(PreNumI) - 50) / 50));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    helLT = TNum[0];
                }
                else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                    LT = ceil(-((float(PreNumI) - 80) / 80));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    arcLT = TNum[0];
                }
                else if (nulLT != 0 && arcLT == 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                    LT = ceil(-((float(PreNumI) - 280) / 280));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    nulLT = TNum[0];
                }
                else {
                    LT = ceil(-((float(PreNumI) - 75) / 75));//获取上限修正
                    TNum = vector<int>(threadNum);
                    threadNum2[0] = LT / threadNum;
                    threadNum2[1] = LT % threadNum;
                    for (int i = 0; i < threadNum; i++) {
                        TNum[i] = threadNum2[0];
                    }
                    if (threadNum2[1] > 0) {
                        for (int i = 0; i < threadNum2[1]; i++) {
                            TNum[i]++;
                        }
                    }
                    addLT = TNum[0];
                }
                if (threadNum > 1) {
                    T = vector<thread>(threadNum - 1); //初始化线程数量，主线程也算一个，所以-1
                    int count2[2] = { TNum[0],0 };
                    for (int i = 0; i < T.size(); i++) {//9 9 8 8(34)
                        threadCaceID++;
                        string FileID = File + to_string(threadCaceID);
                        if (isSaveOrNo) fileCace[threadCaceID].open(FileID, ios::out | ios::app);
                        count2[1] = count2[0] + 1;
                        count2[0] = count2[0] + TNum[i + 1];//算应该分配的范围
                        if (pinLT != 0 && addLT == 0) {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, count2[0], decLT, helLT, arcLT, splLT, threadCaceID, nulLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                        }
                        else if (helLT != 0 && addLT == 0 && pinLT == 0) {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, count2[0], arcLT, splLT, threadCaceID, nulLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                        }
                        else if (arcLT != 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, helLT, count2[0], splLT, threadCaceID, nulLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                        }
                        else if (nulLT != 0 && arcLT == 0 && addLT == 0 && pinLT == 0 && helLT == 0) {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, addLT, pinLT, decLT, helLT, arcLT, splLT, threadCaceID, count2[0]);
                        }
                        else {
                            T[i] = thread(threadCalc, count2[1], startNum, endNum, isSaveOrNo, count2[0], pinLT, decLT, helLT, arcLT, splLT, threadCaceID, nulLT);//int mod,int modLT,int startNum,int endNum,int isSaveOrNo,int addLT, int pinLT,int decLT,int helLT,int arcLT,int splLT
                        }
                    }
                }
                ClockStart = clock();
                for (int add = 0; add <= addLT; add++)
                {
                    for (int pin = 0; pin <= pinLT; pin++)
                    {
                        for (int hel = 0; hel <= helLT; hel++)
                        {
                            for (int arc = 0; arc <= arcLT; arc++)
                            {
                                for (int nul = 0; nul <= nulLT; nul++)
                                {
                                    if (Count > 1) { out = 1; }
                                    Count = 0;
                                    for (int spl = 0; spl <= splLT; spl++)
                                    {
                                        YouNeedNum = -(75 * add + 25 * pin + 50 * hel + 80 * arc - spl * 30);
                                        if (YouNeedNum < 0 && out) {
                                            test = startNum / 30;
                                            if (test == 1) { out = 0; }
                                            if (startNum % 30 == 0) { out = 0; }
                                            if (Count > 1) { out = 0; }
                                            spl = test - 1;
                                            Count++;
                                            continue;
                                        }
                                        if (YouNeedNum >= startNum + 1 && YouNeedNum <= endNum + 1) {//符合条件就是可以永久化的，+1是为了排除一些不合条件的选项
                                            HasAnw++;//如果有了可以永久化的结果自增
                                            if (isSaveOrNo) {
                                                file << "蓝表数量:" << add << ",乒乓数量:" << pin << ",螺旋魔弹数量:" << hel << ",相位弧度数量:" << arc << ",红表数量:" << decLT << ",连锁法术数量:" << spl << ",无害射击数量:" << nul << endl;
                                            }
                                            else {
                                                mtx.lock();
                                                printf("蓝表数量:%d ,乒乓数量:%d ,螺旋魔弹数量:%d ,相位弧度数量:%d ,红表数量:%d ,连锁法术数量:%d ,无害射击数量:%d \n", add, pin, hel, arc, decLT, spl, nul);
                                                mtx.unlock();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }//如果条件均不满足，那么就不循环了，直接结束，也就是没有减存在时间的法术的时候
        calcEnd:ClockEnd = clock();
            if (threadNum > 1 && !isYes) {
                char buf[512] = { '\0' };
                for (int i = 0; i < T.size(); i++) {
                    T[i].join();//释放线程
                }
                if (isSaveOrNo) {
                    for (int i = 0; i < threadNum - 1; i++) {//合并文件
                        string FileID = File + to_string(i);
                        fileCace[i].close();//关闭
                        fileCace[i].open(FileID, ios::in);//以读的形式重新打开
                        while (fileCace[i].getline(buf, sizeof(buf))) {//按行读取文件
                            file << buf << endl;//写入数据
                        }
                        fileCace[i].close();//现在关闭文件
                        remove(FileID.c_str());//删除文件
                    }
                }
            }
            float time = float(ClockEnd - ClockStart) / 1000;//我将原本的换成了1000作为常量，因为我听说其他情况机子跑编译的情况下可能不为1000
            if (isYes) {
                printf("共耗时：0.00000s，你输入的修正数过少，建议在等条件的情况下输入%d\n", YouShouldNum);
            }
            else {
                printf("共耗时：%.5fs\n", time);
            }
            int HasAnw2 = HasAnw;
            if (HasAnw2 == 0) {//有了可以永久化的结果就按条件输出语句
                printf("这次穷举没有可以永久化的结果，你输入的存在时间范围为: %d 到 %d \n\n", startNum, endNum);
                if (isSaveOrNo) {
                    file << "本次穷举没有可以永久化的结果:(" << endl;
                    file.close();
                }
            }
            else {
                printf("这次穷举有可以永久化的结果，结果数量为:%d，你输入的存在时间范围为: %d 到 %d \n\n", HasAnw2, startNum, endNum);
                if (isSaveOrNo) {
                    file << "共计" << HasAnw2 << "个结果" << endl;
                    file.close();
                }
            }
            closeNum = getModMax("如果要继续计算请输入y, 退出输入n:", 0, 1);
            if (closeNum) {
                break;
            }
        }
        return;
    }