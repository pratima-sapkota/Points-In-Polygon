// RangeSearch.h
#ifndef RANGESEARCH_H
#define RANGESEARCH_H

#include <iostream>
#include <vector>
#include <ANN/ANN.h>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include "CustomANNKdTree.h"
#include "ann/include/ANN/ANNperf.h"
#include "ann/src/kd_tree.h"
#include "Utilities.h"
#include <unordered_set>

namespace fs = std::filesystem;


class RangeSearch {

public:
    CustomANNKd_tree* kdTree; // Pointer to a kd-tree instance
    ANNpointArray points;
    int pointCount;
    std::vector<std::vector<int>> inside_points;
    std::unordered_set<ANNkd_ptr> skip_nodes;
    
    // Constructor and destructor
    RangeSearch(const fs::path entry);
    ~RangeSearch();
    std::vector<std::vector<int>> QueryPolyCount(const fs::path entry);
    void dfs(ANNkd_ptr node, ANNpointArray bounds, ANNpoint tree_low, ANNpoint tree_high);
    void add_inside_points(ANNkd_ptr node, Polygon* polygon);
    void dfs(ANNkd_ptr node, Polygon* polygon, ANNpoint tree_low, ANNpoint tree_high);   
};

#endif
