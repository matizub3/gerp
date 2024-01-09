/*
 *  unit_tests.h
 *  Rolando Ortega and Mateusz Zubrzycki 
 *  12/6/23
 *
 *  Tests the stripNonAlphaNum and treeTraversal functions for both regular
 *  and edge cases.   
 *
*/

#include "stringProcessing.h"
#include "hashTable.h"
#include "gerp.h"
#include "FSTree.h"
#include "DirNode.h"
#include <cassert>
#include <iostream>
#include <functional>

using namespace std;


//Testing the stringProcessing() function using a string which has 
//non-alphanumeric characters at the front and back and asserting correct 
//string parsing
void stringProcessingTestOne() {
    string test = "$%!@mateusz^#@&";

    assert(stripNonAlphaNum(test) == "mateusz");
}


//Testing the stringProcessiong() function using a string which has leading and
//trailing non-alphanumeric characters and also has non-alphanumeric characters 
//in the middle of the word
void stringProcessingTestTwo() {
    string test = "!!!hell*o**";

    assert(stripNonAlphaNum(test) == "hell*o");
}


//Testing the stringProcessiong() function using a string which has leading and
//trailing non-alphanumeric characters and also has non-alphanumeric characters 
//in the middle of the word
void stringProcessingTestThree() {
    string test = "@##!!#!@!#COMP-15!!!!!!!";

    assert(stripNonAlphaNum(test) == "COMP-15");
}


//Testing the stringProcessing() function using a string which has no
//alphanumeric characters
void nonAlphaNumeric() {
    string test = "@!&*()!*$^&*@";

    assert(stripNonAlphaNum(test) == "");
}


//Testing the stringProcessing() function using a string which has all
//alphanumeric characters
void allAlphaNumeric() {
    string test = "rolando";

    assert(stripNonAlphaNum(test) == "rolando");
}


//Testing the hashTable constructor to ensure that initial values are
//assigned correctly
void hashTableConstructorTest() {
    hashTable table;

    assert(table.currentTableSize == 100);
    assert(table.numItemsInTable == 0);

}

//Assert that the word is correctly inserted in the corresponding node
//by checking the number of Nodes present in the corresponding Bucket
void checkNodesSizeAfterInsertion() {
    
    hashTable table;

    //Insert 'the' with a filePath and LineNum value
    table.insert("the", 1, 9);

    table.printTable();

    int index = std::hash<string>{}("the") % table.currentTableSize;

    hashTable::Bucket bucket = table.chainingTable[index];

    assert(bucket.nodes.size() == 1);

}

//Testing whether the new node inserted has the correct key of 'the'
void checkNodeInsertion() {
    hashTable table;

    //Insert 'the' with a filePath and LineNum value
    table.insert("the", 1, 9);

    table.printTable();

    int index = std::hash<string>{}("the") % table.currentTableSize;

    hashTable::Bucket bucket = table.chainingTable[index];

    assert(bucket.nodes.at(0).key == "the");

}

//Testing to see if a new WordLocations entry is added to the corresponding
// 'the' Node by checking the size of the Node's entries vector
void checkEntriesSizeAfterInsertion() {
    hashTable table;

    //Insert 'the' with a filePath and LineNum value
    table.insert("the", 1, 9);

    table.printTable();

    int index = std::hash<string>{}("the") % table.currentTableSize;

    hashTable::Bucket bucket = table.chainingTable[index];

    //Since we know that the nodes vector are of size 1 from previous test
    assert(bucket.nodes.at(0).entries.size() == 1);

}


//Assert that WordLocation vector of Instances is updated properly with the
//values specified in the insert function
void checkWordLocationAndInstanceInsertion() {
    
    hashTable table;

    //Insert 'the' with a filePath and LineNum value
    table.insert("the", 1, 9);

    table.printTable();

    int index = std::hash<string>{}("the") % table.currentTableSize;

    hashTable::Bucket bucket = table.chainingTable[index];

    WordLocations wordLocation = bucket.nodes.at(0).entries.at(0);

    assert(wordLocation.word == "the");
    assert(wordLocation.location.at(0).file_path_index == 1);
    assert(wordLocation.location.at(0).lineNum == 9);

}


