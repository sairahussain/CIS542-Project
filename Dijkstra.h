#include <stdio.h> 
#include <deque>
#include <map>
#include "Graph.h"
#include "PriorityQueue.h"
/*Derived Class : Dijkstra inherits the base class Graph and inherits the data structure implementation of the 
map of nodes, their paths along with cost and direction for each path between two nodes*/
class Dijkstra// : public Graph
{
    
	private:	
        unsigned sourceNode;
        unsigned destNode;
        routeMap routes;
        
        public:
        Dijkstra(unsigned, unsigned);
        std::deque<unsigned> findShortest(const Graph&);
        	
};




