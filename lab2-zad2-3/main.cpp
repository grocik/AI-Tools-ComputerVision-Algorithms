#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <map>

using args_array = std::vector<double>;
using search_domain = std::map<std::string,std::string>;
using hill_formatery = std::map<std::string, std::function<double (args_array)>>;
using domain_t = std::vector<double>;

std::random_device rd;
std::mt19937 mt_generator(rd());



domain_t hill_climbing(const std::function<double(args_array)> &f, domain_t minimal_d, domain_t maximal_d, int max_iterations) {
    domain_t current_p(minimal_d.size());
    for (int i = 0; i < minimal_d.size(); i++) {
        std::uniform_real_distribution<double> dist(minimal_d[i], maximal_d[i]);
        current_p[i] = dist(mt_generator);
    }
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        domain_t new_p(minimal_d.size());
        for (int i = 0; i < minimal_d.size(); i++) {
            std::uniform_real_distribution<double> dist(-1.0/128.0, 1.0/128.0);
            double picked = current_p[i] + dist(mt_generator);
            if (minimal_d[i] < picked && maximal_d[i] > picked ){
                new_p[i] = picked;
            }
        }
        if (f(current_p) > f(new_p)) {
            current_p = new_p;
        }

    }
    return current_p;
}

int main(int argc, char **argv) {

    search_domain searchDomain = {{"how to use", "type - Function name - start of domain - end of domain "},
                                  {"sphere", "search domain is -00 <= x <= 00 "},
                                  {"rosenbrock", "search domain is -00 <= x <= 00"},
                                  {"matyas", "search domain is -10 <= x,y <= 10"}};


    hill_formatery hillFormatery;
    domain_t minimal;
    domain_t maximal;



    hillFormatery["sphere"] = [](args_array x){return x[0]*x[0];};
    hillFormatery["rosenbrock"] = [](args_array x) {return 100 * pow((x[0] - (x[0] * x[0])),2) + pow((1 - x[0]),2);};
    hillFormatery["matyas"] = [](args_array x) {return ((x[0]*x[0]) + (x[1]*x[1]))*0.26 - (0.48 * (x[0] * x[1])); };

    std::string compare = argv[1];

    try {
        if (compare == "matyas"){
            maximal.push_back(std::stod(argv[3]));
            maximal.push_back(std::stod(argv[5]));
            minimal.push_back(std::stod(argv[2]));
            minimal.push_back(std::stod(argv[4]));

        }
        else{
            maximal.push_back(std::stod(argv[3]));
            minimal.push_back(std::stod(argv[2]));

        }

        auto best_points = hill_climbing(hillFormatery.at(argv[1]),minimal,maximal,1000000);
        if (compare == "matyas"){
            std::cout << "best x = " << best_points[0] << std::endl;
            std::cout << "best y = " << best_points[1] << std::endl;
        }
        else{
            std::cout << "best x = " << best_points[0] << std::endl;
        }

    }
    catch (std:: exception e){

    }


    return 0;
}
