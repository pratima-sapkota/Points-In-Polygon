#include "Polygon.h"

Polygon::Polygon(json& item)
{
    polyCount = item.size();
    vertices = annAllocPts(polyCount, 2);
    if (vertices == nullptr)
    {
        std::cerr << "Error: Memory allocation failed for polygon." << std::endl;
        throw std::bad_alloc();
    }

    double max_x = -INFINITY;
    double max_y = -INFINITY;

    double min_x = INFINITY;
    double min_y = INFINITY;

    for (int i = 0; i < polyCount; i++)
    {
        vertices[i][0] = item[i][0].get<double>();
        vertices[i][1] = item[i][1].get<double>();

        max_x = std::max(vertices[i][0], max_x);
        max_y = std::max(vertices[i][1], max_y);

        min_x = std::min(vertices[i][0], min_x);
        min_y = std::min(vertices[i][1], min_y);
    }

    bounds = annAllocPts(2, 2);
    bounds[0][0] = min_x;
    bounds[0][1] = min_y;
    bounds[1][0] = max_x;
    bounds[1][1] = max_y;
}

Polygon::~Polygon(){
    if (vertices != nullptr)
    {
        annDeallocPts(vertices);
        vertices = nullptr; // Set to nullptr to prevent further use
    }
    if (bounds != nullptr)
    {
        annDeallocPts(bounds);
        bounds = nullptr; // Set to nullptr to prevent further use
    }
}