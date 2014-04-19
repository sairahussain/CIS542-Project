#include <stdio.h>
#define NODES 8
#define INF 9000

typedef struct heapNode{
    size_t nodeID;
    size_t cost;
} heapNode;

class PriorityQueue{
    
    public:
    struct heapNode node[NODES+1];
    int indexMap[NODES];
    size_t heapSize;
    
    PriorityQueue();
    PriorityQueue(size_t sourceNodeID);    
    void extractMin();
    bool decreaseCost(size_t nodeID,size_t newCost);
    void heapify(size_t index);
    void swapNodes(size_t index,size_t smallest);
    heapNode at(size_t nodeID);
};
