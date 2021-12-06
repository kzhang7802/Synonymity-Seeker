#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <queue>
#include <stack>
#include "Graph.h"

// Returns a vector of class Vertex objects synonymous to the user inputted word using BFS
std::vector<Graph::Vertex> bfs(Graph graph, const std::string &word, const int size) {
    std::vector<Graph::Vertex> result;
    std::queue<Graph::Vertex> vertexQ;
    std::unordered_set<std::string> visited;

    // Find the vertex of the user inputted word
    Graph::Vertex current = graph.findVertex(word);

    // Place found vertex in a queue and mark it visited
    vertexQ.push(current);
    visited.insert(current.getName());

    // While vertex queue is not empty
    while (!vertexQ.empty()) {

        // Take a vertex, u, out of the queue and visit u
        current = vertexQ.front();
        vertexQ.pop();

        // For all vertices, v, adjacent to his vertex, u...
        for (auto &syn : current.getSynonyms())
        {
            // If size of resulting vector is equivalent to specified size, return resulting vector
            if (result.size() == size)
                return result;

            // ...if v has not been visited...
            if (visited.find(syn.getName()) == visited.end())
            {
                // ...mark this vertex v as identified and push into queue
                result.push_back(syn);
                visited.insert(syn.getName());
                vertexQ.push(syn);
            }
        }
    }
    // Return resulting vector
    return result;
}


// Returns a vector of class Vertex objects synonymous to the user inputted word using DFS
std::vector<Graph::Vertex> dfs(Graph graph, const std::string &word, const int size) {
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

        // For all vertices, v, adjacent to his vertex, u...
        for (auto &syn : current.getSynonyms())
        {
            // If size of resulting vector is equivalent to specified size, return resulting vector
            if (result.size() == size)
                return result;

            // ...if v has not been visited...
            if (visited.find(syn.getName()) == visited.end())
            {
                // ...mark this vertex v as identified and push into stack
                result.push_back(syn);
                visited.insert(syn.getName());
                vertexStk.push(syn);
            }
            else
                vertexStk.pop();
        }
    }
    // Return resulting vector
    return result;
}