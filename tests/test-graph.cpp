#define CATCH_CONFIG_MAIN
#include "../catch/catch.hpp"
#include "../graph.h"

#include <iostream>

using data::Graph;
using data::Airport;
using data::Flight;


// Tests for the Graph class should go in here
// Build and run these tests by running the following commands in Terminal:
// make testgraph
// ./testgraph


TEST_CASE("createVertex") {
    Airport* a = new Airport();
    a-> airport_id = 1;

    Airport* b = new Airport();
    b-> airport_id = 2;

    Airport* c = new Airport();
    c-> airport_id = 3;

    Graph g;
    g.createVertex(a);
    g.createVertex(b);
    g.createVertex(c);

    REQUIRE_FALSE(g.areAdjacent(a, b));
    REQUIRE_FALSE(g.areAdjacent(a, c));
}

TEST_CASE("Neighbors") {
    Airport* a = new Airport();
    a-> airport_id = 1;

    Airport* b = new Airport();
    b-> airport_id = 2;

    Airport* c = new Airport();
    c-> airport_id = 3;

    Graph g;
    g.createVertex(a);
    g.createVertex(b);
    g.createVertex(c);

    Flight a_b;
    a_b.departure = a;
    a_b.arrival = b;
    a_b.flight_id = 0;
    g.createEdge(a_b);

    Flight a_c;
    a_c.departure = a;
    a_c.arrival = c;
    a_c.flight_id = 1;
    g.createEdge(a_c);



    REQUIRE(g.areAdjacent(a, b));
    REQUIRE(g.areAdjacent(b, a));
    REQUIRE(g.areAdjacent(a, c));

    REQUIRE(g.outgoingNodes(a).size() == 2);
    REQUIRE(g.incomingNodes(a).size() == 0);
    REQUIRE(g.incomingNodes(b).size() == 1);
    REQUIRE(g.incomingNodes(c).size() == 1);
}

TEST_CASE("incomingNodes"){
    Airport* a = new Airport();
    a-> airport_id = 1;

    Airport* b = new Airport();
    b-> airport_id = 2;

    Airport* c = new Airport();
    c-> airport_id = 3;

    Graph graph;
    graph.createVertex(a);
    graph.createVertex(b);
    graph.createVertex(c);

    Flight a_b;
    a_b.departure = a;
    a_b.arrival = b;
    a_b.flight_id = 0;
    graph.createEdge(a_b);

    Flight b_c;
    b_c.departure = b;
    b_c.arrival = c;
    b_c.flight_id = 1;
    graph.createEdge(b_c);

    Flight a_c;
    a_c.departure = a;
    a_c.arrival = c;
    a_c.flight_id = 2;
    graph.createEdge(a_c);

    std::vector<Airport *> incoming;
    incoming.push_back(a);
    REQUIRE(graph.incomingNodes(b) == incoming);
    incoming.push_back(b);

    bool out1 = graph.incomingNodes(c)[0] == incoming[1] || graph.incomingNodes(c)[0] == incoming[0];
    bool out2 = graph.incomingNodes(c)[1] == incoming[0] || graph.incomingNodes(c)[1] == incoming[1];
    bool out3 = incoming[0] == incoming[1];
    REQUIRE(out1);
    REQUIRE(out2);
    REQUIRE_FALSE(out3);
}


TEST_CASE("outgoingNodesDestination"){
    Airport* a = new Airport();
    a-> airport_id = 1;

    Airport* b = new Airport();
    b-> airport_id = 2;

    Airport* c = new Airport();
    c-> airport_id = 3;

    Graph g;
    g.createVertex(a);
    g.createVertex(b);
    g.createVertex(c);

    Flight a_b;
    a_b.departure = a;
    a_b.arrival = b;
    a_b.flight_id = 0;
    g.createEdge(a_b);

    Flight b_c;
    b_c.departure = b;
    b_c.arrival = c;
    b_c.flight_id = 1;
    g.createEdge(b_c);

    std::vector<Airport *> outgoingDestination;
    outgoingDestination.push_back(b);
    REQUIRE(g.outgoingNodes(a) == outgoingDestination);
}