//Testing whether the key value in the new Node holds the lowercase version of
//the inserted word
void NodeKeyCaseLowercase() {
    
    hashTable table;

    //Insert 'the' with a file_path_index and lineNum value
    table.insert("The", 1, 9);

    table.printTable();

    int index = std::hash<string>{}("the") % table.currentTableSize;

    hashTable::Bucket bucket = table.chainingTable[index];

    assert(bucket.nodes.at(0).key == "the");

}


//Testing whether different casings go to the same Bucket and Node by
//inserting 4 different casing of the and ensuring that 1 node for 'the' was
//created and that the node has 5 different entries, corresponding to each
//different casing
void differentCasesGoToSameNode() {
    
    hashTable table;

    //Insert 'the' with a file_path_index and lineNum value
    table.insert("The", 1, 9);
    table.insert("the", 3, 9);
    table.insert("tHe", 3, 6);
    table.insert("thE", 3, 7);
    table.insert("THE", 4, 7);

    table.printTable();

    int index = std::hash<string>{}("the") % table.currentTableSize;

    hashTable::Bucket bucket = table.chainingTable[index];

    //Assert that all went to the same Node (Nodes are case insensitve)
    assert(bucket.nodes.size() == 1);

    //Assert that all went to different WordLocations (WordLocations are 
    // case sensitive)
    assert(bucket.nodes.at(0).entries.size() == 5);

}

//Testing whether isDuplicate() properly stops duplicates from being added
//to the table by adding the same entry twice and ensuring that only one
//of such an entry exists
void checkDuplicateChecker() {
    
    hashTable table;

    //Insert 'the' with a file_path_index and lineNum value
    table.insert("the", 1, 9);
    table.insert("the", 1, 9);

    table.printTable();

    int index = std::hash<string>{}("the") % table.currentTableSize;

    hashTable::Bucket bucket = table.chainingTable[index];

    WordLocations wordLocation = bucket.nodes.at(0).entries.at(0);

    //Assert that there is only one entry (The duplicate was not added)
    assert(bucket.nodes.at(0).entries.size() == 1);

    assert(bucket.nodes.at(0).entries.at(0).word == "the");

    //Assert correct file_path_index and lineNum
    assert(wordLocation.location.at(0).file_path_index == 1);
    assert(wordLocation.location.at(0).lineNum == 9);
}


//Asserting that the expand function runs when load factor is > 0.7
//by exceeding a load factor of 0.7 and seeing whether currentTableSize changes
//accordingly
void expandTest() {

    hashTable table;

    table.currentTableSize = 10;

    int initialSize = table.currentTableSize;

    table.insert("the", 1, 3);
    table.insert("tHe", 1, 3);
    table.insert("Oogabooga", 2, 4);
    table.insert("Hello", 1, 3);
    table.insert("gerp", 1, 3);
    table.insert("me", 5, 7);
    table.insert("hash", 7, 13);
    table.insert("Me", 7, 9);

    table.printTable();

    //Assert that expand was called and table size doubled
    assert(table.currentTableSize == initialSize * 2);
}

//Assert that elements were correctly re-entered by expand() by causing
//expand to be called and ensuring that the number of elements after expand()
//is the same as the initial number of elements
void expandTestCopiedElements() {

    hashTable table;

    table.currentTableSize = 10;

    int initialSize = table.currentTableSize;

    table.insert("the", 1, 3);
    table.insert("thou", 1, 3);
    table.insert("Oogabooga", 2, 4);
    table.insert("Hello", 1, 3);
    table.insert("gerp", 1, 3);
    table.insert("my", 5, 7);
    table.insert("hash", 7, 13);

    //Insert final element to cause expansion
    table.insert("Me", 7, 9);

    //Assert that expand was called and table size doubled
    assert(table.currentTableSize == initialSize * 2);

    table.printTable();

    //Check that every element was rehashed back into hash
    int entryCount = 0;
    hashTable::Bucket bucket;
    hashTable::Node node;

    for (int i = 0; i < table.currentTableSize; i++){
         bucket = table.chainingTable[i];

        for (int j = 0; j < bucket.nodes.size(); j++) {
            node = bucket.nodes.at(j);

            for (int k = 0; k < node.entries.size(); k++) {
                entryCount++;
            }
        }
    }

    //Assert that every entry was reinserted into hash table
    assert(entryCount == 8);

}

