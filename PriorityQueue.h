
const int NODES = 8;
const int INF = 9000; //defines the infinity distance between nodes

typedef struct heapNode{
    unsigned int nodeID;
    unsigned int priority;
}heapNode;

class PriorityQueue{
    
	//ghost bool index[int];
	//invariant (\count int x: index[x]) <= heapSize
	//coupling invariant (\A int x: index[x] <==> (\E unsigned int i: 0 <= i <= NODES : indexMap[i] == x  && (1 <= x <= heapSize && x != -1)))
	
	//ghost bool heap[heapNode]; //the node is in the heap when this variable is true
	//invariant (\count heapNode n: heap[n]) <= NODES
	//invariant (\A heapNode n: heap[n] <==> (\E unsigned int ID: 0 <= ID < NODES: node[indexMap[ID]].nodeID == n.nodeID && indexMap[n.nodeID] != -1))
	
	//ghost unsigned int ind[unsigned int];   //returns the index of the node in the node array which represents the heap
	//invariant (\A unsigned int i: 0 <= i < NODES && indexMap[i] != -1: ind[i] == indexMap[i])
	//ghost heapNode parent[heapNode];
	//invariant (\A unsigned int i, heapNode n: 0 <= i < NODES: parent[n] == ind[n.nodeID]/2)
	//ghost heapNode leftChild[heapNode];
	//invariant (\A unsigned int i, heapNode n: 0 <= i < NODES: leftchild[n] == 2*ind[n.nodeID])
	//ghost heapNode rightChild[heapNode]
	//invariant (\A unsigned int i, heapNode n: 0 <= i < NODES: rightchild[n] == 2*ind[n.nodeID]+1)
	
	//invariant (\A heapNode n: heap[n]: parent[n].priority <= n.priority)
	//for all nodes n, such that the node is in the heap, the parent of a node have a priority that is less than or equal to the priority of the node
	
	//invariant (\A unsigned int i: 0 <= i <= heapSize: node[i].priority >= node[1].priority)
	//for all nodes i, the priority of the node is greater than or equal t the priority of the root. (Minimum heap)
	
	
	struct heapNode node[NODES+1];
   
	unsigned int heapSize;
    int indexMap[NODES];
      
	void heapify(unsigned int index);
	
	void swapNodes(unsigned int index,unsigned int smallest);
    
 public:
    PriorityQueue();
    PriorityQueue(unsigned int sourceNodeID, unsigned int nodeNo);   
    
    void extractMin();
    bool decreasepriority(unsigned int nodeID,unsigned int newpriority);
    heapNode at(unsigned int nodeID);
    unsigned int ind(unsigned int);
    void SetInd(unsigned int nodeID, int newIndx);
    heapNode getRoot();
	
};

