#include "graph.h"
#include "loadfile.h"
#include "utils.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using data::Graph;
using data::Airport;
using data::Flight;

std::map<std::pair<Airport*, Airport*>, std::vector<Flight>> io::mergeFlights(
    std::map<std::pair<Airport*, Airport*>, std::vector<Flight>> &flights,
    data::Frequency newFreq, size_t threshold) {
    
    // If desired frequency is monthly, we're done, nothing to do.
    if (newFreq == data::MONTHLY) return flights;

    std::map<std::pair<Airport*, Airport*>, std::vector<Flight>> flights_copy;

    // handle curr.frequency;
    // Change from single flights to a flight with frequency (weekly portion)
    for (auto i = flights.begin(); i != flights.end(); i++) {

        // for all pairs of flights from A to B do
        for (auto first = i->second.begin(); first != i->second.end(); first++) {
            size_t weeks = 0;

            for (auto second = i->second.begin(); second != i->second.end(); second++) {
                // Called when two similar flights are found
                if ((newFreq == data::WEEKLY && (*first).isSimilar(*second) && (*first).weekday == (*second).weekday) ||
                     ((*first).isSimilar(*second) && first->frequency == data::WEEKLY && second->frequency == data::WEEKLY)) {
                    weeks++;

                    // If we've reached the threshold, increment the frequency.
                    if (weeks >= threshold) {

                        bool should_add = true;

                        (*first).frequency = newFreq;
                        
                        // Safety check, if we don't have this we could push back to undefined
                        if (flights_copy.find(std::pair<Airport*, Airport*>(first -> departure, first -> arrival)) == flights_copy.end())
                            flights_copy[std::pair<Airport*, Airport*>(first -> departure, first -> arrival)] = std::vector<Flight>();

                        // Check that we don't already have this flight
                        for (Flight f: flights_copy[std::pair<Airport*, Airport*>(first -> departure, first -> arrival)]) {
                            if (f.isSimilar(*first) && f.weekday == first->weekday && newFreq == data::WEEKLY) should_add = false;                            
                            if (f.isSimilar(*first) && newFreq == data::DAILY) should_add = false;
                        }

                        // Add the flight if we need to
                        if (should_add) {
                            flights_copy[std::pair<Airport*, Airport*>(first -> departure, first -> arrival)].push_back(*first);
                        }

                        break;
                    }
                }
            }
            
            // If we haven't reached the threshold, add the flight and don't change it.
            if (weeks < threshold) {

                if (flights_copy.find(std::pair<Airport*, Airport*>(first -> departure, first -> arrival)) == flights_copy.end())
                    flights_copy[std::pair<Airport*, Airport*>(first -> departure, first -> arrival)] = std::vector<Flight>();

                flights_copy[std::pair<Airport*, Airport*>(first -> departure, first -> arrival)].push_back(*first);            
            }
        }
    }

    return flights_copy;

}

std::ifstream io::openFile(const std::string &filepath) {
    if (filepath.size() <= 4) {
        std::cerr << "File is not a CSV. Please try again." << std::endl;
        throw -2;
    }

    std::string extension = filepath.substr(filepath.size() - 4, 4);

    if (extension != ".csv") {
        std::cerr << "File is not a CSV. Please try again." << std::endl;
        throw -2;
    }

    std::ifstream file;
    file.open(filepath);

    if (!file.is_open()) {
        std::cerr << "Invalid filepath. Please try again with a valid file." << std::endl;
        throw -1;
    }

    return file;
}

Graph* io::loadFile(const std::string &filepath) {
    try {
        // checking for valid file (with try/catch block)
        std::ifstream file = openFile(filepath);
        Graph* g = loadFile(file);
        file.close();
        return g;
    } catch (int i) {
        // invalid file found
        throw i;
    }
}

