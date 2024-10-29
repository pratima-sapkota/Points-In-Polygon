#include "RangeSearch.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>

namespace fs = std::filesystem;

int main()
{
    fs::path path = "test";
    if (fs::exists(path) && fs::is_directory(path))
    {
        for (const auto &entry : fs::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                std::cout << "Importing points from " << entry.path() << std::endl;

                // Start preprocessing
                std::cout << "  Pre-Processing Points..." << std::endl;
                auto startPreprocessing = std::chrono::high_resolution_clock::now();
                RangeSearch* rsearch = new RangeSearch(entry.path());

                auto endPreprocessing = std::chrono::high_resolution_clock::now();
                auto preprocessingTime = std::chrono::duration_cast<std::chrono::milliseconds>(endPreprocessing - startPreprocessing).count();
                // End preprocessing

                // Start querying
                
                std::cout << "  Querying Polygon Points..." << std::endl;
                auto startQuery = std::chrono::high_resolution_clock::now();

                rsearch->QueryPolyCount(entry.path());
                // std::cout << "  Points inside the polygon: " << count << std::endl;

                auto endQuery = std::chrono::high_resolution_clock::now();
                auto queryTime = std::chrono::duration_cast<std::chrono::milliseconds>(endQuery - startQuery).count();
                // End querying

                // Start testing

                auto startTest = std::chrono::high_resolution_clock::now();

                fs::path exportPath = entry.path() / "my_output_points.json";
                export_points(rsearch->inside_points, exportPath.string());

                std::cout << "  Points within polygon range exported at " << exportPath << "." << std::endl;

                // testing counts
                 fs::path filePath = entry.path() / "output_count.json";
                 std::string count_accuracy = "Unavailable";
                 if(fs::exists(filePath)){
                    int count_acc = 0;
                    json data = parseJson(filePath);
                    int num_of_polygons = data.size();
                    for (int i = 0; i < num_of_polygons; i++)
                    {
                        if (data[i].get<int>() == rsearch->inside_points[i].size())
                            count_acc += 1;
                            
                    }
                    count_accuracy = to_string(count_acc) + " Counts Passed off of " + to_string(num_of_polygons);
                 }


                // Output results - timings and placeholder accuracy
                std::cout << "  Count Accuracy: " << count_accuracy << std::endl;
                
                // testing points
                 filePath = entry.path() / "output_report.json";
                 if(fs::exists(filePath)){
                    std::cout << "  Range Accuracy: ";
                    json data = parseJson(filePath);
                    int num_of_polygons = data.size();
                    int failed = 0;
                    for (int i = 0; i < num_of_polygons; i++)
                    {
                        int report_acc = 0;
                        std::unordered_set<int> output(rsearch->inside_points[i].begin(), rsearch->inside_points[i].end());
                        for (int j = 0; j<data[i].size(); j++)
                        {
                            if (output.find(data[i][j].get<int>()) == output.end())
                                report_acc += 1;
                        }

                        if (report_acc > 0){
                            failed += 1;
                            std::cout  << std::endl << "  " << to_string(report_acc) << " Points failed in Polygon " << to_string(i) << std::endl;
                        }
                            
                    }
                    if(failed > 0)
                        std:cout<< "  ";
                    std::cout << to_string(num_of_polygons - failed) + " Counts Passed off of " + to_string(num_of_polygons) << std::endl;
                 }
                 else
                    std::cout<< "  Range Accuracy: Unavailable" << endl;



                auto endTest = std::chrono::high_resolution_clock::now();
                auto testTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTest - startTest).count();

                
                std::cout << "  Preprocessing Time: " << preprocessingTime << " milliseconds" << std::endl;
                std::cout << "  Query Time: " << queryTime << " milliseconds" << std::endl;
                std::cout << "  Total Time: " << (preprocessingTime + queryTime ) << " milliseconds" << std::endl;
                std::cout << "  Testing Time: " << testTime << " milliseconds" << std::endl;

                delete rsearch;
	            annClose();	

                std::cout << std::endl << "*********************************************************************************" << std::endl << std::endl;
            }
     
        }
    }
    else
        std::cout << "The path does not exist or is not a directory." << std::endl;

    return 0;
}
