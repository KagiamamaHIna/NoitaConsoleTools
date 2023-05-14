#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <map>
#include <direct.h>
#include <io.h>

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
int getNumber(const char* speaky) { //自定义一个询问并获取数字的函数
    string stringNum = "0";
    char charNum[127] = { '\0' };
    while (true)
    {
        printf(speaky);
        scanf_s("%s", charNum, 127);
        string stringNum = charNum;
        if (stringNum.length() < 10 && isNumber(charNum) || isZero(stringNum)) //数字长度判断和零的判断，避免异常
        {
            if (isNumber(stringNum)) {
                return std::stoi(stringNum);//获取正确的整数
            }
            printf("参数不为纯数字，请重新输入！\n");
        }
        else if (!isNumber(charNum)) {//字符串过长不显示数字过大
            printf("参数不为纯数字，请重新输入！\n");
        }
        else {
            printf("数字过大(>999999999)，请重新输入！\n");
        }
    }
}
string getChar(const char* speaky) { //自定义一个询问并获取字符串的函数
    char charNum[127] = { '\0' };
    printf(speaky);
    scanf_s("%s", charNum, 127);
    return charNum;
}

int getModMax(const char* speaky, int modMax, int outInt = 0) { //自定义一个询问并返回特定数字的函数
    char charNum[2] = { '\0' };
    std::string stringNum = "0";
    while (true)
    {
        printf(speaky);
        scanf_s("%s", charNum, 2);//先用scanf_s获得值
        std::string stringNum = charNum;//然后赋值给string
        if (stringNum == "y") {
            return modMax;
        }
        else if (stringNum == "n") {
            return outInt;
        }
        else {
            printf("你需要输入的是y或n，而不是其他参数！\n");
        }
    }
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
                cerr << "警告:参数" << cfgStr[i] << "在配置文件中被重复定义了" << cfgNumInt2[i] << "次，" << "程序将选择最后被定义的数字:" << cfgNumInt1[i] << endl;
            }
        }
    }
    string getParameter(string Par) {//输入字符串，获取参数
        if (!cfgStr.empty()) {
            for (int i = 0; i < cfgStr.size(); i++) {
                if (Par == cfgStr[i]) {
                    return cfgNumInt1[i];
                }
            }
        }
        if (cfgStr.empty()) {
            cerr << "配置文件里为什么没有参数呢:(" << "\n参数:" << Par << "将返回默认值1\n建议把配置文件删了然后重新打开本程序\n这样的话会新建一个默认的配置文件" << endl;
            return "无参数";
        }
        cerr << "未找到参数:" << Par << "，将返回默认值1" << endl;
        return "无参数";
    }
