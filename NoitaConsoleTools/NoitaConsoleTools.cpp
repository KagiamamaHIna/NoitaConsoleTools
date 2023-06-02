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

extern mutex mtx;
extern atomic<int>HasAnw;

bool autoSaveOpenOrNo = false;
atomic<int> timeCount = 0;
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
    SetConsoleTitle(L"Noita控制台多功能工具v1.0.2.1");
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
    printf("输入help查看帮助 版本为v1.0.2.1\n");
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
            clacwisp.clac(cfgObj);
        }
        else if (Commond[0] == "exit" || Commond[0] == "quit") {
            break;
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
            printf("13.[infwisp] 启用本程序内置的永久法术计算器\n\n");
        }
        else {
            cout << "错误:未知的指令:" << Commond[0] << endl;
            cout << "请重新输入" << endl;
        }
    }
    return 0;
}
