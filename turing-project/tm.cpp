//
// Created by hp on 2023/12/16.
//

#include "tm.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>


TuringMachine::TuringMachine(string path, bool verbose) {
    //verbose需要具体指出异常
    ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file." << endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if(line.length()==0)continue;//空串
        if(line[0]==';')continue;//单行注释
        //处理行尾注释
        size_t pos = line.find(';');
        if (pos != string::npos) {
            line = line.substr(0, pos);
        }

        //进行错误检查，直接输出syntax error
        //无括号
        if(line.substr(0,2).find('q')!=string::npos || line.substr(0,2).find('B')!=string::npos || line.substr(0,2).find('N')!=string::npos){
            if(line.at(0)!='#'){
                cerr<<"syntax error"<<endl;
                throw std::runtime_error("Error in form of TM!");
            }
        }
        //有括号
        if(line.substr(0,2).find('Q')!=string::npos || line.substr(0,2).find('S')!=string::npos || line.substr(0,2).find('G')!=string::npos ||line.substr(0,2).find('F')!=string::npos){
            //允许为空，一个或用逗号隔开的格式
            regex pattern{"#.\\s=\\s\\{(([a-zA-Z0-9_]*,)*([a-zA-Z0-9_]*))|([a-zA-Z0-9_]*)\\}"};
            std::smatch matches;
            if (std::regex_search(line, matches, pattern)) {
                //匹配成功，正确
            } else {
                cerr<<"syntax error"<<endl;
                throw std::runtime_error("Error in form of TM!");
            }
        }
        if(line.substr(0,2)=="#Q"){
            line = line.substr(line.find('{')+1,line.find('}')-line.find('{')-1);
            this->Q = split<string>(line);
            //#Q只有一次，这里顺便初始化delta，把状态和对应的map构造出来
            this->delta = map<string,map<string,string>>();
            for(string state:this->Q){
                delta[state]=map<string,string>();//使用[]或makepair方法插入对象
            }
            continue;
        }
        if(line.substr(0,2)=="#S"){
            line = line.substr(line.find('{')+1,line.find('}')-line.find('{')-1);
            this->S = split<string>(line);
            continue;
        }
        if(line.substr(0,2)=="#G"){
            line = line.substr(line.find('{')+1,line.find('}')-line.find('{')-1);
            this->G = split<string>(line);
            continue;
        }
        if(line.substr(0,3)=="#q0"){
            //等号左右有且只有一个空格
            this->q0 = line.substr(line.find('=')+2);
            continue;
        }
        if(line.substr(0,2)=="#B"){
            this->B = line.substr(line.length()-1,1);
            continue;
        }
        if(line.substr(0,2)=="#F"){
            line = line.substr(line.find('{')+1,line.find('}')-line.find('{')-1);
            this->F = split<string>(line);
            continue;
        }
        if(line.substr(0,2)=="#N"){
            this->N = line[line.length()-1]-'0';
            continue;
        }
        //写入delta
        vector<string> quintuple = split(line);
        std::string outerKeyToFind = quintuple[0];
        auto outerIt = this->delta.find(outerKeyToFind);
        if (outerIt != this->delta.end()) {
            // 外部键存在，访问内部 map
            std::map<std::string, std::string> &innerMap = outerIt->second;
            innerMap[quintuple[1]] = quintuple[2]+" "+quintuple[3]+" "+quintuple[4];
        }
    }
    file.close();
}

ostream& operator<<(ostream& os, const TuringMachine& tm) {
    os << "Q (States): {";
    for (const auto& state : tm.Q) {
        os << state << ", ";
    }
    os << "}" << std::endl;

    os << "S (Input Symbols): {";
    for (const auto& symbol : tm.S) {
        os << symbol << ", ";
    }
    os << "}" << std::endl;

    os << "G (Tape Symbols): {";
    for (const auto& tapeSymbol : tm.G) {
        os << tapeSymbol << ", ";
    }
    os << "}" << std::endl;

    os << "q0 (Initial State): " << tm.q0 << std::endl;
    os << "B (Blank Symbol): " << tm.B << std::endl;

    os << "F (Final States): {";
    for (const auto& finalState : tm.F) {
        os << finalState << ", ";
    }
    os << "}" << std::endl;

    os << "N (Number of Tapes): " << tm.N << std::endl;

    os << "Delta (Transition Function):" << std::endl;
    for (const auto& stateMapPair : tm.delta) {
        for (const auto& inputMapPair : stateMapPair.second) {
            os << "  trans(" << stateMapPair.first << ", " << inputMapPair.first << ") = "
               << inputMapPair.second << std::endl;
        }
    }
    return os;
}

void TuringMachine::printID() {
    cout<<"State  : "<<this->state<<endl;
    cout<<"Acc    : "<<(this->acc?"Yes":"No")<<endl;
    for(int i=0;i<this->N;i++){
        printTape(i);
    }
    cout<<"---------------------------------------------"<<endl;
}

