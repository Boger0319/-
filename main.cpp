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
    system("cls"); // ! ��������

using namespace std;

// ! Ĭ���ļ�����
const string password_filename = "password.dat";             // ! �����ļ�
const string doctorBasic_filename = "doctor_base.dat";       // ! ҽ��������Ϣ
const string doctorVisit_filename = "doctor_duty.dat";       // ! ҽ��������Ϣ
const string patientRegister_filename = "register-sell.dat"; // ! ��������Һ���Ϣ
set<string> offices; // ! ���ܱ����д�Ų��ظ�����
set<pair<string, string>> doctor; // ! ���ܱ����д�Ŷ�Ӧ�Ĳ��ظ�ҽ�������
unordered_map<string, bool> doctorBasicId, doctorDutyId; // ! ��������г��ֵ�ҽ�����
unordered_map<string, int> registerNumber;               // ! ��� Duty �ļ��� �ѹҺ�����
vector<pair<string, string>> summaryList;                // ! ��� summaryReport() ������·�
/*
 ! ���к���������ǰ����
*/
// ! ���˵�
void mainMenu();
// ! �ļ�����
// ? �½��ļ�
void establishFile(const string filename);
// ! �������
// ? �޸�����
void changePassword();
// ! ҽ����Ϣ����
void inputDoctorInformation();
void createVisitInfo();
void reviseDoctorBaiscInfo(); // ! �޸�
// ! ���˵Ǽ�
void patientRegister();
void printAllInfo();   // ! ��ӡ������Ϣ
void searchInfo();     // ! ���ҹ���
void statisticsInfo(); // ! ͳ�ƹ���
void summaryReport();  // ! ���ܱ���

// ! ----------------------------------------------------------------------------

