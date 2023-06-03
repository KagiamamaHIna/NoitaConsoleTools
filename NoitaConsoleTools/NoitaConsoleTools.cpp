#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <map>
#include <direct.h>
#include <io.h>
#include <algorithm>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>

#include "cfgClass.h"
#include "translationsLoad.h"
#include "Functions.h"
#include "clacinfwisp.h"
#include "BoneFileRead.h"

using namespace std;

extern string File;
extern mutex mtx;
extern atomic<int>HasAnw;
extern vector<vector<vector<int>>> infwispCache;
extern vector<int> HasAnwThread;
extern int startClac;
extern clacPar clacA;

bool autoSaveOpenOrNo = false;
atomic<int> timeCount = 0;
bool clearCache = true;

void autoSaveFun(int time,string saveName,const cfgClass *cfgObj) {
    autoSaveOpenOrNo = true;
    int timeCountLT = time*60;//分钟换算成秒
    while (autoSaveOpenOrNo) {
        this_thread::sleep_for(std::chrono::seconds(1));
        timeCount++;
        if (timeCount == timeCountLT) {
            printf("\n自动保存开始...\n");
            timeCount = 0;
            string buf = GetExePath() + getFilePath(cfgObj->getParameter("savePath")) + "\\" + getFilePath(saveName);
            if (_access(buf.c_str(), 0) != -1) {//如果有了该文件夹，那么就删除然后重新生成
                string buf2 = "rmdir " + GetExePath() + "\\" + delFirst(getFilePath(cfgObj->getParameter("savePath")) + "\\" + saveName + " /s /q");
                system(buf2.c_str());
                buf2 = delFirst(getFilePath(cfgObj->getParameter("savePath")) + "\\" + getFilePath(saveName));
                printf("自动保存:原保存的名为%s的存档已删除\n", saveName.c_str());
                //int flag = _rmdir(buf2.c_str());//删除文件夹
                int flag = _mkdir(buf2.c_str());//生成文件夹
            }
            else {//如果没有该文件夹，那么就生成一个
                string buf2 = delFirst(getFilePath(cfgObj->getParameter("savePath")) + "\\" + getFilePath(saveName));
                int flag = _mkdir(buf2.c_str());//生成文件夹
            }
            string copyFile = "xcopy " + getFilePath(cfgObj->getParameter("save00Path")) + "\\save00 " + GetExePath() + getFilePath(cfgObj->getParameter("savePath") + "\\" + saveName) + " /s /f /h /q /y";
            system(copyFile.c_str());
            printf("自动保存:名为%s的存档保存完成！\n", saveName.c_str());
            printf("下一次自动保存将在%d分钟后开始\n输入指令:", time);
        }
    }
    timeCount = 0;
    return;
}