void TuringMachine::printTape(int tapeIndex){
    //找这条纸带的左右index
    string tape(tapes.at(tapeIndex));
    int firstindex = this->firstIndex.at(tapeIndex);
    int head = this->heads.at(tapeIndex);
    //在纸带上的编号
    int l_index = tape.find_first_not_of(B[0]) + firstindex;//x=find+first
    int r_index = tape.find_last_not_of(B[0]) + firstindex;
    //head 不在这个范围的情况
    l_index = min(l_index,head);
    r_index = max(r_index,head);
    if(tape.find_first_not_of(B[0]) == std::string::npos){
        l_index = head;
        r_index = head;
    }
    //用于访问字符串
    int real_l_index = l_index - firstindex;
    int real_r_index = r_index - firstindex;
    cout<<"Index"<<tapeIndex<<" : ";
    string indexOutput="";
    for(int i=l_index;i<=r_index;i++){
        indexOutput = indexOutput + to_string(i) + " ";
    }
    cout<<indexOutput<<endl;

    for(int i=0;i<indexOutput.length();i++){
        if(i==0)continue;
        if(i<indexOutput.length() && indexOutput[i-1]!=' '){
            //三位数以上的对齐处理
            while(true){
                if(i<indexOutput.length() && indexOutput.at(i)!=' '){
                    indexOutput.at(i)=' ';
                    i++;
                }else{
                    break;
                }
            }
        }
    }
    //Clion终端输出会吞掉最后一个空格
    cout<<"Tape"<<tapeIndex<<"  : ";

    string tapeOutput(indexOutput);
    int p=0;
    for(int i=real_l_index;i<=real_r_index;i++){
        //这里判断是不是在输出位于有效字符串之外
        int l = tape.find_first_not_of(B[0]);
        int r = tape.find_last_not_of(B[0]);
        char temp = '_';
        if(i>=l && i<=r)temp = tape[i];
        tapeOutput.at(p)=temp;
        //找下一个写入位置
        p++;
        while(!(p>=tapeOutput.length() || tapeOutput[p]!=' ')){
            p++;
        }
    }
    cout<<tapeOutput<<endl;

    string headOutput(indexOutput);
    int x = head - l_index;//x=head-l, x表示需要skip的次数
    int q=0;
    for(int i=0;i<x;i++){
        headOutput.at(q)=' ';
        //找下一个写入位置
        q++;
        while(!(q>=headOutput.length() || headOutput[q]!=' ')){
            q++;
        }
    }
    headOutput.at(q)='^';
    replace_if(headOutput.begin(), headOutput.end(), [](char c) {
        return c != '^' && c != ' ';
    }, ' ');
    cout<<"Head"<<tapeIndex<<"  : ";
    cout<<headOutput<<endl;
}

//返回是否可以继续执行，即停机状态
bool TuringMachine::step(bool verbose) {
    string input="";//根据读写头的位置与纸带的内容，拼成input（转移函数的第二个输入）
    for(int i=0;i<this->N;i++){
        int visit_index=heads.at(i) - firstIndex.at(i);
        input=input+tapes.at(i).substr(visit_index,1);
    }
    vector<string> transition_result=make_transition(state,input);
    if(transition_result.empty()){
        return false;
    }else{
        //改写纸带
        for(int i=0;i<this->N;i++){
            int visit_index=heads.at(i) - firstIndex.at(i);
            tapes.at(i)[visit_index]=transition_result[0][i];
        }
        //移动磁头
        for(int i=0;i<this->N;i++){
            char d=transition_result[1][i];
            if(d=='l'){
                //如果这次移动不会移到现有的纸带外，则只改变head的值，否则需要扩容，改变head，first_index与tape;r也是同理
                if(heads.at(i)==firstIndex.at(i)){
                    //说明磁头在纸带的最左端
                    tapes.at(i)="_"+tapes.at(i);
                    firstIndex.at(i)--;
                }
                heads.at(i)--;
            }else if(d=='r'){
                if(heads.at(i)==firstIndex.at(i)+tapes.at(i).length()-1){
                    //说明磁头在纸带的最右端
                    tapes.at(i)=tapes.at(i)+"_";
                    //不需要改变firstindex
                }
                heads.at(i)++;
            }else{
                //不动
            }
        }
        //转移状态
        this->state=transition_result[2];
        //检查接受状态
        if (F.find(state) != F.end()) {
            //属于F
            this->acc=true;
        }
        return true;
    }
}

