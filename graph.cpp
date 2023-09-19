#include "graph.h"
#include "utils.h"

#include <algorithm>
#include <set>
#include <map>
#include <queue>

using data::Airport;
using data::Flight;
using data::Graph;

// Graph class implementation goes here

Graph::Graph() {
    vertexList = std::unordered_map<size_t, IncidentEdgeList>();
    edgeList = std::unordered_map<size_t, EdgeListNode>();
}

Graph::Graph(const Graph &other) {
    copy(other);
}

Graph &Graph::operator=(const Graph &other) {
    if (this != &other) {
        clear();
        copy(other);
    }

    return *this;
}

Graph::~Graph() {
    clear();
}

void Graph::copy(const Graph &other) {
    std::vector<Airport*> airports = other.getAirports();
    std::vector<Flight> flights = other.getFlights();

    for (Airport *a : airports) {
        Airport *add = new Airport(*a);
        createVertex(add);
    }

    for (Flight f: flights) {
        Airport *start = other.getVertex(f.departure->airport_id);
        Airport *end = other.getVertex(f.arrival->airport_id);

        Flight add = f;
        add.departure = start;
        add.arrival = end;

        createEdge(add);
    }
}

void Graph::clear() {
    std::vector<Airport*> airports = getAirports();

    for (Airport *a : airports) {
        delete a;
    }
}

Airport *Graph::getVertex(size_t id) const {
    return vertexList.find(id)->second.airport;
}

Airport *Graph::getVertex(std::string code) const {
    for (auto i = vertexList.begin(); i != vertexList.end(); i++) {
        if (i -> second.airport->airport_code == code) return i->second.airport;
    }

    throw -1;
}

Flight Graph::getEdge(size_t id) const {
    return edgeList.find(id) -> second.flight;
}

std::vector<Airport*> Graph::getAirports() const {
    std::vector<Airport*> airports;

    for (auto i = vertexList.begin(); i != vertexList.end(); i++) {
        airports.push_back(i->second.airport);
    }

    return airports;
}
std::vector<Flight> Graph::getFlights() const {
    std::vector<Flight> flights;

    for (auto i = edgeList.begin(); i != edgeList.end(); i++) {
        flights.push_back(i->second.flight);
    }    

    return flights;
}

void Graph::createVertex(Airport* a) {
    vertexList[a->airport_id].airport = a;
}

void Graph::createEdge(Flight& f) {

    vertexList[f.arrival->airport_id].arriving.push_front(f);
    vertexList[f.departure->airport_id].departing.push_front(f);

    edgeList[f.flight_id] = EdgeListNode(f);

}

bool Graph::areAdjacent(Airport* first, Airport* second) const {
    if (first == NULL || second == NULL) return false;

    std::vector<Airport*> inc_first = outgoingNodes(first);
    std::vector<Airport*> inc_second = outgoingNodes(second);

    for (Airport *a : inc_second) {
        if (a-> airport_id == first-> airport_id) return true;
    }

    for (Airport *a : inc_first) {
        if (a-> airport_id == second-> airport_id) return true;
    }

    return false;
}

std::vector<Airport*> Graph::incomingNodes(Airport* arrival) const {
    IncidentEdgeList incidentEdgeList = vertexList.at(arrival->airport_id);
    std::list<Flight> arriving = incidentEdgeList.arriving;

    std::set<Airport*> result;

    for (Flight f : arriving) {
        result.insert(f.departure);
    }

    std::vector<Airport*> out = std::vector<Airport*>(result.size());
    std::copy(result.begin(), result.end(), out.begin());

    return out;
}

std::vector<Airport*> Graph::outgoingNodes(Airport* depart) const {
    // list of edges connected with the current airport
    IncidentEdgeList incidentEdgeList = vertexList.at(depart->airport_id);
    std::list<Flight> departing = incidentEdgeList.departing;

    std::set<Airport*> result;

    for (Flight f: departing) {
        // getting the arrival airport where the flights depart at the current airport
        result.insert(f.arrival);
    }

    std::vector<Airport*> out = std::vector<Airport*>();

    for (Airport *a : result) {
        out.push_back(a);
    }

    return out;
}

std::vector<Airport*> Graph::findFurthestAirports(Airport* start) const {
    std::vector<Airport*> furthest;
    int maxStop = 0;

    std::queue<Airport*> airportQueue;
    std::queue<int> stopsQueue;

    airportQueue.push(start);
    stopsQueue.push(0);

    std::unordered_set<size_t> visited;
    visited.insert(start->airport_id);

    while (!airportQueue.empty()) {
        Airport* currentAirport = airportQueue.front();
        int stopNum = stopsQueue.front();
        airportQueue.pop();
        stopsQueue.pop();

        if (maxStop < stopNum) {
            maxStop = stopNum;
            furthest.clear();
        }

        std::vector<Airport*> neighbors = outgoingNodes(currentAirport);

        for (Airport*& airport: neighbors) {
            if (visited.count(airport->airport_id) == 0) {
                airportQueue.push(airport);
                stopsQueue.push(stopNum + 1);
                visited.insert(airport->airport_id);
            }
        }
        furthest.push_back(currentAirport);
    }

    return furthest;
}

