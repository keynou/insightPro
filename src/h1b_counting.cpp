#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <iomanip>
using namespace std;

// The input file that has all the information. This file will be analyzed so that the certified applications with occupations and states
// be counted and written to the output file.
ifstream myfile("../input/h1b_input.csv");
//ifstream myfile("../input/H1B_FY_2015.csv");
// Other input file tests are (if you add them to the input folder):
/*
ifstream myfile("../input/H1B_FY_2014.csv");
ifstream myfile("../input/H1B_FY_2015.csv");
ifstream myfile("../input/H1B_FY_2016.csv");
*/

// The output file for the top 10 occupations that have the most number of certified applications
ofstream writeOccupationFile;
// The output file for the top 10 states that have the most number of certified applications
ofstream writeStateFile;

//ofstream debugFile("../output/errors.txt");

// Each time we will read the input file lines into this string.
// The line number, will indicate which line we are reading in the input file.
string line;
int lineNum;

// The location of the first word in the line that we just created will be know by the pos.
// The word is extracted from the line and put into the token variable to be analyzed for the output files.
size_t pos;
std::string token;

// Status of each application
string status;

// Initializing the header indices for the status to check, the occupation, and the state.
int statusJ;
int occupationJ;
int stateJ;
// And increment variable
int j;

// Creating a map of occupations. The key to be the occupation name and the value to be the number of certified applications for that occupation.
std::map<std::string, int> myOccupationMap;
// Creating a map of states. The key to be the state name and the value to be the number of certified applications for that state.
std::map<std::string, int> myStateMap;
// An iterator for the maps that just created. We can use this iterator interchangeably.
map<string,int>::iterator itMap;

// Define a comparator for the map elements when we put them in a set
struct myComparator
{
    template<typename T>
	bool operator()(const T& elem1, const T& elem2) const
	{
		if (elem1.second > elem2.second){
            return elem1.second >= elem2.second;
        }else if (elem1.second == elem2.second){
            return elem1.first < elem2.first;
        }else{
            return false;
        }
	}
};

// Header names for the functions to use
void analyzeTheHeaderOfInputFile();
void readInputFileForStateAndOccuptionMaps (int statusJ, int occupationJ, int stateJ);


int main() {

    // Initializing the variables
	writeOccupationFile.open("../output/top_10_occupations.txt");
	writeStateFile.open("../output/top_10_states.txt");
    statusJ = 0;
    stateJ = 0;
    occupationJ = 0;
    lineNum = 0;
    pos = 0;
    j = 0;
    // Read the first line of the input file.
    // Analyze the header of the input file so that we know the indices that are relevant and
    // we should add to the Value of the Map Keys later
    analyzeTheHeaderOfInputFile();

    // Here after we know which header indices are the ones that we should count we read the rest of the input file
    readInputFileForStateAndOccuptionMaps(statusJ, occupationJ, stateJ);

    // Now that we have read the input file, we close it.
    myfile.close();



    // The maps that we just created, will not be sorted.
    // So we need to sort them in a comparator based on
    // the certified number of applications and the alphabetical order of the names (if the number is equal)
	std::set<std::pair<std::string,int>, myComparator> occupationsSorted(myOccupationMap.begin(), myOccupationMap.end());
	std::set<std::pair<std::string,int>, myComparator> statesSorted(myStateMap.begin(), myStateMap.end());


    // The header for the ranked occupations
    writeOccupationFile << "TOP_OCCUPATIONS;NUMBER_CERTIFIED_APPLICATIONS;PERCENTAGE" << endl;

    // First sum all the occupations' certified application numbers
    int occupationSums = 0;
    for (std::set<std::pair<std::string,int> >::iterator it=occupationsSorted.begin(); it!=occupationsSorted.end(); ++it){
        occupationSums += it->second;
    }

    j = 0;
    // Now output the occupations with the status of CERTIFIED and the percentage of the CER applications to overall CER applications for that occupation
    for (std::set<std::pair<std::string,int> >::iterator it=occupationsSorted.begin(); it!=occupationsSorted.end(); ++it){
        if (j<10){
            //cout << it->first << ";" << it->second << ";" << setprecision(1) << fixed << (double)it->second/occupationSums*100 << "%" <<std::endl;
            writeOccupationFile << it->first << ";" << it->second << ";" << setprecision(1) << fixed << (double)it->second/occupationSums*100 << "%" <<std::endl;
            j++;
        }else{
            break;
        }
    }

    // The header for the ranked states
    writeStateFile << "TOP_STATES;NUMBER_CERTIFIED_APPLICATIONS;PERCENTAGE" << endl;

    // For Sorting the States
    // First sum all the states' certified application numbers
    int stateSums = 0;
    for (std::set<std::pair<std::string,int> >::iterator it=statesSorted.begin(); it!=statesSorted.end(); ++it){
        stateSums += it->second;
    }

    j = 0;
    // Now output the states with the status of CERTIFIED and the percentage of the CER applications to overall CER applications for that state name.
    for (std::set<std::pair<std::string,int> >::iterator it=statesSorted.begin(); it!=statesSorted.end(); ++it){
        if (j<10){
            //cout  << it->first << ";" << it->second << ";" << setprecision(1) << fixed << (double)it->second/stateSums*100 << "%" <<std::endl;
            writeStateFile << it->first << ";" << it->second << ";" << setprecision(1) << fixed << (double)it->second/stateSums*100 << "%" <<std::endl;
            j++;
        }else{
            break;
        }
    }

    // Closing the output files
    writeOccupationFile.close();
    writeStateFile.close();
    return 0;
}


