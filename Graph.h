#pragma once
#include <map>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <unordered_map>

// Graph class for graph implementation
class Graph {
public:
    // Nested node class, represented as a vertex
    class Vertex {
        std::string name;
        std::string speech;
        std::list<Vertex> synonyms;
    public:
        // Constructor for when the part of speech isn't known
        Vertex(std::string name) {
            this->name = name;
        }

        // Constructor for when the part of speech is known
        // This is the case when we're adding a vertex as a result of an original word, and not as a synonym of a word
        Vertex(std::string name, std::string speech) {
            this->name = name;
            this->speech = speech;
        }

        // Function to add a vertex to the linked list of synonyms for each vertex
        void addSynonyms(Vertex syn) {
            synonyms.push_back(syn);
        }

        // Setter function for the part of speech
        void setSpeech(std::string speech) {
            this->speech = speech;
        }

        // Getter function for the name
        std::string getName() {
            return name;
        }

        // Getter function for the vertex's synonyms
        std::list<Vertex> getSynonyms()
        {
            return synonyms;
        }
    };


    // Graph constructor
    // Takes in the unordered map derived from reading the data from the CSV file
    Graph(std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> result) {
        // Iterating over each key-value pair in the unordered map
        for (auto i : result) {

            // If the key (word) has not been encountered before
            // If the key cannot be found in the set
            if (allWords.find(i.first) == allWords.end()) {

                // Create a new vertex and give it a name (i.first) and a part of speech (i.second.first)
                Vertex newVertex(i.first, i.second.first);
                // Insert the name into the set of words we've encountered
                allWords.insert(i.first);

                // Iterating over the vector of synonyms of the word we're dealing with
                for (auto j : i.second.second) {

                    // If the synonym has not been encountered before in the entire graph
                    // If the synonym cannot be found in the set
                    if (allWords.find(j) == allWords.end()) {

                        // Creates a new vertex and gives it just a name (j)
                        Vertex newNeighbor(j);
                        // Insert the name into the set of words we've encountered
                        allWords.insert(j);
                        // Add the newly created vertex in the linked list of synonyms of the original word we were dealing with
                        newVertex.addSynonyms(newNeighbor);
                        // Add the newly created vertex in the graph
                        adjList.push_back(newNeighbor);
                    }
                    else {
                        // Otherwise, if the word has already been encountered (and a vertex was thus created for the word)
                        // Simply add it into the linked list of synonyms of the original word we were dealing with
                        newVertex.addSynonyms(findVertex(j));
                    }
                }
                // Add the original word we were dealing with into our graph
                adjList.push_back(newVertex);
            }
            // If the key (word) has been encountered before
            // If the key can be found in the set
            else {
                // Find the vertex we've previously created for this word using its name (i.first)
                Vertex vertex = findVertex(i.first);
                // Now, give the vertex its part of speech (i.second.first)
                vertex.setSpeech(i.second.first);

                // Iterating over the vector of synonyms of the word we're dealing with
                for (auto j : i.second.second) {

                    // If the synonym has not been encountered before in the entire graph
                    // If the synonym cannot be found in the set
                    if (allWords.find(j) == allWords.end()) {

                        // Creates a new vertex and gives it just a name (j)
                        Vertex newNeighbor(j);
                        // Insert the name into the set of words we've encountered
                        allWords.insert(j);
                        // Add the newly created vertex in the linked list of synonyms of the original word we were dealing with
                        vertex.addSynonyms(newNeighbor);
                        // Add the newly created vertex in the graph
                        adjList.push_back(newNeighbor);
                    }

                    else {
                        // Otherwise, if the word has already been encountered (and a vertex was thus created for the word)
                        // Simply add it into the linked list of synonyms of the original word we were dealing with
                        vertex.addSynonyms(findVertex(j));
                    }
                }
            }
        }
    }

    // Helper function to find a particular vertex in a graph based on its name
    Vertex findVertex(std::string nameSearch) {
        // Default vertex if a particular vertex isn't found for whatever reason
        // Essentially, it creates a vertex and gives it just the name of the vertex we were seeking
        Vertex vertex(nameSearch);

        // Iterate through the graph
        for (auto i : adjList) {
            // If a vertex matches the name we're seeking
            // Return the vertex
            if (i.getName() == nameSearch) {
                return i;
            }
        }

        // Otherwise, return the default vertex we created
        return vertex;
    }

    // Getter function for returning the adjacency list
    std::vector<Vertex> getAdjList() {
        return adjList;
    }

    // Getter function for returning the set of words we've encountered
    std::set<std::string> getWordSet() {
        return allWords;
    }

private:
    // The vector essentially stores all the vertices in the graph
    // It's equivalent to the array in an adjacency list
    std::vector<Vertex> adjList;
    // The set keeps track of all the words we've encountered
    std::set<std::string> allWords;

};