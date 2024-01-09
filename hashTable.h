/*
 *  hashTable.h
 *  Rolando Ortega and Mateusz Zubrzycki 
 *  12/11/23
 *
 *  hashTable is a class that is used to build a data structure of words and 
 *  their unique locations in a directory. This includes different case 
 *  sensitive versions of these words. The class has the ability to insert
 *  words into the table using the C++ hash function, as well as retrieve those
 *  words and a list of their locations from the table. 
 *
*/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include <iostream>
#include <set>

using namespace std;

// 
//  Instance struct, used to store a specific location in the directory. A 
//  location is a file number in the directory and a line number in that file.
// 
struct Instance {
    //ints to contain file number in directory and line num in that file 
    int file_path_index;
    int lineNum;

    //default constructor 
    Instance() {
        file_path_index = -1;
        lineNum = -1;
    }

    //constructor to set file and line numbers 
    Instance(int filepath, int linenum){
        file_path_index = filepath;
        lineNum = linenum;
    }
};

// 
//  WordLocations struct, used to store a case sensitive word and all of its 
//  unique locations in the directory 
// 
struct WordLocations {
    //string to contain case sensitive word 
    string word;
    //vector to contain all locations of that word 
    vector<Instance> location;

    //default constructor 
    WordLocations() {
        word = "";
    }

    //constructor to initialize variables with given information 
    WordLocations(string &data, int file, int line) {
        word = data;
        Instance instance(file, line);
        location.push_back(instance);
    }
};

typedef string KeyType;
typedef WordLocations ValueType;

class hashTable {
//public functions available to the client, including other classes 
public:
    hashTable();
    ~hashTable();

    // 
    //  Node struct, used to contain all of the case sensitive variations of a 
    //  all lowercase key in the directory
    //  
    struct Node {
        //KeyType with an all lowercase word that serves as a key
        KeyType key;
        //vector with all of the case sensitive variations of they key
        vector<ValueType> entries;

        //default constructor 
        Node() {
            key = "";
        }

        //constructor to initialize key 
        Node(KeyType k) {
            key = k;
        }
    };

    enum HashFunction {GOOD_HASH_FUNCTION};

    //function for inserting words 
    void insert(KeyType key, int file, int line);

    //functions for getting words based on sensitivity
    WordLocations getSensitiveWord(KeyType &key);
    Node getInsensitiveWord(KeyType &key);

//private functions, comment out when unit testing 
private:

    //bucket to contain keys with the same hash index 
    struct Bucket {
        vector<Node> nodes;
    };

    //variables for information about the table 
    static const int INITIAL_TABLE_SIZE = 100;
    int currentTableSize;
    int numItemsInTable;

    //array to act as table 
    Bucket *chainingTable;

    //helper functions for inserting and accessing 
    void expand();
    string makeLower(string &word);
    float getLoadFactor();
    int getNodeIndex(KeyType &key, Bucket &bucket);
    int getEntriesIndex(string &word, Node &node);
    bool isDuplicate(WordLocations &entry, int &file, int &line);
    void insertWord(KeyType &word, int &file, int &line, Node &node);

    //Function for testing
    //void printTable();
};

#endif