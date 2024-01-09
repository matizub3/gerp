/*
 *  hashTable.cpp
 *  Rolando Ortega and Mateusz Zubrzycki 
 *  12/11/23
 *
 *  Contains an implementation of the hashTable class.   
 *
*/

#include "hashTable.h"

/*
 * name:      hashTable constructor 
 * purpose:   initializes variables for the hash table 
 * arguments: none
 * returns:   none 
 * effects:   sets variables for the table to initial values 
*/
hashTable::hashTable() {
    //set current table size to initial size 
    currentTableSize = INITIAL_TABLE_SIZE;
    //set number of items equal to 0
    numItemsInTable = 0;
    //create empty table 
    chainingTable = new Bucket[currentTableSize];
}

/*
 * name:      destructor 
 * purpose:   frees memory used by the hash table
 * arguments: none
 * returns:   none 
 * effects:   deletes the chainingTable array used for the table  
*/
hashTable::~hashTable() {
    //delete table array
    delete [] chainingTable;
}

/*
 * name:      hashValue 
 * purpose:   provide a hash index for the given key string
 * arguments: a KeyType with a key
 * returns:   a size_t with a hash index 
 * effects:   uses the hash function in std::hash to provide a hash index for 
 *            the given key
*/
static size_t hashValue(KeyType key) {
    //return hash index for the given key
    return std::hash<KeyType>{}(key);
}

/*
 * name:      insert
 * purpose:   inserts a Key and it's location in files into the hash table
 * arguments: a KeyType with a key, an int with a file number in the
 *            directory, and an int with a line number in that file 
 * returns:   none 
 * effects:   inserts the key and location (file and line number) into the hash
 *            table, calling helper functions to achieve this
*/
void hashTable::insert(KeyType key, int file, int line) {
    //set key to all lowercase and us lowercase string to get hash index 
    string lowercaseKey = makeLower(key);
    int index = hashValue(lowercaseKey) % currentTableSize;

    //get index of the lowercase key's node in the bucket 
    int node_index = getNodeIndex(lowercaseKey, chainingTable[index]);

    //if there are no nodes in the bucket, add a node to the bucket
    if (node_index == -1) {
        //create new node at with the given lowercase key
        Node new_node(lowercaseKey);
        //create new WordLocation with case sensitive word and location
        WordLocations new_location(key, file, line);

        //add case sensitive word to the node 
        new_node.entries.push_back(new_location);
        //add node to the bucket and update number of items in the table 
        chainingTable[index].nodes.push_back(new_node);
        numItemsInTable++;        
    } 
    
    //otherwise, insert the word into the prexisting node 
    else {
        Node *existingNode = &chainingTable[index].nodes.at(node_index);
        insertWord(key, file, line, *existingNode);
    }

    //check load factor and expand if necessary 
    if (getLoadFactor() > 0.7) 
        expand();
}

/*
 * name:      insertWord
 * purpose:   insert the case sensitive word into the given node 
 * arguments: a KeyType with a word, a reference to an int with a file number 
 *            in the directory, a reference to an int with a line number in the 
 *            file, and a reference to a node in the table 
 * returns:   none 
 * effects:   inserts the word into the given node in the table 
*/
void hashTable::insertWord(KeyType &word, int &file, int &line, Node &node) {
    //get index of the case sensitive word (entry) in the node 
    int index = getEntriesIndex(word, node);

    //no entry for this case sensitive word, so create new entry
    if (index == -1) {
        //create new WordLocations and add to the node 
        WordLocations newWordLocation(word, file, line);
        node.entries.push_back(newWordLocation);

        //update number of items in table 
        numItemsInTable++;
    } 
    
    //entry exists, so update entry with new instance (location)
    else if (not isDuplicate(node.entries.at(index), file, line)) { 
        //get the entry that corresponds to the given word
        WordLocations *entry = &node.entries.at(index);

        //create new location and add it to the word's vector of locations
        Instance newInstance(file, line);
        entry->location.push_back(newInstance);
    }
}