Graph* io::loadFile(std::ifstream &file) {
    std::string first_line;
    getline(file, first_line);

    if (first_line != R"("DAY_OF_WEEK","FL_DATE","OP_UNIQUE_CARRIER","ORIGIN_AIRPORT_ID","ORIGIN","DEST_AIRPORT_ID","DEST","CRS_DEP_TIME","CRS_ARR_TIME","AIR_TIME","DISTANCE",)") {
        std::cerr << "File formatting is incorrect. Please try again with correctly formatted file." << std::endl;
        throw -3;
    }

    // map depart, arrive pair to a vector of flights from A to B
    std::map<std::pair<Airport*, Airport*>, std::vector<Flight>> flights;

    // map airport code to airport
    std::map<size_t, Airport*> airports;

    std::string line;
    size_t flight_count = 0;
    size_t line_count = 0;

    // read file into vector of flights/airports
    while (file) {
        getline(file, line);
        line_count++; 
        
        std::vector<std::string> fields = utils::split(line, ',');

        if (fields[0] == "" ||
            fields[1] == "" ||
            fields[3] == "" ||
            fields[5] == "" ||
            fields[7] == "" ||
            fields[8] == "") continue;       

        // initialize every single flight
        // flight_id (auto increment) & weekday & airline carrier
        Flight curr;
        curr.flight_id = flight_count++;
        curr.weekday = std::stoi(fields[0]);
        curr.airline = fields[2].substr(1, fields[2].size() - 2);
        
        // departure airport (initialize new airport if necessary)
        if (airports.find(std::stoi(fields[3])) == airports.end()) {
            Airport *a = new Airport();

            a -> airport_id = std::stoi(fields[3]);
            a -> airport_code = fields[4].substr(1, 3);

            curr.departure = a;

            airports[std::stoi(fields[3])] = a;
        } else {
            curr.departure = airports[std::stoi(fields[3])];
        }

        // arrival airport (initialize new airport if necessary)
        if (airports.find(std::stoi(fields[5])) == airports.end()) {
            Airport *a = new Airport();

            a -> airport_id = std::stoi(fields[5]);
            a -> airport_code = fields[6].substr(1, 3);

            curr.arrival = a;

            airports[std::stoi(fields[5])] = a;
        } else {
            curr.arrival = airports[std::stoi(fields[5])];
        }

        // departure/arrival time
        curr.depart_time = utils::timeFromMidnight(fields[7]);
        curr.arrive_time = utils::timeFromMidnight(fields[8]);

        // flight time & distance 
        if (fields[9] != "") curr.airtime = std::stoi(fields[9]);
        if (fields[10] != "") curr.distance = std::stoi(fields[10]);

        // set Monthly frequency as default
        curr.frequency = data::MONTHLY;

        // map each single flight to the corresponding pair of departure/arrival airports
        if (flights.find(std::pair<Airport*, Airport*>(curr.departure, curr.arrival)) == flights.end()) {
            flights[std::pair<Airport*, Airport*>(curr.departure, curr.arrival)] = std::vector<Flight>();
        }
        flights[std::pair<Airport*, Airport*>(curr.departure, curr.arrival)].push_back(curr);

    }

    // flights summary statistics: total number of valid flights 
    size_t valid_flights = 0;
    for (auto i = flights.begin(); i != flights.end(); i++) {
        valid_flights += i->second.size();
    }

    std::cout << "Loaded flights. Proceeding to merge phase..." << std::endl;
    std::cout << "Total valid flights: " << valid_flights << " (" << (double) 100* valid_flights / line_count << "%)" << std::endl;

    // update curr.frequency;
    flights = mergeFlights(flights, data::WEEKLY, 3);
    flights = mergeFlights(flights, data::DAILY, 6);

    valid_flights = 0;
    for (auto i = flights.begin(); i != flights.end(); i++) {
        valid_flights += i->second.size();
    }

    std::cout << "Unique flights detected: " << valid_flights << std::endl;
    std::cout << "Airports detected: " << airports.size() << std::endl;

    // initialize the airline multigraph with airports as vertex and each single flight as a single edge
    Graph* g = new Graph();

    for (auto i = airports.begin(); i != airports.end(); i++) {
        g->createVertex(i -> second);
    }

    for (auto i = flights.begin(); i != flights.end(); i++) {
        for (auto first = i->second.begin(); first != i->second.end(); first++) {
            g->createEdge(*first);
        }
    }

    std::cout << "Finished loading." << std::endl;

    return g;
}
