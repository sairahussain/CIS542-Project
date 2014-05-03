#include "Dijkstra.h"



Dijkstra::Dijkstra(unsigned source, unsigned dest)
{
    sourceNode = source;
    destNode = dest;
	// Initializing RouteMap
    for(unsigned i=1;i<NODES;i++)
    {        
        routes[i] = i;
    }
   
}

// If the returned queue is empty, it means there is no direct path between source and destination
std::deque<unsigned> Dijkstra::findShortest(const Graph& g)
{
    //requires (1 <= sourceNode <= Nodes && 1 <= destNode <= Nodes)
    //ensures 1 <= \result.size() <= Nodes
    //ensures \result.at(0).NodeId == sourceNode && \result.at(\result.size()-1).NodeId == destNode
  
    PriorityQueue currentStatus(sourceNode,NODES);
    
    unsigned currentNode,currNeighbor,newCost,oldCost;    
    currentNode = sourceNode;
    
    map<unsigned,Plan>::iterator it;
    while(currentStatus.indexMap[destNode-1] != -1)
    {
        for(it = g.nodeGraph[currentNode-1].begin(); it!= g.nodeGraph[currentNode-1].end();it++)
        {  
            //invariant forall unsigned n: 1 <= n <= i && currentStatus[n-1] != -2: 
            //             			(currentStatus[currentNode-1])+ p[currentNode-1][n-1].cost <= /old(currentStatus[n-1]) ? currentStatus[n-1] ==       
            currNeighbor = it->first+1;
            
            // If the current neighbor is already visited, skip
			if(currentStatus.indexMap[currNeighbor] != -1)
			{
				    nodeMap N = g.nodeGraph;
		            newCost = N[currentNode-1][currNeighbor-1].cost + currentStatus.at(currentNode).priority;
                	    oldCost = currentStatus.at(currNeighbor).priority;
                
                		if( newCost < oldCost)
                		{
	                		currentStatus.decreasepriority(currNeighbor,newCost);
                    			routes[currNeighbor]= currentNode;
                		}
          		}
        }
        currentStatus.extractMin();
        currentStatus.SetInd(currentNode,-1);
        currentNode = currentStatus.getRoot().nodeID;
    }
    
	// Reverse the order of queue
    unsigned NodeIterator;
    deque<unsigned> shortestPath;
	
	NodeIterator = destNode;
    while (NodeIterator!=sourceNode) {
        shortestPath.push_front(NodeIterator);
        NodeIterator = routes[NodeIterator];
    }
    shortestPath.push_front(NodeIterator);
    return shortestPath;
}

