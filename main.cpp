/*
 *  main.cpp
 *  Rolando Ortega and Mateusz Zubrzycki 
 *  12/11/23
 * 
 *  Creates and runs a new gerp with input directory and initial output file 
 *  that are provided by the client. Gerp first building an index of all of 
 *  the unique locations of words in that directory, and then produces results
 *  of queries from the client based on the index. The results of those 
 *  queries are stored in output files provided by the client. 
 *
*/

#include "gerp.h"
#include <string>
#include <iostream>

using namespace std;

/*
 * name:      main
 * purpose:   creates and runs a new gerp 
 * arguments: a int with number of arguments and an array with the arguments 
 * returns:   an int of whether the program finished successfully 
 * effects:   prints error if client did not produce correct number of 
 *            arguments or valid zap or unzap commands. Creates a new 
 *            gerp and runs the query loop, prints out error and departing 
 *            messages.  
*/
int main(int argc, char *argv[]) {
    //if client does not input correct arguments, print error message
    if (argc != 3) {
        cerr << "Usage: ./gerp inputDirectory outputFile" << endl;
        exit(EXIT_FAILURE);
    }

    //try to create and run new gerp 
    try {
        //create new gerp
        gerp new_gerp(argv[1], argv[2]);

        //run query loop
        new_gerp.handleQuery(cin);

        //print departing message
        cout << "Goodbye! Thank you and have a nice day.\n";
    }

    //print error message if indexing was not successful 
    catch (const runtime_error &e) {
        cerr << "Could not build index, exiting." << endl;
    }

    //return 0 once the program has successfully completed 
    return 0;
}