private:
    vector<string> cfgStr;//定义一个用于存储参数的字符串向量
    vector<string> cfgNumInt1;
    vector<int> cfgNumInt2;//累加重复次数的
};
/* 已完成，但是目前没有用所以注释掉
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
            for (int i = 0; i < bufStr.length(); i++) {
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
                        tranMap.insert(map<string, vector<string>>::value_type(bufStr2, vector<string>(13)));
                        HeadCache = bufStr2;
                        bufStr2 = "";
                        isHead = false;
                    }
                    else if (count3 < 1 && count2 != 13) {
                        tranMap[HeadCache][count2] = bufStr2;
                        bufStr2 = "";
                        count2++;
                        count3++;//写入数据
                    }
                    else if (count2 == 13) goto END;//如果没有数据了就结束这次循环
                    else {
                        tranMap[HeadCache][count2] = tranMap[HeadCache][0];//如果是,,的这种组合，就直接赋值英文默认值
                        count2++;
                    }
                }
                else {
                    count3 = 0;
                    bufStr2 = bufStr2 + bufStr[i];//拼接字符串
                }
            }
        END:;
        }
        cfg.close();
    }
    string getTran(string Par, string lau) {//第一个参数是key，第二个参数是语言
        if (laug.count(lau) == 0) {//en,ru,pt-br,es-es,de,fr-fr,it,pl,zh-cn,jp,ko
            return "语言错误,你输入的是:" + lau;
        }
        if (tranMap.count(Par) == 0) {
            return "翻译表内查无此数据,key:" + Par;
        }
        return this->tranMap[Par][laug[lau]];
    }
private:
    map<string, vector<string>>tranMap;//翻译表
    map<string, int>laug;//语言文本对应数组下标值
};*/

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
    for (int i = 0; i < Path.length(); i++) {//分割参数
        if (Path[i] != '%') {
            ReturnStr += Path[i];
        }
        else if (Path[i] == '%') {
            i++;
            while (Path[i] != '%') {
                sysBuf += Path[i];
                i++;
            }
            if (getenv(sysBuf.c_str())) {
                sysBuf = getenv(sysBuf.c_str());
            }
            ReturnStr += sysBuf;
            sysBuf = "";
        }
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
    while (true) {
        string bufA = getCommond(ask.c_str())[0];
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

int main()
{
    SetConsoleTitle(L"Noita控制台多功能工具v1.0.0");
    system("chcp 65001");//改字符编码
    system("cls");
    //translationsLoad tranObj;
    cfgClass cfgObj;
    string A = GetExePath() + getFilePath(cfgObj.getParameter("savePath"));//路径
    const char* dir = A.c_str();
    if (_access(dir, 0) == -1) { //判断该文件夹是否存在 ==-1为不存在
        int flag = _mkdir(delFirst(getFilePath(cfgObj.getParameter("savePath"))).c_str());//生成文件夹
    }
    printf("输入help查看帮助 版本为v1.0.0\n");
    printf("本程序的Github仓库链接:https://github.com/KagiamamaHIna/NoitaConsoleTools 可以前来下最新版本或者查看源代码\n本程序使用MIT许可证\n\n");
    while(true){
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
                    printf("原保存的名为%s的存档已删除\n",Commond[1].c_str());
                    //int flag = _rmdir(buf2.c_str());//删除文件夹
                    int flag = _mkdir(buf2.c_str());//生成文件夹
                }
                else {//如果没有该文件夹，那么就生成一个
                    string buf2 = delFirst(getFilePath(cfgObj.getParameter("savePath")) + "\\" + getFilePath(Commond[1]));
                    int flag = _mkdir(buf2.c_str());//生成文件夹
                }
                string copyFile = "xcopy " + getFilePath(cfgObj.getParameter("save00Path")) + "\\save00 " + GetExePath() + getFilePath(cfgObj.getParameter("savePath") + "\\"+ Commond[1]) + " /s /f /h /q /y";
                system(copyFile.c_str());
                printf("名为%s的存档保存完成！\n",Commond[1].c_str());
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
                string copyFile = "xcopy " + GetExePath() + getFilePath(cfgObj.getParameter("savePath") + "\\" + Commond[1])+ " " + getFilePath(cfgObj.getParameter("save00Path")) + "\\save00" + " /s /f /h /q /y";
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
        else if (Commond[0] == "exit" || Commond[0] == "quit") {
            break;
        }
        else if (Commond[0] == "help") {
            printf("指令列表:\n\n1.save指令，用法save (可选参数),如果只是单纯输入save的话，会将存档自动保存到默认位置，\n如果输入了可选参数的话，那么则是保存到你输入的参数的位置\n\n");
            printf("2.load指令，用法load (可选参数)，用法和save指令一致，save 00保存了名为00的存档的话，那么输入load 00即可读取这个存档\n\n");
            printf("3.unload指令，在配置文件里开启此功能后(默认打开)，则在load完存档后可以使用，用处是加载刚才被覆盖掉的存档\n\n");
            printf("4.exit和quit指令，用法exit 或 quit，用处是关闭程序\n\n");
        }
        else {
            cout << "错误:未知的指令:" << Commond[0] << endl;
            cout << "请重新输入" << endl;
        }
    }
    return 0;
}
