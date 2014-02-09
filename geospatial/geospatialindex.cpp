#include "geospatialindex.h"

GeospatialIndex::GeospatialIndex() {
}

GeospatialIndex::~GeospatialIndex() {
}

void GeospatialIndex::reset() {
    //remove items
    QHash<unsigned long, UniNode*>::iterator uniNodeIt = uniNodes_.begin();
    while (uniNodeIt != uniNodes_.end()) {
        delete uniNodeIt.value();
        uniNodeIt = uniNodes_.erase(uniNodeIt);
    }
    QHash<unsigned long, MultiNode*>::iterator mulNodeIt = multiNodes_.begin();
    while (mulNodeIt != multiNodes_.end()) {
        delete mulNodeIt.value();
        mulNodeIt = multiNodes_.erase(mulNodeIt);
    }
}


void GeospatialIndex::insert(UniNode *uniNode) {
    uniNodes_[uniNode->getId()] = uniNode;
}
void GeospatialIndex::insert(MultiNode *multiNode) {
    multiNodes_[multiNode->getId()] = multiNode;
}