bool TuringMachine::execute(bool verbose, string inputString) {
    //检查输入字符串
    if(!check_input(inputString,verbose)){
        throw std::runtime_error("Error in input(characters not in S)!");
    }

    //对运行状态进行初始化
    this->state = q0;
    string emptytape(401,B[0]);
    for(int i=0;i<this->N;i++){
        this->tapes.emplace_back(emptytape);
        this->heads.push_back(0);
        this->firstIndex.push_back(-200);
    }
    //把输入放在纸带上
    tapes.at(0).replace(200, inputString.length(), inputString);

    int step = 0;
    if(verbose){
        cout<<"Step   : "<<step<<endl;
        printID();
        step++;
    }

    //检查初始是否为接受状态
    if (F.find(state) != F.end()) {
        //属于F
        this->acc=true;
    }

    int max=0;
    while(true){
        max++;
        //if(max>=10)break;

        bool run = this->step(verbose);
        if(!run)break;
        //系统走到了一个新状态打印出来
        if(verbose){
            cout<<"Step   : "<<step<<endl;
            printID();
            step++;
        }
    }

    //获得第一条纸带的内容
    //此处需要额外处理，如果第一条纸带上的内容为空，就不能用substr了
    int l_index = this->tapes.at(0).find_first_not_of(B[0]);
    int r_index = this->tapes.at(0).find_last_not_of(B[0]);
    string result_on_tape="";
    if(l_index!=string::npos){
        result_on_tape=this->tapes.at(0).substr(l_index,r_index-l_index+1);
    }
    //对verbose/非verbose打印结果
    if(verbose){
        cout<<(this->acc?"ACCEPTED":"UNACCEPTED")<<endl;
        cout<<"Result: "<<result_on_tape<<endl;
        cout<<"==================== END ===================="<<endl;

    }else{
        cout<<(this->acc?"(ACCEPTED) ":"(UNACCEPTED) ")<<result_on_tape<<endl;
    }

    return this->acc;
}


vector<string> TuringMachine::split(string input) {
    char delimiter = ' ';
    std::vector<std::string> result;
    std::istringstream stream(input);
    std::string token;
    while (std::getline(stream, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}

template<typename T>
set<T> TuringMachine::split(const string &input) {
    char delimiter = ',';
    set<T> result;
    T token;
    for (const auto& ch : input) {
        if (ch != delimiter) {
            token += ch;
        } else {
            result.insert(token);
            token = T();  // 清空 token
        }
    }
    result.insert(token);
    return result;
}

void TuringMachine::refresh() {
    //好像并不需要这个函数。。
    this->state="";
    this->acc=false;
    this->tapes.clear();
    this->firstIndex.clear();
    this->heads.clear();
}

vector<string> TuringMachine::make_transition(string state, string input) {
    //或者返回一个空的vector表示停机，或者返回一个三元组vector表示找到了可用的转移
    std::string outerKeyToFind = state;
    auto outerIt = this->delta.find(outerKeyToFind);

    if (outerIt != this->delta.end()) {
        // 外部键存在，访问内部 map
        const std::map<std::string, std::string> &innerMap = outerIt->second;
        std::set<string> keySet;
        for (const auto& pair : innerMap) {
            keySet.insert(pair.first);
        }
        for(int i=0;i<input.length();i++){
            char curr=input[i];
            //本次实验空格符号规定为'_'
            if(curr == '_'){
                //*不能匹配_
                auto removeCondition = [i](const std::string& str) {
                    return (str.size() > i) && (str[i] != '_');
                };
                set<string> difference;
                set<string> to_remove;
                for(string s : keySet){
                    if(removeCondition(s))to_remove.insert(s);
                }
                set_difference(keySet.begin(), keySet.end(),
                               to_remove.begin(), to_remove.end(),
                               std::inserter(difference, difference.begin()));
                keySet=difference;
            }else{
                //正常情况，用*匹配
                auto removeCondition = [i, curr](const std::string& str) -> bool {
                    return (str.size() > i) && (str[i] != '*') && (str[i] != curr);
                };
                set<string> difference;
                set<string> to_remove;
                for(string s : keySet){
                    if(removeCondition(s))to_remove.insert(s);
                }
                set_difference(keySet.begin(), keySet.end(),
                               to_remove.begin(), to_remove.end(),
                               std::inserter(difference, difference.begin()));
                keySet=difference;
            }
        }
        //此时根据输入归约，删除后的set要么是空要么只有一个元素
        if(keySet.empty()){
            return vector<string>();
        }else{
            //这时用这个就可以取出来对应的值
            for(string temp : keySet){
                auto it = innerMap.find(temp);
                vector<string> output = split(it->second);
                string t=output[0];
                for(int i=0;i<t.length();i++){
                    if(t.at(i)=='*'){
                        t.at(i)=input.at(i);
                    }
                }
                output[0]=t;
                return output;
            }
        }
    }else{
        //找不到状态对应的map，直接停机
        return vector<string>();
    }
}

bool TuringMachine::check_input(string input, bool verbose) {
    string output="Input: "+input;
    for(int i=0;i<input.length();i++){
        string temp=input.substr(i,1);
        if (this->S.find(temp) != this->S.end()) {
            continue;
        }else{
            if(verbose){
                cerr<<output<<endl;
                cerr<<"==================== ERR ===================="<<endl;
                cerr<<"error: Symbol \""<<temp<<"\" in input is not defined in the set of input symbols"<<endl;
                cerr<<output<<endl;
                for(int j=0;j<input.length();j++){
                    if(j!=i){
                        input.at(j)=' ';
                    }else{
                        input.at(j)='^';
                    }
                }
                cerr<<"       "+input<<endl;
                cerr<<"==================== END ===================="<<endl;
            }else{
                cerr<<"illegal input string"<<endl;
            }
            return false;
        }
    }
    if(verbose){
        cout<<output<<endl;
        cout<<"==================== RUN ===================="<<endl;
    }
    return true;
}
