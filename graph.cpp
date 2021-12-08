#include "graph.h"

Graph::Vertex::Vertex(std::string name) {
    this->name = name;
}

Graph::Vertex::Vertex(std::string name, std::string speech) {
    this->name = name;
    this->speech = speech;
}

void Graph::Vertex::addSynonyms(Graph::Vertex syn) {
    synonyms.push_back(syn);
}

void Graph::Vertex::setSpeech(std::string speech) {
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
            Vertex newVertex(i.first, i.second.first);
            // Insert the name into the set of words we've encountered
            allWords.insert(i.first);
            allVertices.insert({i.first, newVertex});

            // Iterating over the vector of synonyms of the word we're dealing with
            for (auto j : i.second.second) {

                // If the synonym has not been encountered before in the entire graph
                // If the synonym cannot be found in the set
                if (allWords.find(j) == allWords.end()) {

                    // Creates a new vertex and gives it just a name (j)
                    Vertex newNeighbor(j);
                    // Insert the name into the set of words we've encountered
                    allWords.insert(j);
                    allVertices.insert({j, newNeighbor});
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
                    allVertices.insert({j, newNeighbor});
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

std::vector<Graph::Vertex> Graph::getAdjList() {
    return adjList;
}

std::unordered_set<std::string> Graph::getWordSet() {
    return allWords;
}

Graph::Vertex Graph::findVertex(std::string nameSearch) {
    return allVertices.find(nameSearch)->second;
}