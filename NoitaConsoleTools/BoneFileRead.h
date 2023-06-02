#ifndef BONEFILEREAD_H_
#define BONEFILEREAD_H_
#include <fstream>
#include <iostream>
#include <io.h>
#include <map>
#include <vector>
#include "translationsLoad.h"
#include "Functions.h"

class BoneFileRead {
public:
    BoneFileRead(string path, const translationsLoad& tranObj);
    void getWandList() const;
    void getWand(string par);
private:
    class MagicWand {
    private:
        int getFrNumber(string par) const;
    public:
        void PrintWandList() const;
        string mana;//蓝
        string manaMax;//蓝上限
        string manaChargeSpeed;//回蓝速度
        string spriteFile;//法杖贴图
        string reloadTime;//充能延迟
        string deckCapacity;//法杖容量
        string fireRateWait;//施放延迟
        string stateShuffled;//0为非乱序
        string speedMultiplier;//投射物速度基础倍率
        string spreadDegrees;//散射角度
        vector<string> wandSpell;
    };
    string getPar(string str) const;
    void getFileNames(string path, vector<string>& files);
    map<string, MagicWand>WandC;//第一个存文件名，第二个存法杖属性 AbilityComponent gunaction_config ItemActionComponent要读
    vector<string> files;
    vector<string> fileName;
    map<string, string>NumtoFileName;
};
#endif