#include "RangeSearch.h"
#include <iomanip>
#include <vector>
#include <string>
#include "Polygon.h"

RangeSearch::RangeSearch(const fs::path dirPath)
{
    fs::path filePath = dirPath / "input_points.json";
    json data = parseJson(filePath);
    pointCount = data.size();
    points = annAllocPts(pointCount, 2);
    for (int i = 0; i < pointCount; i++)
    {
        points[i][0] = data[i][0].get<double>();
        points[i][1] = data[i][1].get<double>();
    }
    kdTree = new CustomANNKd_tree(points, pointCount, 2);
    inside_points = {};

    // const std::string ofile = "kdtree.txt";
    // std::ofstream file(ofile);  // Create an ofstream object, not a reference

    // std::cout << "--------------------------------------" << std::endl;
    // kdTree->Dump(ANNfalse, file);
}

RangeSearch::~RangeSearch()
{
    skip_nodes.clear();
    if (kdTree)
    {
        delete kdTree; // Correct if kdTree is of type ANNkd_tree*
        kdTree = nullptr;
    }
    if (points != nullptr)
    {
        annDeallocPts(points);
        points = nullptr; // Set to nullptr to prevent further use
    }
}

ANNpointArray get_tree_bounds(ANNpoint tree_low, ANNpoint tree_high, int cut_dim, ANNcoord cut_val)
{
    ANNpointArray new_bounds = annAllocPts(4, 2);
    if (cut_dim == 0)
    { // cut in y-axis
        new_bounds[0] = tree_low;
        new_bounds[1][0] = cut_val;
        new_bounds[1][1] = tree_high[1];
        new_bounds[2][0] = cut_val;
        new_bounds[2][1] = tree_low[1];
        new_bounds[3] = tree_high;
    }
    else
    { // cut in x-axis
        new_bounds[0] = tree_low;
        new_bounds[1][0] = tree_high[0];
        new_bounds[1][1] = cut_val;
        new_bounds[2][0] = tree_low[0];
        new_bounds[2][1] = cut_val;
        new_bounds[3] = tree_high;
    }
    return new_bounds;
}

// Function to perform DFS traversal on KD-tree on bounds
void RangeSearch::dfs(ANNkd_ptr node, ANNpointArray bounds, ANNpoint tree_low, ANNpoint tree_high)
{
    if (node == nullptr)
        return; // Base case: if node is null, return

    ANNkd_split *splitNode = dynamic_cast<ANNkd_split *>(node);
    // Check if the node is a leaf
    if (splitNode)
    {
        // The node is a split node
        ANNkd_split *split = static_cast<ANNkd_split *>(node);
        std::string status = intersection_rect(tree_low, tree_high, bounds);
        ANNpointArray tree_bounds = get_tree_bounds(tree_low, tree_high, split->cut_dim, split->cut_val);

        if (status == "inside")
        {
            return;
        }
        if (status == "outside")
        {
            // delete node;
            // split->child[ANN_LO] = nullptr;
            // split->child[ANN_HI] = nullptr;
            // node = nullptr;
            skip_nodes.insert(node);
            return;
        }
        if (status == "intersect")
        {
            // Recursively visit the low child
            dfs(split->child[0], bounds, tree_bounds[0], tree_bounds[1]);

            // Recursively visit the high child
            dfs(split->child[1], bounds, tree_bounds[2], tree_bounds[3]);

            return;
        }
    }
    return;
}

void RangeSearch::add_inside_points(ANNkd_ptr node, Polygon* polygon)
{
    if (node == nullptr)
        return; // Base case: if node is null, return

    ANNkd_split *splitNode = dynamic_cast<ANNkd_split *>(node);
    // Check if the node is a leaf
    if (!splitNode)
    {
        ANNkd_leaf *leaf = static_cast<ANNkd_leaf *>(node);
        for (int i = 0; i < leaf->n_pts; i++)
        {
            // std::cout << "Leaf node, point index " << leaf->bkt[i] << std::endl;  // check for trivial and non trivial nodes
            ANNpoint pnt = points[leaf->bkt[i]];
            polygon->inside_poly.emplace_back(leaf->bkt[i]);
        }
        return;
    }
    else
    {
        // The node is a split node
        ANNkd_split *split = static_cast<ANNkd_split *>(node);
        // Recursively visit the low child
        add_inside_points(split->child[0], polygon);

        // Recursively visit the high child
        add_inside_points(split->child[1], polygon);
    }
    return;
}

// Function to perform DFS traversal on KD-tree with polygon and return inside points
void RangeSearch::dfs(ANNkd_ptr node, Polygon* polygon, ANNpoint tree_low, ANNpoint tree_high)
{
    if (node == nullptr || skip_nodes.find(node) != skip_nodes.end())
        return; // Base case: if node is null, return


    ANNkd_split *splitNode = dynamic_cast<ANNkd_split *>(node);
    // Check if the node is a leaf
    if (!splitNode)
    {
        ANNkd_leaf *leaf = static_cast<ANNkd_leaf *>(node);
        for (int i = 0; i < leaf->n_pts; i++)
        {
            // std::cout << "Leaf node, point index " << leaf->bkt[i] << std::endl;  // check for trivial and non trivial nodes
            ANNpoint pnt = points[leaf->bkt[i]];
            if (point_in_polygon(pnt, polygon))
            {
                polygon->inside_poly.emplace_back(leaf->bkt[i]);
            }
        }
        return;
    }
    else
    {
        // The node is a split node
        ANNkd_split *split = static_cast<ANNkd_split *>(node);
        std::string status = intersection_poly(tree_low, tree_high, polygon);

        ANNpointArray tree_bounds = get_tree_bounds(tree_low, tree_high, split->cut_dim, split->cut_val);

        // traverse the kd tree and calculate the points intersection
        if (status == "inside")
        {
            // Recursively visit the low child
            add_inside_points(split, polygon);
        }

        if (status == "intersect")
        {
            // Recursively visit the low child
            dfs(split->child[0], polygon, tree_bounds[0], tree_bounds[1]);

            // Recursively visit the high child
            dfs(split->child[1], polygon, tree_bounds[2], tree_bounds[3]);

            // ANNorthRect bbox = ANNorthRect(2, tree_low, tree_high);
            // ANNkdStats kdStat = ANNkdStats();
            // split->getStats(2, kdStat, bbox);
            // int n = kdStat.n_lf - kdStat.n_tl;
            // std::cout << kdStat.n_pts << " points inside " << n << std::endl;
        }
        if (status == "outside")
        {
            // split->child[0] = nullptr;
            // split->child[1] = nullptr;
            // node = nullptr;
            return;
        }
    }
    return;
}

std::vector<std::vector<int>> RangeSearch::QueryPolyCount(const fs::path dirPath)
{
    fs::path filePath = dirPath / "input_query.json";
    json data = parseJson(filePath);

    // std::vector<Polygon*> polygons;
    for (auto &item : data)
    {
        Polygon* polygon = new Polygon(item);
        skip_nodes.clear();
        ANNpoint tree_low = kdTree->getBoundingBoxLow();
        ANNpoint tree_high = kdTree->getBoundingBoxHigh();

        // dfs for rectangle-rectangle intersection and pruning the tree to minimize search area
        dfs(kdTree->getRoot(), polygon->bounds, tree_low, tree_high);

        // dfs for rectangle-polygon intersection to get the points inside polygon
        dfs(kdTree->getRoot(), polygon, tree_low, tree_high);
        inside_points.push_back(polygon->inside_poly);

        // std::cout << "End" << std::endl;
        polygon = nullptr;
    }

    return inside_points;
}
