#define CATCH_CONFIG_MAIN
#include "../catch/catch.hpp"

#include "../graph.h"
#include "../loadfile.h"

#include <string>
#include <vector>
#include <iostream>

using data::Graph;
using io::openFile;
using io::loadFile;

// Tests for loading in Graph data from files
// To build and run this, run the following commands in Terminal:
// make testio
// ./testio

TEST_CASE("Only CSV files are valid") {
    REQUIRE_THROWS(openFile("notCsv.pdf"));
    REQUIRE_THROWS(openFile("nonexistentFile.csv"));
    REQUIRE_NOTHROW(openFile("data/july-2019-data.csv"));
}

TEST_CASE("Full test of loading from file", "[id=2]") {
    std::string file = "data/july-2019-data.csv";
    std::ifstream f = openFile(file);

    try{
        Graph* flights = loadFile(f);
        delete flights;
    } catch (int) {
        FAIL("Some error occurred");
    }
    
    f.close();
    
}

TEST_CASE("Files other than july 2019 load successfully", "[id=3]") {
    std::string file = "data/mar-1990-data.csv";
    try{
        Graph* flights = loadFile(file);

        Airport *one = flights->getVertex("EWR");
        Airport *two = flights->getVertex("BOS");

        std::vector<Airport*> a = flights->outgoingNodes(one);
        bool found = false;
        for (auto i : a) {
            if (i -> airport_code == "BOS") found = true;
        }

        if (!found) FAIL("EWR-BOS flight should be valid!");

        CHECK(flights->areAdjacent(flights->getVertex(13891), flights->getVertex(13930)));
        CHECK(flights->areAdjacent(flights->getVertex(13891), flights->getVertex(10800)));
        CHECK(flights->areAdjacent(one, two));
        delete flights;

    } catch (int) {
        FAIL("File could not be loaded");
    }
}

// TODO: Add adjacency tests, shortest path tests, and airport ranking tests
