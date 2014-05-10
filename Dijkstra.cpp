#include "Dijkstra.h"



Dijkstra::Dijkstra()
{
	// Initializing RouteMap
    for(unsigned int i=1;i<NODES;i++)
    {        
        predecessor[i] = i;
    }
   
}

// If the returned queue is empty, it means there is no direct path between source and destination
std::deque<unsigned int> Dijkstra::findShortest(unsigned int sourceNode, unsigned int destNode, const Graph& g)
{
    //requires (1 <= sourceNode <= Nodes && 1 <= destNode <= Nodes)
    //ensures 1 <= \result.size() <= Nodes
    //ensures \result.at(0).NodeId == sourceNode && \result.at(\result.size()-1).NodeId == destNode
  
    PriorityQueue currentStatus(sourceNode,NODES);
    
    unsigned int currentNode,currNeighbor,newCost,oldCost;    
    currentNode = sourceNode;
    
    map<unsigned int,Plan>::iterator it;
    while(currentStatus.indexMap[destNode-1] != -1)
    {
        for(it = g.nodeGraph[currentNode-1].begin(); it!= g.nodeGraph[currentNode-1].end();it++)
        {  
            currNeighbor = it->first+1; //the Node ID of the node. Node ID is used as nodeID-1 for indexing later 
            
            // If the current neighbor is already visited, skip
			if(currentStatus.indexMap[currNeighbor] != -1)
			{
				    nodeMap N = g.nodeGraph;
		            newCost = N[currentNode-1][currNeighbor-1].cost + currentStatus.at(currentNode).priority;
                	    oldCost = currentStatus.at(currNeighbor).priority;
                
                		if( newCost < oldCost)
                		{
	                		currentStatus.decreasepriority(currNeighbor,newCost);//change the current cost (priority) of the current neighbour to the new lower cost (priority)
                    			predecessor[currNeighbor]= currentNode;
                		}
          		}
        }
        currentStatus.extractMin(); 
		//extract the current node which is at the root right now and has the minimum cost
        currentStatus.SetInd(currentNode,-1); 
		//since the current node has been extracted from the priority queue, we mark it visited by making its index -1
        currentNode = currentStatus.getRoot().nodeID;
    }
    
	// Reverse the order of queue
    unsigned int NodeIterator;
    deque<unsigned int> shortestPath;
	
	NodeIterator = destNode;
    while (NodeIterator!=sourceNode) {
        shortestPath.push_front(NodeIterator);
        NodeIterator = predecessor[NodeIterator];
    }
    shortestPath.push_front(NodeIterator);
    return shortestPath;
}