TEST_CASE("findFurthestAirports") {
    Airport* a = new Airport();
    a-> airport_id = 1;

    Airport* b = new Airport();
    b-> airport_id = 2;

    Airport* c = new Airport();
    c-> airport_id = 3;

    Airport* d = new Airport();
    d-> airport_id = 4;

    Airport* e = new Airport();
    e-> airport_id = 5;

    Airport* f = new Airport();
    f-> airport_id = 6;

    Airport* g = new Airport();
    g-> airport_id = 7;

    Graph graph;
    graph.createVertex(a);
    graph.createVertex(b);
    graph.createVertex(c);
    graph.createVertex(d);
    graph.createVertex(e);
    graph.createVertex(f);
    graph.createVertex(g);

    Flight a_b;
    a_b.departure = a;
    a_b.arrival = b;
    a_b.flight_id = 0;
    graph.createEdge(a_b);

    Flight a_c;
    a_c.departure = a;
    a_c.arrival = c;
    a_c.flight_id = 1;
    graph.createEdge(a_c);

    Flight b_e;
    b_e.departure = b;
    b_e.arrival = e;
    b_e.flight_id = 2;
    graph.createEdge(b_e);

    Flight d_c;
    d_c.departure = d;
    d_c.arrival = c;
    d_c.flight_id = 3;
    graph.createEdge(d_c);

    Flight d_b;
    d_b.departure = d;
    d_b.arrival = b;
    d_b.flight_id = 4;
    graph.createEdge(d_b);

    Flight d_e;
    d_e.departure = d;
    d_e.arrival = e;
    d_e.flight_id = 5;
    graph.createEdge(d_e);

    Flight e_d;
    e_d.departure = e;
    e_d.arrival = d;
    e_d.flight_id = 6;
    graph.createEdge(e_d);

    Flight e_f;
    e_f.departure = e;
    e_f.arrival = f;
    e_f.flight_id = 7;
    graph.createEdge(e_f);

    Flight g_f;
    g_f.departure = g;
    g_f.arrival = f;
    g_f.flight_id = 8;
    graph.createEdge(g_f);

    std::vector<Airport *> furthests;
    furthests.push_back(d);
    furthests.push_back(f);
    REQUIRE(graph.findFurthestAirports(a) == furthests);
    furthests.clear();
    furthests.push_back(c);
    REQUIRE(graph.findFurthestAirports(b) == furthests);
    REQUIRE(graph.findFurthestAirports(c) == furthests);

}

TEST_CASE("Valgrind createVertex", "[valgrind=true][valgrind]") {
    Graph *g = new Graph();
    Airport *a = new Airport();
    a -> airport_id = 1;
    
    g -> createVertex(a);

    REQUIRE(g-> incomingNodes(a).empty());

    delete g;
}

Graph *createTestGraph(Airport *a, Airport *b, Airport *c) {
    // A -> B <-> C -> A
    Graph *g = new Graph();
    a -> airport_id = 1;

    b -> airport_id = 2;

    c -> airport_id = 3;
    
    g -> createVertex(a);
    g -> createVertex(b);
    g -> createVertex(c);

    Flight f1, f2, f3, f4;
    f1.departure = a;
    f1.arrival = b;
    f1.flight_id = 1;

    f2.departure = b;
    f2.arrival = c;
    f2.flight_id = 2;

    f3.departure = c;
    f3.arrival = b;
    f3.flight_id = 3;

    f4.departure = c;
    f4.arrival = a;
    f4.flight_id = 4;

    g -> createEdge(f1);
    g -> createEdge(f2);
    g -> createEdge(f3);
    g -> createEdge(f4);

    return g;
}

TEST_CASE("Valgrind createVertex + createEdge and areAdjacent", "[valgrind=true][valgrind]") {
    // A -> B <-> C -> A
    Airport *a = new Airport();
    Airport *b = new Airport();
    Airport *c = new Airport();

    Graph *g = createTestGraph(a, b, c);

    REQUIRE(!g-> incomingNodes(a).empty());

    REQUIRE(g -> areAdjacent(a, b));
    REQUIRE(g -> areAdjacent(b, a));

    REQUIRE(g -> areAdjacent(b, c));
    REQUIRE(g -> areAdjacent(c, b));

    REQUIRE(g -> areAdjacent(c, a));
    REQUIRE(g -> areAdjacent(a, c));

    delete g;
}

TEST_CASE("Get flight/airport from id") {
    // A -> B <-> C -> A
    Airport *a = new Airport();
    Airport *b = new Airport();
    Airport *c = new Airport();

    Graph *g = createTestGraph(a, b, c);

    REQUIRE(g -> getVertex(1) == a);
    REQUIRE(g -> getVertex(1) == a);
    REQUIRE(g -> getVertex(1) == a);


    REQUIRE(g -> getEdge(1).departure == a);
    REQUIRE(g -> getEdge(2).departure == b);
    REQUIRE(g -> getEdge(3).departure == c);
    REQUIRE(g -> getEdge(4).departure == c);

    delete g;
}