size_t Graph::stopCount(Airport* start) const {
    int maxStop = 0;

    std::queue<Airport*> airportQueue;
    std::queue<int> stopsQueue;

    airportQueue.push(start);
    stopsQueue.push(0);

    std::unordered_set<size_t> visited;
    visited.insert(start->airport_id);

    while (!airportQueue.empty()) {
        Airport* currentAirport = airportQueue.front();
        int stopNum = stopsQueue.front();
        airportQueue.pop();
        stopsQueue.pop();

        if (maxStop < stopNum) {
            maxStop = stopNum;
        }

        std::vector<Airport*> neighbors = outgoingNodes(currentAirport);

        for (Airport*& airport: neighbors) {
            if (visited.count(airport->airport_id) == 0) {
                airportQueue.push(airport);
                stopsQueue.push(stopNum + 1);
                visited.insert(airport->airport_id);
            }
        }
    }

    return maxStop;
}

std::vector<Airport*> Graph::rankAirports() const {
    std::vector<Airport*> airports = getAirports();

    std::unordered_map<size_t, size_t> airport_id_to_number;

    size_t id = 0;

    for (Airport* a : airports) {
        airport_id_to_number[a -> airport_id] = id++;
    }

    std::vector<Flight> flights = getFlights();

    std::vector<std::vector<double>> transition_matrix = std::vector<std::vector<double>>(airports.size(), std::vector<double>(airports.size(), 0));

    for (Flight f : flights) {
        size_t increment = 0;

        switch (f.frequency) {
            case data::DAILY:
                increment = 7 * 4;
                break;
            case data::WEEKLY:
                increment = 4;
                break;
            case data::MONTHLY:
                increment = 1;
                break;
        }
        
        size_t depart_idx = airport_id_to_number[f.departure->airport_id];
        size_t arrive_idx = airport_id_to_number[f.arrival->airport_id];

        transition_matrix[arrive_idx][depart_idx] += increment;

    }

    // Normalize columns of transition matrix!
    for (size_t j = 0; j < transition_matrix[0].size(); j++) {
        double sum = 0;

        for (size_t i = 0; i < transition_matrix.size(); i++) sum += transition_matrix[i][j];
        for (size_t i = 0; i < transition_matrix.size(); i++) transition_matrix[i][j] /= sum;
    }

    std::vector<double> starting_state = std::vector<double>(airports.size(), 0.0);
    starting_state[0] = 1.0;

    // Use power iteration to find the steady state.
    std::vector<double> steady_state = utils::powerIteration(transition_matrix, starting_state, starting_state.size() * 30);

    // sort in reverse order
    std::vector<double> steady_sorted = steady_state;
    std::sort(steady_sorted.begin(), steady_sorted.end(), std::greater<double>());

    std::vector<Airport*> airports_sorted = std::vector<Airport*>(airports.size());

    for (size_t i = 0; i < steady_state.size(); i++) {
        for (size_t j = 0; j < steady_sorted.size(); j++) {
            if (steady_state[i] == steady_sorted[j] && !airports_sorted[j] &&airports[i]) {
                airports_sorted[j] = airports[i];
                airports[i] = NULL;
            }
        }
    }

    return airports_sorted;
}

std::vector<Flight> Graph::shortestPath(Airport *depart, Airport* arrive, size_t departTime, size_t minConnectionTime) const {
    std::queue<Airport> q;

    // map from airport to arrival time at that airport
    std::map<Airport, Flight> arrive_time;

    arrive_time[*depart] = Flight();
    arrive_time[*depart].depart_time = departTime;
    arrive_time[*depart].arrive_time = departTime; 

    q.push(*depart);
    
    while (!q.empty()) {
        Airport check = q.front();
        q.pop();

        // Enqueue all outgoing nodes with flights after (time you arrive at that node) 
        // if time of arrival + connection < shortestSoFar, add that flight to the map
        for (Flight f: (vertexList.find(check.airport_id) -> second.departing)) {

            if (f.depart_time >= minConnectionTime + arrive_time[*f.departure].arrive_time) {
                
                if (arrive_time.find(*f.arrival) == arrive_time.end()) {
                    q.push(*f.arrival);
                    arrive_time[*f.arrival] = f;
                }

            }

        }
        
    }

    // depart -> arrive are not connected
    if (arrive_time.find(*arrive) == arrive_time.end()) throw -1;

    std::vector<Flight> path;

    Airport current = *arrive;

    // Trace back path from arrival, then reverse and return
    while (!(current == *depart)) {
        path.push_back(arrive_time[current]);
        current = *path.back().departure;
    }

    std::reverse(path.begin(), path.end());
    return path;
}
