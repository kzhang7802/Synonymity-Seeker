#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <sstream>
#include "Traversals.h"
typedef std::chrono::high_resolution_clock Clock;

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
        result.insert({lemma, returnPair});

        /// =========== TESTING FOR SMALL VALUES ============
        /// NOTE: REMOVE AFTER SMALL SCALE TESTING
        if (result.size() == 500)
            break;
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

    // Determine time for traversing the graph using BFS
    auto t1 = Clock::now();
    std::vector<Graph::Vertex> bfsVect = bfs(obj, "absentminded", 2);
    auto t2 = Clock::now();

    // Note: BFS Time Complextiy: O(|V| + |E|)
    auto time = duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    std::cout << "BFS Time : " << time << " nanoseconds" << std::endl;

    // Determine time for traversing the graph using DFS
    t1 = Clock::now();
    std::vector<Graph::Vertex> dfsVect = dfs(obj, "absentminded", 2);
    t2 = Clock::now();

    // Note: DFS Time complexity: O(|V| + |E|)
    time = duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    std::cout << "DFS Time : " << time << " nanoseconds" << std::endl;

    // Although the same time complexity, DFS is faster than BFS
    // This is supported from the calculated duration of time above
}

