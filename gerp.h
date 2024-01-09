/*
 *  gerp.h
 *  Rolando Ortega and Mateusz Zubrzycki 
 *  12/11/23
 *
 *  Gerp is a class with the ability to build an index of words based on a file
 *  directory provided by the client and handle queries from the client on the
 *  index, storing the results of those queries in a provided output file. 
 *  Queries include searching for a word in the directory regardless of case 
 *  sensitive letters, and searching for a word with specific case sensitivity.
 *  Clients also have the ability to change output files while running the 
 *  program. Gerp will produce an error message to the client should a queried
 *  word not be found in the directory; for case sensitive searches, it will 
 *  also recommend that the user do a case insensitive search. Gerp interacts 
 *  with FSTree and DirNode to build a file system tree based on the given 
 *  directory, hashTable to build an index of words and their locations in the
 *  directory, and stringProcessing to remove leading and trailing non alpha
 *  numeric when inserting and searching for words. 
 *
*/

#ifndef GERP_H
#define GERP_H

#include "hashTable.h"
#include "FSTree.h"
#include "DirNode.h"
#include "stringProcessing.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

class gerp {
//public functions available to the client 
public:
    gerp(string directory, string outputFile);
    ~gerp();

    void handleQuery(istream &input);

//private functions, comment out private keyword when testing 
private: 
    template<typename streamtype>
    void open_or_die(streamtype &stream, string &fileName);

    //functions for building index 
    void treeTraversal(DirNode *root, string &path);
    void readWords(string &file, int index);

    //functions for responding to queries 
    void printSensitive(string &word);
    void printInsensitive(string &word);
    void newOutput(string &outputFile);

    //helper functions
    int outputPaths(Instance &location);
    bool checkOutput(const string &path);

    //data structures to contain data 
    vector<string> filepaths;
    hashTable table;

    //variables for the output file (output stream, name of current file, line
    //we're at in that file)
    ofstream output;
    string curr_output;
    int outputLine;

    //variable for case sensitivty 
    bool insensitive;
};

#endif