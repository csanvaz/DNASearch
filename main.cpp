//
//  Project 1- DNA Profiling
//  CS 251
//  Visual Studio
//  Cynthia Sanchez Vazquez 
// 
// Creativity:  I decided to let the user have the option of creating their own database file. This is done through
//              function addPersonInformation. The function is declared in the main via a addInformation command from 
//              user. 
//

#include "ourvector.h"
#include <fstream>
#include <sstream>

using namespace std;

//Collection of database person information
struct PersonData {
    string name;
    ourvector <int> strCount;
};

//Open database file and checking for file errors
void readDataBase(string filename, ourvector<string>& data, ourvector<PersonData>& collection)
{
    ifstream infile(filename);
    string line, dnaString, personName, personFrequency;

    PersonData collect;

    if (!infile.is_open()) {
        cout << " Error: unable to open " << "'" << filename << "'" << endl;
        return;
    }

    if (collection.size()) {
        collection.clear();
    }


    infile >> line; //read entire line from file
    stringstream stringReader(line); //constructing string reader
    getline(stringReader, dnaString, ',');//parse everything in string until first comma


    while (getline(stringReader, dnaString, ',')) { //keep reading into next string until the end of line
        data.push_back(dnaString);
    }

    while (!infile.eof()) { 
        infile >> line;

        if (infile.good()) {
            stringstream stringReader2(line);
            getline(stringReader2, personName, ',');
            collect.name = personName;

            while (getline(stringReader2, personFrequency, ',')) { //keep reading into next string until the end of line
                collect.strCount.push_back(stoi(personFrequency));
            }
            collection.push_back(collect);
            collect.strCount.clear();
        }
    }

    infile.close();
}

void addPersonInformation(string filename) {
    ofstream outFS; // Output file stream

     // Open file
    outFS.open(filename);

    if (!outFS.is_open()) {
        cout << " Error: unable to open " << "'" << filename << "'" << endl;
        return;
    }
    
    //UserInputs personInformation
    string userInput;
    string userStrFrequencies;
    
    cout << "Adding Information to Database" << endl;
    cout << "Enter First Name and STR frequency: ";
    cin.ignore(100, '\n');
    getline(cin, userInput);
    


    // Write to file
    outFS << userInput << endl;

    // Done with file, so close
    outFS.close();
}

//Opening DNA file and checking for file errors
void readDNAData(string filename, ourvector<char>& dnaData) {
    ifstream inFile(filename);
    string strData;

    dnaData.clear();

    if (!inFile.is_open()) {
        cout << " Error: unable to open " << "'" << filename << "'" << endl;
        return;
    }


    inFile >> strData; //read entire line from file

    for (int i = 0; i < strData.size(); i++) {
        dnaData.push_back(strData[i]);
    }
    
}


bool checkForMatch(string currentString, ourvector<char>& currentDna, int strIndex) {
    if (strIndex + currentString.size() >= currentDna.size()) {
        return false;
    }

    for (int i = 0; i < currentString.size(); i++) {
        if (currentString[i] != currentDna[i + strIndex]) {
            return false;
        }
    }
    return true;
}


//Checking how often STR labels match with chosen DNA file
void processingData(ourvector<char> dnaCollection, ourvector<string> strMembers, ourvector<int>& stringCount) {

    bool repetition = false;
    ourvector<int> repetitionCount;

    //Error checking below
    if (strMembers.size() == 0) {
        cout << "No database loaded." << endl;
        return;
    }

    if (dnaCollection.size() == 0) {
        cout << "No DNA loaded." << endl;
        return;
    }

    cout << "Processing DNA..." << endl;
    stringCount.clear();

    for (int i = 0; i < strMembers.size(); i++) {
        repetitionCount.push_back(0);
        stringCount.push_back(0);
    }

    //Matching process begins
    for (int i = 0; i < strMembers.size(); i++) {
        string strL = strMembers[i]; //helpful for debugging and matching
        for (int j = 0; j < dnaCollection.size(); j++) {
            char dnaP = dnaCollection[j];
            if (dnaP == strL[0]) {
                if (checkForMatch(strL, dnaCollection, j)) { 
                    j = j + (strL.size() - 1);
                    if (repetitionCount[i] == 0) {
                        repetition = true;
                    }

                    if (repetition) { 
                        repetitionCount[i]++;
                    }
                }
                else {
                    stringCount[i] = max(repetitionCount[i], stringCount[i]);
                    repetition = false;
                    repetitionCount[i] = 0;
                }
            }
            else {
                stringCount[i] = max(repetitionCount[i], stringCount[i]);
                repetition = false;
                repetitionCount[i] = 0;
            }
        }
    }
    

}

