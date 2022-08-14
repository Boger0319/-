#include <iostream>
#include <ctime>
#include <vector>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <windows.h>
#include <set>
#include <unordered_map>

#define rint register int
#define qp() \
    ;        \
    system("cls"); // ! 清屏操作

using namespace std;

// ! 默认文件配置
const string password_filename = "password.dat";             // ! 密码文件
const string doctorBasic_filename = "doctor_base.dat";       // ! 医生基本信息
const string doctorVisit_filename = "doctor_duty.dat";       // ! 医生出诊信息
const string patientRegister_filename = "register-sell.dat"; // ! 病人门诊挂号信息
set<string> offices; // ! 汇总报表中存放不重复科室
set<pair<string, string>> doctor; // ! 汇总报表中存放对应的不重复医生与科室
unordered_map<string, bool> doctorBasicId, doctorDutyId; // ! 存放链表中出现的医生编号
unordered_map<string, int> registerNumber;               // ! 存放 Duty 文件中 已挂号人数
vector<pair<string, string>> summaryList;                // ! 存放 summaryReport() 内年份月份
/*
 ! 所有函数进行提前声明
*/
// ! 主菜单
void mainMenu();
// ! 文件操作
// ? 新建文件
void establishFile(const string filename);
// ! 密码操作
// ? 修改密码
void changePassword();
// ! 医生信息操作
void inputDoctorInformation();
void createVisitInfo();
void reviseDoctorBaiscInfo(); // ! 修改
// ! 病人登记
void patientRegister();
void printAllInfo();   // ! 打印所有信息
void searchInfo();     // ! 查找管理
void statisticsInfo(); // ! 统计管理
void summaryReport();  // ! 汇总报表

// ! ----------------------------------------------------------------------------

// ! 结构体配置
typedef struct doctorBasicInformation // ! 医生基本信息
{
    // ?  医生编号、医生姓名、科室、挂号人数、挂号价格、职称、是否专家、备注
    string id;
    string name;
    string office;
    int visitNum;
    double price;
    string rank;
    string expert;
    string remarks;
    struct doctorBasicInformation *prev;
    struct doctorBasicInformation *next;
} doctorBasicInformation;

typedef struct doctorDutyInformation // ! 医生出诊信息
{
    // ?  医生编号、医生姓名、值班日期、所在科室、可挂号人数，已挂号人数
    string id;
    string name;
    string Date;
    string office;
    int visitNum;
    int nowNum;
    struct doctorDutyInformation *next;
} doctorDutyInformation;

typedef struct registersell
{
    // ? 挂号单号、挂号日期、挂号价格、医生编号、病人编号、病人名称、挂号序号
    string number;
    string registerDate;
    double price;
    string doctorId;
    string patientId;
    string patientName;
    string registernum;
    string office;
    struct registersell *next;
} registersell;

// ! 类配置

class passwordObject
{
private:
    string password;

public:
    passwordObject()
    {
        ifstream checkfile(password_filename);
        if (!checkfile)
        {
            establishFile(password_filename);
            ofstream passwordfile(password_filename);
            cout << "第一次进入系统，请设置初始密码：";
            cin >> password;
            passwordfile << password;
            passwordfile.close();
            checkfile.close();
        }
        else
        {
            checkfile >> password;
            checkfile.close();
        }
    }
    void enterSystem()
    {
        string tmp_password;
        ifstream passwordfile(password_filename);
        passwordfile >> password;
        cout << "请输入系统密码：";
        cin >> tmp_password;
        while (tmp_password != password)
        {
            cout << "您所输入的密码有误，请重新输入：";
            cin >> tmp_password;
        }
    }
    bool checkPassword()
    {
        string tmp_password;
        cin >> tmp_password;
        if (tmp_password == password)
        {
            return true;
        }
        return false;
    }
    void saveNewPassword()
    {
        ofstream passwordfile(password_filename);
        cin >> password;
        passwordfile << password;
        passwordfile.close();
    }
};

