#include "Widget.h"
#include "ui_Widget.h"

Widget::Widget(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Synonymity Seeker");

    readFile();
    graph = Graph(readWords);

    isBfs = false;
    isDfs = false;
}

Widget::~Widget() { delete ui; }

void Widget::clearAll()
{
    ui->lineEdit_word->clear();
    ui->lineEdit_numSyn->clear();
    ui->checkBox_bfs->setChecked(false);
    ui->checkBox_dfs->setChecked(false);

    word = "";
    numSyn = "";
    isBfs = false;
    isDfs = false;

    bfsTime = 0;
    dfsTime = 0;
}

void Widget::on_pushBtn_findSyn_clicked()
{
    // Determines which box was checked by the user
    isBfs = ui->checkBox_bfs->isChecked();
    isDfs = ui->checkBox_dfs->isChecked();

    // Stores user input values
    word = ui->lineEdit_word->text();
    numSyn = ui->lineEdit_numSyn->text();

    if ((!isBfs && !isDfs) || (word.isEmpty() || numSyn.isEmpty()))
    {
        QMessageBox::warning(this, "Missing Inputs", "Missing inputs.");
        return;
    }

    bool valid;
    numSyn.toInt(&valid);

    if (!valid || numSyn.toInt() < 1 || numSyn.toInt() > MAX_NUMSYN)
    {
        QMessageBox::warning(this, "Input Error", "Please enter a valid number from 1 to " +
        QString::number(MAX_NUMSYN) + ".");
        return;
    }

    if (isBfs) {
        // Determine time for traversing the graph using DFS
        auto t1 = Clock::now();
        bfs(word.toStdString(), numSyn.toInt());
        auto t2 = Clock::now();

        if (bfsVect.empty())
        {
            QMessageBox::information(this, "Message", "There are no synonyms for \'" + word + "\'");
            clearAll();
            return;
        }

        // Note: BFS Time Complextiy: O(|V| + |E|)
        bfsTime = duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
        bfsRatingAlgorithm(word.toStdString());
    }

    if (isDfs) {
        // Determine time for traversing the graph using DFS
        auto t1 = Clock::now();
        dfs(word.toStdString(), numSyn.toInt());
        auto t2 = Clock::now();

        if (dfsVect.empty())
        {
            QMessageBox::information(this, "Message", "There are no synonyms for \'" + word + "\'");
            clearAll();
            return;
        }

        // Note: DFS Time complexity: O(|V| + |E|)
        dfsTime = duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
        dfsRatingAlgorithm(word.toStdString());
    }

    printResult();
    clearAll();
}

void Widget::printResult()
{
    QString header = "";
    QString info = "";

    // Sets the header for the message box
    header += "Top " + numSyn + " Synonyms for \'" + word + "\' ";

    // Sets the information to be displayed
    info += "Synonyms [Rating]\n\n";

    if (isBfs) {
        info += "  BFS Traversal\n\n";
        for (int i = 0; i < bfsRating.size(); i++) {
            info += "        ";
            info += QString::number(i + 1) + ". ";
            info += QString::fromStdString(bfsRating[i].first);
            info += " [" + QString::number(bfsRating[i].second) + "]\n";
        }
    }

    if (isDfs) {

        if (isBfs) info += "\n";

        info += "  DFS Traversal\n\n";
        for (int i = 0; i < dfsRating.size(); i++) {
            info += "        ";
            info += QString::number(i + 1) + ". ";
            info += QString::fromStdString(dfsRating[i].first);
            info += " [" + QString::number(dfsRating[i].second) + "]\n";
        }
    }

    info += "\nSearch Time (in nanoseconds)\n";

    // For displaying the time taken for each traversal chosen
    if (isBfs)
        info += "\n     BFS: " + QString::number(bfsTime);
    if (isDfs)
        info += "\n     DFS: " + QString::number(dfsTime);

    info += "\n";

    dfsRating.clear();
    bfsRating.clear();
    dfsVect.clear();
    bfsVect.clear();

    QMessageBox msg;
    msg.setText(header);

    // Display as detailed text (with scroll bar)
    msg.setDetailedText(info);
    msg.exec();
}


