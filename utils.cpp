#include "utils.h"

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> utils::split(const std::string &toSplit, char splitOn) {
    // "splitted" here is not a typo, it's used to differentiate the function and vector names
    std::vector<std::string> splitted = {""};
    size_t idx = 0;

    for (const char c: toSplit) {
        if (c == splitOn) {
            splitted.push_back("");
            idx++;
        } else {
            splitted[idx] += c;
        }

    }

    return splitted;
}

size_t utils::timeFromMidnight(std::string &time) {
    std::string hours = time.substr(1, 2);
    std::string mins = time.substr(3, 2);

    return 60 * std::stoi(hours) + std::stoi(mins);
}

std::string utils::printTime(size_t minsFromMidnight) {
    std::stringstream s = std::stringstream();

    size_t m = minsFromMidnight % 60;
    size_t h = minsFromMidnight / 60;

    if (h < 10) s << "0" << h;
    else s << h;

    s << ":";

    if (m < 10) s << "0" << m;
    else s << m;

    return s.str();
}

std::vector<double> utils::matrixVectorProduct(std::vector<std::vector<double>> &matrix, std::vector<double> &vector) {
    if (matrix.empty() || matrix[0].size() != vector.size()) throw -1;

    std::vector<double> product = std::vector<double>(matrix.size(), 0);

    for (size_t i = 0; i < product.size(); i++) {
        for (size_t j = 0; j < vector.size(); j++) {
            product[i] += vector[j]*matrix[i][j]; 
        }
    }

    return product;
}

std::vector<double> utils::normalize(std::vector<double> &vec) {
    size_t sum = 0;

    for (double i : vec) {
        sum += std::abs(i);
    }

    for (double i : vec) {
        i /= sum;
    }

    return vec;
}

std::vector<double> utils::powerIteration(std::vector<std::vector<double>> &matrix, std::vector<double> &vector, size_t iter) {
    for (size_t i = 0; i < iter; i++) {
        vector = utils::matrixVectorProduct(matrix, vector);
        vector = utils::normalize(vector);
    }

    return vector;
}