// RangeSearch.h
#ifndef CUSTOMANNKDTREE_H
#define CUSTOMANNKDTREE_H

#include "Utilities.h"

class CustomANNKd_tree : public ANNkd_tree {
public:
    CustomANNKd_tree(ANNpointArray points, int pointCount, int dimension, int bucketSize= 12, ANNsplitRule split = ANN_KD_STD);

    int getDim() const;
    int getNPoints() const;
    int getBucketSize() const;
    ANNpointArray getPoints() const;
    ANNidxArray getPointIndices() const;
    ANNkd_node* getRoot() const;
    ANNpoint getBoundingBoxLow() const;
    ANNpoint getBoundingBoxHigh() const;

    void setRoot(ANNkd_ptr node);

};

#endif