class doctorInfoObject
{
private:
    doctorBasicInformation *infohead = new doctorBasicInformation;
    doctorBasicInformation *infoend = infohead;

public:
    friend class doctorDutyObject;
    friend class registerObject;
    doctorInfoObject() // ! 医生对象的构造函数 用于创建文件以及构建链表
    {
        ifstream checkfile(doctorBasic_filename);
        if (!checkfile)
        {
            establishFile(doctorBasic_filename);
        }
        doctorBasicInformation *q = new doctorBasicInformation;
        infohead = q;
        while (checkfile.peek() != EOF)
        {
            doctorBasicInformation *p = new doctorBasicInformation;
            checkfile >> p->id >> p->name >> p->office >> p->visitNum >> p->price >> p->rank >> p->expert >> p->remarks;
            if (p->id == "")
                break;
            doctorBasicId[p->id] = true;

            p->prev = q;
            q->next = p;
            p->next = NULL;
            q = p;

            infoend = q;
        }

        checkfile.close();
    }
    ~doctorInfoObject() // ! 医生对象的析构函数 用于删除链表
    {
        doctorBasicInformation *p = infohead->next, *q;
        while (p != NULL)
        {
            q = p;
            p = p->next;
            delete q;
        }
        delete infohead;
    }
    bool checkId(string id) // 存在返回true
    {
        if (doctorBasicId[id])
        {
            return true;
        }
        return false;
    }
    void inputInfoOperate(string id)
    {
        doctorBasicInformation *p = new doctorBasicInformation;

        p->id = id;
        doctorBasicId[id] = true;
        cout << "请输入医生姓名：";
        cin >> p->name;

        cout << "请输入所在科室：";
        cin >> p->office;

        cout << "请输入挂号人数：";
        cin >> p->visitNum;

        cout << "请输入挂号价格：";
        cin >> p->price;

        cout << "请输入医生职称：";
        cin >> p->rank;

        cout << "请输入是否为专家(是/否): ";
        cin >> p->expert;

        cout << "请输入备注(没有请填无): ";
        cin >> p->remarks;

        qp();

        string Info;
        Info = p->id + '\t' + p->name + '\t' + p->office + '\t' + to_string(p->visitNum) + '\t' + to_string(p->price) + '\t' + p->rank + '\t' + p->expert + '\t' + p->remarks;
        // ! 验证操作
        cout << "医生编号：" << p->id << endl;
        cout << "医生姓名：" << p->name << endl;
        cout << "所在科室：" << p->office << endl;
        cout << "挂号人数：" << p->visitNum << endl;
        cout << "挂号价格：" << p->price << endl;
        cout << "医生职称：" << p->rank << endl;
        cout << "是否专家：" << p->expert << endl;
        cout << "备注：" << p->remarks << endl;

        int mode;
        cout << "请检查所填信息是否有误！\n输入 0 取消保存\n输入 1 确定保存\n输入 2 保存且继续录入新信息：";
        cin >> mode;
        switch (mode)
        {
        case 0:
            break;
        case 1:
            infoend->next = p;
            p->prev = infoend;
            infoend = infoend->next;
            saveNewDoctor(Info);
            break;
        case 2:
            infoend->next = p;
            p->prev = infoend;
            infoend = infoend->next;
            saveNewDoctor(Info);
            inputDoctorInformation();
            break;
        }
    }
    void saveNewDoctor(string Info)
    {
        ofstream doctorBasicInfofile;
        doctorBasicInfofile.open(doctorBasic_filename, ios::app);
        doctorBasicInfofile << Info << endl;
        doctorBasicInfofile.close();
    }
    void doctorBasicSave()
    {
        doctorBasicInformation *p = infohead->next;
        ofstream doctorBasicInfofile;
        doctorBasicInfofile.open(doctorBasic_filename, ios::out | ios::trunc);
        while (p != NULL)
        {
            string Info = p->id + '\t' + p->name + '\t' + p->office + '\t' + to_string(p->visitNum) + '\t' + to_string(p->price) + '\t' + p->rank + '\t' + p->expert + '\t' + p->remarks;
            doctorBasicInfofile << Info << endl;
            p = p->next;
        }

        doctorBasicInfofile.close();
    }
    void printAllDoctorBasicInfo()
    {
        doctorBasicInformation *p = infohead->next;

        // ! 表头
        cout << "\033[31;1m┏";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┓" << endl;
        cout << "┃" << setw(60) << "医生基本信息" << setw(56) << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫\033[0m" << endl;
        cout << "┃" << setw(10) << "医生编号" << setw(10) << "医生姓名" << setw(10) << "所在科室" << setw(10) << "挂号人数" << setw(10) << "挂号价格" << setw(14) << "医生职称" << setw(10) << "是否专家" << right << setw(40) << "医生备注"
             << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;

        while (p != NULL)
        {
            cout << "┃";
            cout << setw(10) << p->id << setw(10) << p->name << setw(10) << p->office << setw(10) << p->visitNum << setw(10) << p->price << setw(14) << p->rank << setw(10) << p->expert << right << setw(40) << p->remarks << "┃" << endl;
            p = p->next;
        }
        cout << "┗";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┛" << endl;
    }
    void printSingleDoctorBasicInfo(string id)
    {
        qp();
        doctorBasicInformation *p = infohead->next;

        // ! 表头
        cout << "┏";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┓" << endl;
        cout << "┃" << setw(60) << "医生基本信息" << setw(56) << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;
        cout << "┃" << setw(10) << "医生编号" << setw(10) << "医生姓名" << setw(10) << "所在科室" << setw(10) << "挂号人数" << setw(10) << "挂号价格" << setw(14) << "医生职称" << setw(10) << "是否专家" << right << setw(40) << "医生备注"
             << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;

        while (p != NULL)
        {
            if (p->id == id)
            {
                cout << "┃";
                cout << setw(10) << p->id << setw(10) << p->name << setw(10) << p->office << setw(10) << p->visitNum << setw(10) << p->price << setw(14) << p->rank << setw(10) << p->expert << right << setw(40) << p->remarks << "┃" << endl;
            }
            p = p->next;
        }
        cout << "┗";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┛" << endl;
    }
    void printSingleDoctorBasicInfoInList(doctorBasicInformation *p)
    {
        cout << "┃";
        cout << setw(10) << p->id << setw(10) << p->name << setw(10) << p->office << setw(10) << p->visitNum << setw(10) << p->price << setw(14) << p->rank << setw(10) << p->expert << right << setw(40) << p->remarks << "┃" << endl;
    }
    void searchDoctorBasicInfo(int condition)
    {
        doctorBasicInformation *doctorInfo = infohead->next;

        int visitNum;
        double price;
        string all;

        switch (condition)
        {
        case 1:
            cout << "请输入医生编号: ";
            cin >> all;
            break;
        case 2:
            cout << "请输入医生姓名: ";
            cin >> all;
            break;
        case 3:
            cout << "请输入所在科室: ";
            cin >> all;
            break;
        case 4:
            cout << "请输入挂号人数: ";
            cin >> visitNum;
            break;
        case 5:
            cout << "请输入挂号价格: ";
            cin >> price;
            break;
        case 6:
            cout << "请输入职称: ";
            cin >> all;
            break;
        default:
            return;
            break;
        }

        qp();

        int cnt = 0; // ! 统计符合条件信息的数量 0则输出没有符合信息

        // ! 表头
        cout << "┏";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┓" << endl;
        cout << "┃" << setw(60) << "医生基本信息" << setw(56) << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;
        cout << "┃" << setw(10) << "医生编号" << setw(10) << "医生姓名" << setw(10) << "所在科室" << setw(10) << "挂号人数" << setw(10) << "挂号价格" << setw(14) << "医生职称" << setw(10) << "是否专家" << right << setw(40) << "医生备注"
             << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;

        while (doctorInfo != NULL)
        {
            switch (condition)
            {
            case 1:
                if (doctorInfo->id == all)
                {
                    printSingleDoctorBasicInfoInList(doctorInfo);
                    cnt++;
                }
                break;
            case 2:
                if (doctorInfo->name == all)
                {
                    printSingleDoctorBasicInfoInList(doctorInfo);
                    cnt++;
                }
                break;
            case 3:
                if (doctorInfo->office == all)
                {
                    printSingleDoctorBasicInfoInList(doctorInfo);
                    cnt++;
                }
                break;
            case 4:
                if (doctorInfo->visitNum == visitNum)
                {
                    printSingleDoctorBasicInfoInList(doctorInfo);
                    cnt++;
                }
                break;
            case 5:
                if (doctorInfo->price == price)
                {
                    printSingleDoctorBasicInfoInList(doctorInfo);
                    cnt++;
                }
                break;
            case 6:
                if (doctorInfo->rank == all)
                {
                    printSingleDoctorBasicInfoInList(doctorInfo);
                    cnt++;
                }
                break;
            default:
                break;
            }
            doctorInfo = doctorInfo->next;
        }

        // cout << cnt << endl;

        if (cnt == 0)
        {
            cout << "┃\033[31;1m";
            cout << setw(65) << "无符合条件信息！" << setw(49) << "";
            cout << "\033[00m┃" << endl;
        }
        cout << "┗";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┛" << endl;
        int re;
        cout << "输入 1 返回主菜单: ";
        while (cin >> re)
        {
            if (re == 1)
                break;
        }
    }
};

class doctorDutyObject : public doctorInfoObject
{
private:
    doctorDutyInformation *dutyhead = new doctorDutyInformation;
    string date;

public:
    friend class registerObject;
    doctorDutyObject() // ! 医生对象的构造函数 用于创建文件以及构建链表
    {

        ifstream checkfile(doctorVisit_filename);
        if (!checkfile)
        {
            establishFile(doctorVisit_filename);
        }
        doctorDutyInformation *q = new doctorDutyInformation;
        dutyhead = q;
        // checkfile >> p->id >> p->name >> p->office >> p->visitNum >> p->price >> p->rank >> p->expert >> p->remarks;
        while (checkfile.peek() != EOF)
        {
            doctorDutyInformation *p = new doctorDutyInformation;
            checkfile >> p->id >> p->name >> p->Date >> p->office >> p->visitNum >> p->nowNum;
            if (p->id == "")
                break;
            doctorDutyId[p->id] = true;
            offices.insert(p->office);
            doctor.insert(make_pair(p->name, p->office));
            q->next = p;
            p->next = NULL;
            q = p;
        }
        checkfile.close();
    }
    ~doctorDutyObject() // ! 医生对象的析构函数 用于删除链表
    {
        doctorDutyInformation *p = dutyhead->next, *q;
        while (p != NULL)
        {
            q = p;
            p = p->next;
            delete q;
        }
        delete dutyhead;
    }
    bool checkDuty(string id)
    {
        doctorDutyInformation *check = dutyhead->next;
        cout << "请输入值班日期（格式：YYYY-MM-DD）：";
        cin >> date;
        while (check != NULL)
        {
            if (check->id == id && check->Date == date)
            {
                return true;
            }
            check = check->next;
        }
        return false;
    }
    void generateDutyInfo(string id)
    {
        doctorBasicInformation *search = infohead->next;
        doctorDutyInformation *save = new doctorDutyInformation;
        while (search != NULL)
        {
            if (search->id == id)
            {
                save->id = search->id;
                save->name = search->name;
                save->Date = date;
                save->office = search->office;
                save->visitNum = search->visitNum;
                save->nowNum = 0;
            }
            search = search->next;
        }

        qp();
        cout << "出诊信息已生成：" << endl;
        printf("\n");
        cout << "医生编号：" << save->id << endl;
        cout << "医生姓名：" << save->name << endl;
        cout << "值班日期：" << save->Date << endl;
        cout << "所在科室：" << save->office << endl;
        cout << "可挂号人数：" << save->visitNum << endl;
        cout << "已挂号人数：" << save->nowNum << endl
             << endl;
        cout << "5秒后返回主菜单";
        Sleep(5000);
        doctorDutyId[save->id] = true;
        ofstream doctorDutyInfofile;
        doctorDutyInfofile.open(doctorVisit_filename, ios::app);
        doctorDutyInfofile << save->id << '\t' << save->name << '\t' << save->Date << '\t' << save->office << '\t' << save->visitNum << '\t' << save->nowNum << endl;
        doctorDutyInfofile.close();
    }
    void deleteBaiscInfo(string id) // ! 删除医生基本信息
    {
        doctorDutyInformation *visitNumsearch = dutyhead->next;
        doctorBasicInformation *infoSearch = infohead->next;

        string today;
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int year = 1900 + ltm->tm_year, month = ltm->tm_mon + 1, day = ltm->tm_mday;
        string mon = to_string(month), d = to_string(day);

        if (mon.size() == 1)
            mon = '0' + mon;
        if (d.size() == 1)
            d = '0' + d;

        today = to_string(year) + '-' + mon + '-' + d;

        int nowAndAfternum = 0;
        while (visitNumsearch != NULL)
        {
            if (visitNumsearch->id == id)
            {
                // cout << visitNumsearch->Date << " " << today << endl;
                if (visitNumsearch->Date >= today)
                {
                    nowAndAfternum += visitNumsearch->nowNum;
                }
            }
            visitNumsearch = visitNumsearch->next;
        }

        if (nowAndAfternum > 0)
        {
            // cout << nowAndAfternum << endl;
            cout << "该医生已有病人挂号，不能删除医生基本信息!" << endl;
        }
        else
        {
            while (infoSearch != NULL)
            {
                cout << infoSearch->id << endl;
                if (infoSearch->id == id)
                {
                    infoSearch->prev->next = infoSearch->next;
                    doctorBasicId[id] = false;
                    break;
                }
                infoSearch = infoSearch->next;
            }
            cout << "该医生基本信息删除成功!" << endl;
            doctorBasicSave();
            Sleep(2000);
        }
    }
    void printAllDoctorDutyInfo()
    {
        doctorDutyInformation *p = dutyhead->next;

        // ! 表头
        cout << "\033[32;1m┏";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┓" << endl;
        cout << "┃" << setw(60) << "医生出诊信息" << setw(54) << ""
             << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫\033[00m" << endl;
        cout << "┃" << setw(18) << "医生编号" << setw(18) << "医生姓名" << setw(18) << "所在科室" << setw(24) << "出诊日期" << setw(18) << "可挂号人数" << setw(18) << "已挂号人数"
             << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;

        while (p != NULL)
        {
            cout << "┃";
            cout << setw(18) << p->id << setw(18) << p->name << setw(18) << p->office << setw(24) << p->Date << setw(18) << p->visitNum << setw(18) << p->nowNum << "┃" << endl;
            p = p->next;
        }
        cout << "┗";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┛" << endl;
    }
    void reviseDoctorBasicInfo(string id)
    {
        doctorBasicInformation *search = infohead->next;

        // ? 用于存储修改后数据
        string newid, name, office, rank, expert, remarks;
        int visitNum;
        double price;

        while (search != NULL)
        {
            if (search->id == id)
            {
                // ! 输入新信息
                cout << "修改医生编号: ";
                cin >> newid;
                cout << "修改医生姓名: ";
                cin >> name;
                cout << "修改医生科室: ";
                cin >> office;
                cout << "修改挂号人数: ";
                cin >> visitNum;
                cout << "修改挂号价格: ";
                cin >> price;
                cout << "修改医生职称: ";
                cin >> rank;
                cout << "修改专家称号: ";
                cin >> expert;
                cout << "修改医生备注: ";
                cin >> remarks;

                // ! 验证操作
                qp();
                cout << "请验证修改后信息："
                     << "\n"
                     << "\n";
                cout << "医生编号: " << newid << endl;
                cout << "医生姓名: " << name << endl;
                cout << "医生科室: " << office << endl;
                cout << "挂号人数: " << visitNum << endl;
                cout << "挂号价格: " << price << endl;
                cout << "医生职称: " << rank << endl;
                cout << "专家称号: " << expert << endl;
                cout << "医生备注: " << remarks << endl;

                cout << "请检查所填信息是否有误！\n输入 0 取消保存\n输入 1 确定保存: ";

                int mode;
                cin >> mode;
                if (mode == 0)
                    break;
                else if (mode == 1)
                {
                    search->id = newid;
                    search->name = name;
                    search->office = office;
                    search->visitNum = visitNum;
                    search->price = price;
                    search->rank = rank;
                    search->expert = expert;
                    search->remarks = remarks;
                    doctorBasicSave();
                }
                break;
            }
            search = search->next;
        }
    }
    void printSingleDoctorDutyInfoInList(doctorDutyInformation *p)
    {
        cout << "┃";
        cout << setw(18) << p->id << setw(18) << p->name << setw(18) << p->office << setw(24) << p->Date << setw(18) << p->visitNum << setw(18) << p->nowNum << "┃" << endl;
    }
    void searchDoctorDutyInfo(int condition)
    {
        doctorDutyInformation *doctorInfo = dutyhead->next;

        // ? 医生编号、医生姓名、值班日期、科室、可挂号人数、已挂号人数

        int visitNum, nowNum;
        string all;

        switch (condition)
        {
        case 1:
            cout << "请输入医生编号: ";
            cin >> all;
            break;
        case 2:
            cout << "请输入医生姓名: ";
            cin >> all;
            break;
        case 3:
            cout << "请输入值班日期: ";
            cin >> all;
            break;
        case 4:
            cout << "请输入所在科室: ";
            cin >> all;
            break;
        case 5:
            cout << "请输入可挂号人数: ";
            cin >> visitNum;
            break;
        case 6:
            cout << "请输入已挂号人数: ";
            cin >> nowNum;
            break;
        default:
            return;
            break;
        }

        qp();

        int cnt = 0; // ! 统计符合条件信息的数量 0则输出没有符合信息

        // ! 表头
        cout << "┏";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┓" << endl;
        cout << "┃" << setw(60) << "医生出诊信息" << setw(54) << ""
             << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;
        cout << "┃" << setw(18) << "医生编号" << setw(18) << "医生姓名" << setw(18) << "所在科室" << setw(24) << "出诊日期" << setw(18) << "可挂号人数" << setw(18) << "已挂号人数"
             << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;

        while (doctorInfo != NULL)
        {
            switch (condition)
            {
            case 1:
                if (doctorInfo->id == all)
                {
                    printSingleDoctorDutyInfoInList(doctorInfo);
                    cnt++;
                }
                break;
            case 2:
                if (doctorInfo->name == all)
                {
                    printSingleDoctorDutyInfoInList(doctorInfo);
                    cnt++;
                }
                break;
            case 3:
                if (doctorInfo->Date == all)
                {
                    printSingleDoctorDutyInfoInList(doctorInfo);
                    cnt++;
                }
                break;
            case 4:
                if (doctorInfo->office == all)
                {
                    printSingleDoctorDutyInfoInList(doctorInfo);
                    cnt++;
                }
                break;
            case 5:
                if (doctorInfo->visitNum == visitNum)
                {
                    printSingleDoctorDutyInfoInList(doctorInfo);
                    cnt++;
                }
                break;
            case 6:
                if (doctorInfo->nowNum == nowNum)
                {
                    printSingleDoctorDutyInfoInList(doctorInfo);
                    cnt++;
                }
                break;
            default:
                break;
            }
            doctorInfo = doctorInfo->next;
        }

        if (cnt == 0)
        {
            cout << "┃\033[31;1m";
            cout << setw(65) << "无符合条件信息！" << setw(49) << "";
            cout << "\033[00m┃" << endl;
        }
        cout << "┗";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┛" << endl;
        int re;
        cout << "输入 1 返回主菜单: ";
        while (cin >> re)
        {
            if (re == 1)
                break;
        }
    }
    void statisticsDoctorDutyInfo(int condition)
    {
        doctorDutyInformation *doctorInfo = dutyhead->next;
        doctorDutyInformation *showInfo;

        int visitNum = 0, nowNum = 0;
        string all;

        switch (condition)
        {
        case 1:
            cout << "请输入医生编号: ";
            cin >> all;
            break;
        case 2:
            cout << "请输入医生姓名: ";
            cin >> all;
            break;
        case 3:
            cout << "请输入值班日期: ";
            cin >> all;
            break;
        case 4:
            cout << "请输入所在科室: ";
            cin >> all;
            break;
        default:
            return;
            break;
        }

        qp();

        int cnt = 0; // ! 统计符合条件信息的数量 0则输出没有符合信息

        while (doctorInfo != NULL)
        {
            switch (condition)
            {
            case 1:
                if (doctorInfo->id == all)
                {
                    showInfo = doctorInfo;
                    visitNum += doctorInfo->visitNum;
                    nowNum += doctorInfo->nowNum;
                    cnt++;
                }
                break;
            case 2:
                if (doctorInfo->name == all)
                {
                    showInfo = doctorInfo;
                    visitNum += doctorInfo->visitNum;
                    nowNum += doctorInfo->nowNum;
                    cnt++;
                }
                break;
            case 3:
                if (doctorInfo->Date == all)
                {
                    showInfo = doctorInfo;
                    visitNum += doctorInfo->visitNum;
                    nowNum += doctorInfo->nowNum;
                    cnt++;
                }
                break;
            case 4:
                if (doctorInfo->office == all)
                {
                    showInfo = doctorInfo;
                    visitNum += doctorInfo->visitNum;
                    nowNum += doctorInfo->nowNum;
                    cnt++;
                }
                break;
            default:
                break;
            }
            doctorInfo = doctorInfo->next;
        }

        // ! 表头
        cout << "┏";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┓" << endl;
        cout << "┃" << setw(60) << "医生出诊信息" << setw(54) << ""
             << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;
        cout << "┃";

        switch (condition)
        {
        case 1:
            cout << setw(38) << "医生编号";
            break;
        case 2:
            cout << setw(38) << "医生姓名";
            break;
        case 3:
            cout << setw(38) << "所在科室";
            break;
        case 4:
            cout << setw(38) << "挂号人数";
            break;
        case 5:
            cout << setw(38) << "挂号价格";
            break;
        case 6:
            cout << setw(38) << "医生职称";
            break;
        default:
            return;
            break;
        }
        cout << setw(38) << "可挂号人数" << setw(38) << "已挂号人数"
             << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;

        if (cnt == 0)
        {
            cout << "┃\033[31;1m";
            cout << setw(65) << "无符合条件信息！" << setw(49) << "";
            cout << "\033[00m┃" << endl;
        }
        else
        {
            cout << "┃";
            // cout << setw(18) << showInfo->id << setw(18) << showInfo->name << setw(18) << showInfo->office << setw(24) << showInfo->Date << setw(18) << visitNum << setw(18) << nowNum << "┃" << endl;
            cout << setw(38) << all << setw(38) << visitNum << setw(38) << nowNum << "┃" << endl;
        }

        cout << "┗";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┛" << endl;
        int re;
        cout << "输入 1 返回主菜单: ";
        while (cin >> re)
        {
            if (re == 1)
                break;
        }
    }

    void doctorDutySave() // ! 覆盖写入文件 用于 修改删除
    {
        doctorDutyInformation *p = dutyhead->next;
        ofstream doctorDutyInfofile;
        doctorDutyInfofile.open(doctorVisit_filename, ios::out | ios::trunc);
        while (p != NULL)
        {
            string Info = p->id + '\t' + p->name + '\t' + p->Date + '\t' + p->office + '\t' + to_string(p->visitNum) + '\t' + to_string(p->nowNum);
            // cout << Info << endl;
            doctorDutyInfofile << Info << endl;
            p = p->next;
        }

        doctorDutyInfofile.close();
    }
    void summaryMonthlyReport()
    {
        qp();
        // 科室	起始日期	终止日期	挂号人数
        cout << "┏";
        for (int i = 0; i < 57; i++)
            cout << "━";
        cout << "┓" << endl;
        cout << "┃" << setw(11) << "科室" << setw(16) << "起始日期" << setw(16) << "终止日期" << setw(14) << "挂号人数"
             << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 57; i++)
            cout << "━";
        cout << "┫" << endl;
        for (auto o : offices)
        {
            for (rint j = 0; j < summaryList.size(); j++)
            {
                rint cnt = 0;
                doctorDutyInformation *search = dutyhead->next;
                while (search != NULL)
                {
                    if (search->Date >= summaryList[j].first && search->Date <= summaryList[j].second && search->office == o)
                    {
                        cnt += search->nowNum;
                    }
                    search = search->next;
                }
                cout << "┃" << setw(11) << o << setw(16) << summaryList[j].first << setw(16) << summaryList[j].second << setw(14) << cnt << "┃" << endl;
            }
        }
        cout << "┗";
        for (int i = 0; i < 57; i++)
            cout << "━";
        cout << "┛" << endl;
        int re;
        cout << "输入 1 返回主菜单: ";
        while (cin >> re)
        {
            if (re == 1)
                break;
        }
    }
    void summaryDoctorReport()
    {
        qp();
        // 科室	起始日期	终止日期	挂号人数
        cout << "┏";
        for (int i = 0; i < 68; i++)
            cout << "━";
        cout << "┓" << endl;
        cout << "┃" << setw(11) << "科室" << setw(11) << "医生编号" << setw(16) << "起始日期" << setw(16) << "终止日期" << setw(14) << "挂号人数"
             << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 68; i++)
            cout << "━";
        cout << "┫" << endl;
        for (auto o : offices)
        {
            for (auto d : doctor)
            {
                for (rint j = 0; j < summaryList.size(); j++)
                {
                    rint cnt = 0;
                    doctorDutyInformation *search = dutyhead->next;
                    while (search != NULL)
                    {
                        if (d.first == search->name && d.second == o && search->Date >= summaryList[j].first && search->Date <= summaryList[j].second && search->office == o)
                        {
                            cnt += search->nowNum;
                        }
                        search = search->next;
                    }
                    if (d.second == o)
                    {
                        cout << "┃" << setw(11) << o << setw(11) << d.first << setw(16) << summaryList[j].first << setw(16) << summaryList[j].second << setw(14) << cnt << "┃" << endl;
                    }
                }
            }
        }
        cout << "┗";
        for (int i = 0; i < 68; i++)
            cout << "━";
        cout << "┛" << endl;
        int re;
        cout << "输入 1 返回主菜单: ";
        while (cin >> re)
        {
            if (re == 1)
                break;
        }
    }
};

