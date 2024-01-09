/*
 *  stringProcessing.h
 *  Rolando Ortega and Mateusz Zubrzycki 
 *  12/6/23
 *
 *  The stripNonAlphaNum function removes all leading and trailing non 
 *  alphanumeric characters from the string. It returns the string with just
 *  alphanumeric characters and nonalphanumeric characters that are within,
 *  but not leading nor trailing.   
 *
*/

#include <string>
#include <iostream>

using namespace std;

//function declaration 
string stripNonAlphaNum(string input);