/*
 * name:      getNodeIndex
 * purpose:   get the index of the given key in the given bucket 
 * arguments: a KeyType with a lowercase key and a bucket to search through
 * returns:   returns the index of the key's node in the bucket (-1 if there is
 *            no corresponding node for the key) 
 * effects:   searches through the bucket until it finds the nodes 
 *            corresponding to the given key and returns the node's index
*/
int hashTable::getNodeIndex(KeyType &key, Bucket &bucket) {
    //get size of the bucket's vector of nodes 
    int size = bucket.nodes.size();
    
    //search through bucket 
    for (int i = 0; i < size; i++) {
        //if node with key is found, return index 
        if (bucket.nodes.at(i).key == key) {
            return i;
        }
    }

    //return -1 if key was not found in bucket 
    return -1;
}

/*
 * name:      getEntriesIndex
 * purpose:   get the index of the given case sensitive word in the given node 
 * arguments: a string with a case sensitive word and a node to search through
 * returns:   returns the index of the word in the node (-1 if there is
 *            no corresponding WordLocations for the key) 
 * effects:   searches through the bucket until it finds the nodes 
 *            corresponding to the given key and returns the node's index
*/

int hashTable::getEntriesIndex(string &word, Node &node) {
    //get size of the node's vector of WordLocations
    int size = node.entries.size();

    //search through vector
    for (int i = 0; i < size; i++) {
        //if node was found, return index 
        if (node.entries.at(i).word == word) {
            return i;
        }
    }

    //return -1 if word doesn't have corresponding WordLocations in the node
    return -1;
}

/*
 * name:      isDuplicate
 * purpose:   checks if a location (file index and line number) already exists
 *            in the given WordLocations 
 * arguments: a reference to a WordLocations, a reference to an int with a file
 *            number in the directory, and a reference to an int with a line 
 *            number in that file 
 * returns:   returns true if the location is equal to the last location 
 *            inserted in the WordLocations' vector of Instances, return false
 *            otherwise 
 * effects:   gets the file number and the line number of the last Instance 
 *            (location) that was inserted into the given WordLocatons, 
 *            and checks if the they are equal to the given file and line
 *            number 
*/

bool hashTable::isDuplicate(WordLocations &entry, int &file, int &line) {
    //get file and line numbers for the last Instance in Wordlocations' vector
    int filepath = entry.location.back().file_path_index;
    int lineNum = entry.location.back().lineNum;

    //checks to see if they are equal to the file and line numbers given
    return filepath == file and lineNum == line;
}

/*
 * name:      getLoadFactor 
 * purpose:   calculates the current load factor of the table
 * arguments: none
 * returns:   a float with the load factor 
 * effects:   returns the current load factor for the table, which is the 
 *            number of items in the table divided by the number of buckets in
 *            the table. currentTableSize is never 0.
*/

float hashTable::getLoadFactor() {
    //calculate load factor and return result
    return (float) numItemsInTable / (float) currentTableSize;
}

/*
 * name:      expand
 * purpose:   increases the size of the table and updates its contents  
 * arguments: none
 * returns:   none 
 * effects:   expands the size of the table to be twice as large, and updates 
 *            its contents by rehashing all of the lowercase keys 
*/

void hashTable::expand() {
    //double table size and create new table 
    currentTableSize *= 2;
    Bucket *newTable = new Bucket[currentTableSize];

    //copy contents of original table over to new one
    for (int i = 0; i < currentTableSize / 2; i++) {
        //get vector of nodes from the bucket at i 
        vector<Node> curr = chainingTable[i].nodes;

        //if bucket has contents, rehash everything and insert into new table
        //with new hash index 
        if (not curr.empty()) {
            int size = curr.size();
            for (int j = 0; j < size; j++) {
                int index = hashValue(curr.at(j).key) % currentTableSize;

                newTable[index].nodes.push_back(curr.at(j));
            }
        }
    }

    //delete original table
    delete [] chainingTable;
    //set new table as the current table 
    chainingTable = newTable;
}

