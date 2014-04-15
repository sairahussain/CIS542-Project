#include <deque>
#include <stdio.h> 
#include "types.h"
class Dijkstra
{
    public:
        nodeMap nodeGraph;
        std::deque<size_t> findShortest(size_t sourceNode,size_t destNode);
        void setNodes();   
};