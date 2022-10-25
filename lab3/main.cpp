#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <algorithm>
#include <chrono>

using domain_t = std::vector<double>;
using function_formatery = std::function<double (domain_t)>;

std::random_device rd;
std::mt19937 mt_generator(rd());


domain_t brute_force_method(const std::function<double(domain_t)> &f, domain_t start_point, int max_iterations) {
    auto current_p = start_point;
    auto best_p = current_p;
    for (int i = 0; i < max_iterations; i++) {
        if (f(current_p) < f(best_p)) {
            best_p = current_p;
        }
        current_p = start_point;
    }
    return best_p;
}

domain_t hill_climbing(const std::function<double(domain_t)> &f, domain_t start_point, std::function<std::vector<domain_t>(domain_t)> get_close_points, int max_iterations) {
    domain_t best_p = start_point;
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        auto close_points = get_close_points(best_p);
        auto best_neighbour = *std::min_element(close_points.begin(), close_points.end(), [f](auto a, auto b){return f(a) > f(b);});
        if (f(best_neighbour) < f(best_p)) best_p = best_neighbour;
    }
    return best_p;
}

domain_t simulated_annealing(const std::function<double(domain_t)> &f, domain_t start_point, int max_iterations){
    auto best_p = start_point;
    std::vector<domain_t> vector;
    std::uniform_real_distribution<double> uk(0,1);
    double ukValue = uk(mt_generator);
    vector.push_back(best_p);

    for (int i = 0; i < max_iterations; ++i) {
        auto tk = start_point;
        if(f(tk) <= f(best_p)){
            vector.push_back(best_p);
        } else {
            if (ukValue < exp(-(abs(f(tk) - f(best_p)) / (1 / log(i))))) {
                best_p = tk;
                vector.push_back(best_p);
            }
        }
    }
    return best_p;
}

int main() {
    // ~~ Implemented Functions ~~
    function_formatery sphere_f = [](domain_t x){return x[0]*x[0];};
    function_formatery rosenbrock_f = [](domain_t x) {return 100 * pow((x[0] - (x[0] * x[0])),2) + pow((1 - x[0]),2);};
    function_formatery matyas_f = [](domain_t x) {return ((x[0]*x[0]) + (x[1]*x[1]))*0.26 - (0.48 * (x[0] * x[1])); };
    function_formatery beale_f = [](domain_t x) {return pow((1.5-x[0]+x[0]*x[1]),2) + pow(2.25-x[0]+x[0]*x[1]*x[1],2) + pow(2.265-x[0]+x[0]*x[1]*x[1]*x[1],2);};
    // ~~

    // ~~ Getting Random domain points for start ~~
    auto get_random_point = []() -> domain_t {
        std::uniform_real_distribution<double>distr(-5,5);
        return {distr(mt_generator), distr(mt_generator)};
    };
    //~~

    //~~ Getting random close points ~~
    auto get_close_points_random = [](domain_t p0) -> std::vector<domain_t> {
        std::uniform_real_distribution<double>distr(p0.at(0),p0.at(1));
        return {{distr(mt_generator), distr(mt_generator)}};
    };
    //~~

    // timer
    std::cout << "-----Matyas-----" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    auto best_matyas_annealing = simulated_annealing(matyas_f,get_random_point(),1000000);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "simulated_annealing = " << best_matyas_annealing[0] << " time = " << elapsed.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto best_matyas_brute = brute_force_method(matyas_f,get_random_point(),1000000);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "brute_force = " << best_matyas_brute[0] << " time = " << elapsed.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto best_matyas_hill_climb = hill_climbing(matyas_f,get_random_point(),get_close_points_random,1000000);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "Hill_climbing = " << best_matyas_hill_climb[0] << " time = " << elapsed.count() << std::endl;

    std::cout << "-----Sphere-----" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto best_sphere_annealing = simulated_annealing(sphere_f,get_random_point(),1000000);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "simulated_annealing = " << best_sphere_annealing[0] << " time = " << elapsed.count() << std::endl;


    start = std::chrono::high_resolution_clock::now();
    auto best_sphere_brute = brute_force_method(sphere_f,get_random_point(),1000000);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "brute_force = " << best_sphere_brute[0] << " time = " << elapsed.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto best_sphere_hill_climb = hill_climbing(sphere_f,get_random_point(),get_close_points_random,1000000);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "Hill_climbing = " << best_sphere_hill_climb[0] << " time = " << elapsed.count() << std::endl;

    std::cout << "-----Beale-----" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto best_beale_annealing = simulated_annealing(beale_f,get_random_point(),1000000);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "simulated_annealing = " << best_beale_annealing[0] << " time = " << elapsed.count() << std::endl;


    start = std::chrono::high_resolution_clock::now();
    auto best_beale_brute = brute_force_method(beale_f,get_random_point(),1000000);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "brute_force = " << best_beale_brute[0] << " time = " << elapsed.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto best_beale_climb = hill_climbing(beale_f,get_random_point(),get_close_points_random,1000000);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "Hill_climbing = " << best_beale_climb[0] << " time = " << elapsed.count() << std::endl;


    return 0;
}
