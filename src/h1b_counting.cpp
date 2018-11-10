#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <iomanip>
using namespace std;

// The input file that has all the information. This file will be analyzed so that the certified applications with jobs and states
// be counted and written to the output file.
ifstream myfile("../input/h1b_input.csv");
// Other input file tests are (if you add them to the input folder):
/*
ifstream myfile("../input/H1B_FY_2014.csv");
ifstream myfile("../input/H1B_FY_2015.csv");
ifstream myfile("../input/H1B_FY_2016.csv");
*/

// The output file for the top 10 occupations that have the most number of certified applications
ofstream writeJobFile;
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
int jobJ;
int stateJ;
// And increment variable
int j;

// Creating a map of jobs. The key to be the job name and the value to be the number of certified applications for that job.
map<string,int> myJobMap;
// Creating a map of states. The key to be the state name and the value to be the number of certified applications for that state.
map<string,int> myStateMap;
// An iterator for the maps that just created. We can use this iterator interchangeably.
map<string,int>::iterator itMap;

// Defining a comparator function
typedef std::function<bool(std::pair<std::string, int>, std::pair<std::string, int>)> Comparator;

// Defining a lambda function to compare two pairs. It will compare two pairs using second field
// To compare two states. The job that has more number of certified applications will be ranked higher.
// The jobs that have a same number of certified applications will be ranked based on their alphabetical names
Comparator compJobFunctor =
        [](std::pair<std::string, int> elem1 ,std::pair<std::string, int> elem2)
        {
            if (elem1.second > elem2.second){
                return elem1.second >= elem2.second;
            }else if (elem1.second == elem2.second){
                return elem1.first < elem2.first;
            }else{
                return false;
            }
        };
// Defining a lambda function to compare two pairs. It will compare two pairs using second field
// To compare two states. The state that has more certified jobs will be ranked higher.
// The states that have a same number of certified applications will be ranked based on their alphabetical names
Comparator compStateFunctor =
        [](std::pair<std::string, int> elem1 ,std::pair<std::string, int> elem2)
        {
            if (elem1.second > elem2.second){
                return elem1.second >= elem2.second;
            }else if (elem1.second == elem2.second){
                return elem1.first < elem2.first;
            }else{
                return false;
            }
        };

void analyzeTheHeaderOfInputFile();
void readInputFileForStateAndOccuptionMaps (int statusJ, int jobJ, int stateJ);

int main() {

    // Initializing the variables
	writeJobFile.open("../output/top_10_occupations.txt");
	writeStateFile.open("../output/top_10_states.txt");
    statusJ = 0;
    stateJ = 0;
    jobJ = 0;
    lineNum = 0;
    pos = 0;
    j = 0;
    // Read the first line of the input file.
    // Analyze the header of the input file so that we know the indices that are relevant and
    // we should add to the Value of the Map Keys later
    analyzeTheHeaderOfInputFile();

    // Here after we know which header indices are the ones that we should count we read the rest of the input file
    readInputFileForStateAndOccuptionMaps(statusJ, jobJ, stateJ);

    // Now that we have read the input file, we close it.
    myfile.close();


    // The maps that we just created, will not be sorted.
    // So we need to sort them in a comparator based on
    // the certified number of applications and the alphabetical order of the names (if the number is equal)
    std::set<std::pair<std::string, int>, Comparator> jobsSorted(
			myJobMap.begin(), myJobMap.end(), compJobFunctor);

    std::set<std::pair<std::string, int>, Comparator> statesSorted(
			myStateMap.begin(), myStateMap.end(), compStateFunctor);


    // The header for the ranked occupations
    writeJobFile << "TOP_OCCUPATIONS;NUMBER_CERTIFIED_APPLICATIONS;PERCENTAGE" << endl;

    // First sum all the jobs' certified application numbers
    int jobSums = 0;
    for (std::pair<std::string, int> element : jobsSorted){
        jobSums += element.second;
    }

    j = 0;
    // Now output the jobs with the status of CERTIFIED and the percentage of the CER applications to overall CER applications for that occupation
    for (std::pair<std::string, int> element : jobsSorted){
        if (j<10){
            //cout << element.first << ";" << element.second << ";" << setprecision(1) << fixed << (double)element.second/jobSums*100 << "%" <<std::endl;
            writeJobFile << element.first << ";" << element.second << ";" << setprecision(1) << fixed << (double)element.second/jobSums*100 << "%" <<std::endl;
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
    for (std::pair<std::string, int> element : statesSorted){
        stateSums += element.second;
    }

    j = 0;
    // Now output the states with the status of CERTIFIED and the percentage of the CER applications to overall CER applications for that state name.
    for (std::pair<std::string, int> element : statesSorted){
        if (j<10){
            //cout << element.first << ";" << element.second << ";" << setprecision(1) << fixed << (double)element.second/stateSums*100 << "%" <<std::endl;
            writeStateFile << element.first << ";" << element.second << ";" << setprecision(1) << fixed << (double)element.second/stateSums*100 << "%" <<std::endl;
            j++;
        }else{
            break;
        }
    }

    // Closing the output files
    writeJobFile.close();
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
                jobJ = j;
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



void readInputFileForStateAndOccuptionMaps (int statusJ, int jobJ, int stateJ){
    // If the appropriate headers existed, we start counting the jobs and states that have a certified application
    // Then the occupation name will be put into a map and the certified applications will add to its value.
    // A similar approach is taken for the states map.
    if  (jobJ>0 && statusJ>0){
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
                // I noticed some of the job files had a extra semicolon as for the employer name, for example for Chase we have "Chase &AMP; CO."
                // So I ignored the token number and decreased the increment that we had done using &AMP symbol wherever applicable.
                // Then I put the occupation name and state in the maps associated with them.
                // Putting the occupation in the Map (increment the Value if the Key exist in the map or create the Key in the map if
                // the Key does n't exist in the Map.
                // The occupation map
                if (j==jobJ && status == "CERTIFIED"){
                    if ( myJobMap.find(token) == myJobMap.end() ){
                        token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
                        myJobMap.insert(make_pair(token,0));
                    }
                    for (itMap = myJobMap.begin();itMap != myJobMap.end();++itMap){
                        if(itMap->first == token){
                            myJobMap[itMap->first]++;
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

