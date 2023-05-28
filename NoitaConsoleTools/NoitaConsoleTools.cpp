#define _CRT_SECURE_NO_WARNINGS
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

using namespace std;
mutex mtx;

clock_t ClockStart, ClockEnd;
fstream file;
vector<fstream> fileCace;
const char* File = "infwispList.txt";
atomic<int>HasAnw = 0;

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

class cfgClass {//配置文件读取
public:
    cfgClass() {
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
    string getParameter(string Par) const {//输入字符串，获取参数
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
private:
    vector<string> cfgStr;//定义一个用于存储参数的字符串向量
    vector<string> cfgNumInt1;
    vector<int> cfgNumInt2;//累加重复次数的
};

class translationsLoad {
public:
    translationsLoad() {//en,ru,pt-br,es-es,de,fr-fr,it,pl,zh-cn,jp,ko
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
                        tranMap.insert(map<string, vector<string>>::value_type(bufStr2, vector<string>(11)));
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
    string getTran(string Par, string lau) const {//第一个参数是key，第二个参数是语言
        if (laug.count(lau) == 0) {//en,ru,pt-br,es-es,de,fr-fr,it,pl,zh-cn,jp,ko
            return "语言错误,你输入的是:" + lau;
        }
        if (tranMap.count(Par) == 0) {
            return "翻译表内查无此数据,key:" + Par;
        }
        return this->tranMap.find(Par)->second.at(laug.find(lau)->second);//增加了程序的健壮性的改动，和原代码等价
    }
private:
    map<string, vector<string>>tranMap;//翻译表
    map<string, int>laug;//语言文本对应数组下标值
};

class MagicWand {
private:
    int getFrNumber(string par) const {//换算负数或者是正数
        string bufParStr = "";
        if (par[0] == '-') {
            for (int i = 1; i < par.length(); i++) {
                bufParStr += par[i];
            }
            return -stoi(bufParStr);
        }
        return stoi(par);
    }
public:
    void PrintWandList() const {
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
        printf("投射物速度基础倍率:%sx\n",speedMultiplier.c_str());
        printf("散射角度:%s\n", spreadDegrees.c_str());
        printf("是否乱序:%s\n", stateShuffled.c_str());
        printf("法术列表:");
        for (int i = 0; i < wandSpell.size(); i++) {
            printf("%s ", wandSpell[i].c_str());
        }
        if (wandSpell.size() == 0) {
            printf("无法术");
        }
        printf("\n");
    }
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

class BoneFileRead {
public:
    BoneFileRead(string path,const translationsLoad& tranObj) {//一坨四
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
            WandC.insert(map<string, MagicWand>::value_type(Strbuf,MagicWand()));//按照文件数量建立对应的数组
            string a = to_string(i + 1);
            NumtoFileName.insert(map<string, string>::value_type(a, Strbuf));
            fileName.push_back(Strbuf);
            while (getline(cfg, bufStr)) {
                if (bufStr.find("AbilityComponent") != -1  || bufStr.find("ItemActionComponent") != -1 || bufStr.find("gun_config") != -1) {//找不到是-1，找到了不是-1
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
                    else if (bufStr.find(" speed_multiplier=") != -1 && WirteSpeedMul){
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
    void getWandList() const {
        printf("文件总数:%d\n", (int)fileName.size());
        for (int i = 0; i < fileName.size(); i++) {
            printf("%d.%s\n", i+1, fileName[i].c_str());
        }
    }
    void getWand(string par) {
        if (WandC.count(NumtoFileName[par]) == 0) {
            printf("查无为此序号的法杖数据:%s\n", par.c_str());
        }
        else {
            printf("\n文件:%s\n\n", NumtoFileName[par].c_str());
            WandC[NumtoFileName[par]].PrintWandList();
            printf("\n");
        }
    }
private:
    string getPar(string str) const {//获取参数
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
    void getFileNames(string path, vector<string>& files)//获得文件夹下所有文件
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
    map<string, MagicWand>WandC;//第一个存文件名，第二个存法杖属性 AbilityComponent gunaction_config ItemActionComponent要读
    vector<string> files;
    vector<string> fileName;
    map<string,string>NumtoFileName;
};        

class clacinfwisp {
    vector<thread> T;//动态存储线程
    vector<int> TNum;//动态存储分配后的参数
public:
    void clac(const cfgClass& cfg) {//addLT蓝表,pinLT乒乓,decLT红表，helLT螺旋魔弹,arcLT相位弧度,splLT连锁法术
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
};
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
    while(Path[i]) {//分割参数
        if (Path[i] != '%') {
            ReturnStr += Path[i];
        }
        else if (Path[i] == '%') {
            i++;
            while (Path[i] != '%') {
                sysBuf += Path[i];
                i++;
                if (i > Path.length()-1) {
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

int returnIntOrDef(string par,string num) {
    if (isNumber(num)) {
        return stoi(num);
    }
    printf("配置文件中的参数:%s，不为纯数字，返回默认值0", par.c_str());
    return 0;
}
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

int main()
{
    SetConsoleTitle(L"Noita控制台多功能工具v1.0.2");
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
    printf("输入help查看帮助 版本为v1.0.2\n");
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
            printf("指令列表:\n\n1.save指令，用法save (可选参数),如果只是单纯输入save的话，会将存档自动保存到默认位置，\n如果输入了可选参数的话，那么则是保存到你输入的参数的位置\n\n");
            printf("2.load指令，用法load (可选参数)，用法和save指令一致，save 00保存了名为00的存档的话，那么输入load 00即可读取这个存档\n\n");
            printf("3.unload指令，在配置文件里开启此功能后(默认打开)，则在load完存档后可以使用，用处是加载刚才被覆盖掉的存档\n\n");
            printf("4.exit和quit指令，用法exit 或 quit，用处是关闭程序\n\n");
            printf("5.savelist指令，用法savelist，返回存档列表\n\n");
            printf("6.delsave指令，用法delsave 存档名，删除特定存档\n\n");
            printf("7.bonelist指令，用法bonelist，返回老古法杖文件列表及其序号\n\n");
            printf("8.bone指令，用法bone 老古法杖序号，读取某一老古法杖其数据并打印\n\n");
            printf("9.rebonelist指令，用法rebonelist,如果老古法杖文件已经改变，那么输入这个指令可以重新加载\n\n");
            printf("10.autosave指令，用法autosave (参数) (可选参数),自动存档的指令，第一个参数是存档名，\n第二个参数是每隔时间，如果不填写则是默认每隔30分钟\n\n");
            printf("11.closesave指令，用法closesave,输入后关闭自动存档\n\n");
            printf("12.nextsave指令，用法nextsave,返回距离下次自动存档还剩时间\n\n");
            printf("13.infwisp指令，用法infwisp,启用本程序内置的永久法术计算器\n\n");
        }
        else {
            cout << "错误:未知的指令:" << Commond[0] << endl;
            cout << "请重新输入" << endl;
        }
    }
    return 0;
}
