#pragma once

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

namespace data {

    /** Stores the data for a single airport.
     * @param airport_code The most recent three-letter code for the airport.
     * @param airport_id A unique identifier for the airport.
     */
    struct Airport {
        std::string airport_code;
        size_t airport_id;

        bool operator==(const Airport &other) const { return airport_id == other.airport_id; }
        bool operator<(const Airport &other) const { return airport_id < other.airport_id; }
    };

    /** Stores the frequency of each flight.
     */
    enum Frequency {
        DAILY,
        WEEKLY,
        MONTHLY,
    };

    /** Stores a single flight.
     * @param departure A pointer to the departure airport
     * @param arrival A pointer to the arrival airport
     * 
     * @param depart_time The departure time (in seconds from midnight)
     * @param arrive_time The arrival time (in seconds from midnight)
     * @param weekday The day of the flight's departure. 1 = Monday, 2 = Tuesday, ..., 7 = Sunday
     * 
     * @param airline The airline operating the flight
     * 
     * @param flight_id The unique id of the flight (procedurally generated)
     * @param distance Distance between departure and arrival points
     * @param airtime The average airtime of the flight
     * 
     * @param frequency The flight's frequency (monthly, hourly, etc.)
     */
    struct Flight {
        Airport *departure = NULL;
        Airport *arrival = NULL;

        size_t depart_time;
        size_t arrive_time;
        size_t weekday;

        std::string airline;

        size_t flight_id;
        size_t distance = 0;
        size_t airtime = 0;

        Frequency frequency;

        // Check if two flights are the same.
        bool operator==(const Flight &other) const { return flight_id == other.flight_id; }

        // Check if two flights are similar (used to merge single flights into scheduled flights)
        bool isSimilar(const Flight &other) const {    return (departure == other.departure && arrival == other.arrival && airline == other.airline && depart_time == other.depart_time);}

    };

    struct EdgeListNode {
        EdgeListNode() = default;
        EdgeListNode(Flight &f): flight(f) {}

        Flight flight;
        std::list<Flight> departing;
        std::list<Flight> arriving;
    };

    struct IncidentEdgeList {
        Airport *airport = NULL;
        std::list<Flight> departing;
        std::list<Flight> arriving;
    };

    class Graph {
        public:
            // Constructor
            Graph();

            Graph(const Graph &other);
            Graph &operator=(const Graph &other);
            ~Graph();

            Airport *getVertex(size_t id) const;
            Airport *getVertex(std::string code) const;
            Flight getEdge(size_t id) const;

            std::vector<Airport*> getAirports() const;
            std::vector<Flight> getFlights() const;

            // Create a vertex or an edge.
            void createVertex(Airport* a);
            void createEdge(Flight& f);

            // Returns true if there is a flight between first and second.
            bool areAdjacent(Airport* first, Airport* second) const;

            // Returns all airports with a flight to arrival.
            std::vector<Airport*> incomingNodes(Airport* arrival) const;

            // Returns all airports with a flight from depart.
            std::vector<Airport*> outgoingNodes(Airport* depart) const;

            // Find furthest airports by num connections and return the airports or the number of flights to get there
            std::vector<Airport*> findFurthestAirports(Airport* start) const;
            size_t stopCount(Airport *start) const;

            // Find shortest path (using min of arrival time) from A to B.
            // May occasionally return path not found when a path exists
            std::vector<Flight> shortestPath(Airport *depart, Airport* arrive, size_t departTime=0, size_t minConnectionTime=0) const;
            
            // Returns a sorted vector of airports by importance. Uses Markov chains on outgoing flights.
            std::vector<Airport*> rankAirports() const;
            
        private:
            void copy(const Graph &other);
            void clear();
            std::unordered_map<size_t, IncidentEdgeList> vertexList;
            std::unordered_map<size_t, EdgeListNode> edgeList;
    };

}
