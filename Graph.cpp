#include "Graph.h"

Graph::Vertex::Vertex(const std::string&  name) {
    this->name = name;
}

Graph::Vertex::Vertex(const std::string&  name, const std::string&  speech) {
    this->name = name;
    this->speech = speech;
}

void Graph::Vertex::addSynonyms(const Graph::Vertex& syn) {
    synonyms.push_back(syn);
}

void Graph::Vertex::setSpeech(const std::string&) {
    this->speech = speech;
}

std::string Graph::Vertex::getName() {
    return name;
}

std::list<Graph::Vertex> Graph::Vertex::getSynonyms() {
    return synonyms;
}

Graph::Graph(std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> result) {
    // Iterating over each key-value pair in the unordered map
    for (auto i : result) {

        // If the key (word) has not been encountered before
        // If the key cannot be found in the set
        if (allWords.find(i.first) == allWords.end()) {

            // Create a new vertex and give it a name (i.first) and a part of speech (i.second.first)
            Vertex vertex = newVertex(i.first, i.second.first);

            // Iterating over the vector of synonyms of the word we're dealing with
            for (const auto& j : i.second.second) {

                // If the synonym has not been encountered before in the entire graph
                // If the synonym cannot be found in the set
                if (allWords.find(j) == allWords.end()) {

                    // Creates a new vertex and gives it just a name (j)
                    Vertex neighbor = newNeighbor(j, vertex);
                    // Add the newly created vertex in the graph
                    adjList.push_back(neighbor);
                }
                else {
                    // Otherwise, if the word has already been encountered (and a vertex was thus created for the word)
                    // Simply add it into the linked list of synonyms of the original word we were dealing with
                    vertex.addSynonyms(findVertex(j));
                }
            }

            // Add the original word we were dealing with into our graph
            adjList.push_back(vertex);
        }
            // If the key (word) has been encountered before
            // If the key can be found in the set
        else {
            // Find the vertex we've previously created for this word using its name (i.first)
            Vertex vertex = findVertex(i.first);
            // Now, give the vertex its part of speech (i.second.first)
            vertex.setSpeech(i.second.first);

            // Iterating over the vector of synonyms of the word we're dealing with
            for (const auto& j : i.second.second) {

                // If the synonym has not been encountered before in the entire graph
                // If the synonym cannot be found in the set
                if (allWords.find(j) == allWords.end()) {

                    // Creates a new vertex and gives it just a name (j)
                    Vertex neighbor = newNeighbor(j, vertex);
                    // Add the newly created vertex in the graph
                    adjList.push_back(neighbor);
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

std::vector<Graph::Vertex> Graph::getAdjList() {
    return adjList;
}

std::unordered_set<std::string> Graph::getWordSet() {
    return allWords;
}

Graph::Vertex Graph::findVertex(const std::string& nameSearch) {
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

Graph::Vertex Graph::newVertex(const std::string& name, const std::string& syn) {
    // Create a new vertex and give it a name and a part of speech
    Vertex vertex(name, syn);
    // Insert the name into the unordered_set of words we've encountered
    allWords.insert(name);
    // Insert the vertex into the unordered_map of vertices we've encountered
    allVertices.insert({name, vertex});

    return vertex;
}

Graph::Vertex Graph::newNeighbor(const std::string& name, Vertex& vertex) {
    // Creates a new vertex and gives it just a name
    Vertex newVertex(name);
    // Insert the name into the unordered_set of words we've encountered
    allWords.insert(name);
    // Insert the vertex into the unordered_map of vertices we've encountered
    allVertices.insert({name, newVertex});

    // Add the newly created vertex in the linked list of synonyms of the original word we were dealing with
    vertex.addSynonyms(newVertex);

    return newVertex;
}