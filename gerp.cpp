/*
 *  gerp.cpp
 *  Rolando Ortega and Mateusz Zubrzycki 
 *  12/11/23
 *
 *  Contains an implementation of the gerp class.   
 *
*/

#include "gerp.h"

/*
 * name:      gerp constructor 
 * purpose:   initializes variables for the gerp class 
 * arguments: a string with the name of a directory to index and a string with
 *            the name of the initial output file 
 * returns:   none 
 * effects:   opens the output file and stores initial information about that
 *            file (it's name and line we are at in the file, 0) in correspond-
 *            ing variables. Builds a file tree with the given input directory
 *            and calls treeTraversal to build the index of words in that 
 *            directory. 
*/
gerp::gerp(string directory, string outputFile) {
    //open the given output file and update output file information variables 
    open_or_die(output, outputFile);
    curr_output = outputFile;
    outputLine = 0;

    //build the file tree using the given input directory
    FSTree tree(directory);

    //create string to contain the name of the root 
    string pathString = tree.getRoot()->getName();

    //traverse the tree to build the index 
    treeTraversal(tree.getRoot(), pathString);
}

/*
 * name:      destructor
 * purpose:   frees all memory used in the gerp class 
 * arguments: none
 * returns:   none 
 * effects:   deletes an memory that was used in the gerp class once the 
 *            program is finished 
*/
gerp::~gerp() { 

}

/*
 * name:      handleQuery 
 * purpose:   handles commands and queries provided by the client and calls
 *            corresponding helper functions 
 * arguments: an input stream for commands and queries from the client 
 * returns:   none 
 * effects:   takes in commands and queries from the client and calls any 
 *            helper functions to handle those queries. Updates any variables
 *            needed to conduct those queries. Continues to accept input from
 *            the client until the commands for quiting the program (@q and 
 *            @quit) are produced 
*/
void gerp::handleQuery(istream &input) {
    //string to contain commands and queries
    string command = "", query = "";

    //continue accepting input until client quits program 
    while ((not input.eof()) and (command != "@q" and command != "@quit")) {
        //prompt for query and take in first input
        cout << "Query? ";
        input >> command;

        //if command is for insensitive, get query and handle it 
        if (command == "@i" or command == "@insensitive") {
            input >> query;
            insensitive = true;
            string stripped = stripNonAlphaNum(query);
            printInsensitive(stripped);
        }
        //if command is for new output file, get query and handle it 
        else if (command == "@f") {
            input >> query;
            newOutput(query);
        } 
        //if command is for any word, handle it 
        else if (command != "@q" and command != "@quit") {
            insensitive = false;
            string sensitive_stripped = stripNonAlphaNum(command);
            printSensitive(sensitive_stripped);
        }
    }
    //close current output file once client quits program
    output.close();
}

/*
 * name:      open_file
 * purpose:   opens the file with provided file name with the provided stream
 * arguments: a stream and the name of a file to open
 * returns:   none 
 * effects:   attempts to open the file with the given file name and throws a
 *            runtime_error if this operation is not successful 
*/
template<typename streamtype>
void gerp::open_or_die(streamtype &stream, string &fileName) {
    //open file
    stream.open(fileName);

    //check if file was opened successfully, throw error if not 
    if (not stream.is_open()) {
        throw runtime_error("Unable to open file " + fileName);
    }
}

/*
 * name:      treeTraversal 
 * purpose:   reads through all of the files in the directory 
 * arguments: a DirNode with the current directory and a reference to a string
 *            containing the path 
 * returns:   none 
 * effects:   traverses the tree and adds names of directories to the given 
 *            path. Once files are reached, build the name of the current path 
 *            and each file and updates the index for the new file
*/
void gerp::treeTraversal(DirNode *root, string &path) {
    //print out path if directory is empty
    if (root->isEmpty()) {
        return;
    }

    //finish the paths for the files in the directory, insert words into table
    for (int i = 0; i < root->numFiles(); i++) {
        filepaths.push_back(path + "/" + root->getFile(i));
        readWords(filepaths.back(), filepaths.size() - 1);
    }

    //traverse each subdirectory 
    for (int i = 0; i < root->numSubDirs(); i++) {
        string temp = path + "/" + root->getSubDir(i)->getName();
        treeTraversal(root->getSubDir(i), temp);
    }
}

/*
 * name:      readWords
 * purpose:   adds the words in the given output file to the index
 * arguments: a string with the name of a file in the directory and that file's
 *            index in the vector of file paths 
 * returns:   none 
 * effects:   opens the given file and adds all of the words in that file to
 *            the hashTable index using the table's insert function
*/
void gerp::readWords(string &file, int index) {
    //create input stream and open the file 
    ifstream input;
    open_or_die(input, file);

    //variables to store a line in the file and number of that line in the file 
    string line;
    int lineNum = 1;

    //go through each line in the file
    while (getline(input, line)) {
        //create stream for the line and string to contain a word in the line
        istringstream ss(line);
        string word;

        //go through each word in the line
        while (ss >> word) {
            //remove non alpha numeric characters from the word
            string processed = stripNonAlphaNum(word);

            //add word to index if it has alpha numeric characters 
            if (not processed.empty()) 
                table.insert(processed, index, lineNum);
        }

        //increment line number
        lineNum++;
    }

    //close input file
    input.close();
}

