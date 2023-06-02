#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <array>
#include "translationsLoad.h"

using namespace std;

translationsLoad::translationsLoad() {//en,ru,pt-br,es-es,de,fr-fr,it,pl,zh-cn,jp,ko
        laug["en"] = 0; laug["ru"] = 1; laug["pt-br"] = 2; laug["es-es"] = 3; laug["de"] = 4; laug["fr-fr"] = 5; laug["it"] = 6; laug["pl"] = 7; laug["zh-cn"] = 8; laug["jp"] = 9; laug["ko"] = 10;//初始化语言选项
        string bufStr;//读行缓存
        string bufStr2;//内容缓存
        string HeadCache;
        string cfgFile = "common.csv";
        fstream cfg;
        cfg.open(cfgFile, ios::in);
        if (!cfg.is_open())//如果没有文件就报错！
        {
            cfg.close();
            cerr << "错误:未找到common.csv文件！" << endl;
            return;
        }
        bool isFirst = true;
        while (getline(cfg, bufStr)) {
            if (isFirst) {
                isFirst = false;
                continue;//不读取开头
            }
            bool isHead = true;//重置
            bool isLoad = true;
            bufStr2 = "";
            int count = 0;//判断"用的
            int count2 = 0;//写入用的
            int count3 = 0;//判断是否为空用的
            //for (int i = 0; i < bufStr.length(); i++) {
            int i = 0;
            while (bufStr[i]) {//遍历字符 因为for循环还需要返回字符串长度，如果用while循环的话，因为字符串结束符是00，相当于false，所以理论上可以不需要知道字符串长度，也就是可以加快读取速度
                if (bufStr[i] == '"') {
                    isLoad = false;//如果判断到"就即使判断到,也不跳过，正常写入
                    count++;//自增判断是否到第二个
                    if (count == 2) {
                        count = 0;
                        isLoad = true;//重置
                    }
                }
                if (bufStr[i] == ',' && isLoad) {
                    if (isHead) {//存入头
                        tranMap.insert(map<string, array<string, 11>>::value_type(bufStr2, array<string,11>()));
                        HeadCache = bufStr2;
                        bufStr2 = "";
                        isHead = false;
                    }
                    else if (count3 < 1 && count2 != 11) {
                        tranMap[HeadCache][count2] = bufStr2;
                        bufStr2 = "";
                        count2++;
                        count3++;//写入数据
                    }
                    else if (count2 == 11) goto END;//如果没有数据了就结束这次循环
                    else {
                        tranMap[HeadCache][count2] = tranMap[HeadCache][0];//如果是,,的这种组合，就直接赋值英文默认值
                        count2++;
                    }
                }
                else {
                    count3 = 0;
                    bufStr2 = bufStr2 + bufStr[i];//拼接字符串
                }
                i++;
            }
        END:;
        }
        cfg.close();
    }

string translationsLoad::getTran(string Par, string lau) const {//第一个参数是key，第二个参数是语言
    if (laug.count(lau) == 0) {//en,ru,pt-br,es-es,de,fr-fr,it,pl,zh-cn,jp,ko
        return "语言错误,你输入的是:" + lau;
    }
    if (tranMap.count(Par) == 0) {
        return "翻译表内查无此数据,key:" + Par;
    }
    return this->tranMap.find(Par)->second.at(laug.find(lau)->second);//增加了程序的健壮性的改动，和原代码等价
}