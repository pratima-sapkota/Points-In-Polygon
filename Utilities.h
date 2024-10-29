// RangeSearch.h
#ifndef UTILITIES_H
#define UTILITIES_H

#include "nlohmann/json.hpp"
#include <ANN/ANN.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include "Polygon.h"

using json = nlohmann::json;

json parseJson(const std::string &filePath);
std::string intersection_poly(ANNpoint treelow, ANNpoint treehigh, Polygon* polygon);
void export_points(std::vector<std::vector<int>> points, const std::string &filePath);
std::string intersection_rect(ANNpoint tree_low, ANNpoint tree_high, ANNpointArray poly_bounds);
bool point_in_polygon(ANNpoint point, Polygon* polygon);

#endif