// Helper functions
void Widget::readFile()
{
    // Create an unordered map that stores strings as keys, and the value as a pair of an additional string and a vector
    // The key string is the original word, and the additional string is the part of speech while the vector stores the synonyms of the particular word

    // Create an input filestream
    std::ifstream myFile(FILENAME);

    // Make sure the file is open
    if (!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Helper variables
    std::string line, colname, lemma, partOfSpeech, allSynonyms;

    // Read the column names
    if (myFile.good())
    {
        // Extract the first line in the file
        std::getline(myFile, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each column name
        while (std::getline(ss, colname, ',')) {
            // Effectively disregard the column names "lemma," "part_of_speech," and "synonyms"
        }
    }

    // Read data, line by line
    while (std::getline(myFile, line))
    {
        // Create a stringstream of the current line
        std::stringstream ss(line);
        // Use a buffer and a while loop to separate the string stream by commas instead of spaces
        std::string buffer;

        // Use a counter to determine the order of the strings produced by the stream
        int counter = 0;
        while (std::getline(ss, buffer, ',')) {
            // The first string of the current line should be the lemma
            if (counter == 0) {
                lemma = buffer;
            }
                // The second string of the current line should be the part of speech
            else if (counter == 1) {
                partOfSpeech = buffer;
            }
                // The third string of the current line should be all the synonyms separated by semicolons
            else {
                allSynonyms = buffer;
            }
            counter++;
        }

        // Create a vector to store the vector of synonyms
        std::vector<std::string> synonymVector;

        // Split the third string by the delimiter of semicolons
        std::string buffer2;
        std::stringstream ss2(allSynonyms);

        while (std::getline(ss2, buffer2, ';')) {
            // Check if the word has an appended "\r" return key at the end
            // If so, delete the appended return key
            auto possibleReturnKey = buffer2.find("\r");
            if (possibleReturnKey != std::string::npos) {
                buffer2.erase(possibleReturnKey, 2);
            }

            // Push the synonym into the vector
            synonymVector.push_back(buffer2);
        }

        // Create a pair and store the part of speech and the vector of synonyms
        std::pair<std::string, std::vector<std::string>> returnPair;
        returnPair.first = partOfSpeech;
        returnPair.second = synonymVector;

        // Insert the key value pair into the map
        readWords.insert({ lemma, returnPair });
    }

    // Close file
    myFile.close();
}

void Widget::bfs(const std::string &originalWord, const int &size) {
    std::queue<Graph::Vertex> vertexQ;
    std::unordered_set<std::string> visited;

    // Find the vertex of the user inputted word
    Graph::Vertex current = graph.findVertexTraversal(originalWord);

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

        // Check if size of resulting vector is equivalent to specified size
        if (bfsVect.size() == size)
            return;

        // Take a vertex, u, out of the queue and visit u
        current = vertexQ.front();
        vertexQ.pop();

        // If all the synonyms sharing the same number of edges traversed from the source vertex are exhausted
        // If it's not the vertices one edge away from the source vertex
        if (numSynonyms == 0 && !firstTime) {
            // The edges traversed from the source vertex must increase by one
            edgesTraversed++;
            // The saved next iteration of number of synonyms sharing the same number of edges to be traversed is replacing the previous counter
            numSynonyms = nextNumSynonyms;
            // Reset the next iteration
            nextNumSynonyms = 0;
        }

        // Helper variable to store the list of synonyms
        auto synonymList = current.getSynonyms(graph);
        // Size of the synonym list
        numSynonyms = synonymList.size();

        // For all vertices, v, adjacent to his vertex, u...
        for (auto& syn : synonymList)
        {
            // If the synonyms are not exhausted, decrement its counter
            numSynonyms--;

            // Additional check for number of synonyms pushed in vector, terminate method
            if (bfsVect.size() == size)
                return;

            // ...if v has not been visited...
            if (visited.find(syn) == visited.end())
            {
                // Only increment the counter keeping track of the next iteration of synonyms if not pertaining to the synonyms of the source vector
                if (!firstTime) {
                    nextNumSynonyms++;
                }

                // ...mark this vertex v as identified and push into queue
                bfsVect.emplace_back(syn, edgesTraversed);
                visited.insert(syn);
                vertexQ.push(syn);
            }
        }

        // Keep the boolean as false for subsequent iterations
        firstTime = false;
    }
}

void Widget::dfs(const std::string &originalWord, const int &size) {

    if (size <= 0)
        return;

    std::stack<Graph::Vertex> vertexStk;
    std::unordered_set<std::string> visited;
    std::unordered_map<std::string, int> edgesTraversed;
    int edgesTraveled = 0;

    // Find the vertex of the user inputted word
    Graph::Vertex current = graph.findVertexTraversal(originalWord);

    // Place found vertex in the stack and mark it visited
    vertexStk.push(current);
    visited.insert(current.getName());
    // Place vertex in map denoting edges traversed from source vertex
    edgesTraversed.insert({ current.getName(), edgesTraveled + 1 });

    // While vertex stack is not empty
    while (!vertexStk.empty()) {

        // If size of resulting vector is equivalent to specified size, terminate method
        if (dfsVect.size() == size)
            return;

        // Take a vertex, u, out of the stack and visit u
        current = vertexStk.top();

        // Find the number of edges traveled from source vertex to current vertex
        edgesTraveled = edgesTraversed[current.getName()];

        // Variable to hold the list of synonyms of the current vertex
        auto synList = current.getSynonyms(graph);

        // If current vertex's synonym vector is empty, then pop stack
        if (synList.empty()) {
            vertexStk.pop();
            continue;
        }

        // If traversing through the synonyms of a vertex, remove the vertex off of stack
        vertexStk.pop();

        // For all vertices, v, adjacent to his vertex, u...
        for (auto& syn : synList)
        {
            // Additional check for number of synonyms pushed in vector, terminate method
            if (dfsVect.size() == size)
                return;

            // ...if v has not been visited...
            if (visited.find(syn) == visited.end()) {
                // ...mark this vertex v as identified and push into stack (as well as insert into the edgesTraversed map)
                dfsVect.emplace_back(syn, edgesTraveled);
                visited.insert(syn);
                vertexStk.push(syn);
                edgesTraversed.insert({ syn, edgesTraveled + 1 });
            }
        }
    }
}

bool Widget::sortByRating(const std::pair<std::string, double> &a, const std::pair <std::string, double> &b) {
    return (a.second > b.second);
}

void Widget::bfsRatingAlgorithm(const std::string &originalWord)
{
    // Declare variable for rating
    double rating;
    // Find the part of speech of the original word
    std::string originalPartOfSpeech = graph.findVertexTraversal(originalWord).getSpeech();

    // Iterate through the vectors derived from the DFS traversal
    for (auto &i : bfsVect) {
        // Begin with the rating at 10
        rating = 10.0;
        // Find the part of speech of this particular vertex
        std::string partOfSpeech = graph.findVertexTraversal(i.first.getName()).getSpeech();
        // If the part of speech doesn't match, remove 1.5 rating points
        if (partOfSpeech != originalPartOfSpeech) {
            rating -= 1.5;
        }
        // For every edge greater than the first edge, remove 2 rating points (no negatives)
        for (int j = 1; j < i.second; j++) {
            if (rating >= 2.0) {
                rating -= 2.0;
            }
        }

        // Push the pair into the vector to be returned
        bfsRating.emplace_back(i.first.getName(), rating);
    }

    // Call the sorting function using the helper sorting method
    std::sort(bfsRating.begin(), bfsRating.end(), sortByRating);

}

void Widget::dfsRatingAlgorithm(const std::string &originalWord)
{
    // Declare variable for rating
    double rating;

    // Find the part of speech of the original word
    std::string originalPartOfSpeech = graph.findVertexTraversal(originalWord).getSpeech();

    // Iterate through the vectors derived from the DFS traversal
    for (auto & i : dfsVect) {
        // Begin with the rating at 10
        rating = 10.0;
        // Find the part of speech of this particular vertex
        std::string partOfSpeech = graph.findVertexTraversal(i.first.getName()).getSpeech();
        // If the part of speech doesn't match, remove 1 rating point
        if (partOfSpeech != originalPartOfSpeech) {
            rating -= 1;
        }
        // For every edge greater than the first edge, remove 1.5 rating points (no negatives)
        for (int j = 1; j < i.second; j++) {
            if (rating >= 1.5) {
                rating -= 1.5;
            }
        }

        // Push the pair into the vector to be returned
        dfsRating.emplace_back(i.first.getName(), rating);
    }

    // Call the sorting function using the helper sorting method
    std::sort(dfsRating.begin(), dfsRating.end(), sortByRating);

}