/*
 * name:      newOutput
 * purpose:   updates output file to be the file provided by the client
 * arguments: a string with the name of an output file 
 * returns:   none 
 * effects:   closes the output file that was currently open and opens the new
 *            output file, updating variables that are used to store 
 *            informtion on that file 
*/
void gerp::newOutput(string &outputFile) {
    //close the current output file 
    output.close();

    //open new output file 
    open_or_die(output, outputFile);

    //update output file variables 
    curr_output = outputFile;
    outputLine = 0;
}

/*
 * name:      printSensitive 
 * purpose:   prints the case sensitive locations of the given word 
 * arguments: a string with a word that is being queried  
 * returns:   none 
 * effects:   gets all of the locations of the given case sensitive word in the
 *            directory and prints them out by calling a helper function. 
 *            Prints a message to query the word with insensitive command if 
 *            there are no instances of the case sensitive word in the 
 *            directory
*/
void gerp::printSensitive(string &word) {
    //get the case sensitive locations of the word
    WordLocations entry = table.getSensitiveWord(word);

    //print message if there are no locations of the case sensitive word
    if (entry.location.empty()) {
        output << word << " Not Found. Try with @insensitive or @i.\n";
    }

    //otherwise, print out word
    else {

        //get size of the locations vector
        int size = entry.location.size();
        //store the number of lines that have been printed
        int printed_lines = 0;

        //try and print each location, updating number of lines printed
        for (int i = 0; i < size; i++) {
            printed_lines += outputPaths(entry.location.at(i));
        }

        //update the current line we are at in the output file 
        outputLine += printed_lines;
    }
}

/*
 * name:      printInsensitive 
 * purpose:   prints all of the locations of the word in the directory 
 *            regardless of case sensitivity
 * arguments: a string with a word that was queried  
 * returns:   none 
 * effects:   gets all of the locations of the word, regardless of case 
 *            sensitive letters, and prints them out by calling a helper 
 *            function. Prints a message that the word is not found if there
 *            are no locations of that word in the directory
*/
void gerp::printInsensitive(string &word) {
    //get all of the locations of the case insensitive word 
    hashTable::Node node = table.getInsensitiveWord(word);

    //if there are no locations, print not found
    if (node.entries.empty()) { 
        output << word << " Not Found.\n";
    }

    //otherwise, print out locations
    else {

        //get size of the entries vector
        int size = node.entries.size();
        //int to store number of lines printed 
        int printed_lines = 0;

        //go through each case sensitive entry of the word 
        for (int i = 0; i < size; i++) {
            int entry_size = node.entries.at(i).location.size();

            //go through each location of word, update number of lines printed
            for (int j = 0; j < entry_size; j++) {
                printed_lines += outputPaths(node.entries.at(i).location.at(j));
            }
        }

        //update line we are at in output once finished printing
        outputLine += printed_lines;
    }
}

/*
 * name:      outputPaths
 * purpose:   prints the given location of a word to the output file
 * arguments: an Instance variable with a location in the directory   
 * returns:   returns 1 if a line was printed to the output file, 0 if no line
 *            was printed 
 * effects:   gets the file and line where the queried word was located in the
 *            directory, checking to make sure that the line was not printed
 *            already for case insensitive searches. Returns whether a line was
 *            printed to the output file in order to update the line in the 
 *            output file 
*/
int gerp::outputPaths(Instance &location) {
    //get the name of the file at the given index in the file paths vectory
    string path = filepaths.at(location.file_path_index);
    //open the file 
    ifstream input;
    open_or_die(input, path);

    string line;    //store line
    int line_number = 1, printed = 0;    //output line num, whether line printed
    bool found = false;     //store whether word was found 

    //go through each line until location in file is reached 
    while (getline(input, line) and (not found)) {
        //if line number reached, print location to output file 
        if (line_number == location.lineNum) {
            string temp = ":" + to_string(line_number) + ": " + line;
            path += temp;

            //make sure path was not printed, especially for insensitive 
            if ((not insensitive) or (checkOutput(path) and insensitive)) {
                output << path << endl;
                printed++;
            }   
            //indicate we found location
            found = true;
        }

        line_number++;
    }
    input.close();
    return printed;
}

/*
 * name:      checkOutput
 * purpose:   checks whether a line was already printed to output during the
 *            current query
 * arguments: a string with a path and location in the directory
 * returns:   returns true if the line is unique, false if it is not  
 * effects:   opens the output file and checks if the given location was 
 *            already printed during the current query
*/
bool gerp::checkOutput(const string &path) {
    //open output file 
    ifstream input;
    open_or_die(input, curr_output);

    //variables to store line and line number in output
    string line;
    int curr_line = 0;

    //go through each line in the output file 
    while (getline(input, line)) {
        //line already printed during current query, end loop & return false
        if ((line == path) and (curr_line >= outputLine)) {
            input.close();
            return false;
        }

        //increment line number 
        curr_line++;
    }

    //close file and return true
    input.close();
    return true;
}
