#include "Orientation.h"
#include "Utilities.h"
using namespace std;

json parseJson(const std::string &filePath)
{
    // read from the file path
    std::ifstream f(filePath);
    if (!f)
    {
        std::cerr << "Error: Could not open file '" << filePath << "' for reading." << std::endl;
        throw std::runtime_error("Failed to open file");
    }
    json data;

    // parsing the data
    try
    {
        data = json::parse(f);
    }

    // failed parsing
    catch (const json::parse_error &e)
    {
        std::cerr << "Parse error: " << e.what() << std::endl;
        throw std::runtime_error("JSON file parse error");
    }

    // handling empty input
    if (data.empty())
    {
        std::cerr << "Error: No data in JSON file or JSON is malformed." << std::endl;
        throw std::runtime_error("No data in JSON file");
    }
    f.close();

    return data;
}

std::string intersection_poly(ANNpoint treelow, ANNpoint treehigh, Polygon* polygon)
{

    // Allocating the point for rectangle of the kd-tree.
    // Building rectangle based on lower left and upper right points.
    ANNpointArray rectangle = annAllocPts(4, 2);
    rectangle[0] = treelow;
    rectangle[1][0] = treehigh[0];
    rectangle[1][1] = treelow[1];
    rectangle[2] = treehigh;
    rectangle[3][0] = treelow[0];
    rectangle[3][1] = treehigh[1];

    // Looping each line of polygon to check if it intersects with any one line of rectangle.
    // line_1 to represent two points of rectangle's line.
    // line_2 to represent two points of polygon's line.
    ANNpointArray line_1 = annAllocPts(2, 2);
    ANNpointArray line_2 = annAllocPts(2, 2);
    line_2[0] = polygon->vertices[0];

    for(int i = 1; i <= polygon->polyCount; i++){
        line_1[0] = rectangle[0];
        line_2[1] = polygon->vertices[i % polygon->polyCount];

        for(int j = 1; j <= 4; j++){
            line_1[1] = rectangle[i % 4];

            // Bounding box test of two lines.
            // If any of these is false, then there is no chance of intersection.
            // Skipping the orientation test for speed.
            if( treelow[0] <= max(line_2[0][0], line_2[1][0]) &&
                treehigh[0] >= min(line_2[0][0], line_2[1][0]) &&
                treelow[1] <= max(line_2[0][1], line_2[1][1]) &&
                treehigh[1] >= min(line_2[0][1], line_2[1][1]))
                {
                    // Orientation of two points of rectangle's line compared to polygon's line.
                    REAL o_1_first = orient2d(&line_2[0][0], &line_2[1][0], &line_1[0][0]);
                    REAL o_1_second = orient2d(&line_2[0][0], &line_2[1][0], &line_1[1][0]);

                    // Orientation of two points of polygon's line compared to rectangle's line.
                    REAL o_2_first = orient2d(&line_1[0][0], &line_1[1][0], &line_2[0][0]);
                    REAL o_2_second = orient2d(&line_1[0][0], &line_1[1][0], &line_2[1][0]);

                    // Given the bounding box test being true, they must intersect, if this condition is true.
                    if (o_1_first == 0 || o_1_second == 0 || o_2_first == 0 || o_2_second == 0){
                        return "intersect";

                    }

                    bool o_1_first_pos = true;
                    bool o_1_second_pos = true;
                    bool o_2_first_pos = true;
                    bool o_2_second_pos = true;

                    if (o_1_first < 0)
                        bool o_1_first_pos = false;
                    if (o_1_second < 0)
                        bool o_1_second_pos = false;
                    if (o_2_first < 0)
                        bool o_2_first_pos = false;
                    if (o_2_second < 0)
                        bool o_2_second_pos = false;
                    
                    // XOR to see if both points of one line are opposite to another line
                    // If so, they must intersect.
                    if (o_1_first_pos ^ o_1_second_pos && o_2_first_pos ^ o_2_second_pos){
                        return "intersect";

                    }
                }
            line_1[0] = line_1[1];
        }
        line_2[0] = line_2[1];
    }

    // If it exited this loop, no rectangular side intersects with polygon side.
    // Checking if the point in polygon test holds or not to see if it is inside or outside.
    if(point_in_polygon(treelow, polygon))
        return "inside";
    
    return "outside";
}