// ! �ṹ������
typedef struct doctorBasicInformation // ! ҽ��������Ϣ
{
    // ?  ҽ����š�ҽ�����������ҡ��Һ��������Һż۸�ְ�ơ��Ƿ�ר�ҡ���ע
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

typedef struct doctorDutyInformation // ! ҽ��������Ϣ
{
    // ?  ҽ����š�ҽ��������ֵ�����ڡ����ڿ��ҡ��ɹҺ��������ѹҺ�����
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
    // ? �Һŵ��š��Һ����ڡ��Һż۸�ҽ����š����˱�š��������ơ��Һ����
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

// ! ������

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
            cout << "��һ�ν���ϵͳ�������ó�ʼ���룺";
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
        cout << "������ϵͳ���룺";
        cin >> tmp_password;
        while (tmp_password != password)
        {
            cout << "��������������������������룺";
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
    doctorInfoObject() // ! ҽ������Ĺ��캯�� ���ڴ����ļ��Լ���������
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
    ~doctorInfoObject() // ! ҽ��������������� ����ɾ������
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
    bool checkId(string id) // ���ڷ���true
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
        cout << "������ҽ��������";
        cin >> p->name;

        cout << "���������ڿ��ң�";
        cin >> p->office;

        cout << "������Һ�������";
        cin >> p->visitNum;

        cout << "������Һż۸�";
        cin >> p->price;

        cout << "������ҽ��ְ�ƣ�";
        cin >> p->rank;

        cout << "�������Ƿ�Ϊר��(��/��): ";
        cin >> p->expert;

        cout << "�����뱸ע(û��������): ";
        cin >> p->remarks;

        qp();

        string Info;
        Info = p->id + '\t' + p->name + '\t' + p->office + '\t' + to_string(p->visitNum) + '\t' + to_string(p->price) + '\t' + p->rank + '\t' + p->expert + '\t' + p->remarks;
        // ! ��֤����
        cout << "ҽ����ţ�" << p->id << endl;
        cout << "ҽ��������" << p->name << endl;
        cout << "���ڿ��ң�" << p->office << endl;
        cout << "�Һ�������" << p->visitNum << endl;
        cout << "�Һż۸�" << p->price << endl;
        cout << "ҽ��ְ�ƣ�" << p->rank << endl;
        cout << "�Ƿ�ר�ң�" << p->expert << endl;
        cout << "��ע��" << p->remarks << endl;

        int mode;
        cout << "����������Ϣ�Ƿ�����\n���� 0 ȡ������\n���� 1 ȷ������\n���� 2 �����Ҽ���¼������Ϣ��";
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

        // ! ��ͷ
        cout << "\033[31;1m��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(60) << "ҽ��������Ϣ" << setw(56) << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��\033[0m" << endl;
        cout << "��" << setw(10) << "ҽ�����" << setw(10) << "ҽ������" << setw(10) << "���ڿ���" << setw(10) << "�Һ�����" << setw(10) << "�Һż۸�" << setw(14) << "ҽ��ְ��" << setw(10) << "�Ƿ�ר��" << right << setw(40) << "ҽ����ע"
             << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;

        while (p != NULL)
        {
            cout << "��";
            cout << setw(10) << p->id << setw(10) << p->name << setw(10) << p->office << setw(10) << p->visitNum << setw(10) << p->price << setw(14) << p->rank << setw(10) << p->expert << right << setw(40) << p->remarks << "��" << endl;
            p = p->next;
        }
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
    }
    void printSingleDoctorBasicInfo(string id)
    {
        qp();
        doctorBasicInformation *p = infohead->next;

        // ! ��ͷ
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(60) << "ҽ��������Ϣ" << setw(56) << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(10) << "ҽ�����" << setw(10) << "ҽ������" << setw(10) << "���ڿ���" << setw(10) << "�Һ�����" << setw(10) << "�Һż۸�" << setw(14) << "ҽ��ְ��" << setw(10) << "�Ƿ�ר��" << right << setw(40) << "ҽ����ע"
             << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;

        while (p != NULL)
        {
            if (p->id == id)
            {
                cout << "��";
                cout << setw(10) << p->id << setw(10) << p->name << setw(10) << p->office << setw(10) << p->visitNum << setw(10) << p->price << setw(14) << p->rank << setw(10) << p->expert << right << setw(40) << p->remarks << "��" << endl;
            }
            p = p->next;
        }
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
    }
    void printSingleDoctorBasicInfoInList(doctorBasicInformation *p)
    {
        cout << "��";
        cout << setw(10) << p->id << setw(10) << p->name << setw(10) << p->office << setw(10) << p->visitNum << setw(10) << p->price << setw(14) << p->rank << setw(10) << p->expert << right << setw(40) << p->remarks << "��" << endl;
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
            cout << "������ҽ�����: ";
            cin >> all;
            break;
        case 2:
            cout << "������ҽ������: ";
            cin >> all;
            break;
        case 3:
            cout << "���������ڿ���: ";
            cin >> all;
            break;
        case 4:
            cout << "������Һ�����: ";
            cin >> visitNum;
            break;
        case 5:
            cout << "������Һż۸�: ";
            cin >> price;
            break;
        case 6:
            cout << "������ְ��: ";
            cin >> all;
            break;
        default:
            return;
            break;
        }

        qp();

        int cnt = 0; // ! ͳ�Ʒ���������Ϣ������ 0�����û�з�����Ϣ

        // ! ��ͷ
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(60) << "ҽ��������Ϣ" << setw(56) << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(10) << "ҽ�����" << setw(10) << "ҽ������" << setw(10) << "���ڿ���" << setw(10) << "�Һ�����" << setw(10) << "�Һż۸�" << setw(14) << "ҽ��ְ��" << setw(10) << "�Ƿ�ר��" << right << setw(40) << "ҽ����ע"
             << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;

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
            cout << "��\033[31;1m";
            cout << setw(65) << "�޷���������Ϣ��" << setw(49) << "";
            cout << "\033[00m��" << endl;
        }
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        int re;
        cout << "���� 1 �������˵�: ";
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
    doctorDutyObject() // ! ҽ������Ĺ��캯�� ���ڴ����ļ��Լ���������
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
    ~doctorDutyObject() // ! ҽ��������������� ����ɾ������
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
        cout << "������ֵ�����ڣ���ʽ��YYYY-MM-DD����";
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
        cout << "������Ϣ�����ɣ�" << endl;
        printf("\n");
        cout << "ҽ����ţ�" << save->id << endl;
        cout << "ҽ��������" << save->name << endl;
        cout << "ֵ�����ڣ�" << save->Date << endl;
        cout << "���ڿ��ң�" << save->office << endl;
        cout << "�ɹҺ�������" << save->visitNum << endl;
        cout << "�ѹҺ�������" << save->nowNum << endl
             << endl;
        cout << "5��󷵻����˵�";
        Sleep(5000);
        doctorDutyId[save->id] = true;
        ofstream doctorDutyInfofile;
        doctorDutyInfofile.open(doctorVisit_filename, ios::app);
        doctorDutyInfofile << save->id << '\t' << save->name << '\t' << save->Date << '\t' << save->office << '\t' << save->visitNum << '\t' << save->nowNum << endl;
        doctorDutyInfofile.close();
    }
    void deleteBaiscInfo(string id) // ! ɾ��ҽ��������Ϣ
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
            cout << "��ҽ�����в��˹Һţ�����ɾ��ҽ��������Ϣ!" << endl;
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
            cout << "��ҽ��������Ϣɾ���ɹ�!" << endl;
            doctorBasicSave();
            Sleep(2000);
        }
    }
    void printAllDoctorDutyInfo()
    {
        doctorDutyInformation *p = dutyhead->next;

        // ! ��ͷ
        cout << "\033[32;1m��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(60) << "ҽ��������Ϣ" << setw(54) << ""
             << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��\033[00m" << endl;
        cout << "��" << setw(18) << "ҽ�����" << setw(18) << "ҽ������" << setw(18) << "���ڿ���" << setw(24) << "��������" << setw(18) << "�ɹҺ�����" << setw(18) << "�ѹҺ�����"
             << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;

        while (p != NULL)
        {
            cout << "��";
            cout << setw(18) << p->id << setw(18) << p->name << setw(18) << p->office << setw(24) << p->Date << setw(18) << p->visitNum << setw(18) << p->nowNum << "��" << endl;
            p = p->next;
        }
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
    }
    void reviseDoctorBasicInfo(string id)
    {
        doctorBasicInformation *search = infohead->next;

        // ? ���ڴ洢�޸ĺ�����
        string newid, name, office, rank, expert, remarks;
        int visitNum;
        double price;

        while (search != NULL)
        {
            if (search->id == id)
            {
                // ! ��������Ϣ
                cout << "�޸�ҽ�����: ";
                cin >> newid;
                cout << "�޸�ҽ������: ";
                cin >> name;
                cout << "�޸�ҽ������: ";
                cin >> office;
                cout << "�޸ĹҺ�����: ";
                cin >> visitNum;
                cout << "�޸ĹҺż۸�: ";
                cin >> price;
                cout << "�޸�ҽ��ְ��: ";
                cin >> rank;
                cout << "�޸�ר�ҳƺ�: ";
                cin >> expert;
                cout << "�޸�ҽ����ע: ";
                cin >> remarks;

                // ! ��֤����
                qp();
                cout << "����֤�޸ĺ���Ϣ��"
                     << "\n"
                     << "\n";
                cout << "ҽ�����: " << newid << endl;
                cout << "ҽ������: " << name << endl;
                cout << "ҽ������: " << office << endl;
                cout << "�Һ�����: " << visitNum << endl;
                cout << "�Һż۸�: " << price << endl;
                cout << "ҽ��ְ��: " << rank << endl;
                cout << "ר�ҳƺ�: " << expert << endl;
                cout << "ҽ����ע: " << remarks << endl;

                cout << "����������Ϣ�Ƿ�����\n���� 0 ȡ������\n���� 1 ȷ������: ";

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
        cout << "��";
        cout << setw(18) << p->id << setw(18) << p->name << setw(18) << p->office << setw(24) << p->Date << setw(18) << p->visitNum << setw(18) << p->nowNum << "��" << endl;
    }
    void searchDoctorDutyInfo(int condition)
    {
        doctorDutyInformation *doctorInfo = dutyhead->next;

        // ? ҽ����š�ҽ��������ֵ�����ڡ����ҡ��ɹҺ��������ѹҺ�����

        int visitNum, nowNum;
        string all;

        switch (condition)
        {
        case 1:
            cout << "������ҽ�����: ";
            cin >> all;
            break;
        case 2:
            cout << "������ҽ������: ";
            cin >> all;
            break;
        case 3:
            cout << "������ֵ������: ";
            cin >> all;
            break;
        case 4:
            cout << "���������ڿ���: ";
            cin >> all;
            break;
        case 5:
            cout << "������ɹҺ�����: ";
            cin >> visitNum;
            break;
        case 6:
            cout << "�������ѹҺ�����: ";
            cin >> nowNum;
            break;
        default:
            return;
            break;
        }

        qp();

        int cnt = 0; // ! ͳ�Ʒ���������Ϣ������ 0�����û�з�����Ϣ

        // ! ��ͷ
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(60) << "ҽ��������Ϣ" << setw(54) << ""
             << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(18) << "ҽ�����" << setw(18) << "ҽ������" << setw(18) << "���ڿ���" << setw(24) << "��������" << setw(18) << "�ɹҺ�����" << setw(18) << "�ѹҺ�����"
             << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;

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
            cout << "��\033[31;1m";
            cout << setw(65) << "�޷���������Ϣ��" << setw(49) << "";
            cout << "\033[00m��" << endl;
        }
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        int re;
        cout << "���� 1 �������˵�: ";
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
            cout << "������ҽ�����: ";
            cin >> all;
            break;
        case 2:
            cout << "������ҽ������: ";
            cin >> all;
            break;
        case 3:
            cout << "������ֵ������: ";
            cin >> all;
            break;
        case 4:
            cout << "���������ڿ���: ";
            cin >> all;
            break;
        default:
            return;
            break;
        }

        qp();

        int cnt = 0; // ! ͳ�Ʒ���������Ϣ������ 0�����û�з�����Ϣ

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

        // ! ��ͷ
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(60) << "ҽ��������Ϣ" << setw(54) << ""
             << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��";

        switch (condition)
        {
        case 1:
            cout << setw(38) << "ҽ�����";
            break;
        case 2:
            cout << setw(38) << "ҽ������";
            break;
        case 3:
            cout << setw(38) << "���ڿ���";
            break;
        case 4:
            cout << setw(38) << "�Һ�����";
            break;
        case 5:
            cout << setw(38) << "�Һż۸�";
            break;
        case 6:
            cout << setw(38) << "ҽ��ְ��";
            break;
        default:
            return;
            break;
        }
        cout << setw(38) << "�ɹҺ�����" << setw(38) << "�ѹҺ�����"
             << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;

        if (cnt == 0)
        {
            cout << "��\033[31;1m";
            cout << setw(65) << "�޷���������Ϣ��" << setw(49) << "";
            cout << "\033[00m��" << endl;
        }
        else
        {
            cout << "��";
            // cout << setw(18) << showInfo->id << setw(18) << showInfo->name << setw(18) << showInfo->office << setw(24) << showInfo->Date << setw(18) << visitNum << setw(18) << nowNum << "��" << endl;
            cout << setw(38) << all << setw(38) << visitNum << setw(38) << nowNum << "��" << endl;
        }

        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        int re;
        cout << "���� 1 �������˵�: ";
        while (cin >> re)
        {
            if (re == 1)
                break;
        }
    }

    void doctorDutySave() // ! ����д���ļ� ���� �޸�ɾ��
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
        // ����	��ʼ����	��ֹ����	�Һ�����
        cout << "��";
        for (int i = 0; i < 57; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(11) << "����" << setw(16) << "��ʼ����" << setw(16) << "��ֹ����" << setw(14) << "�Һ�����"
             << "��" << endl;
        cout << "��";
        for (int i = 0; i < 57; i++)
            cout << "��";
        cout << "��" << endl;
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
                cout << "��" << setw(11) << o << setw(16) << summaryList[j].first << setw(16) << summaryList[j].second << setw(14) << cnt << "��" << endl;
            }
        }
        cout << "��";
        for (int i = 0; i < 57; i++)
            cout << "��";
        cout << "��" << endl;
        int re;
        cout << "���� 1 �������˵�: ";
        while (cin >> re)
        {
            if (re == 1)
                break;
        }
    }
    void summaryDoctorReport()
    {
        qp();
        // ����	��ʼ����	��ֹ����	�Һ�����
        cout << "��";
        for (int i = 0; i < 68; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(11) << "����" << setw(11) << "ҽ�����" << setw(16) << "��ʼ����" << setw(16) << "��ֹ����" << setw(14) << "�Һ�����"
             << "��" << endl;
        cout << "��";
        for (int i = 0; i < 68; i++)
            cout << "��";
        cout << "��" << endl;
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
                        cout << "��" << setw(11) << o << setw(11) << d.first << setw(16) << summaryList[j].first << setw(16) << summaryList[j].second << setw(14) << cnt << "��" << endl;
                    }
                }
            }
        }
        cout << "��";
        for (int i = 0; i < 68; i++)
            cout << "��";
        cout << "��" << endl;
        int re;
        cout << "���� 1 �������˵�: ";
        while (cin >> re)
        {
            if (re == 1)
                break;
        }
    }
};

