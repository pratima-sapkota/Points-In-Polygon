#include "CustomANNKdTree.h"

// Constructor that forwards parameters to ANNkd_tree
CustomANNKd_tree::CustomANNKd_tree(ANNpointArray points, int pointCount, int dimension, int bucketSize, ANNsplitRule split)
    : ANNkd_tree(points, pointCount, dimension, bucketSize, split) {}

// Implementations of accessor functions
int CustomANNKd_tree::getDim() const { return dim; }
int CustomANNKd_tree::getNPoints() const { return n_pts; }
int CustomANNKd_tree::getBucketSize() const { return bkt_size; }
ANNpointArray CustomANNKd_tree::getPoints() const { return pts; }
ANNidxArray CustomANNKd_tree::getPointIndices() const { return pidx; }
ANNkd_node* CustomANNKd_tree::getRoot() const { return root; }
ANNpoint CustomANNKd_tree::getBoundingBoxLow() const { return bnd_box_lo; }
ANNpoint CustomANNKd_tree::getBoundingBoxHigh() const { return bnd_box_hi; }
void CustomANNKd_tree::setRoot(ANNkd_ptr node){
    ANNkd_tree::root = node;
}