class registerObject : public doctorDutyObject // 病人信息登记
{
private:
    registersell *registerhead = new registersell;

public:
    registerObject() // ! 初始化
    {
        ifstream checkfile(patientRegister_filename);
        if (!checkfile)
        {
            establishFile(patientRegister_filename);
        }
        registersell *q = new registersell;
        registerhead = q;
        // checkfile >> p->id >> p->name >> p->office >> p->visitNum >> p->price >> p->rank >> p->expert >> p->remarks;
        while (checkfile.peek() != EOF)
        {
            registersell *p = new registersell;
            checkfile >> p->number >> p->registerDate >> p->price >> p->doctorId >> p->office >> p->patientId >> p->patientName >> p->registernum;
            if (p->number == "")
                break;

            initRegisterNumber(p->doctorId, p->registerDate, p->registernum);

            q->next = p;
            p->next = NULL;
            q = p;
        }
        checkfile.close();
    }
    ~registerObject() // ! 病人挂号的析构函数 用于删除链表
    {
        registersell *p = registerhead->next, *q;
        while (p != NULL)
        {
            q = p;
            p = p->next;
            delete q;
        }
        delete registerhead;
    }
    bool checkDocotr(string id, string visitDate)
    {
        doctorDutyInformation *search = dutyhead->next;
        while (search != NULL)
        {
            if (search->id == id && search->Date == visitDate)
            {
                if (search->visitNum - search->nowNum > 0)
                {
                    return true;
                }
            }
            search = search->next;
        }
        return false;
    }
    double searchPrice(string id)
    {
        doctorBasicInformation *search = infohead->next;
        while (search != NULL)
        {
            if (search->id == id)
            {
                return search->price;
            }
            search = search->next;
        }
    }
    string searchOffice(string id)
    {
        doctorBasicInformation *search = infohead->next;
        while (search != NULL)
        {
            if (search->id == id)
            {
                return search->office;
            }
            search = search->next;
        }
    }
    void initRegisterNumber(string doctorId, string registerDate, string registernum)
    {
        string newNumber = doctorId + registerDate.substr(0, 4) + registerDate.substr(5, 2) + registerDate.substr(8, 2);
        registerNumber[newNumber] = stoi(registernum);
    }
    int generateRegisterNumber(string doctorId, string registerDate)
    {
        string newNumber = doctorId + registerDate.substr(0, 4) + registerDate.substr(5, 2) + registerDate.substr(8, 2);

        doctorDutyInformation *search = dutyhead->next;
        while (search != NULL)
        {
            if (search->id == doctorId && search->Date == registerDate)
            {
                search->nowNum = registerNumber[newNumber] + 1;
                // cout << "nowNum = " << search->nowNum << endl;
                doctorDutySave();
                break;
            }
            search = search->next;
        }

        return registerNumber[newNumber] + 1;
    }
    void generateRegisterInfo(string id, string registerDate)
    {
        registersell *patient = new registersell;
        // cout << " registerNumber :" << generateRegisterNumber(id, registerDate) << endl;
        string number;
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int year = 1900 + ltm->tm_year, month = ltm->tm_mon + 1, day = ltm->tm_mday, hour = ltm->tm_hour, min = ltm->tm_min, sec = ltm->tm_sec;
        string mon = to_string(month), d = to_string(day), h = to_string(hour), m = to_string(min), s = to_string(sec);
        if (mon.size() == 1)
            mon = '0' + mon;
        if (d.size() == 1)
            d = '0' + d;
        if (h.size() == 1)
            h = '0' + h;
        if (m.size() == 1)
            m = '0' + m;
        if (s.size() == 1)
            s = '0' + s;
        number = to_string(year) + mon + d + h + m + s;

        int registernum = generateRegisterNumber(id, registerDate);

        patient->doctorId = id;               // ? 医生编号
        patient->registerDate = registerDate; // ? 挂号日期
        cout << "请输入病人编号：";
        cin >> patient->patientId;
        cout << "请输入病人名称：";
        cin >> patient->patientName;
        // cout << "请输入挂号价格：";
        patient->price = searchPrice(id);
        patient->office = searchOffice(id);
        patient->number = number;

        patient->registernum = to_string(registernum);

        qp();
        cout << "挂号信息已生成：" << endl;
        printf("\n");
        cout << "挂号单号：" << patient->number << endl;
        cout << "挂号日期：" << patient->registerDate << endl;
        cout << "挂号价格：" << patient->price << endl;
        cout << "医生编号：" << patient->doctorId << endl;
        cout << "病人编号：" << patient->patientId << endl;
        cout << "病人名称：" << patient->patientName << endl;
        cout << "挂号序号：" << patient->registernum << endl
             << endl;
        int re;
        cout << "输入 1 返回主菜单: ";
        while (cin >> re)
        {
            if (re == 1)
                break;
        }
        ofstream patientRegisterFile;
        patientRegisterFile.open(patientRegister_filename, ios::app);
        patientRegisterFile << patient->number << "\t" << patient->registerDate << "\t" << patient->price << "\t" << patient->doctorId << "\t" << patient->office << "\t" << patient->patientId << "\t" << patient->patientName << "\t" << patient->registernum << endl;
        patientRegisterFile.close();
    }
    void printAllRegisterSell()
    {
        registersell *p = registerhead->next;

        // ! 表头
        cout << "\033[34;1m┏";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┓" << endl;
        cout << "┃" << setw(62) << "病人门诊挂号信息" << setw(54) << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫\033[00m" << endl;
        cout << "┃" << setw(18) << "挂号单号" << setw(16) << "挂号日期" << setw(16) << "挂号价格" << setw(16) << "医生编号" << setw(16) << "病人编号" << setw(16) << "病人名称" << setw(16) << "挂号序号"
             << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;

        while (p != NULL)
        {
            cout << "┃";
            cout << setw(18) << p->number << setw(16) << p->registerDate << setw(16) << p->price << setw(16) << p->doctorId << setw(16) << p->patientId << setw(16) << p->patientName << setw(16) << p->registernum << "┃" << endl;
            p = p->next;
        }
        cout << "┗";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┛" << endl;
    }
    void printSingleRegisterSellInfoInList(registersell *p)
    {
        cout << "┃";
        cout << setw(18) << p->number << setw(16) << p->registerDate << setw(16) << p->price << setw(16) << p->doctorId << setw(16) << p->patientId << setw(16) << p->patientName << setw(16) << p->registernum << "┃" << endl;
    }
    void searchRegisterSellnfo(int condition)
    {
        registersell *patientInfo = registerhead->next;

        // ? 挂号单号、医生编号、病人编号、病人名称、挂号序号

        string all;

        switch (condition)
        {
        case 1:
            cout << "请输入挂号单号: ";
            cin >> all;
            break;
        case 2:
            cout << "请输入医生编号: ";
            cin >> all;
            break;
        case 3:
            cout << "请输入病人编号: ";
            cin >> all;
            break;
        case 4:
            cout << "请输入病人名称: ";
            cin >> all;
            break;
        case 5:
            cout << "请输入挂号序号: ";
            cin >> all;
            break;
        default:
            return;
            break;
        }

        qp();

        int cnt = 0; // ! 统计符合条件信息的数量 0则输出没有符合信息

        // ! 表头
        cout << "┏";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┓" << endl;
        cout << "┃" << setw(66) << "病人门诊挂号信息" << setw(50) << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;
        cout << "┃" << setw(18) << "挂号单号" << setw(16) << "挂号日期" << setw(16) << "挂号价格" << setw(16) << "医生编号" << setw(16) << "病人编号" << setw(16) << "病人名称" << setw(16) << "挂号序号"
             << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;

        while (patientInfo != NULL)
        {
            switch (condition)
            {
            case 1:
                if (patientInfo->number == all)
                {
                    printSingleRegisterSellInfoInList(patientInfo);
                    cnt++;
                }
                break;
            case 2:
                if (patientInfo->doctorId == all)
                {
                    printSingleRegisterSellInfoInList(patientInfo);
                    cnt++;
                }
                break;
            case 3:
                if (patientInfo->patientId == all)
                {
                    printSingleRegisterSellInfoInList(patientInfo);
                    cnt++;
                }
                break;
            case 4:
                if (patientInfo->patientName == all)
                {
                    printSingleRegisterSellInfoInList(patientInfo);
                    cnt++;
                }
                break;
            case 5:
                if (patientInfo->registernum == all)
                {
                    printSingleRegisterSellInfoInList(patientInfo);
                    cnt++;
                }
                break;
            default:
                break;
            }
            patientInfo = patientInfo->next;
        }

        if (cnt == 0)
        {
            cout << "┃\033[31;1m";
            cout << setw(65) << "无符合条件信息！" << setw(49) << "";
            cout << "\033[00m┃" << endl;
        }
        cout << "┗";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┛" << endl;
        int re;
        cout << "输入 1 返回主菜单: ";
        while (cin >> re)
        {
            if (re == 1)
                break;
        }
    }
    void statisticsRegisterSellInfo(int condition)
    {
        registersell *sell = registerhead->next;
        registersell *showInfo;

        int num = 0;
        string all;

        switch (condition)
        {
        case 1:
            cout << "请输入医生编号: ";
            cin >> all;
            break;
        case 2:
            cout << "请输入病人编号: ";
            cin >> all;
            break;
        case 3:
            cout << "请输入病人名称: ";
            cin >> all;
            break;
        case 4:
            cout << "请输入所在科室: ";
            cin >> all;
            break;
        default:
            return;
            break;
        }

        qp();

        int cnt = 0; // ! 统计符合条件信息的数量 0则输出没有符合信息

        while (sell != NULL)
        {
            switch (condition)
            {
            case 1:
                if (sell->doctorId == all)
                {
                    showInfo = sell;
                    num++;
                    cnt++;
                }
                break;
            case 2:
                if (sell->patientId == all)
                {
                    num++;
                    cnt++;
                }
                break;
            case 3:
                if (sell->patientName == all)
                {
                    num++;
                    cnt++;
                }
                break;
            case 4:
                if (sell->office == all)
                {
                    num++;
                    cnt++;
                }
                break;
            default:
                break;
            }
            sell = sell->next;
        }

        // ! 表头
        cout << "┏";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┓" << endl;
        cout << "┃" << setw(66) << "病人门诊挂号信息" << setw(50) << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;
        cout << "┃";
        switch (condition)
        {
        case 1:
            cout << setw(54) << "医生编号";
            break;
        case 2:
            cout << setw(54) << "病人编号";
            break;
        case 3:
            cout << setw(54) << "病人姓名";
            break;
        case 4:
            cout << setw(54) << "所在科室";
            break;
        default:
            break;
        }
        cout << setw(60) << "挂号人数";
        cout << "┃" << endl;
        cout << "┣";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┫" << endl;

        if (cnt == 0)
        {
            cout << "┃\033[31;1m";
            cout << setw(65) << "无符合条件信息！" << setw(49) << "";
            cout << "\033[00m┃" << endl;
        }
        else
        {
            cout << "┃";
            cout << setw(54) << all << setw(60) << num << "┃" << endl;
        }

        cout << "┗";
        for (int i = 0; i < 114; i++)
            cout << "━";
        cout << "┛" << endl;
        int re;
        cout << "输入 1 返回主菜单: ";
        while (cin >> re)
        {
            if (re == 1)
                break;
        }
    }
};

// ! 函数体
void mainMenu()
{
    qp();
    string selectedMode;
    cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓" << endl;
    cout << "┃                     医院门诊信息管理系统                     ┃" << endl;
    cout << "┣━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫" << endl;
    cout << "┃   1   ┃设置及修改密码                                        ┃" << endl;
    cout << "┃   2   ┃录入医生基本信息                                      ┃" << endl;
    cout << "┃   3   ┃医生出诊信息的生成                                    ┃" << endl;
    cout << "┃   4   ┃病人门诊挂号管理                                      ┃" << endl;
    cout << "┃   5   ┃修改医生基本信息（包括修改、删除功能）                ┃" << endl;
    cout << "┃   6   ┃输出全部医生基本信息、医生出诊信息、病人门诊挂号信息  ┃" << endl;
    cout << "┃   7   ┃查询管理                                              ┃" << endl;
    cout << "┃   8   ┃统计管理                                              ┃" << endl;
    cout << "┃   9   ┃汇总报表                                              ┃" << endl;
    cout << "┃   0   ┃退出系统                                              ┃" << endl;
    cout << "┗━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛" << endl;
    cout << "请输入对应序号选择您所需要的操作：";

    while (cin >> selectedMode)
    {
        if (selectedMode.size() > 1)
        {
            cout << "您的输入有误，请重新输入：";
        }
        else if (selectedMode[0] < '0' || selectedMode[0] > '9')
        {
            cout << "您的输入有误，请重新输入：";
        }
        else
        {
            break;
        }
    }
    // getchar();
    switch (selectedMode[0])
    {
    case '0':
        exit(0);
        break;
    case '1':
        changePassword();
        break;
    case '2':
        inputDoctorInformation();
        break;
    case '3':
        createVisitInfo();
        break;
    case '4':
        patientRegister();
        break;
    case '5':
        reviseDoctorBaiscInfo();
        break;
    case '6':
        printAllInfo();
        break;
    case '7':
        searchInfo();
        break;
    case '8':
        statisticsInfo();
        break;
    case '9':
        summaryReport();
        break;
    }
}

void establishFile(const string filename)
{
    ofstream file(filename);
    file.close();
}

void changePassword()
{
    qp();
    passwordObject password;
    cout << "请输入原密码：";
    while (!password.checkPassword())
    {
        cout << "密码输入错误，请重新输入：";
    }
    cout << "请输入新密码：";
    password.saveNewPassword();
    mainMenu();
}

void inputDoctorInformation()
{
    qp();
    doctorInfoObject doctor;
    string id;
    cout << "请输入医生编号：";
    cin >> id;
    if (!doctor.checkId(id))
    {
        doctor.inputInfoOperate(id);
    }
    else
    {
        cout << "您所输入的医生编号已存在，为您返回主菜单" << endl;
        Sleep(3000);
    }
    mainMenu();
}

void createVisitInfo()
{
    qp();
    doctorDutyObject doctorDuty;
    string id;
    cout << "请输入值班医生编号：";
    cin >> id;
    if (doctorDuty.checkId(id))
    {
        if (doctorDuty.checkDuty(id))
        {
            cout << "您所输入的医生值班信息已生成，请勿重复操作" << endl;
            Sleep(3000);
        }
        else
        {
            doctorDuty.generateDutyInfo(id);
        }
    }
    else
    {
        cout << "您所输入的医生编号不存在，为您返回主菜单" << endl;
        Sleep(3000);
    }
    mainMenu();
}

void reviseDoctorBaiscInfo()
{
    doctorDutyObject doctor;
    string id;
    int selectedMode;
    qp();
    cout << "请输入进行操作的医生编号：";
    cin >> id;
    if (doctor.checkId(id))
    {
        doctor.printSingleDoctorBasicInfo(id);
        cout << "请输入您的操作\n输入 0 返回主程序\n输入 1 进行修改\n输入 2 进行删除：";
        cin >> selectedMode;
        switch (selectedMode)
        {
        case 0:
            break;
        case 1:
            // ? 修改
            doctor.reviseDoctorBasicInfo(id);
            break;
        case 2:
            doctor.deleteBaiscInfo(id);
            Sleep(3000);
        default:
            break;
        }
    }
    else
    {
        cout << "医生信息不存在！3秒后返回主菜单" << endl;
        Sleep(3000);
    }
    mainMenu();
}

void patientRegister()
{
    qp();
    string visitdate, id;
    registerObject patient;
    cout << "请输入值班日期（格式：YYYY-MM-DD）：";
    cin >> visitdate;
    cout << "请输入医生编号：";
    cin >> id;
    if (doctorBasicId[id] == false)
    {
        cout << "\033[1;5m医生信息不存在！\033[0m" << endl;
        Sleep(3000);
    }
    else
    {
        if (patient.checkDocotr(id, visitdate))
        {
            patient.generateRegisterInfo(id, visitdate);
        }
        else
        {
            cout << "此医生当日已无号！";
            Sleep(3000);
        }
    }
    mainMenu();
}

void printAllInfo()
{
    registerObject info;
    int retu;
    qp();
    info.printAllDoctorBasicInfo();
    info.printAllDoctorDutyInfo();
    info.printAllRegisterSell();
    // Sleep(5000);
    int re;
    cout << "输入 1 返回主菜单: ";
    while (cin >> re)
    {
        if (re == 1)
            break;
    }
    mainMenu();
}

void searchInfo()
{
    registerObject all;
    int mode;
    qp();
    cout << " 1 医生基本信息\n 2 医生出诊信息\n 3 病人门诊挂号信息\n请输入查找类别: ";
    cin >> mode;
    // ? 1 医生基本信息、2 医生出诊信息、3 病人门诊挂号信息
    if (mode == 1)
    {
        qp();
        int condition;
        // ? 医生编号、医生姓名、所在科室、挂号人数、挂号价格、职称
        cout << " 1 医生编号\n 2 医生姓名\n 3 所在科室\n 4 挂号人数\n 5 挂号价格\n 6 职称\n请输入查找条件: ";
        cin >> condition;
        all.searchDoctorBasicInfo(condition);
    }
    else if (mode == 2)
    {
        qp();
        int condition;
        // ? 医生编号、医生姓名、值班日期、科室、可挂号人数、已挂号人数
        cout << " 1 医生编号\n 2 医生姓名\n 3 值班日期\n 4 挂号人数\n 5 可挂号人数\n 6 已挂号人数\n请输入查找条件序号: ";
        cin >> condition;
        all.searchDoctorDutyInfo(condition);
    }
    else if (mode == 3)
    {
        qp();
        int condition;
        // ? 挂号单号、医生编号、病人编号、病人名称、挂号序号
        cout << " 1 挂号单号\n 2 医生编号\n 3 病人编号\n 4 病人名称\n 5 挂号序号\n请输入查找条件序号: ";
        cin >> condition;
        all.searchRegisterSellnfo(condition);
    }
    mainMenu();
}

void statisticsInfo()
{
    qp();
    registerObject all;
    int mode;
    qp();
    cout << " 1 医生出诊信息\n 2 病人门诊挂号信息\n请输入统计类别: ";
    cin >> mode;
    // * 医生出诊信息：用户可以按照
    // ! 医生编号、医生姓名、值班日期、科室等
    // * 对医生出诊信息中的可挂号人数、已挂号人数进行统计，
    // * 统计到的人数信息以列表形式全部显示。
    // * 病人门诊挂号信息：用户可以按照3
    // ! 科室、医生编号、病人编号、病人名称
    // * 对挂号人数进行统计，
    // * 统计的挂号人数以列表形式全部显示。

    if (mode == 1)
    {
        qp();
        int condition;
        cout << " 1 医生编号\n 2 医生姓名\n 3 值班日期\n 4 所在科室\n请输入统计条件序号: ";
        cin >> condition;
        all.statisticsDoctorDutyInfo(condition);
    }
    else if (mode == 2)
    {
        qp();
        int condition;
        cout << " 1 医生编号\n 2 病人编号\n 3 病人名称\n 4 所在科室\n请输入统计条件序号: ";
        cin >> condition;
        all.statisticsRegisterSellInfo(condition);
    }
    mainMenu();
}

void generateMonthList(int length)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int year = 1900 + ltm->tm_year, month = ltm->tm_mon + 1;
    // string mon = to_string(month - length - 1);
    int startMonth = month - length + 1; // ? 报表的初始月份