void analyzeTheHeaderOfInputFile(){
    // Reading the first line of the input file to extract for the headers that are important for the analysis
    if (myfile.peek() != EOF) {
        getline(myfile, line);
        while ((pos = line.find(";")) != std::string::npos) {
            token = line.substr(0, pos);
            j++;
            //std::cout << j << " :  "<< token << std::endl;
            //debugFile << j << " :  "<< token << std::endl;
            if (token.find("STATUS") != std::string::npos) {
                statusJ = j;
            }
            if (token.find("SOC_NAME") != std::string::npos) {
                occupationJ = j;
            }
            if (stateJ == 0 && token.find("WORK") != std::string::npos) {
                    if (token.find("STATE") != std::string::npos) {
                        stateJ = j;
                    }
            }

            line.erase(0, pos + 1);
        }
    }
}



void readInputFileForStateAndOccuptionMaps (int statusJ, int occupationJ, int stateJ){
    // If the appropriate headers existed, we start counting the occupations and states that have a certified application
    // Then the occupation name will be put into a map and the certified applications will add to its value.
    // A similar approach is taken for the states map.
    if  (occupationJ>0 && statusJ>0){
        while (myfile.peek() != EOF) { // && lineNum<300
            getline(myfile, line);
            j = 0;
            // We use the semicolon as the delimiter parameter and we extract each column information
            while ((pos = line.find(";")) != std::string::npos) {
                token = line.substr(0, pos);
                j++;
                //std::cout << j << " :  "<< token << std::endl;
                //debugFile << j << " :  "<< token << std::endl;
                line.erase(0, pos + 1);
                if (j == statusJ){
                    status = token;
                }
                if (token.find("&AMP") != std::string::npos){ //token=="" || token == " " ||
                    j--;
                }
                // I noticed some of the occupation files had a extra semicolon as for the employer name, for example for Chase we have "Chase &AMP; CO."
                // So I ignored the token number and decreased the increment that we had done using &AMP symbol wherever applicable.
                // Then I put the occupation name and state in the maps associated with them.
                // Putting the occupation in the Map (increment the Value if the Key exist in the map or create the Key in the map if
                // the Key does n't exist in the Map.
                // The occupation map
                if (j==occupationJ && status == "CERTIFIED"){
                    if ( myOccupationMap.find(token) == myOccupationMap.end() ){
                        token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
                        myOccupationMap.insert(make_pair(token,0));
                    }
                    for (itMap = myOccupationMap.begin();itMap != myOccupationMap.end();++itMap){
                        if(itMap->first == token){
                            myOccupationMap[itMap->first]++;
                        }
                    }
                }
                // The states map
                if (j==stateJ && status == "CERTIFIED"){
                    if ( myStateMap.find(token) == myStateMap.end() ){
                        token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
                        myStateMap.insert(make_pair(token,0));
                    }
                    for (itMap = myStateMap.begin();itMap != myStateMap.end();++itMap){
                        if(itMap->first == token){
                            myStateMap[itMap->first]++;
                        }
                    }
                }
            }
            // Keep track of the line number we just read from the input file.
            lineNum++;
        }
    }
}

