# Points In Polygon

Returns all the points in a simple polygon using kD-tree.

**Assumptions:**
1. The polygon is a simple polygon with no intersecting sides or holes.
2. The test cases are put inside a folder in the `test` directory with filenames:

        - input_points.json -> for points in 2D
        
        - input_query.json -> for multiple query polygons in a list of list format where each list is a polygon

        - output_points.json -> _optional_ correct set of points specified as the point number

        - output_counts.json -> _optioanl_ number of points inside each query polygon and has a length equal to number of polygons

**Build and Run:**

- cd to the directory and `make` build the objects.
- `rangesearch` executable will be created which can be run to get the desired output.

**More about the code base and approach:**
1. The project uses David Mount's ANN Library to build KD-Tree. The KD-tree is traversed to find the number of points and report them. The runtime is O(logn + k).
2. For precise orientation tests, Jonathan Shewchuk's "Adaptive Precision Floating-Point Arithmetic and Fast Robust Predicates for Computational Geometry" C-program is utilized.
3. The parameters used for KD tree are: bucket size 5, splitting rule (balanced KD tree) and dimension 2.


**Optimization:**
1. First, the KD-tree is built as a preprocessing step. Once built, it is stored for series of queries with multiple polygons.
2. Each polygon is first bounded by a rectangle to speed-up the process of discarding larger portion of KD-tree faster. Bounding Box test is simply used to know if the rectangle and KD-tree branch intersect or not. If not, the branch is pruned and if intersecting, the traversal continues. KD tree is recursively traversed using DFS.
3. In the next step, KD tree is tested with the polygon. Each side of polygon is tested with each branch of KD-tree and checked if they intersect or not. If the branch is completely inside, the leaf nodes are traversed and points added, and if they intersect, the branch is traversed further. Outside polygon means, pruning that branch.

**Plotting is supported via python.** 
 - `python3 plot.py` can be used to plot the graph.
 - plots are placed within the same test directory under the test instance subdirectory like `test00`.
 
 _DISCLAIMER_ If the input points have more than 1000 points, only the points inside the polygon are shown.
