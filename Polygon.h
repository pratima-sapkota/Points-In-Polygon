// RangeSearch.h
#ifndef POLYGON_H
#define POLYGON_H

#include "nlohmann/json.hpp"
#include <ANN/ANN.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <string>

using json = nlohmann::json;

namespace fs = std::filesystem;


class Polygon {

public:
    ANNpointArray vertices;
    ANNpointArray bounds;
    int polyCount;
    std::vector<int> inside_poly;

// Constructor and destructor
    Polygon(json& item);
    ~Polygon();
};

#endif
