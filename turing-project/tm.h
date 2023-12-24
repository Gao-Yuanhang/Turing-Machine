//
// Created by hp on 2023/12/16.
//

#ifndef TMP_TM_H
#define TMP_TM_H


#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;
class TuringMachine  {
public:
    TuringMachine(string path, bool verbose);
    friend ostream& operator<<(std::ostream& os, const TuringMachine& tm);
    void printID();
    void printTape(int tapeIndex);
    bool step(bool verbose);
    bool execute(bool verbose, string inputString);
    void refresh();
    vector<string> make_transition(string state, string input);//停机返回true
    bool check_input(string input, bool verbose);


private:
    //描述图灵机本身的性质
    set<string> Q;//状态集
    set<string> S;//输入符号集
    set<string> G;//纸带符号集
    string q0;//初始状态
    string B;//空格符号
    set<string> F;//终结状态集
    int N;//纸带数
    map<string, map<string,string>> delta;//转移函数，使用五元组的前两个索引，取出使用空格隔开的后三个

    //描述图灵机运行时的状态（使用printID规格化输出）
    string state;
    bool acc = false;//初始不接受
    vector<string> tapes;//正负各200＋0位置
    vector<int> firstIndex;//用于模拟无穷纸带时的扩容，标定string的一个位置（初始的最左端）
    vector<int> heads;

    template<typename T>
    static set<T> split(const std::string& input);

    static vector<string> split(string basicString);
};


#endif //TMP_TM_H
