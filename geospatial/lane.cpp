#include "lane.h"

Lane::Lane(unsigned long segmentId, unsigned int index, bool isSideWalk)
{
    segmentId_ = segmentId;
    index_ = index;
    isSideWalk_ = isSideWalk;
}