    if (startMonth <= 0) // ? 跳转至上一年
    {
        year -= 1;
        startMonth += 12;
    }

    summaryList.clear();

    int monthList[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // ! 闰年判断
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
    {
        monthList[3] = 29;
    }

    for (int i = 0; i < length; i++)
    {
        if (startMonth > 12)
        {
            year++;
            startMonth -= 12;
        }
        string op = "01";                             // * 每月开始日
        string ed = to_string(monthList[startMonth]); // * 每月结束日
        string monthstr;
        if (startMonth < 10)
        {
            monthstr = "0" + to_string(startMonth);
        }
        else
        {
            monthstr = to_string(startMonth);
        }
        string opstr = to_string(year) + "-" + monthstr + "-" + op;
        string edstr = to_string(year) + "-" + monthstr + "-" + ed;
        pair<string, string> m = make_pair(opstr, edstr);
        summaryList.push_back(m);
        // cout << opstr << " " << edstr << endl;
        startMonth++;
    }
}

void summaryReport()
{
    registerObject report;
    // 读取registersell 获取总人数
    // 科室	起始日期	终止日期	挂号人数
    // 内科	2022-1-1	2022-1-31	50
    // 内科	2022-2-1	2022-2-28	70
    // 内科	2022-3-1	2022-3-31	90
    // 外科	2022-1-1	2022-1-31	70
    // 外科	2022-2-1	2022-2-28	80
    int mode, length;
    qp();
    cout << " 1 查看科室汇总\n 2 查看医生汇总\n请输入查看序号: ";
    cin >> mode;
    qp();
    cout << " 1 3月内报表\n 2 6月内报表\n 3 1年内报表\n请输入报表序号: ";
    cin >> length;

    switch (length)
    {
    case 1:
        length = 3;
        break;
    case 2:
        length = 6;
        break;
    case 3:
        length = 12;
        break;
    default:
        break;
    }
    generateMonthList(length);
    switch (mode)
    {
    case 1:
        report.summaryMonthlyReport();
        break;
    case 2:
        report.summaryDoctorReport();
        break;
    default:
        break;
    }
    mainMenu();
}

// ! 主函数
int main()
{
    passwordObject password;
    password.enterSystem();
    mainMenu();
    return 0;
}