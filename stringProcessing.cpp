/*
 *  stringProcessing.cpp
 *  Rolando Ortega and Mateusz Zubrzycki 
 *  12/6/23
 *
 *  Contains an implementation of the stripNonAlphaNum function.   
 *
*/

#include "stringProcessing.h"


/*
 * name:      stripNonAlphaNum 
 * purpose:   removes leading and trailing non alphanumeric chars from the 
 *            inputted string 
 * arguments: a string with input 
 * returns:   a processed string   
 * effects:   strips any leading and trailing non alphanumeric characters from 
 *            the string and returns a parsed version of the inputted string 
 *            that contains alphanumeric characters and nay non alphanumeric 
 *            characters in the middle of the string 
 *              
*/
string stripNonAlphaNum(string input) {
    //bool to contain whether we reached alphanumeric char 
    bool alpha = false;

    //find index of first alphanumeric char 
    int index_front = 0;
    while (not alpha and index_front < input.length()){ 
        if (isalnum(input[index_front])) 
            alpha = true;
        
        else 
            index_front++;
    }

    //if all chars are non alphanumeric, return empty string 
    if (index_front == input.length()) 
        return "";

    //set bool back to false 
    alpha = false;
    //find index of last alphanumeric char 
    int index_back = input.length() - 1;
    while (not alpha) {
        if (isalnum(input[index_back]))
            alpha = true;

        else 
            index_back--;
    }

    //remove leading and trailing non alphanumeric chars and return new string 
    return input.substr(index_front, index_back - index_front + 1);
}