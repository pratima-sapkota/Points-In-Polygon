import os
import json
import matplotlib.pyplot as plt

def plot_points_and_polygon(directory):
    print("Plotting for :", directory)

    # Paths to the files
    query_polygon_path = os.path.join(directory, 'input_query.json')
    output_points_path = os.path.join(directory, 'my_output_points.json')
    input_points_path = os.path.join(directory, 'input_points.json')


    # Read JSON data from files
    with open(query_polygon_path, 'r') as file:
        query_polygon = json.load(file)
    with open(output_points_path, 'r') as file:
        output_points = json.load(file)
    with open(input_points_path, 'r') as file:
        input_points = json.load(file)
    


    # Plot the polygon
    # Add the first point at the end to close the polygon
    for i, polygon in enumerate(query_polygon):
        
        plt.figure(figsize=(15, 15))
        
        if(len(input_points) <= 1000):
            for point in input_points:
                plt.scatter(*point, color='red')
        

        print(len(output_points[i]))
        for point in output_points[i]:
            plt.scatter(*input_points[point], color='green')

        polygon_for_plotting = polygon + [polygon[0]]
        x_poly, y_poly = zip(*polygon_for_plotting)
        plt.plot(x_poly, y_poly, 'k-', label='Query Polygon')  # 'k-' means black line

        plt.legend()
        plt.title(f"Plot for the directory: {directory}")
        plt.savefig(os.path.join(directory, 'plot_polygon_' + str(i) + '.png'))


# Main execution block to loop through each subdirectory in 'test_cases'
root_dir = 'test'
for subdir in os.listdir(root_dir):
    dir_path = os.path.join(root_dir, subdir)
    if os.path.isdir(dir_path):
        plot_points_and_polygon(dir_path)