/*
 * name:      getSensitiveWord
 * purpose:   get the WordLocations struct that correspondings to the case 
 *            sensitive key 
 * arguments: a KeyType with a key
 * returns:   returns the key's WordLocations
 * effects:   searches the table for the given case sensitive key and returns
 *            its corresponding WordLocations, return an empty one if the case
 *            sensitive key does not exist in the table 
*/

WordLocations hashTable::getSensitiveWord(KeyType &key) {
    //get lowercase version of key and use it to get its index and bucket 
    string lower = makeLower(key);
    int index = hashValue(lower) % currentTableSize;
    Bucket bucket = chainingTable[index];

    //search through the bucket if it's not empty
    if (not bucket.nodes.empty()) {
        int node_index = getNodeIndex(lower, bucket);

        //search through the node with the lowercase key if its not empty
        if (node_index != -1) {
            Node node = bucket.nodes.at(node_index);

            int entry_index = getEntriesIndex(key, node);

            //if the case sensitive word has an entry, return it 
            if (entry_index != -1) {
                return node.entries.at(entry_index); 
            }
        }
    }

    //if case sensitive word does not have a Wordlocations, return an empty one
    return WordLocations();
}

/*
 * name:      getInsensitive Word 
 * purpose:   get all of the WordLocations for the given key regardless of case
 *            sensitivity 
 * arguments: a KeyType with a key
 * returns:   returns the Node that contains all of the WordLocations for the 
 *            key, regardless of case sensitive lettering 
 * effects:   searches through the table for the node that corresponds to the
 *            lowercase key and return it, returning an empty node if the key
 *            does not exist in the table
*/

hashTable::Node hashTable::getInsensitiveWord(KeyType &key) {
    //make key all lowercase and use it to find it's bucket in the table 
    string lower = makeLower(key);
    int index = hashValue(lower) % currentTableSize;
    Bucket bucket = chainingTable[index];

    //search through bucket if it's not empty
    if (not bucket.nodes.empty()) {
        int node_index = getNodeIndex(lower, bucket);

        //if node that corresponds with key is not empty, return it 
        if (node_index != -1) {
            return bucket.nodes.at(node_index);
        }
    }

    //otherwise, return an empty node 
    return Node();
}

/*
 * name:      makeLower 
 * purpose:   sets the given word to all lowercase 
 * arguments: a reference to a string with a word 
 * returns:   returns the all lowercase version of the word 
 * effects:   goes through each char in the word string and makes it lowercase,
 *            then returns the result 
*/

string hashTable::makeLower(string &word) {
    //string to contain lowercase word
    string lower = "";
    //int to contain size of word
    int length = word.length();

    //go through each character in the word, make it lowercase, and add to lower
    for (int i = 0; i < length; i++) {
        lower += tolower(word.at(i));
    }

    //return lowercase string
    return lower;
}

//USED FOR TESTING, UNCOMMENT WHEN RUNNING UNIT TESTS 
// void hashTable::printTable(){

//      for (int i = 0; i < currentTableSize; i++){
//         Bucket bucket = chainingTable[i];

//         for (int j = 0; j < bucket.nodes.size(); j++) {
//             Node node = bucket.nodes.at(j);

//             cout << i << " " << node.key << ": ";

//             for (int k = 0; k < node.entries.size(); k++) {
//                 WordLocations entry = node.entries.at(k);

//                 cout << entry.word << "[";

//                 for (int l = 0; l < entry.location.size(); l++) {
//                     Instance location = entry.location.at(l);

//                     cout << "(" << location.file_path_index << ", "
//                         << location.lineNum << ")";
//                 }

//                 cout << "] ";
//             }

//             cout << endl;
//         }
//     }
// }