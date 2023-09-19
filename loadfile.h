#pragma once

#include "graph.h"

#include <fstream>
#include <map>
#include <vector>

using data::Airport;
using data::Flight;

namespace io {

    /** Merges flights into a new map given a frequency.
     * For example, if there are 4 flights to ORD from LAX on the same weekday by the same airline,
     * set a single flight with weekly frequency instead.
     * @param flights A map<departure,arrival> , <flights from depart to arrive> to merge
     * @param newFreq The new frequency to use (merge monthly -> weekly using weekly, or weekly -> daily using daily)
     * @return a new map, with the same formatting as flights, with merged flights.
     */
    std::map<std::pair<Airport*, Airport*>, std::vector<Flight>> mergeFlights(
        std::map<std::pair<Airport*, Airport*>, std::vector<Flight>> &flights,
        data::Frequency newFreq, size_t threshold);

    /** Checks that the file is valid
     * @param filepath The filepath
     * @throws -1 if the file cannot be opened
     * @throws -2 if the file is not a .csv
     * @throws -3 if the file is valid, but does not contain readable data
     * @return an ifstream of the file
     */
    std::ifstream openFile(const std::string &filepath);

    // Load the file given a filepath.
    data::Graph* loadFile(const std::string &filepath);

    // Load the file given an ifstream.
    data::Graph* loadFile(std::ifstream &file);

} // namespace io