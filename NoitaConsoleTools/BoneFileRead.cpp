#include <fstream>
#include <iostream>
#include <io.h>
#include <map>
#include <vector>
#include "translationsLoad.h"
#include "Functions.h"
#include "BoneFileRead.h"
using namespace std;

BoneFileRead::BoneFileRead(string path, const translationsLoad& tranObj) {//一坨四
        string SwapPath = slashSwap(path);
        getFileNames(SwapPath, files);
        string bufStr;//读行缓存
        fstream cfg;
        bool WirteMode = false;//开始写入的标志
        for (int i = 0; i < files.size(); i++) {
            bool WirteSpell = true;//判断是否需要写入法术
            bool WirteReload = false;
            bool WirteSprite = true;
            bool WirteMana = true;
            bool WirteManaMax = true;
            bool WirteSpeedMul = true;
            int count = 0;
            int reloadcount = 0;
            cfg.open(files[i], ios::in);
            string Strbuf = getFileName(files[i]);
            WandC.insert(map<string, MagicWand>::value_type(Strbuf, MagicWand()));//按照文件数量建立对应的数组
            string a = to_string(i + 1);
            NumtoFileName.insert(map<string, string>::value_type(a, Strbuf));
            fileName.push_back(Strbuf);
            while (getline(cfg, bufStr)) {
                if (bufStr.find("AbilityComponent") != -1 || bufStr.find("ItemActionComponent") != -1 || bufStr.find("gun_config") != -1) {//找不到是-1，找到了不是-1
                    if (bufStr.find("gun_config") != -1 && reloadcount == 0) {
                        reloadcount++;
                        WirteReload = true;
                        continue;
                    }
                    else if (bufStr.find("gun_config") != -1) {
                        WirteReload = false;
                        continue;
                    }
                    if (count == 0) {
                        count++;
                        WirteMode = true;
                        WirteSpell = true;
                        WirteReload = false;
                        if (bufStr.find("AbilityComponent") != -1) {
                            WirteSpell = false;
                        }
                    }
                    else {
                        count = 0;
                        WirteMode = false;
                        WirteReload = false;
                    }
                    continue;
                }
                if (WirteMode) {
                    if (bufStr.find("mana_max") != -1 && WirteManaMax) {
                        WirteManaMax = false;
                        WandC[Strbuf].manaMax = getPar(bufStr);
                    }
                    else if (bufStr.find("mana_charge_speed") != -1) {
                        WandC[Strbuf].manaChargeSpeed = getPar(bufStr);
                    }
                    else if (bufStr.find("mana") != -1 && WirteMana) {
                        WirteMana = false;
                        WandC[Strbuf].mana = getPar(bufStr);
                    }
                    else if (bufStr.find("sprite_file") != -1 && WirteSprite) {
                        WirteSprite = false;
                        WandC[Strbuf].spriteFile = getPar(bufStr);
                    }
                    else if (bufStr.find(" speed_multiplier=") != -1 && WirteSpeedMul) {
                        WirteSpeedMul = false;
                        WandC[Strbuf].speedMultiplier = getPar(bufStr);
                    }
                    else if (bufStr.find("spread_degrees") != -1) {
                        WandC[Strbuf].spreadDegrees = getPar(bufStr);
                    }
                    else if (bufStr.find("reload_time") != -1 && WirteReload) {
                        WandC[Strbuf].reloadTime = getPar(bufStr);
                    }
                    else if (bufStr.find("deck_capacity") != -1) {
                        WandC[Strbuf].deckCapacity = getPar(bufStr);
                    }
                    else if (bufStr.find("fire_rate_wait") != -1) {
                        WandC[Strbuf].fireRateWait = getPar(bufStr);
                    }
                    else if (bufStr.find("shuffle_deck_when_empty") != -1) {
                        if (getPar(bufStr) == "0") {
                            WandC[Strbuf].stateShuffled = "否";
                        }
                        else {
                            WandC[Strbuf].stateShuffled = "是";
                        }
                    }
                    else if (bufStr.find("action_id") != -1 && WirteSpell) {
                        string sbnolla = getPar(bufStr);
                        if (sbnolla == "LASER_LUMINOUS_DRILL") {
                            sbnolla = "luminous_drill_timer";
                        }
                        WandC[Strbuf].wandSpell.push_back(tranObj.getTran("action_" + toLower(sbnolla), "zh-cn"));//将获取的参数转成小写，然后获得翻译文本，然后再写入

                    }
                }
            }
            cfg.close();
        }
    }
    void BoneFileRead::getWandList() const {
        printf("文件总数:%d\n", (int)fileName.size());
        for (int i = 0; i < fileName.size(); i++) {
            printf("%d.%s\n", i + 1, fileName[i].c_str());
        }
    }
    void BoneFileRead::getWand(string par) {
        if (WandC.count(NumtoFileName[par]) == 0) {
            printf("查无为此序号的法杖数据:%s\n", par.c_str());
        }
        else {
            printf("\n文件:%s\n\n", NumtoFileName[par].c_str());
            WandC[NumtoFileName[par]].PrintWandList();
            printf("\n");
        }
    }
        int BoneFileRead::MagicWand::getFrNumber(string par) const {//换算负数或者是正数
            string bufParStr = "";
            if (par[0] == '-') {
                for (int i = 1; i < par.length(); i++) {
                    bufParStr += par[i];
                }
                return -stoi(bufParStr);
            }
            return stoi(par);
        }
        void BoneFileRead::MagicWand::PrintWandList() const {
            printf("法杖当前蓝量:%s\n", mana.c_str());
            printf("法杖蓝量上限:%s\n", manaMax.c_str());
            printf("法杖回蓝速度:%s\n", manaChargeSpeed.c_str());
            printf("法杖贴图在data.wak中的路径:%s\n", spriteFile.c_str());
            if (getFrNumber(reloadTime) < 0) {
                printf("充能延迟:%.2f(秒) %s(帧)\n", (float)getFrNumber(reloadTime) / 60, reloadTime.c_str());
            }
            else {
                printf("充能延迟:+%.2f(秒) +%s(帧)\n", (float)getFrNumber(reloadTime) / 60, reloadTime.c_str());
            }
            if (getFrNumber(fireRateWait) < 0) {
                printf("施放延迟:%.2f(秒) %s(帧)\n", (float)getFrNumber(fireRateWait) / 60, fireRateWait.c_str());
            }
            else {
                printf("施放延迟:+%.2f(秒) +%s(帧)\n", (float)getFrNumber(fireRateWait) / 60, fireRateWait.c_str());
            }
            printf("法术容量:%s\n", deckCapacity.c_str());
            printf("投射物速度基础倍率:%sx\n", speedMultiplier.c_str());
            printf("散射角度:%s\n", spreadDegrees.c_str());
            printf("是否乱序:%s\n", stateShuffled.c_str());
            printf("法术列表:");
            for (int i = 0; i < wandSpell.size(); i++) {
                printf("[%s] ", wandSpell[i].c_str());
            }
            if (wandSpell.size() == 0) {
                printf("无法术");
            }
            printf("\n");
        }

    string BoneFileRead::getPar(string str) const {//获取参数
        int pos = str.find('"');
        string buf = "";
        for (int i = pos + 1; i < str.length(); i++) {
            if (str[i] != '"') {
                buf += str[i];
            }
            else {
                return buf;//判断到直接返回值
            }
        }
    }
    void BoneFileRead::getFileNames(string path, vector<string>& files)//获得文件夹下所有文件
    {
        //文件句柄
        intptr_t hFile = 0;
        //文件信息
        struct _finddata_t fileinfo;
        string p;
        if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
        {
            do
            {
                if ((fileinfo.attrib & _A_SUBDIR))
                {
                    if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                        getFileNames(p.assign(path).append("\\").append(fileinfo.name), files);
                }
                else
                {
                    files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                }
            } while (_findnext(hFile, &fileinfo) == 0);
            _findclose(hFile);
        }
    }