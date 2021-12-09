#pragma once
#include <map>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

// Graph class for graph implementation
class Graph {
public:
    // Nested node class, represented as a vertex
    class Vertex {
        // Private variables
        std::string name;
        std::string speech;
        std::list<std::string> synonyms;
    public:
        // Constructor for when the part of speech isn't known
        Vertex(const std::string& name);

        // Constructor for when the part of speech is known
        // This is the case when we're adding a vertex as a result of an original word, and not as a synonym of a word
        Vertex(const std::string& name, const std::string& speech);

        // Function to add a string to the linked list of synonyms for each vertex
        void addSynonyms(const std::string syn);

        // Getter function for the name
        std::string getName();

        // Getter function for the part of speech
        std::string getSpeech();

        // Getter function for the vertex's synonyms
        std::list<std::string> getSynonyms(Graph& graph);
    };

    Graph();
    // Graph constructor
    // Takes in the unordered map derived from reading the data from the CSV file
    Graph(std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> result);

    // Helper function to find a particular vertex in a graph based on its name
    Vertex findVertex(const std::string& nameSearch);

    // Helper function to find a particular vertex during traversal
    Vertex findVertexTraversal(const std::string& nameSearch);

    // Getter function for returning the adjacency list
    std::vector<Vertex> getAdjList();


private:
    // The vector essentially stores all the vertices in the graph
    // It's equivalent to the array in an adjacency list
    std::vector<Vertex> adjList;

    // A set of all vertices
    std::unordered_map<std::string, Vertex> allVertices;
};