//Testing makeLower by inputting an uppercase word and asserting that the
//correct lowercase version is returned
void makeLowerTest() {

    hashTable test;

    string word = "The";
    string lower = test.makeLower(word);

    assert(lower == "the");
}


//Testing makeLower by inputting a word with uppercase letters in the middle
//of the word and ensuring that the correct lowercase version is returned
void makeLowerMiddleUppercaseTest() {
    
    hashTable test;
    
    string word = "tHe";
    string lower = test.makeLower(word);

    assert(lower == "the");
}


//Testing makeLower by inputting a fully uppercase word and ensuring that
//the correct lowercase version is returned
void makeLowerAllUppercaseTest() {
    
    hashTable test;
    
    string word = "THE";
    string lower = test.makeLower(word);

    assert(lower == "the");
}


//Testing getNodeIndex by inserting a word into the table, calling
//getNodeIndex, and ensuring that the correct Node index was returned by
//accessing the node at that index and verifying the key
void getNodeIndexTest() {
    
    hashTable table;
    string key = "the";

    table.insert("the", 9, 11);

    int theIndex = std::hash<string>{}("the") % table.currentTableSize;
    hashTable::Bucket bucket = table.chainingTable[theIndex];
    
    int theNodeIndex = table.getNodeIndex(key, bucket);

    //Assert a node holding the key has been found
    assert(theNodeIndex != -1);

    //Assert that the calculated nodeIndex truly has a key of the
    assert(bucket.nodes.at(theNodeIndex).key == "the");

}


//Testing getNodeIndex in the case where no corresponding Node is found
//and ensuring that an index of -1 is returned
void getNodeIndexTestNoNodeFound() {
    
    hashTable table;
    string key = "empty";
    
    //Note that the hashTable does not have any input, thus no corresponding 
    //Node exists
    hashTable::Bucket bucket = table.chainingTable[0];
    
    int theNodeIndex = table.getNodeIndex(key, bucket);

    //Assert no Node was found and -1 was returned
    assert(theNodeIndex == -1);

}


//Testing getEntriesIndex by inserting a word and calling getEntriesIndex to
//get the index of the corresponding WordLocations struct and ensuring that
//the correct index was returned by verifying the WordLocations word value
void getEntriesIndexTest() {

    hashTable table;
    string word = "HELLO";

    table.insert(word, 4, 20);

    int helloIndex = std::hash<string>{}("hello") % table.currentTableSize;

    hashTable::Bucket bucket = table.chainingTable[helloIndex];
    hashTable::Node node = bucket.nodes.at(0);

    int entryIndex = table.getEntriesIndex(word, node);

    assert(entryIndex != -1);
    assert(node.entries.at(entryIndex).word == "HELLO");

}


//Testing getEntriesIndex when there is no corresponding WordLocations struct
//present in the inputted Node and ensuring that an index of -1 is returned
void getEntriesIndexTestNoEntry() {

    hashTable table;

    table.insert("HELLO", 4, 20);

    int helloIndex = std::hash<string>{}("hello") % table.currentTableSize;

    hashTable::Bucket bucket = table.chainingTable[helloIndex];
    hashTable::Node node = bucket.nodes.at(0);

    //Look for "hi" entry in the hello Node, no such entry should be found
    string incorrectEntry = "hi";
    int entryIndex = table.getEntriesIndex(incorrectEntry, node);

    //No entry is found since we are in hello Node, returns -1
    assert(entryIndex == -1);
}


