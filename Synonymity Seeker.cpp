#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <sstream>
#include "Traversals.h"
#include "Graph.h"
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

// Prototypes for rating algorithm functions
std::vector<std::pair<std::string, double>> bfsRatingAlgorithm(Graph graph, std::vector<std::pair<Graph::Vertex, int>> bfsVect, std::string originalWord);
std::vector<std::pair<std::string, double>> dfsRatingAlgorithm(Graph graph, std::vector<std::pair<Graph::Vertex, int>> dfsVect, std::string originalWord);

std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> readCSV(std::string filename) {

    // Create an unordered map that stores strings as keys, and the value as a pair of an additional string and a vector
    // The key string is the original word, and the additional string is the part of speech while the vector stores the synonyms of the particular word
    std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> result;
    // std::vector<std::vector<std::string>> result;

    // Create an input filestream
    std::ifstream myFile(filename);

    // Make sure the file is open
    if (!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Helper variables
    std::string line, colname, lemma, partOfSpeech, allSynonyms;

    // Read the column names
    if (myFile.good())
    {
        // Extract the first line in the file
        std::getline(myFile, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each column name
        while (std::getline(ss, colname, ',')) {
            // Effectively disregard the column names "lemma," "part_of_speech," and "synonyms"
        }
    }

    // Read data, line by line
    while (std::getline(myFile, line))
    {
        // Create a stringstream of the current line
        std::stringstream ss(line);
        // Use a buffer and a while loop to separate the string stream by commas instead of spaces
        std::string buffer;

        // Use a counter to determine the order of the strings produced by the stream
        int counter = 0;
        while (std::getline(ss, buffer, ',')) {
            // The first string of the current line should be the lemma
            if (counter == 0) {
                lemma = buffer;
            }
            // The second string of the current line should be the part of speech
            else if (counter == 1) {
                partOfSpeech = buffer;
            }
            // The third string of the current line should be all the synonyms separated by semicolons
            else {
                allSynonyms = buffer;
            }
            counter++;
        }

        // Create a vector to store the vector of synonyms
        std::vector<std::string> synonymVector;

        // Split the third string by the delimiter of semicolons
        std::string buffer2;
        std::stringstream ss2(allSynonyms);

        while (std::getline(ss2, buffer2, ';')) {
            // Check if the word has an appended "\r" return key at the end
            // If so, delete the appended return key
            auto possibleReturnKey = buffer2.find("\r");
            if (possibleReturnKey != std::string::npos) {
                buffer2.erase(possibleReturnKey, 2);
            }

            // Push the synonym into the vector
            synonymVector.push_back(buffer2);
        }

        // Create a pair and store the part of speech and the vector of synonyms
        std::pair<std::string, std::vector<std::string>> returnPair;
        returnPair.first = partOfSpeech;
        returnPair.second = synonymVector;

        // Insert the key value pair into the map
        result.insert({ lemma, returnPair });

        /// =========== TESTING FOR SMALL VALUES ============
        /// NOTE: REMOVE AFTER SMALL SCALE TESTING
        //if (result.size() == 500)
        //   break;
    }

    // Close file
    myFile.close();

    return result;
}


int main()
{
    std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> result = readCSV("synonyms.csv");

    // Prints everything in results vector

//    for (auto x : result) {
//        for (auto i = x.second.second.begin(); i != x.second.second.end(); i++) {
//            std::cout << *i << " ";
//        }
//        std::cout << std::endl;
//    }


    // Stores elements of result vector as specified in the graph class


    Graph obj(result);

    /*
    std::vector<std::pair<Graph::Vertex, int>> dfsvect = dfs(obj, "ablaze", 25);

    std::vector<std::pair<std::string, double>> rating = dfsRatingAlgorithm(obj, dfsvect, "ablaze");

    for (auto i : rating) {
        std::cout << i.first << " " << i.second << std::endl;
    }*/
}

// Helper function to allow for sorting of pairs in descending order
// Compares the pairs' second values, which would be the ratings
bool sortByRating(const std::pair<std::string, double>& a, const std::pair <std::string, double>& b) {
    return (a.second > b.second);
}

// Rating algorithm for BFS traversals
std::vector<std::pair<std::string, double>> bfsRatingAlgorithm(Graph graph, std::vector<std::pair<Graph::Vertex, int>> bfsVect, std::string originalWord) {
    // Create a vector to return the strings and ratings of the results
    std::vector<std::pair<std::string, double>> returnVect;
    // Declare variable for rating
    double rating;
    // Find the part of speech of the original word
    std::string originalPartOfSpeech = graph.findVertex(originalWord).getSpeech();

    // Iterate through the vectors derived from the DFS traversal
    for (int i = 0; i < bfsVect.size(); i++) {
        // Begin with the rating at 10
        rating = 10.0;
        // Find the part of speech of this particular vertex
        std::string partOfSpeech = graph.findVertex(bfsVect[i].first.getName()).getSpeech();
        // If the part of speech doesn't match, remove 2 rating points
        if (partOfSpeech != originalPartOfSpeech) {
            rating -= 2.0;
        }
        // For every edge greater than the first edge, remove 2 rating points (no negatives)
        for (int j = 1; j < bfsVect[i].second; j++) {
            if (rating >= 2.0) {
                rating -= 2.0;
            }
        }

        // Push the pair into the vector to be returned
        returnVect.push_back(std::make_pair(bfsVect[i].first.getName(), rating));
    }

    // Call the sorting function using the helper sorting method
    std::sort(returnVect.begin(), returnVect.end(), sortByRating);

    // Return the vector
    return returnVect;
}

// Rating algorithm for DFS traversals
std::vector<std::pair<std::string, double>> dfsRatingAlgorithm(Graph graph, std::vector<std::pair<Graph::Vertex, int>> dfsVect, std::string originalWord) {
    // Create a vector to return the strings and ratings of the results
    std::vector<std::pair<std::string, double>> returnVect;
    // Declare variable for rating
    double rating;
    // Find the part of speech of the original word
    std::string originalPartOfSpeech = graph.findVertex(originalWord).getSpeech();

    // Iterate through the vectors derived from the DFS traversal
    for (int i = 0; i < dfsVect.size(); i++) {
        // Begin with the rating at 10
        rating = 10.0;
        // Find the part of speech of this particular vertex
        std::string partOfSpeech = graph.findVertex(dfsVect[i].first.getName()).getSpeech();
        // If the part of speech doesn't match, remove 1 rating point
        if (partOfSpeech != originalPartOfSpeech) {
            rating -= 1;
        }
        // For every edge greater than the first edge, remove 1.5 rating points (no negatives)
        for (int j = 1; j < dfsVect[i].second; j++) {
            if (rating >= 1.5) {
                rating -= 1.5;
            }
        }

        // Push the pair into the vector to be returned
        returnVect.push_back(std::make_pair(dfsVect[i].first.getName(), rating));
    }

    // Call the sorting function using the helper sorting method
    std::sort(returnVect.begin(), returnVect.end(), sortByRating);

    // Return the vector
    return returnVect;
}