class registerObject : public doctorDutyObject // ������Ϣ�Ǽ�
{
private:
    registersell *registerhead = new registersell;

public:
    registerObject() // ! ��ʼ��
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
    ~registerObject() // ! ���˹Һŵ��������� ����ɾ������
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

        patient->doctorId = id;               // ? ҽ�����
        patient->registerDate = registerDate; // ? �Һ�����
        cout << "�����벡�˱�ţ�";
        cin >> patient->patientId;
        cout << "�����벡�����ƣ�";
        cin >> patient->patientName;
        // cout << "������Һż۸�";
        patient->price = searchPrice(id);
        patient->office = searchOffice(id);
        patient->number = number;

        patient->registernum = to_string(registernum);

        qp();
        cout << "�Һ���Ϣ�����ɣ�" << endl;
        printf("\n");
        cout << "�Һŵ��ţ�" << patient->number << endl;
        cout << "�Һ����ڣ�" << patient->registerDate << endl;
        cout << "�Һż۸�" << patient->price << endl;
        cout << "ҽ����ţ�" << patient->doctorId << endl;
        cout << "���˱�ţ�" << patient->patientId << endl;
        cout << "�������ƣ�" << patient->patientName << endl;
        cout << "�Һ���ţ�" << patient->registernum << endl
             << endl;
        int re;
        cout << "���� 1 �������˵�: ";
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

