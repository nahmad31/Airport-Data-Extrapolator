#include "graph.h"
#include "loadfile.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using data::Graph;
using data::Airport;
using io::loadFile;

/** Main function.
 * 
 * Make instructions: "make" in terminal
 * Run instructions: "./main" in terminal
 *
 * 
 * Command line parameters:
 * - First parameter MUST be the name of the file with the flight data (input file).
 *   This can be left blank, but this forces manual mode, and you must input the file at the command line.
 * - -a option: Executes the entire program automatically, with no input from cin. 
 *   If this is an option, it MUST be passed second.
 *   If -a is used, only one command can be executed.
 *   If -a is used, the next argument MUST be the command. 
 *   (for example: ... -a shortestpath DEN ORD)
 * 
 * 
 * Commands:
 * - quit: quits the program.
 * - rank: Ranks airports.
 * - shortestpath [X] [Y] [start] [connection]: returns shortest path from X to Y. Arguments X and Y are required.
 *   Start and connection are optional. Connection is the minimum connection time (in minutes, must be an integer)
 *   Start is the starting time given as an hhmm string. (ie 1340 is 13:40 or 1:40 PM).
 *   If one of [start, connection] is provided, both must be provided. You cannot provide just one.
 * 
 * - furthest [X]: return the furthest airports from X. This algorithm counts using number of stopovers.
 * 
 * Here, X and Y must be a three letter airport code, i.e. ORD.
 * 
 */

void handleRank(const Graph &g) {
    std::vector<Airport*> ranking = g.rankAirports();

    size_t rank = 1;
    std::cout << "Ranking of all airports using Markov chain:" << std::endl;

    for (Airport *a : ranking) {
        std::cout << rank << ". " << a -> airport_code << std::endl;
        rank++;
    }
}

void handleShortestPath(const std::string &command, const Graph &g) {
    std::vector<std::string> command_split = utils::split(command, ' ');

    size_t start = 0;
    size_t connection = 0;

    if (command_split.size() < 3 || command_split.size() == 4) {
        std::cout << "Command is invalid. Please try again." << std::endl;
        return;
    }

    if (command_split.size() == 5) {
        try {
            start = utils::timeFromMidnight(command_split[3]);
            connection = std::stoi(command_split[4]);
        } catch (std::invalid_argument) {
            std::cout << "One of start or connection was not understood. Using default value of 0 for both." << std::endl;
        }
    }

    try{
        std::vector<Flight> path = g.shortestPath(g.getVertex(command_split[1]), g.getVertex(command_split[2]), start, connection);
        size_t flight_count = 1;

        for (Flight f: path) {
            std::cout << flight_count << ". " << f.departure->airport_code << " -> " << f.arrival->airport_code <<
            " from " << utils::printTime(f.depart_time) << " to " << utils::printTime(f.arrive_time) << std::endl;
            flight_count++;
        }
    
    } catch (int i) {
        std::cout << "No same day flight itinerary exists with these parameters." << std::endl;
    }


}

void handleBFS(const std::string &command, const Graph &g) {
    std::vector<std::string> command_split = utils::split(command, ' ');

    if (command_split.size() != 2) {
        std::cout << "Command is invalid. Please try again." << std::endl;
        return;
    }

    try {

        std::vector<Airport*> airports = g.findFurthestAirports(g.getVertex(command_split[1]));

        std::cout << "Furthest airports from " << command_split[1] << ": ";
        for (Airport *a: airports) {
            std::cout << a -> airport_code << ", ";
        }

        std::cout << std::endl;

        std::cout << "Flight count: " << g.stopCount(g.getVertex(command_split[1])) << std::endl; 
    } catch (int i) {
        std::cout << "The airport you entered is not in the database. Please try again." << std::endl;
    }
}

void handleHelp() {
    std::cout << "quit: quits the program." << std::endl;
    std::cout << std::endl;

    std::cout << "help: displays list of commands." << std::endl;
    std::cout << std::endl;

    std::cout << "rank: rank airports using Markov chain." << std::endl;
    std::cout << std::endl;

    std::cout << "shortestpath [X][Y] (start) (connection): Find the shortest path between X and Y." << std::endl;
    std::cout << "X and Y are required and must be 3-letter airport codes." << std::endl;
    std::cout << "start and connection are optional. You must provide 0 or 2 of these arguments." << std::endl;
    std::cout << "start: The departure time, connection: The minimum time to catch a connecting flight." << std::endl;
    std::cout << "example: shortestpath DEN ORD 1500 20 means shortest path from DEN to ORD starting at 15h0m and with a min connection of 20 minutes." << std::endl;
    std::cout << std::endl;

    std::cout << "furthest [X]: Find the airports furthest from X. Counts using the number of stopovers." << std::endl;
    std::cout << "X is the three-letter code of an airport. It is required." << std::endl;
    std::cout << "example: furthest DFW means find the airports that require the most connections to get to from DFW." << std::endl;
    std::cout << std::endl;

}

void handleCommand(const std::string &command, const Graph &g) {
    std::string word = utils::split(command, ' ')[0];

    if (word == "quit")
        exit(0);
    else if (word == "rank")
        handleRank(g);
    else if (word == "shortestpath")
        handleShortestPath(command, g);
    else if (word == "furthest")
        handleBFS(command, g);
    else if (word == "help")
        handleHelp();
    else
        std::cout << command << ": " << "Command not understood." << std::endl;
        
}

int main(int argc, char *argv[]) {
    std::string s;
    if (argc > 1) {
        s = argv[1];
    } else {
        std::cout << "File name: ";
        getline(std::cin, s);
    }

    Graph *g1;

    try {
        g1 = loadFile(s);
    } catch (int i) {
        std::cout << "File cannot be read. Please try again." << std::endl;
        return i;
    }

    Graph g = *g1;

    // Runs in automatic mode.
    if (argc > 2) {
        std::string s(argv[2]);

        if (s == "-a") {
            std::string command = "";
            for (int i = 3; i < argc; i++) {
                command += argv[i];
                command += " ";
            }

            handleCommand(command, g);
            exit(0);
        }
    }

    // Runs in manual mode.
    std::cout << "Airport Data Analyzer 1.0. Type \"help\" for a list of commands." << std::endl;

    while (true) {
        std::string command;
        std::cout << std::endl << "Enter command: ";
        getline(std::cin, command);
        handleCommand(command, g);
    }

    delete g1;

    return 0;

}