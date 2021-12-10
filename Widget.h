#pragma once
#include <QWidget>
#include <QMessageBox>
#include <QCoreApplication>
#include <utility>
#include <stack>
#include <queue>
#include <unordered_set>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include "Graph.h"
typedef std::chrono::high_resolution_clock Clock;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    // Clears all widgets and resets private variables
    void clearAll();
    void printResult();

    // Read from csv file
    void readFile();

    // Makes a vector of pairs including Vertex objects and their respective distances from the source
    // vertex (as integers),all synonymous to the user inputted word using BFS
    void bfs(const std::string &orig, const int &size);

    // Makes a vector of pairs including Vertex objects and their respective distances from the source
    // vertex (as integers), all synonymous to the user inputted word using DFS
    void dfs(const std::string &orig, const int &size);

    // Helper function to allow for sorting of pairs in descending order
    // Compares the pairs' second values, which would be the ratings
    static bool sortByRating(const std::pair<std::string, double> &a, const std::pair <std::string, double> &b);

    // Rating algorithm for BFS traversals
    void bfsRatingAlgorithm(const std::string &originalWord);

    // Rating algorithm for DFS traversals
    void dfsRatingAlgorithm(const std::string &originalWord);

private slots:
    void on_pushBtn_findSyn_clicked();

private:
    Ui::Widget *ui;

    int MAX_NUMSYN = 100;
    std::string FILENAME = "synonyms.csv";

    QString word;
    QString numSyn;

    bool isBfs;
    bool isDfs;

    Graph graph;
    std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> readWords;

    long long bfsTime;
    long long dfsTime;

    std::vector<std::pair<Graph::Vertex, int>> bfsVect;
    std::vector<std::pair<Graph::Vertex, int>> dfsVect;

    std::vector<std::pair<std::string, double>> bfsRating;
    std::vector<std::pair<std::string, double>> dfsRating;
};
