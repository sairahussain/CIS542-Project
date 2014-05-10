#include <stdio.h> 
#include <deque>
#include <map>
#include "Graph.h"
#include "PriorityQueue.h"

typedef std::vector<unsigned int> predMap;
/*Class Dijkstra consists of the function findShortest to find the shortest path from source node to destination node 
 such that the graph of the interconnection of nodes is passed as an const reference object to findShortest function */

class Dijkstra
{
    
	private:	
        
        predMap predecessor;
        
        public:
        Dijkstra();
        std::deque<unsigned int> findShortest(unsigned int sourceNode, unsigned int destNode,const Graph&);
        	
};




