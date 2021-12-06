#pragma once
#include <map>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <unordered_map>

class Graph {

    // Nested node class
    class Vertex {
        std::string name;
        std::string speech;
        list<Vertex> synonyms;

    public:
        // For basic synonyms
        Vertex(std::string name) {
            this->name = name;
        }

        // For words defined in cvs file
        Vertex(std::string name, std::string speech) {
            this->name = name;
            this->speech = speech;
        }

        void addSynonyms(Vertex syn) {
            synonyms.push_back(syn);
        }

        void setSpeech(std::string speech) {
            this->speech = speech;
        }

        std::string getName() {
            return name;
        }
    };

    vector<Vertex> adjList;
    set<std::string> allWords;

public:
    Graph(unordered_map<std::string, pair<std::string, vector<std::string>>> result) {
        for (auto i : result) {

            // If word is unique
            if (allWords.find(i.first) == allWords.end()) {

                Vertex newVertex(i.first, i.second.first);
                allWords.insert(i.first);

                // Adds neighbors
                for (auto j : i.second.second) {

                    // Checks if synonym is unique
                    if (allWords.find(j) == allWords.end()) {

                        // Constructs new vector
                        Vertex newNeighbor(j);
                        allWords.insert(j);
                        newVertex.addSynonyms(newNeighbor);
                        adjList.push_back(newNeighbor);
                    }
                    else {
                        newVertex.addSynonyms(findVertex(j));
                    }
                }
                adjList.push_back(newVertex);
            }
            else {
                Vertex vertex = findVertex(i.first);
                vertex.setSpeech(i.second.first);

                // Adds neighbors
                for (auto j : i.second.second) {

                    // Checks if synonym is unique
                    if (allWords.find(j) == allWords.end()) {

                        // Constructs new vector
                        Vertex newNeighbor(j);
                        allWords.insert(j);
                        vertex.addSynonyms(newNeighbor);
                        adjList.push_back(newNeighbor);
                    }

                    else {
                        vertex.addSynonyms(findVertex(j));
                    }
                }
            }
        }
    }

    Vertex findVertex(std::string nameSearch) {
        Vertex vertex(nameSearch);

        for (auto i : adjList) {
            if (i.getName() == nameSearch) {
                return i;
            }
        }

        return vertex;
    }

    // ADDED ACCESSOR METHODS
    vector<Vertex> getAdjList() {
        return adjList;
    }

    set<std::string> getWordSet() {
        return allWords;
    }
};

