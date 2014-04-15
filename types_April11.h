#include <map>
#include <deque>
#include <vector>



typedef struct Plan{
    size_t cost;   // INT_MAX denotes infinity
    char direction; // i denotes infinity, 'N','W','S','E' are respective directions
}Plan;

typedef vector< map <size_t,Plan> > nodeMap;
nodeMap nodeGraph;
 
typedef struct PathNode{
    size_t NodeID;
    size_t cost;   // INT_MAX denotes infinity
    char direction; // i denotes infinity, 'N','W','S','E' are respective directions
}PathNode;

void setNodes();

typedef map<size_t,size_t > routeMap;