        // ! ��ͷ
        cout << "\033[34;1m��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(62) << "��������Һ���Ϣ" << setw(54) << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��\033[00m" << endl;
        cout << "��" << setw(18) << "�Һŵ���" << setw(16) << "�Һ�����" << setw(16) << "�Һż۸�" << setw(16) << "ҽ�����" << setw(16) << "���˱��" << setw(16) << "��������" << setw(16) << "�Һ����"
             << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;

        while (p != NULL)
        {
            cout << "��";
            cout << setw(18) << p->number << setw(16) << p->registerDate << setw(16) << p->price << setw(16) << p->doctorId << setw(16) << p->patientId << setw(16) << p->patientName << setw(16) << p->registernum << "��" << endl;
            p = p->next;
        }
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
    }
    void printSingleRegisterSellInfoInList(registersell *p)
    {
        cout << "��";
        cout << setw(18) << p->number << setw(16) << p->registerDate << setw(16) << p->price << setw(16) << p->doctorId << setw(16) << p->patientId << setw(16) << p->patientName << setw(16) << p->registernum << "��" << endl;
    }
    void searchRegisterSellnfo(int condition)
    {
        registersell *patientInfo = registerhead->next;

        // ? �Һŵ��š�ҽ����š����˱�š��������ơ��Һ����

        string all;

        switch (condition)
        {
        case 1:
            cout << "������Һŵ���: ";
            cin >> all;
            break;
        case 2:
            cout << "������ҽ�����: ";
            cin >> all;
            break;
        case 3:
            cout << "�����벡�˱��: ";
            cin >> all;
            break;
        case 4:
            cout << "�����벡������: ";
            cin >> all;
            break;
        case 5:
            cout << "������Һ����: ";
            cin >> all;
            break;
        default:
            return;
            break;
        }

        qp();

        int cnt = 0; // ! ͳ�Ʒ���������Ϣ������ 0�����û�з�����Ϣ

        // ! ��ͷ
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(66) << "��������Һ���Ϣ" << setw(50) << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(18) << "�Һŵ���" << setw(16) << "�Һ�����" << setw(16) << "�Һż۸�" << setw(16) << "ҽ�����" << setw(16) << "���˱��" << setw(16) << "��������" << setw(16) << "�Һ����"
             << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;

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
            cout << "��\033[31;1m";
            cout << setw(65) << "�޷���������Ϣ��" << setw(49) << "";
            cout << "\033[00m��" << endl;
        }
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        int re;
        cout << "���� 1 �������˵�: ";
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
            cout << "������ҽ�����: ";
            cin >> all;
            break;
        case 2:
            cout << "�����벡�˱��: ";
            cin >> all;
            break;
        case 3:
            cout << "�����벡������: ";
            cin >> all;
            break;
        case 4:
            cout << "���������ڿ���: ";
            cin >> all;
            break;
        default:
            return;
            break;
        }

        qp();

        int cnt = 0; // ! ͳ�Ʒ���������Ϣ������ 0�����û�з�����Ϣ

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

        // ! ��ͷ
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��" << setw(66) << "��������Һ���Ϣ" << setw(50) << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        cout << "��";
        switch (condition)
        {
        case 1:
            cout << setw(54) << "ҽ�����";
            break;
        case 2:
            cout << setw(54) << "���˱��";
            break;
        case 3:
            cout << setw(54) << "��������";
            break;
        case 4:
            cout << setw(54) << "���ڿ���";
            break;
        default:
            break;
        }
        cout << setw(60) << "�Һ�����";
        cout << "��" << endl;
        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;

        if (cnt == 0)
        {
            cout << "��\033[31;1m";
            cout << setw(65) << "�޷���������Ϣ��" << setw(49) << "";
            cout << "\033[00m��" << endl;
        }
        else
        {
            cout << "��";
            cout << setw(54) << all << setw(60) << num << "��" << endl;
        }

        cout << "��";
        for (int i = 0; i < 114; i++)
            cout << "��";
        cout << "��" << endl;
        int re;
        cout << "���� 1 �������˵�: ";
        while (cin >> re)
        {
            if (re == 1)
                break;
        }
    }
};

