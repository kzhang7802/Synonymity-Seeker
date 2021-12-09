#include "Graph.h"

Graph::Graph() {}

Graph::Vertex::Vertex(const std::string& name) {
    this->name = name;
}

Graph::Vertex::Vertex(const std::string& name, const std::string& speech) {
    this->name = name;
    this->speech = speech;
}

void Graph::Vertex::addSynonyms(const std::string syn) {
    synonyms.push_back(syn);
}

void Graph::Vertex::setSpeech(const std::string &speech) {
    this->speech = speech;
}

std::string Graph::Vertex::getSpeech() {
    return speech;
}

std::string Graph::Vertex::getName() {
    return name;
}

// Searches through the adjacency list and returns the synonyms of a vertex
std::list<std::string> Graph::Vertex::getSynonyms(Graph graph) {
    std::list<std::string> returnList;
    for (auto i : graph.adjList) {
        if (i.getName() == this->getName()) {
            return i.synonyms;
        }
    }
    return returnList;
}

Graph::Graph(std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> result) {
    // Iterating over each key-value pair in the unordered map
    for (auto i : result) {

        // Create a new vertex and give it a name (i.first) and a part of speech (i.second.first)
        Vertex newVertex(i.first, i.second.first);

        // Insert the vertex into the unordered map
        allVertices.insert({ i.first, newVertex });

        // Iterating over the vector of synonyms of the word we're dealing with
        for (const auto& j : i.second.second) {

            // Add the string value of the synonym
            newVertex.addSynonyms(j);
        }

        // Add the original word we were dealing with into our graph
        adjList.push_back(newVertex);
    }

}

std::vector<Graph::Vertex> Graph::getAdjList() {
    return adjList;
}

// Similar helper function to find a particular vertex in a graph based on its name
Graph::Vertex Graph::findVertex(const std::string& nameSearch) {
    for (auto i : adjList) {
        if (i.getName() == nameSearch) {
            return i;
        }
    }
    return allVertices.find(nameSearch)->second;
}

// Helper function to find a particular vertex in a graph based on its name
Graph::Vertex Graph::findVertexTraversal(const std::string& nameSearch) {
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

