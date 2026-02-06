#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>
#include <chrono>
#include <functional>


using std::string;
using std::vector;
using std::array;


int method1(const vector<string>& v){
    int hits = 0;
    
    
    for (const string& s : v) {
        static std::unordered_map<char, bool> map;
        map.reserve(26);

        for(char c : s){
            if (map.find(c) == map.end())
                map[c] = true;
            else
                map.erase(c);
        }
        hits += (map.size() <= 1);  // false = 0
        map.clear();
    }
    return hits;
}

int method2(const vector<string>& v){
    int hits = 0;
    

    for (const string& s : v) {
        std::unordered_set<char> set;
		set.reserve(26);

        for(char c : s){
			auto [it, inserted] = set.insert(c);
            if (!inserted)
                set.erase(it);
           
        }
        hits += (set.size() <= 1);  // false = 0
    }
    return hits;
}

int method3(const vector<string>& v){
    int hits = 0;
    
    for (const string& s : v) {
        array<int, 26> array{0};
        for(char c : s){
            if (array[c-'a'])
                array[c-'a']--;
            else 
                array[c-'a']++;
        }
        hits += (std::accumulate(array.begin(), array.end(), 0) <= 1);
    }
    return hits;
}


int method4(const vector<string>& v){
    int hits = 0;
    
    for (const string& s : v) {
        uint32_t boolMap = false;

        for(char c : s){
            boolMap ^= (1 << (c-'a'));

        }
        hits += ((boolMap & (boolMap - 1)) == 0);
    }
    return hits;
}

int method5(const vector<string>& v) {
    int hits = 0;
    int size = v.size();

    #pragma omp parallel for reduction(+:hits)
    for (int i = 0; i < size; i++) {
        uint32_t boolMap{false};
        const string& s = v[i];

        // Calcolo bit map
        for (char c : s) {
            boolMap ^= (1 << (c - 'a'));
        }

        hits += ((boolMap & (boolMap - 1)) == 0);
    }

    return hits;
}

int method6(const vector<char>& v, int threads) {
    constexpr unsigned int STRING_LENGTH = 1000;
    constexpr unsigned int STRING_NUMBER = 1000000;
    int hits = 0;

    const char* raw_data = v.data(); // puntatore raw facilita il compilatore

    #pragma omp parallel for reduction(+:hits)
    for (int i = 0; i < STRING_NUMBER; i++) {
        uint32_t boolMap = 0;

        const char* current_str = &raw_data[i * STRING_LENGTH];
        for (int j = 0; j < STRING_LENGTH; j++) {
            boolMap ^= (1 << (current_str[j] - 'a'));
        }
        hits += ((boolMap & (boolMap - 1)) == 0);
    }
    return hits;
}


int method7(const vector<char>& v, int threads) {
    constexpr unsigned int STRING_LENGTH = 1000;
    constexpr unsigned int STRING_NUMBER = 1000000;
    int hits = 0;

    const char* raw_data = v.data(); // puntatore raw facilita il compilatore

    #pragma omp parallel for reduction(+:hits) num_threads(threads)
    for (int i = 0; i < STRING_NUMBER; i++) {
        uint32_t boolMap = 0;

        const char* current_str = &raw_data[i * STRING_LENGTH];
        for (int j = 0; j < STRING_LENGTH; j++) {
            boolMap ^= (1 << (current_str[j] - 'a'));
        }
        hits += ((boolMap & (boolMap - 1)) == 0);
    }
    return hits;
}

template <typename Func>
void benchmark(const string& name, Func&& func, const vector<string>& data, int repetitions) {
    auto start = std::chrono::high_resolution_clock::now();
    
    int result = 0; // just a check for the correctness
    
    for (int i = 0; i < repetitions; ++i) {
        result += func(data); // ensure the function is called despite aggressive -O3 optimizations
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    double avg_time = duration.count() / (repetitions);

    std::cout << name << " executed in " << avg_time << " seconds on average. Result: "<< result << std::endl;
}

template <typename Func>
void benchmark2(const string& name, Func&& func, const vector<char>& data, int thread, int repetitions) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = 0; // just a check for the correctness

    for (int i = 0; i < repetitions; ++i) {
        result += func(data, thread); // ensure the function is called despite aggressive -O3 optimizations
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    double avg_time = duration.count() / (repetitions);

    std::cout << name << " executed in " << avg_time << " seconds on average. Result: " << result << std::endl;
}


int main() {

    std::ifstream in("strings.txt");

    if (!in) {
        std::cerr << "can't open" << std::endl;
        return 1;
    }

    vector<string> strings;
    string line;

    std::cout << "Reading..." << std::endl;

    while (std::getline(in, line)) {
        strings.emplace_back(std::move(line));
    }

    vector<char> data;
    data.resize(1000 * strings.size());
    int i = 0;
    for (auto& s : strings)
        for (int j = 0; j < 1000; j++)
            data[i++] = s[j];
  
    std::cout<<"Done. Benchmarking.."<<std::endl;

    benchmark("Method 1, map,    5 rep,", method1, strings, 5);
    benchmark("Method 2, set,    5 rep,", method2, strings, 5);
    benchmark("Method 3, arr,  150 rep,", method3, strings, 150);
    benchmark("Method 4, int,  500 rep,", method4, strings, 500);
    benchmark("Method 5, omp, 1000 rep", method5, strings, 1000);
    benchmark2("Method 6, omp + cache locality, 16 threads, 1300 rep", method6, data, 0, 1300);
    benchmark2("Method 7, omp + cache locality,  6 threads, 1500 rep", method7, data, 6, 1500);


    char a;
    std::cin >> a;
    std::cout << a;
    return 0;
}