// ! ������
void mainMenu()
{
    qp();
    string selectedMode;
    cout << "��������������������������������������������������������������������������������������������������������������������������������" << endl;
    cout << "��                     ҽԺ������Ϣ����ϵͳ                     ��" << endl;
    cout << "�ǩ��������������ש�������������������������������������������������������������������������������������������������������������" << endl;
    cout << "��   1   �����ü��޸�����                                        ��" << endl;
    cout << "��   2   ��¼��ҽ��������Ϣ                                      ��" << endl;
    cout << "��   3   ��ҽ��������Ϣ������                                    ��" << endl;
    cout << "��   4   ����������ҺŹ���                                      ��" << endl;
    cout << "��   5   ���޸�ҽ��������Ϣ�������޸ġ�ɾ�����ܣ�                ��" << endl;
    cout << "��   6   �����ȫ��ҽ��������Ϣ��ҽ��������Ϣ����������Һ���Ϣ  ��" << endl;
    cout << "��   7   ����ѯ����                                              ��" << endl;
    cout << "��   8   ��ͳ�ƹ���                                              ��" << endl;
    cout << "��   9   �����ܱ���                                              ��" << endl;
    cout << "��   0   ���˳�ϵͳ                                              ��" << endl;
    cout << "�����������������ߩ�������������������������������������������������������������������������������������������������������������" << endl;
    cout << "�������Ӧ���ѡ��������Ҫ�Ĳ�����";

    while (cin >> selectedMode)
    {
        if (selectedMode.size() > 1)
        {
            cout << "���������������������룺";
        }
        else if (selectedMode[0] < '0' || selectedMode[0] > '9')
        {
            cout << "���������������������룺";
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
    cout << "������ԭ���룺";
    while (!password.checkPassword())
    {
        cout << "��������������������룺";
    }
    cout << "�����������룺";
    password.saveNewPassword();
    mainMenu();
}

void inputDoctorInformation()
{
    qp();
    doctorInfoObject doctor;
    string id;
    cout << "������ҽ����ţ�";
    cin >> id;
    if (!doctor.checkId(id))
    {
        doctor.inputInfoOperate(id);
    }
    else
    {
        cout << "���������ҽ������Ѵ��ڣ�Ϊ���������˵�" << endl;
        Sleep(3000);
    }
    mainMenu();
}

void createVisitInfo()
{
    qp();
    doctorDutyObject doctorDuty;
    string id;
    cout << "������ֵ��ҽ����ţ�";
    cin >> id;
    if (doctorDuty.checkId(id))
    {
        if (doctorDuty.checkDuty(id))
        {
            cout << "���������ҽ��ֵ����Ϣ�����ɣ������ظ�����" << endl;
            Sleep(3000);
        }
        else
        {
            doctorDuty.generateDutyInfo(id);
        }
    }
    else
    {
        cout << "���������ҽ����Ų����ڣ�Ϊ���������˵�" << endl;
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
    cout << "��������в�����ҽ����ţ�";
    cin >> id;
    if (doctor.checkId(id))
    {
        doctor.printSingleDoctorBasicInfo(id);
        cout << "���������Ĳ���\n���� 0 ����������\n���� 1 �����޸�\n���� 2 ����ɾ����";
        cin >> selectedMode;
        switch (selectedMode)
        {
        case 0:
            break;
        case 1:
            // ? �޸�
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
        cout << "ҽ����Ϣ�����ڣ�3��󷵻����˵�" << endl;
        Sleep(3000);
    }
    mainMenu();
}

void patientRegister()
{
    qp();
    string visitdate, id;
    registerObject patient;
    cout << "������ֵ�����ڣ���ʽ��YYYY-MM-DD����";
    cin >> visitdate;
    cout << "������ҽ����ţ�";
    cin >> id;
    if (doctorBasicId[id] == false)
    {
        cout << "\033[1;5mҽ����Ϣ�����ڣ�\033[0m" << endl;
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
            cout << "��ҽ���������޺ţ�";
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
    cout << "���� 1 �������˵�: ";
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
    cout << " 1 ҽ��������Ϣ\n 2 ҽ��������Ϣ\n 3 ��������Һ���Ϣ\n������������: ";
    cin >> mode;
    // ? 1 ҽ��������Ϣ��2 ҽ��������Ϣ��3 ��������Һ���Ϣ
    if (mode == 1)
    {
        qp();
        int condition;
        // ? ҽ����š�ҽ�����������ڿ��ҡ��Һ��������Һż۸�ְ��
        cout << " 1 ҽ�����\n 2 ҽ������\n 3 ���ڿ���\n 4 �Һ�����\n 5 �Һż۸�\n 6 ְ��\n�������������: ";
        cin >> condition;
        all.searchDoctorBasicInfo(condition);
    }
    else if (mode == 2)
    {
        qp();
        int condition;
        // ? ҽ����š�ҽ��������ֵ�����ڡ����ҡ��ɹҺ��������ѹҺ�����
        cout << " 1 ҽ�����\n 2 ҽ������\n 3 ֵ������\n 4 �Һ�����\n 5 �ɹҺ�����\n 6 �ѹҺ�����\n����������������: ";
        cin >> condition;
        all.searchDoctorDutyInfo(condition);
    }
    else if (mode == 3)
    {
        qp();
        int condition;
        // ? �Һŵ��š�ҽ����š����˱�š��������ơ��Һ����
        cout << " 1 �Һŵ���\n 2 ҽ�����\n 3 ���˱��\n 4 ��������\n 5 �Һ����\n����������������: ";
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
    cout << " 1 ҽ��������Ϣ\n 2 ��������Һ���Ϣ\n������ͳ�����: ";
    cin >> mode;
    // * ҽ��������Ϣ���û����԰���
    // ! ҽ����š�ҽ��������ֵ�����ڡ����ҵ�
    // * ��ҽ��������Ϣ�еĿɹҺ��������ѹҺ���������ͳ�ƣ�
    // * ͳ�Ƶ���������Ϣ���б���ʽȫ����ʾ��
    // * ��������Һ���Ϣ���û����԰���3
    // ! ���ҡ�ҽ����š����˱�š���������
    // * �ԹҺ���������ͳ�ƣ�
    // * ͳ�ƵĹҺ��������б���ʽȫ����ʾ��

    if (mode == 1)
    {
        qp();
        int condition;
        cout << " 1 ҽ�����\n 2 ҽ������\n 3 ֵ������\n 4 ���ڿ���\n������ͳ���������: ";
        cin >> condition;
        all.statisticsDoctorDutyInfo(condition);
    }
    else if (mode == 2)
    {
        qp();
        int condition;
        cout << " 1 ҽ�����\n 2 ���˱��\n 3 ��������\n 4 ���ڿ���\n������ͳ���������: ";
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
    int startMonth = month - length + 1; // ? ����ĳ�ʼ�·�

    if (startMonth <= 0) // ? ��ת����һ��
    {
        year -= 1;
        startMonth += 12;
    }

    summaryList.clear();

    int monthList[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // ! �����ж�
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
        string op = "01";                             // * ÿ�¿�ʼ��
        string ed = to_string(monthList[startMonth]); // * ÿ�½�����
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
    // ��ȡregistersell ��ȡ������
    // ����	��ʼ����	��ֹ����	�Һ�����
    // �ڿ�	2022-1-1	2022-1-31	50
    // �ڿ�	2022-2-1	2022-2-28	70
    // �ڿ�	2022-3-1	2022-3-31	90
    // ���	2022-1-1	2022-1-31	70
    // ���	2022-2-1	2022-2-28	80
    int mode, length;
    qp();
    cout << " 1 �鿴���һ���\n 2 �鿴ҽ������\n������鿴���: ";
    cin >> mode;
    qp();
    cout << " 1 3���ڱ���\n 2 6���ڱ���\n 3 1���ڱ���\n�����뱨�����: ";
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

// ! ������
int main()
{
    passwordObject password;
    password.enterSystem();
    mainMenu();
    return 0;
}