TEST_CASE("Outgoing nodes") {
    // A -> B <-> C -> A
    Airport *a = new Airport();
    Airport *b = new Airport();
    Airport *c = new Airport();

    Graph *g = createTestGraph(a, b, c);

    REQUIRE(g -> outgoingNodes(a).size() == 1);
    REQUIRE(g -> outgoingNodes(b).size() == 1);
    REQUIRE(g -> outgoingNodes(c).size() == 2);

    REQUIRE(g -> outgoingNodes(a)[0] == b);
    REQUIRE(g -> outgoingNodes(b)[0] == c);

    bool out1 = g -> outgoingNodes(c)[0] == b || g -> outgoingNodes(c)[0] == a;
    bool out2 = g -> outgoingNodes(c)[1] == b || g -> outgoingNodes(c)[1] == a;

    REQUIRE(out1);
    REQUIRE(out2);

    delete g;    
}

TEST_CASE("Incoming nodes with multi-edge") {
    Airport *a = new Airport();
    Airport *b = new Airport();
    Airport *c = new Airport();

    Graph *g = createTestGraph(a, b, c);

    Flight f;
    f.departure = a;
    f.arrival = b;
    f.flight_id = 5;

    g -> createEdge(f);

    REQUIRE(g -> incomingNodes(a).size() == 1);
    REQUIRE(g -> incomingNodes(b).size() == 2);
    REQUIRE(g -> incomingNodes(c).size() == 1);

    REQUIRE(g -> incomingNodes(c)[0] == b);
    REQUIRE(g -> incomingNodes(a)[0] == c);

    bool out1 = (g -> incomingNodes(b)[0] == a) || (g -> incomingNodes(b)[0] == c);
    bool out2 = (g -> incomingNodes(b)[1] == a) || (g -> incomingNodes(b)[1] == c);
    bool out3 = (g -> incomingNodes(b)[0] != g -> incomingNodes(b)[1]);

    REQUIRE(out1);
    REQUIRE(out2);
    REQUIRE(out3);

    delete g;    
}


TEST_CASE("Shortest Path") {
    Airport* a = new Airport();
    a-> airport_id = 1;

    Airport* b = new Airport();
    b-> airport_id = 2;

    Airport* c = new Airport();
    c-> airport_id = 3;

    Airport* d = new Airport();
    d-> airport_id = 4;

    Airport* e = new Airport();
    e-> airport_id = 5;

    Airport* f = new Airport();
    f-> airport_id = 6;

    Graph graph;
    graph.createVertex(a);
    graph.createVertex(b);
    graph.createVertex(c);
    graph.createVertex(d);
    graph.createVertex(e);
    graph.createVertex(f);

    Flight a_b;
    a_b.departure = a;
    a_b.arrival = b;
    a_b.flight_id = 0;
    a_b.depart_time = 700;
    a_b.arrive_time = 900;
    graph.createEdge(a_b);

    Flight a_c;
    a_c.departure = a;
    a_c.arrival = c;
    a_c.flight_id = 1;
    a_c.depart_time = 1700;
    a_c.arrive_time = 1800;
    graph.createEdge(a_c);

    Flight b_e1;
    b_e1.departure = b;
    b_e1.arrival = e;
    b_e1.flight_id = 2;
    b_e1.depart_time = 800;
    b_e1.arrive_time = 1000;
    graph.createEdge(b_e1);

    Flight b_e2;
    b_e2.departure = b;
    b_e2.arrival = e;
    b_e2.flight_id = 3;
    b_e2.depart_time = 930;
    b_e2.arrive_time = 1130;
    graph.createEdge(b_e2);

    Flight d_e;
    d_e.departure = d;
    d_e.arrival = e;
    d_e.flight_id = 4;
    d_e.depart_time = 1200;
    d_e.arrive_time = 1230;
    graph.createEdge(d_e);

    Flight e_d;
    e_d.departure = e;
    e_d.arrival = d;
    e_d.flight_id = 5;
    e_d.depart_time = 1700;
    e_d.depart_time = 1740;
    graph.createEdge(e_d);

    Flight e_f1;
    e_f1.departure = e;
    e_f1.arrival = f;
    e_f1.flight_id = 6;
    e_f1.depart_time = 1130;
    e_f1.arrive_time = 1750;
    graph.createEdge(e_f1);
    
    Flight e_f2;
    e_f2.departure = e;
    e_f2.arrival = f;
    e_f2.flight_id = 7;
    e_f2.depart_time = 1145;
    e_f2.arrive_time = 1730;
    graph.createEdge(e_f2);

    std::vector<Flight> flights;
    flights.push_back(a_b);
    flights.push_back(b_e2);
    flights.push_back(e_f2);
    REQUIRE(graph.shortestPath(a, f, 600, 10) == flights);
    
    flights.clear();
    flights.push_back(e_f2);
    REQUIRE(graph.shortestPath(e, f, 1100, 0) == flights);

    flights.clear();
    flights.push_back(b_e2);
    flights.push_back(e_f2);
    REQUIRE(graph.shortestPath(b, f, 700, 0) == flights);

    flights.clear();
    REQUIRE_THROWS(graph.shortestPath(a, c, 1800, 0) == flights);

}