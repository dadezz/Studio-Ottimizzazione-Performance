#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>

constexpr int NUM_STRINGS = 1000000;
constexpr int STRING_LENGTH = 1000;
constexpr char ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";

void writeRandomString(std::ostream& os, std::mt19937& gen, std::uniform_int_distribution<>& distrib) {
    std::string result;
    result.reserve(STRING_LENGTH);
    for (int i = 0; i < STRING_LENGTH; ++i) {
        result += ALPHABET[distrib(gen)];
    }
    os << result << '\n'; 
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ofstream outputFile("random_strings.txt");

    if (!outputFile) return 1;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 25);

    for (int i = 0; i < NUM_STRINGS; ++i) {
        writeRandomString(outputFile, gen, distrib);
    }

    outputFile.close();
    return 0;
}