//Testing getSensitiveWord by inserting a word into the table and calling
//getSensitiveWord to get the corresponding WordLocations struct and ensuring
//that the correct WordLocations struct is returned
void getSensitiveWordTest() {
    
    hashTable table;
    string word = "Happiness";
    table.insert(word, 12, 20);

    WordLocations happWordLocation = table.getSensitiveWord(word);

    //Assert that correct corresponding WordLocations struct was found
    assert(happWordLocation.word == "Happiness");
    assert(happWordLocation.location.at(0).file_path_index == 12);
    assert(happWordLocation.location.at(0).lineNum == 20);

}


//Testing getSensitiveWord with a word that does not exist in the table and
//ensuring that an empty WordLocations struct is returned
void getSensitiveWordTestNoWordLocations() {

    hashTable table;
    string word = "Happiness";

    //"Happiness" does not exist in table
    WordLocations happWordLocation = table.getSensitiveWord(word);

    //Assert that an empty WordLocations struct was returned
    assert(happWordLocation.location.empty());
        
}


//Testing getInsensitiveWord by inserting 5 words that are equivalent when
//considered without casings in mind and ensuring that getInsensitiveWord
//returns the corresponding Node by ensuring the contents and size of the Node
//returned
void getInsensitiveWordTest() {

    hashTable table;
    table.insert("CS15", 12, 13);
    table.insert("cS15", 34, 1);
    table.insert("Cs15", 12, 12);
    table.insert("cs15", 14, 2);

    //Use getInsensitiveWord to get Node with all CS15 insertions as 
    //Nodes are case insensitive

    string key = "cs15";
    hashTable::Node node = table.getInsensitiveWord(key);

    //Assert that size corresponds to number of suspected entries in Node
    assert(node.entries.size() == 4);

    //Assert that each entry corresponds to insertions
    assert(node.entries.at(0).word == "CS15");
    assert(node.entries.at(1).word == "cS15");
    assert(node.entries.at(2).word == "Cs15");
    assert(node.entries.at(3).word == "cs15");
    
}


//Testing getInsensitiveWord with a word which is not in the table and ensuring
//that an empty Node is returned
void getInsensitiveWordTestNoNodeFound() {

    hashTable table;
    string word = "hope";

    //Note "hope" does not exist in the table, Node is not found
    hashTable::Node node = table.getInsensitiveWord(word);

    //Assert that an empty Node was returned by the function
    assert(node.entries.empty());
    assert(node.key == "");

}


//Testing the getLoadFactor function by inserting 5 words into a table of
//currentTableSize 10 and asserting that getLoadFactor returnes 0.5
void getLoadFactorTest() {
    
    hashTable table;
    table.currentTableSize = 10;

    table.insert("Hello", 11, 12);
    table.insert("hi", 12, 13);
    table.insert("czesc", 13, 14);
    table.insert("hola", 14, 17);
    table.insert("bonjour", 15, 19);

    //Assert that load factor is calculated correctly
    assert(table.numItemsInTable == 5);
    assert(table.currentTableSize == 10);
    assert(table.getLoadFactor() == 0.5);

}


//Testing the table constructor to ensure that the value of currentTableSize
//is never 0, as this is the value that getLoadFactor divides by to obtain the
//load factor
void getLoadFactorIsNotZero() {

    hashTable table;

    //Assert that currentTableSize is never 0, as getLoadFactor divides by
    //its value

    assert(table.currentTableSize != 0);
}


//Visually confirm that tree is built correctly
void testInsertion() {
    hashTable table;

    table.insert("the", 1, 9);
    table.insert("The", 13, 4);
    table.insert("champion", 20, 1);
    table.insert("dog", 18, 23);
    table.insert("Puppy", 7, 5);
    table.insert("puPpy", 33, 1);
    table.insert("dog", 7, 21);

    //Visually assert correct building of tree
    table.printTable();
}


//Testing the treeTraversal function by creating a directory with one file in
//it and calling the gerp constructor (which calls the treeTraversal function),
//ensuring that the filepaths string vector in the gerp class is updated with
//the correct number of possible filepaths: 1
void treeTraversalTest() {

    //dir is a directory with one file in it
    //Constructor appends all filepaths into a string vector
    gerp the_gerp("dir", "output.txt");

    //Assert that size of filepaths vector is 1 (corresponding to the 1 file)
    assert(the_gerp.filepaths.size() == 1);
}