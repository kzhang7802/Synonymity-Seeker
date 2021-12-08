#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <queue>
#include <stack>
#include "Graph.h"

// Returns a vector of pairs including Vertex objects and their respective distances from the source vertex (as integers), all synonymous to the user inputted word using BFS
std::vector<std::pair<Graph::Vertex, int>> bfs(Graph graph, const std::string &word, const int size) {
    std::vector<std::pair<Graph::Vertex, int>> result;
    std::queue<Graph::Vertex> vertexQ;
    std::unordered_set<std::string> visited;

    // Find the vertex of the user inputted word
    Graph::Vertex current = graph.findVertex(word);

    // Place found vertex in a queue and mark it visited
    vertexQ.push(current);
    visited.insert(current.getName());

    // Counts the number of edges traversed from the source vertex
    int edgesTraversed = 1;
    // Counts the number of synonyms sharing the particular number of edges from the source vertex
    int numSynonyms = 0;
    // Keeps track of the next iteration of the number of synonyms sharing the particular number of edges from the source vertex
    int nextNumSynonyms = 0;
    // Boolean to keep track if the synonym is one edge away from the source vertex
    bool firstTime = true;

    // While vertex queue is not empty
    while (!vertexQ.empty()) {

        // Take a vertex, u, out of the queue and visit u
        current = vertexQ.front();
        vertexQ.pop();

        // Helper variable to store the list of synonyms
        auto synonymList = current.getSynonyms();
        // Size of the synonym list
        numSynonyms = synonymList.size();

        // If all the synonyms sharing the same number of edges traversed from the source vertex are exhausted
        if (numSynonyms == 0) {
            // The edges traversed from the source vertex must increase by one
            edgesTraversed++;
            // The saved next iteration of number of synonyms sharing the same number of edges to be traversed is replacing the previous counter
            numSynonyms = nextNumSynonyms;
            // Reset the next iteration
            nextNumSynonyms = 0;
        }
        // If the synonyms are not exhausted, decrement its counter
        numSynonyms--;

        // For all vertices, v, adjacent to his vertex, u...
        for (auto &syn : synonymList)
        {
            // If size of resulting vector is equivalent to specified size, return resulting vector
            if (result.size() == size)
                return result;

            // ...if v has not been visited...
            if (visited.find(syn.getName()) == visited.end())
            {
                // Only increment the counter keeping track of the next iteration of synonyms if not pertaining to the synonyms of the source vector
                if (!firstTime) {
                    nextNumSynonyms++;
                }

                // ...mark this vertex v as identified and push into queue
                result.push_back(std::make_pair(syn, edgesTraversed));
                visited.insert(syn.getName());
                vertexQ.push(syn);
            }
        }

        // Keep the boolean as false for subsequent iterations
        firstTime = false;
    }
    // Return resulting vector
    return result;
}


// Returns a vector of class Vertex objects synonymous to the user inputted word using DFS
std::vector<Graph::Vertex> dfs(Graph graph, const std::string &word, const int size) {

    if (size <= 0)
        return {};

    std::vector<Graph::Vertex> result;
    std::stack<Graph::Vertex> vertexStk;
    std::unordered_set<std::string> visited;

    // Find the vertex of the user inputted word
    Graph::Vertex current = graph.findVertex(word);

    // Place found vertex in the stack and mark it visited
    vertexStk.push(current);
    visited.insert(current.getName());

    // While vertex stack is not empty
    while (!vertexStk.empty()) {

        // Take a vertex, u, out of the stack and visit u
        current = vertexStk.top();

        // If current vertex's synonym vector is empty, then pop stack
        if (current.getSynonyms().size() == 0) {
            vertexStk.pop();
            continue;
        }

        // For all vertices, v, adjacent to his vertex, u...
        for (auto &syn : current.getSynonyms())
        {
            // If size of resulting vector is equivalent to specified size, return resulting vector
            if (result.size() == size)
                return result;

            // ...if v has not been visited...
            if (visited.find(syn.getName()) == visited.end()) {
                // ...mark this vertex v as identified and push into stack
                result.push_back(syn);
                visited.insert(syn.getName());
                vertexStk.push(syn);
            }
            else if (!vertexStk.empty())
                vertexStk.pop();
        }
    }
    // Return resulting vector
    return result;
}