//Searching for STR label frequencies with database information to identify individual that matches
void searchingData(ourvector<PersonData> searchInformation, ourvector<char> dnaCollection, ourvector<int> processingInformation)
{
    //Error Checking
    if (searchInformation.size() == 0) {
        cout << "No database loaded." << endl;
        return;
    }

    if (dnaCollection.size() == 0) {
        cout << "No DNA loaded." << endl;
        return;
    }

    if (processingInformation.size() == 0) {
        cout << "No DNA processed." << endl;
        return;
    }

    cout << "Searching database..." << endl;
    string personFound;
    int count = 0;
    int indexIncrease;

               
    for (int i = 0; i < processingInformation.size(); i++) {
         int strFreq = processingInformation[0];//Set to index 0 to address situations where a match isn't found
         for (int j = 0; j < searchInformation.size(); j++)
         {
             int check = searchInformation[j].strCount[0]; //assuring match happens at 1st element in strCount
            if (check == strFreq) { //if statements to help make sure there is a match
               
                for (int m = 1; m < processingInformation.size(); m++) {
                    int strFreq2 = processingInformation[m];
                    for (int k = 1; k < searchInformation[k].strCount.size(); k++)
                    {
                        int personStr2 = searchInformation[j].strCount[k]; //needs fixing the k is not incrementing
                        if (strFreq2 == personStr2) {
                            personFound = searchInformation[j].name; //catch the name at index j
                            break;
                        }
                        else {
                            break;
                        }
                        break;
                    }
                }
            }
        }
    }

    if (personFound.size() != 0) {
        cout << "Found in database!  DNA matches: " << personFound << endl;
    }
    else {
        cout << "Not found in database." << endl;
    }
}

void displayData(ourvector<PersonData>& dataBaseDisplay, ourvector<char>& dnaDisplay,
    ourvector<int> strAmount, ourvector<string> strTitles) {                                                                                   

    if (dataBaseDisplay.size() != 0) {
        cout << "Database loaded: " << endl;
        for (int i = 0; i < dataBaseDisplay.size(); i++) {
            cout << dataBaseDisplay[i].name; 
            for (int j = 0; j < dataBaseDisplay[i].strCount.size(); j++) { 
                cout << " " << dataBaseDisplay[i].strCount[j];
            }
            cout << endl;
        }
    }
    else {
        cout << "No database loaded." << endl;
    }


    if (dnaDisplay.size() != 0) {
        cout << "DNA loaded: " << endl;
        for (int k = 0; k < dnaDisplay.size(); k++) {
            cout << dnaDisplay[k];
        }
        cout << endl << endl;
    }
    else {
        cout << "No DNA loaded." << endl << endl;
    }

    if (strAmount.size() != 0) {
        cout << "DNA processed, STR counts:" << endl;
        for (int i = 0; i < strTitles.size(); i++) {

            cout << strTitles[i] << ": " << strAmount[i] << endl;
        }
    }
    else {
        cout << "No DNA has been processed." << endl << endl;
    }

}



int main() {
   
    string userCommand;
    string fileInput, fileInput2;
    ourvector<string> strLabels;
    ourvector<PersonData> personInformation;
    ourvector<char> dnaString;
    ourvector<int> strProcessing;
   

    cout << "Welcome to the DNA Profiling Application.\n";

    //Command Loop
    while (true) {
        
        cout << "Enter command or # to exit : ";
        cin >> userCommand;
        
        if (userCommand == "#") {
            exit(0);
        }
         

        if (userCommand == "load_db") {
            cin >> fileInput;
            cout << "Loading database..." << endl;
            readDataBase(fileInput, strLabels, personInformation);
            
        }
        
        if (userCommand == "load_dna") {
            cin >> fileInput2;
            cout << "Loading DNA..." << endl;
            readDNAData(fileInput2, dnaString);
            
        }
        

        if (userCommand == "display") {

            displayData(personInformation, dnaString, strProcessing, strLabels);
        }

        if (userCommand == "process") {
            processingData(dnaString, strLabels, strProcessing);
        }

        if (userCommand == "search") {
            searchingData(personInformation, dnaString, strProcessing);
        }

        if (userCommand == "addInformation") {
            cin >> fileInput;
            addPersonInformation(fileInput);
        }
        
    }

    return 0;
}
