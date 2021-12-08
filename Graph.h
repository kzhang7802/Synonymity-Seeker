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
        std::string name;
        std::string speech;
        std::list<Vertex> synonyms;
    public:
        // Constructor for when the part of speech isn't known
        Vertex(const std::string& name);

        // Constructor for when the part of speech is known
        // This is the case when we're adding a vertex as a result of an original word, and not as a synonym of a word
        Vertex(const std::string& name, const std::string& speech);

        // Function to add a vertex to the linked list of synonyms for each vertex
        void addSynonyms(const Vertex& syn);

        // Setter function for the part of speech
        void setSpeech(const std::string& speech);

        // Getter function for the name
        std::string getName();

        // Getter function for the vertex's synonyms
        std::list<Vertex> getSynonyms();
    };


    // Graph constructor
    // Takes in the unordered map derived from reading the data from the CSV file
    Graph(std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> result);

    // Helper function to find a particular vertex in a graph based on its name
    Vertex findVertex(const std::string& nameSearch);

    // Helper function to find a particular vertex during traversal
    Vertex findVertexTraversal(const std::string& nameSearch);

    // Getter function for returning the adjacency list
    std::vector<Vertex> getAdjList();

    // Getter function for returning the set of words we've encountered
    std::unordered_set<std::string> getWordSet();

private:
    // The vector essentially stores all the vertices in the graph
    // It's equivalent to the array in an adjacency list
    std::vector<Vertex> adjList;

    // The set keeps track of all the words we've encountered
    std::unordered_set<std::string> allWords;

    // A set of all vertices
    std::unordered_map<std::string, Vertex> allVertices;

    // Creates new vertex (including name and synonym)
    Vertex newVertex(const std::string& name, const std::string& syn);

    // Creates new neighbor (including name and linking)
    Vertex newNeighbor(const std::string& name, Vertex& vertex);
};