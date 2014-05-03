#include "PriorityQueue.h"

/* Default constructor */
PriorityQueue::PriorityQueue()
{
	heapSize = 0;
}

/* Parameterized constructor initializes heap size with number of nodes */
PriorityQueue::PriorityQueue(unsigned int sourceNodeID, unsigned int nodeNo)
{
    // Node at first position is reserved for implementation of binary heap
    node[0].nodeID = 0;
    node[0].priority = 0;
    heapSize = nodeNo;
    // All nodes except sourceNode would have priority of infinity with source node having value of 0
    for(unsigned int i=1;i<=heapSize;i++)
    {
        node[i].nodeID = i;
        indexMap[i-1] = i;
        if(sourceNodeID == i)
            node[i].priority = 0;
        else
            node[i].priority = INF;
    }
    // Heapify all elements
    for(unsigned int j=heapSize/2;j>=1;j--)
        heapify(j);
}


/* Extract the highest priority element */
void PriorityQueue::extractMin()
{
    // increase the starting index
    swapNodes(1,heapSize);
    heapSize--;
    
    heapify(1);
}

/* Update the priority of a specified node */
bool PriorityQueue::decreasepriority(unsigned int nodeID,unsigned int newpriority)
{
    if(indexMap[nodeID-1] != -1)
    {
        node[indexMap[nodeID-1]].priority = newpriority;
        return true;
    }
     
    unsigned int thisNode = indexMap[nodeID-1];
    unsigned int parentNode = thisNode/2;
    
    
    //percolate the node with the decreased value up
    while(node[thisNode].priority < node[parentNode].priority && thisNode > 1)
    {
       swapNodes(thisNode, parentNode);
       thisNode = parentNode;
       parentNode = thisNode/2;
        
    }
    
    
    return false;
}

/*void PriorityQueue::heapify(unsigned int parent)
{
   
    
    unsigned int smallest,left,right;
    
    left = 2*parent;
    right = 2*parent + 1;
    if(left <= heapSize && node[left].priority < node[parent].priority)
        smallest = left;
    else
        smallest = parent;
   
    if(right <= heapSize && node[right].priority < node[smallest].priority)
        smallest = right;
    if(smallest != parent)
    {
        swapNodes(parent,smallest);
        heapify(smallest);
    }  
}*/


/* Iterative Solution heapify -  Given the index of parent it maintains the min heap structure*/
void PriorityQueue::heapify(unsigned parent)
{
	unsigned smallest, left, right;
	
	while(1)
	{
		left = parent * 2;
		right = parent * 2 +1;
		/* compare priority with left child */
		if(left <= heapSize && node[left].priority < node[parent].priority)
        		smallest = left;
    		else
        		smallest = parent;
        	/* compare priority with right child */	
    		if(right <= heapSize && node[right].priority < node[smallest].priority)
	        	smallest = right;
        	if(smallest != parent)
    		{
        		swapNodes(parent,smallest);
			parent = smallest;
    		}
    		else
    		{
    			break;
    		}
	} // end of while
}

/* Swap two nodes */
void PriorityQueue::swapNodes(unsigned int leftIndex,unsigned int rightIndex)
{
    heapNode temp = node[leftIndex];
    node[leftIndex] = node[rightIndex];
    node[rightIndex] = temp;
    
    indexMap[node[leftIndex].nodeID-1] = leftIndex;
    indexMap[node[rightIndex].nodeID-1] = rightIndex;
}


/* Return the node from Priority Queue at specified index */
heapNode PriorityQueue::at(unsigned int nodeID)
{
    return (node[indexMap[nodeID-1]]);
}

/* Returns the index of the node given its nodeID in the Priority Queue */
unsigned int PriorityQueue::ind(unsigned int nodeID)
{
	return (indexMap[nodeID-1]);
}

/* Update the index (which corresponds to the node's position in Priority Queue)of a node specified by its nodeID */
void PriorityQueue::SetInd(unsigned int nodeID, int newIndx)
{
	indexMap[nodeID-1] = newIndx;
}

/* Return the highest prioirty index */
heapNode PriorityQueue::getRoot()
{
	return node[1];
}

