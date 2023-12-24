#include <iostream>
#include <algorithm>
#include <random>
#include "tm.h"


using namespace std;

void test1(){
    //非回文
    for(int length=2;length<30;length++){
        std::string result;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 1);
        for (int i = 0; i < length; ++i) {
            result += std::to_string(dis(gen));
            if (i > 0 && result[i] == result[length - i - 1]) {
                result[i] = (result[i]=='1')?'0':'1';
            }
        }
        cout<<"gen : "<<result<<endl;
        TuringMachine tm = TuringMachine("../programs/palindrome_detector_2tapes.tm", false);
        if(tm.execute(false,result))return;
    }
    cout<<"pass 1"<<endl;

    //回文
    for(int length=2;length<30;length++){
        std::string result;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 1);
        for (int i = 0; i < length; ++i) {
            result += std::to_string(dis(gen));
        }
        string rresult(result);
        reverse(rresult.begin(),rresult.end());
        result += rresult;
        cout<<"gen : "<<result<<endl;
        TuringMachine tm = TuringMachine("../programs/palindrome_detector_2tapes.tm", false);
        if(!tm.execute(false,result))return;
    }
    cout<<"pass 2"<<endl;

}

void test2(){

}

void test3(){

}


int main(int argc, char* argv[]){
    //TODO返回exit code

    try {
        //TuringMachine tm = TuringMachine("../programs/case1.tm", false);
        //string input="abb";
        TuringMachine tm = TuringMachine("../programs/case2.tm", false);
        string input="aaacaaac";
        tm.execute(true,input);
        return 0;
    } catch (const std::exception& e) {
        return 1; // 返回非零退出码表示异常情况
    }


    //test1();

//    if(false){
//        vector<string> value=tm.make_transition("reject5","00");
//        if(!value.empty()){
//            for(string t:value){
//                cout<<t<<" ";
//            }
//            cout<<endl;
//        }else{
//            cout<<"halt"<<endl;
//        }
//    }
//    if(argc == 1){
//        return 1;
//    }
//    cout<<"This is for testing"<<endl;
    return 0;
}