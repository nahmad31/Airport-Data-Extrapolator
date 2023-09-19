#define CATCH_CONFIG_MAIN
#include "../catch/catch.hpp"
#include "../utils.h"
#include "../graph.h"

#include <vector>


/** This file should be used for testing the Markov method.
 * Compile using: make testmarkov
 * Run using: ./testmarkov
 */

// Matrix-vector product testing
TEST_CASE("Invalid dimensions: dim=0") {
    try {
        std::vector<double> a;
        std::vector<std::vector<double>> matrix;

        utils::matrixVectorProduct(matrix, a);
        FAIL("No exception thrown.");
    } catch(int i) {
        if (i != -1) {
            FAIL("Unknown exception occurred.");
            throw i;
        }
    }
}

TEST_CASE("Dimensions are different") {
    try {
        std::vector<double> a = {1, 2, 3, 5};
        std::vector<std::vector<double>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}; 
        utils::matrixVectorProduct(matrix, a);

        FAIL("No exception thrown."); 
    } catch (int i) {
        REQUIRE(i == -1);
    }
}

TEST_CASE("Valid dimensions") {
    std::vector<double> a = {1, 2, 3};
    std::vector<std::vector<double>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}; 
    std::vector<double> product = utils::matrixVectorProduct(matrix, a); 

    std::vector<double> expected = {14, 32, 50};  

    REQUIRE(product == expected);     
}

TEST_CASE("Matrix vector product (with decimals)", "[id=3]") {
    std::vector<double> a = {0.5, 0.5, 0};
    std::vector<std::vector<double>> matrix = {{0.5, 1.0/3.0, 0}, {0.5, 1.0/3.0, 0.5}, {0, 1.0/3.0, 0.5}}; 
    std::vector<double> product = utils::matrixVectorProduct(matrix, a); 

    std::vector<double> expected = {5.0/12, 5.0/12, 1.0/6};  

    REQUIRE(std::abs(product[0] - expected[0]) < 0.0001);
    REQUIRE(std::abs(product[1] - expected[1]) < 0.0001); 
    REQUIRE(std::abs(product[2] - expected[2]) < 0.0001);       
}

TEST_CASE("Power iteration") {
    std::vector<double> a = {1, 0, 0};
    std::vector<std::vector<double>> matrix = {{0.5, 1.0/3, 0}, {0.5, 1.0/3, 0.5}, {0, 1.0/3, 0.5}}; 
    std::vector<double> product = utils::powerIteration(matrix, a, 100); 

    std::vector<double> expected = {0.2857142857, 0.4285714286, 0.2857142857};  

    REQUIRE(std::abs(product[0] - expected[0]) < 0.0001);
    REQUIRE(std::abs(product[1] - expected[1]) < 0.0001); 
    REQUIRE(std::abs(product[2] - expected[2]) < 0.0001);  
}