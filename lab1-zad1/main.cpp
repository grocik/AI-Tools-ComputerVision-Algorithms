#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>

using funkcja_t = std::function<float(float,float)>;
using vector_t = std::vector<std::string>;
using formatery_t = std::map<std::string,funkcja_t>;

void calculate(formatery_t function,vector_t arguments){
    using namespace std;

    auto functionPicked = function[arguments.at(1)];

    //tworzymy stringa z charów
    basic_string<char> a = arguments.at(2);
    basic_string<char> b = arguments.at(3);

    string fs = a;
    string fs2 = b;
    //zamiana stringa na float
    float f= stof(fs);
    float f2 = stof(fs2);

    cout << functionPicked(f,f2);
}
int main(int argc, char **argv) {
    using namespace std;


    formatery_t formatery;
    map<string,string> lista = {{"add","type add numberA numberB - to add two numbers"},
                                {"mod","type mod numberA numberB - to get modulo of two numbers"},
                                {"sin","type sin number - to get solution of sin using this numbers"}};

    formatery["add"] = [](float a, float b) {
        return a + b;
    };
    formatery["mod"] = [](float a, float b){

        return fmod(a,b);
    };
    formatery["sin"] = [](float a,float b){
        return sin(a);
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