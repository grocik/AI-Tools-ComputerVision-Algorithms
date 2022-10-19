#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>

using funkcja_t = std::function<float(std::vector<float>)>;
using vector_t = std::vector<std::string>;
using formatery_t = std::map<std::string,funkcja_t>;

void calculate(formatery_t function,vector_t arguments){
    using namespace std;

    auto functionPicked = function[arguments.at(1)];


    vector<float> x;

    for(int i = 2 ; i <  arguments.size() ; i++){
        x.push_back(stof(arguments.at(i)));
    }

    cout << functionPicked(x);
}
int main(int argc, char **argv) {
    using namespace std;

    map<string,string> lista = {{"add","type add numberA numberB - to add two numbers"},
                                {"mod","type mod numberA numberB - to get modulo of two numbers"},
                                {"sin","type sin number - to get solution of sin using this numbers"}};

    formatery_t formatery;

    formatery["add"] = [](vector<float> x) {
        return x.at(0) + x.at(1);
    };
    formatery["mod"] = [](vector<float> x){

        return fmod(x.at(0),x.at(1));
    };
    formatery["sin"] = [](vector<float> x){
        return sin(x.at(0));
    };

    try {
        vector<string> arguments(argv, argv+argc);
        calculate(formatery,arguments);


    }
    catch (std::out_of_range ofr){
        cout << "pass corect argument, avelivable arguments: " << endl;
        for(auto [k,v] : lista){
            cout << v << endl;
        }
    }
    catch (std::bad_function_call ofr2){

        vector<string> arguments(argv, argv+argc);
        bool first = true;
        cout << "bad arguments !! you typed: " << endl;
        for(string arg : arguments){

            if (first){
                first = false;
                continue;
            }
            else{
                cout << arg << " ";
            }
        }
    }

    return 0;
}
