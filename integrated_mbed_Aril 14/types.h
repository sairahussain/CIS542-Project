
#include <map>
#include <deque>
#include <vector>
#define INF 9000
#define NODES 8

using namespace std;


typedef struct Plan{
    size_t cost;   // INT_MAX denotes infinity
    char direction; // i denotes infinity, 'N','W','S','E' are respective directions
}Plan;

typedef vector< map <size_t,Plan> > nodeMap;

 
typedef struct PathNode{
    size_t NodeID;
    size_t cost;   // INT_MAX denotes infinity
    char direction; // i denotes infinity, 'N','W','S','E' are respective directions
}PathNode;

void setNodes();

typedef map<size_t,size_t > routeMap;