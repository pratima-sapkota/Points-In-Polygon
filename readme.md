# Points In Polygon

Returns all the points in a simple polygon using kD-tree.

## Assumptions
1. The polygon is a simple polygon with no intersecting sides or holes.
2. The test cases are put inside a folder in the `test` directory with filenames:
   - For points in 2D:
     `input_points.json`
   - For multiple query polygons in a list of list format where each list is a polygon:
     `input_query.json`
   - Correct set of points specified as the point number _optional_:
     `output_points.json`
   - Number of points inside each query polygon and has a length equal to the number of polygons  _optional_: 
       `output_counts.json`

## Build and Run

- `cd` to the directory and run `make`
- `rangesearch` executable will be created which can be run to get the desired output.

## Approach
1. The project uses David Mount's ANN Library to build KD-Tree. The KD-tree is traversed to find the number of points and report them. The runtime is `O(logn + k)`.
2. For precise orientation tests, Jonathan Shewchuk's "Adaptive Precision Floating-Point Arithmetic and Fast Robust Predicates for Computational Geometry" C-program is utilized.
3. The parameters used for the KD tree are: bucket size `5`, splitting rule `balanced KD tree`, and dimension `2`.

**Optimization:**
1. First, the KD-tree is built as a preprocessing step. Once built, it is stored for a series of queries with multiple polygons.
2. Each polygon is first bounded by a rectangle to speed-up the process of discarding larger portion of KD-tree faster. The Bounding Box test is simply used to know if the rectangle and KD-tree branch intersect or not. If not, the branch is pruned and if intersecting, the traversal continues. KD-tree is recursively traversed using DFS.
3. In the next step, the KD-tree is tested with the polygon. Each side of the polygon is tested with each branch of the KD-tree and checked if they intersect or not. If the branch is completely inside, the leaf nodes are traversed and points added, and if they intersect, the branch is traversed further. Outside polygon means, pruning that branch.

## Visualization 
 - `python3 plot.py` can be used to plot the graph.
 - plots are placed within the same test directory under the test instance subdirectory like `test00`.
 
 _DISCLAIMER_ If the input points have more than 1000 points, only the points inside the polygon are shown.
