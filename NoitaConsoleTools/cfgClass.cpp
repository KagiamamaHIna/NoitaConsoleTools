#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include "cfgClass.h"
using namespace std;

cfgClass::cfgClass() {
    fstream cfg;
    string cfgFile = "config.cfg";
    int charPos[2];
    char buf[1024] = { '\0' };//缓存
    char buf2[1024] = { '\0' };
    string bufStr = "0";
    int count = -1;
    bool duplicateStr = false;//判重用的变量
    bool decideSyntax = true;
    int bufInt;
    cfg.open(cfgFile, ios::in);
    if (!cfg.is_open())//如果没有文件就新建一个！
    {
        cfg.close();
        cfg.open(cfgFile, ios::out | ios::app);
        cfg << "save00Path{%systemdrive%\\Users\\%USERNAME%\\AppData\\LocalLow\\Nolla_Games_Noita}" << endl;
        cfg << "//存档路径，%字符串%是系统变量，可以自动匹配，如果定位不到存档请手动输入地址" << endl;
        cfg << "savePath{\\save}" << endl;
        cfg << "//备份存档主路径，使用save指令备份后会保存在相应位置，请注意，这个始终是根目录/为开头的" << endl;
        printf("已检测到你没有配置文件，已经自动生成了一个默认的配置文件\n");
        cfg << "unload{1}" << endl;
        cfg << "//如果为1，那么会在加载备份存档之前备份一次现在的存档，使用unload指令可以读取这个存档，给你个后悔的机会,为0则关闭此备份功能，可以提高读取存档速度" << endl;
        cfg << "threadNum{1}" << endl;
        cfg << "//更改线程数量，一线程为主线程，二线程为主线程+子线程，如此类推下去" << endl;
        cfg << "//警告：本功能开启后强烈建议启动写入文件，不然有可能性能提升不明显，如果为1那么就只有主线程运算" << endl;
        cfg << "//所以，拿来玩玩倒是可以就对了（）" << endl;
        cfg.close();
        cfg.open(cfgFile, ios::in);
    }
    while (cfg.getline(buf, sizeof(buf)))//按行读文件 这里存入参数的原理为，将参数线性存入向量cfgStr和向量cfgNumInt1/2中，然后循环到符合其中一个时，这时候累加器的参数写入向量下标进行寻址时就等同于该参数的其他数据了
    {
        if (!(buf[0] == '/' && buf[1] == '/') && !buf[0] == '\0') { //判断注释
            if (!(strchr(buf, '{') && strchr(buf, '}')) || (strchr(buf, '}') - buf) - (strchr(buf, '{') - buf) == 1 || (strchr(buf, '}') - buf) - (strchr(buf, '{') - buf) <= -1) {//如果配置文件语法不正确报错，只报一次，右边的第一条的判断语句是判断是否只是单纯的{}组合，第二条是 是否反过来
                if (decideSyntax) {
                    cerr << "检查到不符合配置文件语法的内容，已跳过" << endl;
                    decideSyntax = false;
                }
                continue;//退出这次循环，避免异常
            }
            duplicateStr = false;
            //cout << buf << endl;//这些cout是调试用的，所以被注释掉了
            count++;
            memset(buf2, '\0', sizeof(buf2));//清空缓存
            charPos[0] = strchr(buf, '{') - buf;//计算位置
            charPos[1] = strchr(buf, '}') - buf;
            //cout << charPos[0] << endl;
            //cout << charPos[1] << endl;
            for (int i = 0; i < charPos[0]; i++) {//存入参数名
                buf2[i] = buf[i];
            }
            if (!cfgStr.empty()) {
                bufStr = buf2;//判断参数名是否重复
                for (int i = 0; i < cfgStr.size(); i++) {
                    if (bufStr == cfgStr[i]) {
                        duplicateStr = true;
                        bufInt = i;
                        cfgNumInt2[bufInt]++;
                        count--;//恢复正常计数
                    }
                }
            }
            if (!duplicateStr) {
                cfgStr.push_back(buf2);//如果不为重复，则把参数名存入向量中
            }
            memset(buf2, '\0', sizeof(buf2));//重置
            for (int i = charPos[0] + 1, i2 = 0; i < charPos[1]; i++, i2++) {
                buf2[i2] = buf[i];//取出参数
            }
            bufStr = buf2;
            if (!duplicateStr) {//存入参数
                cfgNumInt1.push_back(bufStr);
                cfgNumInt2.push_back(0);//重复累加器，默认是0，先要压入
            }
            else {//如果是重复定义的就重新赋值
                cfgNumInt1[bufInt] = bufStr;

            }
            //cout << cfgNumInt1[count] << endl;
            //cout << cfgNumInt2[count] << endl;
        }
    }
    cfg.close();//关闭文件
    for (int i = 0; i < cfgNumInt2.size(); i++) {
        if (cfgNumInt2[i] != 0) {
            cerr << "警告:参数" << cfgStr[i] << "在配置文件中被重复定义了" << cfgNumInt2[i] << "次，" << "程序将选择最后被定义的参数:" << cfgNumInt1[i] << endl;
        }
    }
}

string cfgClass::getParameter(string Par) const {//输入字符串，获取参数
    if (!cfgStr.empty()) {
        for (int i = 0; i < cfgStr.size(); i++) {
            if (Par == cfgStr[i]) {
                return cfgNumInt1[i];
            }
        }
    }
    if (cfgStr.empty()) {
        cerr << "配置文件里为什么没有参数呢:(" << "\n参数:" << Par << "将返回默认值 无参数\n建议把配置文件删了然后重新打开本程序\n这样的话会新建一个默认的配置文件" << endl;
        return "无参数";
    }
    cerr << "未找到参数:" << Par << "，将返回默认值 无参数" << endl;
    return "无参数";
}