void export_points(std::vector<std::vector<int>> points, const std::string &filePath)
{
    json j;
    j = json::array(); // JSON array for points
    // Serialize points
    for (auto &item : points)
    {
        json k = json::array();
        for (int i: item)
        {
            k.push_back(i);
        }
        j.push_back(k);
    }

    // Write the JSON object to the file
    std::ofstream file(filePath);
    file << std::setw(4) << j; // Pretty print with 4 spaces indentation
    file.close();
}

// orientation test approach to know if a point is inside or outside a polygon
bool point_in_polygon(ANNpoint point, Polygon* polygon)
{
    bool inside = false;
    //get bounding box for infinity
    double bounding_box_max_x =  polygon->bounds[1][0];
    ANNpoint inf = annAllocPt(2);
    inf[0] = bounding_box_max_x + 1.0;
    inf[1] = point[1];

    ANNpoint p1 = polygon->vertices[0], p2;

    // Loop through each edge in the polygon
    for (int i = 1; i <= polygon->polyCount; i++)
    {   
        // Get the next point in the polygon
        p2 = polygon->vertices[i % polygon->polyCount];

        double max_y = max(p1[1], p2[1]);
        double min_y = min(p1[1], p2[1]);
        double max_x = max(p1[0], p2[0]);

        if (point[1] >= min_y && point[1] <= max_y && point[0] <= max_x){

            double orientation_point = orient2d(&p1[0], &p2[0], &point[0]);

            if (orientation_point == 0){
                if (point[0] >= min(p1[0], p2[0]) && point[0] <= max(p1[0], p2[0])){
                    inside = true;
                    break;
                }
            }

            if (point[1] != min_y){
                double orientation_infinity = orient2d(&p1[0], &p2[0], &inf[0]);
                if ((orientation_point<0 && orientation_infinity>0 || orientation_point>0 && orientation_infinity<0))
                    inside = !inside;
            }
        }
        
        p1 = p2;
    }
    // annDeallocPt(inf);
    return inside;
}


std::string intersection_rect(ANNpoint tree_low, ANNpoint tree_high, ANNpointArray poly_bounds){
    //A is KNN Tree Branch. Reactangle denoted by tree_low (x1, y1) -> bottom left co-ordinate and tree_high (x2, y2) -> top right co-ordinate
    //B is polygon bounding rectangle denoted by poly_bounds[0] (x1, y1) -> bottom left co-ordinate and poly_bounds[1] (x2, y2) -> top right co-ordinate

    // Check if A is inside B
    if (tree_low[0] >= poly_bounds[0][0] && tree_high[0] <= poly_bounds[1][0] &&
        tree_low[1] >= poly_bounds[0][1] && tree_high[1] <= poly_bounds[1][1]) {
        return "inside"; // A is inside B
    }

    // Check if B is inside A
    if (tree_low[0] <= poly_bounds[0][0] && tree_high[0] >= poly_bounds[1][0] &&
        tree_low[1] <= poly_bounds[0][1] && tree_high[1] >= poly_bounds[1][1]) {
        return "intersect"; // B is inside A completely
    }

    // Check if A is outside B
    if (tree_high[0] < poly_bounds[0][0] || tree_low[0] > poly_bounds[1][0] ||
        tree_high[1] < poly_bounds[0][1] || tree_low[1] > poly_bounds[1][1]) {
        return "outside"; // A is outside B
    }


    // If not inside or outside, then it must be intersecting
    return "intersect"; // A is intersecting B
}