int main()
{
    SetConsoleTitle(L"Noita控制台多功能工具v1.0.3");
    system("chcp 65001");//改字符编码
    system("cls");
    const translationsLoad tranObj;
    const cfgClass cfgObj;
    BoneFileRead BoneFile = BoneFileRead(getFilePath(cfgObj.getParameter("save00Path")) + "\\save00\\persistent\\bones_new", tranObj);
    string A = GetExePath() + getFilePath(cfgObj.getParameter("savePath"));//路径
    thread autoSave;
    int saveTime = 0;;
    const char* dir = A.c_str();
    if (_access(dir, 0) == -1) { //判断该文件夹是否存在 ==-1为不存在
        int flag = _mkdir(delFirst(getFilePath(cfgObj.getParameter("savePath"))).c_str());//生成文件夹
    }
    printf("输入help查看帮助 版本为v1.0.3\n");
    printf("本程序的Github仓库链接:https://github.com/KagiamamaHIna/NoitaConsoleTools 可以前来下最新版本或者查看源代码\n本程序使用MIT许可证\n\n");
    clacinfwisp clacwisp;
    while (true) {
        vector<string> Commond = getCommond("输入指令:");
        if (Commond[0] == "save") {//save指令
            bool out = false;
            if (Commond.size() < 2) {//默认文件夹
                string buf = GetExePath() + getFilePath(cfgObj.getParameter("savePath")) + "\\def";
                if (_access(buf.c_str(), 0) != -1) {//如果有了该文件夹，那么就删除然后重新生成
                    bool out = LineGetOut("已检测到默认存储文件夹内已有存档，输入y会删除其中所有文件然后再复制, 输入n则放弃这次存档:");
                    if (out) {
                        goto out;
                    }
                    string buf2 = "rmdir " + GetExePath() + "\\" + delFirst(getFilePath(cfgObj.getParameter("savePath")) + "\\def /s /q");
                    system(buf2.c_str());
                    //int flag = _rmdir(buf2.c_str());//删除文件夹
                    buf2 = delFirst(getFilePath(cfgObj.getParameter("savePath")) + "/def");
                    printf("原保存的默认存档已删除\n");
                    int flag = _mkdir(buf2.c_str());//生成文件夹
                }
                else {//如果没有该文件夹，那么就生成一个
                    string buf2 = delFirst(getFilePath(cfgObj.getParameter("savePath")) + "/def");
                    int flag = _mkdir(buf2.c_str());//生成文件夹
                }
                string copyFile = "xcopy " + getFilePath(cfgObj.getParameter("save00Path")) + "\\save00 " + GetExePath() + getFilePath(cfgObj.getParameter("savePath")) + "\\def" + " /s /f /h /q /y";
                system(copyFile.c_str());
                printf("默认存档保存完成！\n");
            }
            else {//自定义文件夹
                string buf = GetExePath() + getFilePath(cfgObj.getParameter("savePath")) + "\\" + getFilePath(Commond[1]);
                if (_access(buf.c_str(), 0) != -1) {//如果有了该文件夹，那么就删除然后重新生成
                    bool out = LineGetOut("已检测到自定义存储文件夹内已有存档，输入y会删除其中所有文件然后再复制, 输入n则放弃这次存档:");
                    if (out) {
                        goto out;
                    }
                    string buf2 = "rmdir " + GetExePath() + "\\" + delFirst(getFilePath(cfgObj.getParameter("savePath")) + "\\" + Commond[1] + " /s /q");
                    system(buf2.c_str());
                    buf2 = delFirst(getFilePath(cfgObj.getParameter("savePath")) + "\\" + getFilePath(Commond[1]));
                    printf("原保存的名为%s的存档已删除\n", Commond[1].c_str());
                    //int flag = _rmdir(buf2.c_str());//删除文件夹
                    int flag = _mkdir(buf2.c_str());//生成文件夹
                }
                else {//如果没有该文件夹，那么就生成一个
                    string buf2 = delFirst(getFilePath(cfgObj.getParameter("savePath")) + "\\" + getFilePath(Commond[1]));
                    int flag = _mkdir(buf2.c_str());//生成文件夹
                }
                string copyFile = "xcopy " + getFilePath(cfgObj.getParameter("save00Path")) + "\\save00 " + GetExePath() + getFilePath(cfgObj.getParameter("savePath") + "\\" + Commond[1]) + " /s /f /h /q /y";
                system(copyFile.c_str());
                printf("名为%s的存档保存完成！\n", Commond[1].c_str());
            }
        out:;
        }
        else if (Commond[0] == "load") {//加载
            if (Commond.size() < 2) {
                string buf3 = GetExePath() + getFilePath(cfgObj.getParameter("savePath")) + "\\def";
                if (_access(buf3.c_str(), 0) == -1) {//如果没有文件夹，就停止读取
                    printf("警告：没有检测默认存档，停止读取\n");
                    continue;
                }
            }
            else {
                string buf3 = GetExePath() + getFilePath(cfgObj.getParameter("savePath")) + "\\" + Commond[1];
                if (_access(buf3.c_str(), 0) == -1) {//如果没有文件夹，就停止读取
                    printf("警告：没有检测到名为%s的存档，停止读取\n", Commond[1].c_str());
                    continue;
                }
            }
            string buf = getFilePath(cfgObj.getParameter("save00Path")) + "\\save00";//先删除
            int unload = returnIntOrDef("unload", cfgObj.getParameter("unload"));
            if (unload) {
                string A = GetExePath() + "\\unloadSave";//路径
                const char* dir = A.c_str();
                if (_access(dir, 0) == -1) { //判断该文件夹是否存在 ==-1为不存在
                    int flag = _mkdir("unloadSave");//生成文件夹
                }
                else {
                    string buf2 = "rmdir " + GetExePath() + "\\" + "unloadSave " + " /s /q";
                    system(buf2.c_str());
                    int flag = _mkdir("unloadSave");//生成文件夹
                }
                string copyFile = "xcopy " + getFilePath(cfgObj.getParameter("save00Path")) + "\\save00 " + A + " /s /f /h /q /y";
                system(copyFile.c_str());
                printf("后悔措施已准备！（输入unload即可读取刚才被覆盖掉的存档)\n");
            }
            if (_access(buf.c_str(), 0) != -1) {//如果有了该文件夹，那么就删除然后重新生成
                string buf2 = "rmdir " + buf + " /s /q";
                system(buf2.c_str());
                //int flag = _rmdir(buf.c_str());//删除文件夹和里面的文件
                int flag = _mkdir(buf.c_str());//重新生成文件夹
            }
            if (Commond.size() < 2) {//按条件复制
                string copyFile = "xcopy " + GetExePath() + getFilePath(cfgObj.getParameter("savePath")) + "\\def " + getFilePath(cfgObj.getParameter("save00Path")) + "\\save00" + " /s /f /h /q /y";
                system(copyFile.c_str());
                printf("读取完成！\n");
            }
            else {
                string copyFile = "xcopy " + GetExePath() + getFilePath(cfgObj.getParameter("savePath") + "\\" + Commond[1]) + " " + getFilePath(cfgObj.getParameter("save00Path")) + "\\save00" + " /s /f /h /q /y";
                system(copyFile.c_str());
                printf("读取完成！\n");
            }
        }
        else if (Commond[0] == "unload") {
            string A = GetExePath() + "\\unloadSave";//路径
            const char* dir = A.c_str();
            if (_access(dir, 0) == -1) { //判断该文件夹是否存在 ==-1为不存在
                printf("unload存档不存在，停止恢复\n");
            }
            else {
                string buf = getFilePath(cfgObj.getParameter("save00Path")) + "\\save00";//先删除
                string buf2 = "rmdir " + buf + " /s /q";
                system(buf2.c_str());
                int flag = _mkdir(buf.c_str());//重新生成文件夹
                string copyFile = "xcopy " + GetExePath() + "\\unloadSave " + getFilePath(cfgObj.getParameter("save00Path")) + "\\save00" + " /s /f /h /q /y";
                system(copyFile.c_str());
                printf("恢复完成！\n");
            }
        }
        else if (Commond[0] == "savelist") {
            int count114514 = 0;
            vector<struct _finddata_t> files;
            vector<struct _finddata_t> save00FilePath;
            fstream readFile;
            bool readEnd = true;
            string buf;
            string bufPar;
            string saveFile = GetExePath() + getFilePath(cfgObj.getParameter("savePath"));
            string save00File = getFilePath(cfgObj.getParameter("save00Path")) + "\\save00";
            getSubdirs(saveFile, files);
            printf("\n关于游戏本身的存档:\n");
            if (_access(save00File.c_str(), 0) == -1) {
                printf("未成功找到Noita本身的游戏存档\n");
            }
            else {
                getSubdirs(getFilePath(cfgObj.getParameter("save00Path")), save00FilePath);
                readFile.open(save00File + "\\world_state.xml", ios::in);
                while (getline(readFile, buf) && readEnd) {
                    if (buf.find("session_stat_file") != -1) {//session_stat_file存储了一些状态文件的路径，这里需要获得
                        bufPar = getFilePath(cfgObj.getParameter("save00Path")) + "\\save00\\stats\\sessions\\" + getFileName(getPar(buf)) + "_stats.xml";
                        readFile.close();
                        readFile.open(bufPar, ios::in);
                        getline(readFile, bufPar);//这个时间+_stats.xml的文件第一行就是需要的内容，所以只需要读取第一行""之间的数据就行
                        bufPar = getPar(bufPar);
                        readFile.close();
                        readEnd = false;
                    }
                }
                for (int i = 0; i < save00FilePath.size(); i++) {
                    string testStr = save00FilePath[i].name;
                    if (testStr == "save00") {
                        if (bufPar == "error") {
                            printf("游戏版本:读取错误\n文件夹创建时间:%s", Stamp2Time(save00FilePath[i].time_create).c_str());
                        }
                        else {
                            printf("游戏版本:%s\n文件夹创建时间:%s", bufPar.c_str(), Stamp2Time(save00FilePath[i].time_create).c_str());
                        }
                        break;
                    }
                }
            }
            printf("\n\n本程序所存的存档总数:%d\n\n", (int)files.size());
            for (int i = 0; i < files.size(); i++) {
                count114514 = 1;
                string saveFileBuf = GetExePath() + getFilePath(cfgObj.getParameter("savePath")) + "\\" + files[i].name;
                readFile.open(saveFileBuf + "\\world_state.xml", ios::in);
                readEnd = true;
                while (getline(readFile, buf) && readEnd) {
                    if (buf.find("session_stat_file") != -1) {
                        bufPar = saveFileBuf+ "\\stats\\sessions\\" + getFileName(getPar(buf)) + "_stats.xml";
                        readFile.close();
                        readFile.open(bufPar, ios::in);
                        getline(readFile, bufPar);
                        bufPar = getPar(bufPar);
                        readFile.close();
                        readEnd = false;
                    }
                }
                if (bufPar == "error") {
                    printf("%d.%s\n游戏版本:读取错误\n存储时间:%s\n", i + 1, files[i].name, Stamp2Time(files[i].time_create).c_str());
                }
                else {
                    printf("%d.%s\n游戏版本:%s\n存储时间:%s\n", i + 1, files[i].name,bufPar.c_str(), Stamp2Time(files[i].time_create).c_str());
                }

            }
            if (count114514) {
                printf("\n");
            }
        }
        else if (Commond[0] == "delsave") {
            if (Commond.size() > 1) {
                string delPath = GetExePath() + getFilePath(cfgObj.getParameter("savePath")) + "\\" + Commond[1];//路径
                const char* delPathdir = delPath.c_str();
                if (_access(delPathdir, 0) == -1) { //判断该文件夹是否存在 ==-1为不存在
                    printf("错误:存档%s不存在\n", Commond[1].c_str());
                }
                else {
                    bool isDelorNo = LineGetOut("请输入y确认删除，输入n则不删除该存档:");
                    if (isDelorNo) {
                        goto No;
                    }
                    string delPathbuf2 = "rmdir " + delPath + " /s /q";
                    system(delPathbuf2.c_str());
                    printf("存档%s删除完成!\n", Commond[1].c_str());
                }
            }
            else {
                printf("错误:缺失第二个参数，即存档名\n");
            }
        No:;
        }
        else if (Commond[0] == "bonelist") {
            BoneFile.getWandList();
            printf("使用指令:bone (对应文件序号) 读取对应老古法杖文件\n");
        }
        else if (Commond[0] == "bone") {
            if (Commond.size() > 1) {
                BoneFile.getWand(Commond[1]);
            }
            else {
                printf("错误:缺失第二个参数，是老古法杖序号\n");
            }
        }
        else if (Commond[0] == "rebonelist") {
            BoneFile = BoneFileRead(getFilePath(cfgObj.getParameter("save00Path")) + "\\save00\\persistent\\bones_new", tranObj);
            printf("老古的法杖文件已经重新加载完成！\n");
        }
        else if (Commond[0] == "autosave") {
            if (!autoSaveOpenOrNo) {
                if (Commond.size() == 2) {//第二个为存档名，第三个为时间
                    autoSave = thread(autoSaveFun, 30, Commond[1], &cfgObj);
                    autoSave.detach();
                    saveTime = 30;
                    printf("已启动自动存档，将每隔30分钟存储一次名为%s的存档\n", Commond[1].c_str());
                }
                else if (Commond.size() > 2) {
                    if (Commond[2].length() < 10 && isNumber(Commond[2]) || isZero(Commond[2])) //数字长度判断和零的判断，避免异常
                    {
                        if (isNumber(Commond[2])) {
                            autoSave = thread(autoSaveFun, stoi(Commond[2]), Commond[1], &cfgObj);
                            autoSave.detach();//分离
                            saveTime = stoi(Commond[2]);
                            printf("已启动自动存档，将每隔%s分钟存储一次名为%s的存档\n", Commond[2].c_str(), Commond[1].c_str());
                        }
                        else {
                            printf("错误:时间的参数不为纯数字\n");
                        }
                    }
                    else if (!isNumber(Commond[2])) {//字符串过长不显示数字过大
                        printf("错误:时间的参数不为纯数字\n");
                    }
                    else {
                        printf("错误:数字过大(>999999999)\n");
                    }
                }
                else {
                    printf("错误:参数不全\n");
                }
            }
            else {
                printf("错误:自动存档已经启动\n");
            }
        }
        else if (Commond[0] == "nextsave") {
            if (autoSaveOpenOrNo) {
                float isNextTime = (float)(saveTime*60 - timeCount) / 60.0;
                printf("距离下次存档还有%f分钟\n", isNextTime);
            }
            else {
                printf("未开启自动存档\n");
            }
        }
        else if (Commond[0] == "closesave") {
            if (autoSaveOpenOrNo) {
                autoSaveOpenOrNo = false;
                printf("已关闭自动存档\n");
            }
            else {
                printf("未开启自动存档\n");
            }
        }
        else if (Commond[0] == "infwisp") {
            if (clearCache) {
                if (Commond.size() == 1) {
                    clacwisp.clac(cfgObj);
                }
                else if (Commond.size() >= 10) {
                    int startNum = getTest(Commond[1]);
                    if (startNum == -1) {
                        printf("你输入的起始值有问题!\n");
                        continue;
                    }
                    int endNum = getTest(Commond[2]);
                    if (endNum == -1) {
                        printf("你输入的终止值有问题!\n");
                        continue;
                    }
                    int SiwtchNum = getTest(Commond[3]);
                    if (SiwtchNum == -1) {
                        printf("你输入的1,2,3模式切换的值有问题!\n");
                        continue;
                    }
                    if (!(SiwtchNum == 1 || SiwtchNum == 2 || SiwtchNum == 3)) {
                        printf("你输入的1,2,3模式切换的值不是1或2或3!\n");
                        continue;
                    }
                    int addLT = getTest(Commond[4]);
                    if (addLT == -1) {
                        printf("你输入的蓝表上限值有问题!\n");
                        continue;
                    }
                    int pinLT = getTest(Commond[5]);
                    if (pinLT == -1) {
                        printf("你输入的乒乓上限值有问题!\n");
                        continue;
                    }
                    int decLT = getTest(Commond[6]);
                    if (decLT == -1) {
                        printf("你输入的红表上限值有问题!\n");
                        continue;
                    }
                    int helLT = getTest(Commond[7]);
                    if (helLT == -1) {
                        printf("你输入的螺旋魔弹上限值有问题!\n");
                        continue;
                    }
                    int arcLT = getTest(Commond[8]);
                    if (arcLT == -1) {
                        printf("你输入的相位弧度上限值有问题!\n");
                        continue;
                    }
                    int splLT = getTest(Commond[9]);
                    if (splLT == -1) {
                        printf("你输入的相位弧度上限值有问题!\n");
                        continue;
                    }
                    int nulLT = getTest(Commond[10]);
                    if (nulLT == -1) {
                        printf("你输入的相位弧度上限值有问题!\n");
                        continue;
                    }
                    if (Commond.size() > 10) {
                        clacwisp.clacNoInput(cfgObj, startNum, endNum, SiwtchNum, addLT, pinLT, decLT, helLT, arcLT, splLT, nulLT, Commond[11]);
                    }
                    else {
                        clacwisp.clacNoInput(cfgObj, startNum, endNum, SiwtchNum, addLT, pinLT, decLT, helLT, arcLT, splLT, nulLT, File);
                    }
                }
                else {
                    printf("错误:你选择了infwisp指令自选参数，但是参数数量少了些，请重新输入！\n");
                }
            }
            else {
                bool out = LineGetOut("已检测到你没有清除永久法术缓存数据，输入y将会自动清除并进入计算模式，输入n则放弃这次计算:");
                if (out) {
                    continue;
                }
                if (Commond.size() == 1) {
                    clacwisp.clac(cfgObj);
                }
                else if (Commond.size() >= 9) {
                    int startNum = getTest(Commond[1]);
                    if (startNum == -1) {
                        printf("你输入的起始值有问题!\n");
                        continue;
                    }
                    int endNum = getTest(Commond[2]);
                    if (endNum == -1) {
                        printf("你输入的终止值有问题!\n");
                        continue;
                    }
                    int SiwtchNum = getTest(Commond[3]);
                    if (SiwtchNum == -1) {
                        printf("你输入的1,2,3模式切换的值有问题!\n");
                        continue;
                    }
                    if (!(SiwtchNum == 1 || SiwtchNum == 2 || SiwtchNum == 3)) {
                        printf("你输入的1,2,3模式切换的值不是1或2或3!\n");
                        continue;
                    }
                    int addLT = getTest(Commond[4]);
                    if (addLT == -1) {
                        printf("你输入的蓝表上限值有问题!\n");
                        continue;
                    }
                    int pinLT = getTest(Commond[5]);
                    if (pinLT == -1) {
                        printf("你输入的乒乓上限值有问题!\n");
                        continue;
                    }
                    int decLT = getTest(Commond[6]);
                    if (decLT == -1) {
                        printf("你输入的红表上限值有问题!\n");
                        continue;
                    }
                    int helLT = getTest(Commond[7]);
                    if (helLT == -1) {
                        printf("你输入的螺旋魔弹上限值有问题!\n");
                        continue;
                    }
                    int arcLT = getTest(Commond[8]);
                    if (arcLT == -1) {
                        printf("你输入的相位弧度上限值有问题!\n");
                        continue;
                    }
                    int splLT = getTest(Commond[9]);
                    if (splLT == -1) {
                        printf("你输入的相位弧度上限值有问题!\n");
                        continue;
                    }
                    int nulLT = getTest(Commond[10]);
                    if (nulLT == -1) {
                        printf("你输入的相位弧度上限值有问题!\n");
                        continue;
                    }
                    if (Commond.size() > 10) {
                        clacwisp.clacNoInput(cfgObj, startNum, endNum, SiwtchNum, addLT, pinLT, decLT, helLT, arcLT, splLT, nulLT, Commond[11]);
                    }
                    else {
                        clacwisp.clacNoInput(cfgObj, startNum, endNum, SiwtchNum, addLT, pinLT, decLT, helLT, arcLT, splLT, nulLT,File);
                    }
                }
                else {
                    printf("错误:你选择了infwisp指令自选参数，但是参数数量少了些，请重新输入！\n");
                }
            }
        }
        else if (Commond[0] == "exit" || Commond[0] == "quit") {
            break;
        }
        else if (Commond[0] == "clinflist") {
            infwispCache = vector<vector<vector<int>>>();
            HasAnwThread = vector<int>();
            clearCache = true;
            HasAnw = 0;
            printf("清除永久化法术数据缓存成功！\n");
        }
        else if (Commond[0] == "inflist") {//穷举得出的数据打印成表
            if (!clearCache) {
                if (Commond.size() > 2) {
                    int startPos = getTest(Commond[1]);
                    if (startPos == -1) {
                        printf("你的范围参数中的起始值有问题!\n");
                        continue;
                    }
                    int endPos = getTest(Commond[2]);
                    if (endPos == -1) {
                        printf("你的范围参数中的终止值有问题!\n");
                        continue;
                    }
                    if (startPos > endPos) {//如果起始值大于结束值，则交换数据
                        startPos ^= endPos;
                        endPos ^= startPos;
                        startPos ^= endPos;
                    }
                    if (startPos > HasAnw || endPos > HasAnw) {
                        printf("错误:超过访问范围！\n");
                        continue;
                    }
                    else if (startPos == 0 && endPos == 0) {
                        printf("错误：怎么想也没有0到0个数据吧\n");
                        continue;
                    }
                    int countSize = 0;
                    int startVector = 0;
                    int pStartPos = startPos;
                    int pEndPos = endPos;
                    int endVector = 0;
                    for (int i = 0; i < infwispCache.size(); i++) {
                        countSize += infwispCache[i].size();
                        if (countSize >= startPos) {
                            startVector = i + 1;//计算起始数组
                            break;//退出循环
                        }
                    }
                    if (infwispCache.size() > 1) {//第一维元素判断 计算起始位置
                        for (int i = 0; i < startVector; i++) {
                            if (pStartPos < infwispCache[i].size()) break;
                            pStartPos = pStartPos - infwispCache[i].size();
                        }
                    }
                    countSize = 0;
                    for (int i = 0; i < infwispCache.size(); i++) {
                        countSize += infwispCache[i].size();
                        if (countSize >= endPos) {
                            endVector = i + 1;//计算终止数组
                            break;//退出循环
                        }
                    }
                    if (infwispCache.size() > 1) {//第一维元素判断 计算终止位置
                        for (int i = 0; i < endVector; i++) {
                            if (pEndPos < infwispCache[i].size()) break;
                            pEndPos = pEndPos - infwispCache[i].size();
                        }
                    }
                    startVector--;//正确的下标寻址
                    endVector--;
                    /*
                    cout << "startVector:" << startVector << endl;
                    cout << "endVector:" << endVector << endl;
                    cout << "pStartPos:" << pStartPos << endl;
                    cout << "pEndPos:" << pEndPos << endl;
                    for (int i = 0; i < infwispCache.size(); i++) {
                        cout << infwispCache[i].size() << endl;
                    }*/
                    bool endAndStart = false;
                    if (endVector == startVector) endAndStart = true;
                    if (endAndStart) {
                        if (pEndPos == 0 && pStartPos == 0) {//如果都为0的时候就相当于要全部打印
                            pEndPos = infwispCache[endVector].size();//获得正确的参数
                        }
                        int i = startVector;
                        for (int j = pStartPos; j != pEndPos; j++) {
                            for (int k = 0; k != infwispCache[i][j].size(); k++) {
                                switch (k)
                                {
                                case 0: {
                                    printf("蓝表数量:%d ,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 1: {
                                    printf("乒乓数量:%d ,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 2: {
                                    printf("螺旋魔弹数量:%d ,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 3: {
                                    printf("相位弧度数量:%d ,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 4: {
                                    printf("红表数量:%d ,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 5: {
                                    printf("连锁法术数量:%d ,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 6: {
                                    printf("无害射击数量:%d", infwispCache[i][j][k]);
                                    break;
                                }
                                }
                            }
                            printf("\n");
                        }
                    }
                    else {//打印起始的那个数组
                        int i = startVector;
                        for (int j = pStartPos; j != infwispCache[i].size(); j++) {
                            for (int k = 0; k != infwispCache[i][j].size(); k++) {
                                switch (k)
                                {
                                case 0: {
                                    printf("蓝表数量:%d ,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 1: {
                                    printf("乒乓数量:%d ,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 2: {
                                    printf("螺旋魔弹数量:%d ,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 3: {
                                    printf("相位弧度数量:%d ,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 4: {
                                    printf("红表数量:%d ,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 5: {
                                    printf("连锁法术数量:%d ,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 6: {
                                    printf("无害射击数量:%d", infwispCache[i][j][k]);
                                    break;
                                }
                                }
                            }
                            printf("\n");
                        }//打印起始与结尾中间的数组
                        for (int i = startVector + 1; i < endVector; i++) {
                            for (int j = 0; j != infwispCache[i].size(); j++) {
                                for (int k = 0; k != infwispCache[i][j].size(); k++) {
                                    switch (k)
                                    {
                                    case 0: {
                                        printf("蓝表数量:%d ,", infwispCache[i][j][k]);
                                        break;
                                    }
                                    case 1: {
                                        printf("乒乓数量:%d ,", infwispCache[i][j][k]);
                                        break;
                                    }
                                    case 2: {
                                        printf("螺旋魔弹数量:%d ,", infwispCache[i][j][k]);
                                        break;
                                    }
                                    case 3: {
                                        printf("相位弧度数量:%d ,", infwispCache[i][j][k]);
                                        break;
                                    }
                                    case 4: {
                                        printf("红表数量:%d ,", infwispCache[i][j][k]);
                                        break;
                                    }
                                    case 5: {
                                        printf("连锁法术数量:%d ,", infwispCache[i][j][k]);
                                        break;
                                    }
                                    case 6: {
                                        printf("无害射击数量:%d", infwispCache[i][j][k]);
                                        break;
                                    }
                                    }
                                }
                                printf("\n");
                            }
                        }
                        {//块代码区分i变量 打印结尾数组
                            int i = endVector;
                            if (pEndPos) {//如果为0的时候就不能减了，不然会出问题
                                pEndPos = infwispCache[endVector].size() - pEndPos;//获得正确的参数
                            }
                            for (int j = pEndPos; j != infwispCache[i].size(); j++) {
                                for (int k = 0; k != infwispCache[i][j].size(); k++) {
                                    switch (k)
                                    {
                                    case 0: {
                                        printf("蓝表数量:%d ,", infwispCache[i][j][k]);
                                        break;
                                    }
                                    case 1: {
                                        printf("乒乓数量:%d ,", infwispCache[i][j][k]);
                                        break;
                                    }
                                    case 2: {
                                        printf("螺旋魔弹数量:%d ,", infwispCache[i][j][k]);
                                        break;
                                    }
                                    case 3: {
                                        printf("相位弧度数量:%d ,", infwispCache[i][j][k]);
                                        break;
                                    }
                                    case 4: {
                                        printf("红表数量:%d ,", infwispCache[i][j][k]);
                                        break;
                                    }
                                    case 5: {
                                        printf("连锁法术数量:%d ,", infwispCache[i][j][k]);
                                        break;
                                    }
                                    case 6: {
                                        printf("无害射击数量:%d", infwispCache[i][j][k]);
                                        break;
                                    }
                                    }
                                }
                                printf("\n");
                            }
                        }
                    }
                    printf("\n");
                    cout << "本次输入的存在时间范围为:" << clacA.startNum << "到" << clacA.endNum << endl;
                    cout << "蓝表上限为:" << clacA.addLT << endl;
                    cout << "乒乓上限为:" << clacA.pinLT << endl;
                    cout << "螺旋魔弹上限为:" << clacA.helLT << endl;
                    cout << "相位上限为:" << clacA.arcLT << endl;
                    cout << "红表上限为:" << clacA.decLT << endl;
                    cout << "连锁上限为:" << clacA.splLT << endl;
                    cout << "无害射击上限为:" << clacA.nulLT << endl;
                    cout << "一共有" << HasAnw << "行数据" << endl;
                    printf("打印了从第%d到第%d行的数据\n\n", startPos, endPos);
                    /* 将所有的打印完，这个也是测试用的
                    for (int i = 0; i < infwispCache.size(); i++) {
                        for (int j = 0; j != infwispCache[i].size(); j++) {
                            for (int k = 0; k != infwispCache[i][j].size(); k++) {
                                switch (k)
                                {
                                case 0: {
                                    printf("蓝表数量:%d,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 1: {
                                    printf("乒乓数量:%d,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 2: {
                                    printf("螺旋魔弹数量:%d,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 3: {
                                    printf("相位弧度数量:%d,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 4: {
                                    printf("红表数量:%d,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 5: {
                                    printf("连锁法术数量:%d,", infwispCache[i][j][k]);
                                    break;
                                }
                                case 6: {
                                    printf("无害射击数量:%d", infwispCache[i][j][k]);
                                    break;
                                }
                                }
                            }
                            printf("\n");
                        }
                    }*/
                }
                else {
                    printf("参数过少\n");
                }
            }
            else {
                printf("错误:现在这里面还没有缓存数据呢\n");
            }
        }
        else if (Commond[0] == "help") {
            printf("\n输入[  ]中的内容执行该指令\n指令列表:\n1.[save 存档名称(可选)] 只输入save会将存档保存为默认存档名,输入了存档名称会将存档存为指定名称的读档\n\n");
            printf("2.[load 存档名称(可选)] 只输入load会读取默认存档名的存档,输入了存档名称会读取指定名称的存档\n\n");
            printf("3.[unload] 在配置文件里开启此功能后(默认打开),在load完存档后可以使用,输入后会加载刚才被覆盖掉的存档\n\n");
            printf("4.[exit]/[quit] 关闭程序\n\n");
            printf("5.[savelist] 返回存档列表,包括游戏本身的存档\n\n");
            printf("6.[delsave 存档名称] 删除特定存档\n\n");
            printf("7.[bonelist] 返回老古法杖文件列表及其序号\n\n");
            printf("8.[bone 序号] 读取某一老古法杖的数据并打印\n\n");
            printf("9.[rebonelist] 输入后重新加载老古法杖文件,用于在切换存档后重新读取\n\n");
            printf("10.[autosave 存档名称 每隔时间(可选)] 自动存档,效果类似于save,不同的是它是自动的,第二个参数是每隔时间,如果不填写则是默认每隔30分钟\n\n");
            printf("11.[closesave] 输入后关闭自动存档\n\n");
            printf("12.[nextsave] 返回距离下次自动存档时间\n\n");
            printf("13.[infwisp] 启用本程序内置的永久法术计算器，会手动请你输入数据\n");
            printf("[infwisp 起始值 终止值 选择模式 蓝表上限 乒乓上限 红表上限 螺旋魔弹上限 相位弧度上限 连锁法术上限 无害法术上限 写入文件的文件名(可选)]\n");
            printf("一键启动永久法术计算，具体参数如上所述，选择模式的参数为\n [1] 直接打印到控制台\n [2] 写入指定文件或者默认文件\n [3] 将数据缓存到内存中，使用inflist指令打印指定范围的数据\n");
            printf("选择模式的参数就是[]里的数字\n\n");
            printf("[inflist 数字1 数字2] 打印从数字1到数字2范围内的数据\n\n");
            printf("[clinflist] 清除永久法术数据的缓存\n\n");
        }
        else {
            cout << "错误:未知的指令:" << Commond[0] << endl;
            cout << "请重新输入" << endl;
        }
    }
    return 0;
}
