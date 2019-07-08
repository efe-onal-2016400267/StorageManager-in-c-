#include <iostream>
#include "Systemcatalog.h"
#include <vector>
#include <sstream>
#include <fstream>
template <class Container>


void split1(const string& str, Container& cont){
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(cont));
}
//This function restores the system so it can execute in  accordance to previous executions.
//SystemCatalog.txt is read line by line and every type is created again in the Types vector.
Systemcatalog updateDB(Systemcatalog myCatalog){
    vector<string> words;
    int numberOfTypes1;
    ifstream infile("SystemCatalog.txt");  //infile is the input file.
    string line;
    string typeName;
    int numberOfFields;
    int numberOfPages;
    int numberOfRecords;
    if (infile.is_open()) {
        //We split the file into lines.
        getline(infile, line);
        //We split the lines into words.
        split1(line, words);
        //infile.close();
        numberOfTypes1 = stoi(words[0]);
        myCatalog.numberOfTypes = numberOfTypes1;
        words.clear();
        for (int i = 0; i < numberOfTypes1; i++) {
            getline(infile, line);
            split1(line, words);
            if(words.size() == 0){
                return myCatalog;
            }
            typeName = words[0];
            numberOfFields = stoi(words[3]);
            numberOfPages = stoi(words[4]);
            numberOfRecords = stoi(words[2]);
            //myCatalog.createTypeAgain(typeName, numberOfFields);

            words.clear();
            myCatalog.restoreSytemsType(numberOfFields, typeName);
            for(int p = 0; p < myCatalog.Types.size(); p++){
                if(myCatalog.Types[p].typeName == typeName){
                    myCatalog.Types[p].numOfPages = numberOfPages;
                    myCatalog.Types[p].numOfRecords = numberOfRecords;
                }
            }

        }
    }
    infile.close();
    return myCatalog;
};
int main(int argc, char* argv[]) {
    if(argc != 3){
        return 1;
    }
    cout << "input file: " << argv[1] << endl;
    cout << "output file: " << argv[2] << endl;
    Systemcatalog myCatalog;
    myCatalog.numberOfTypes=0;
    myCatalog.Types.clear();
    myCatalog=updateDB(myCatalog);
    ifstream infile(argv[1]);
    string line;
    vector<string> words;
    vector<int> fields;
    vector<string> output;
    vector<string> myVec;
    vector<string> yourVector;
    int i = 0;
    if (infile.is_open())  //if the input file is open.
    {

        while ( infile.good() ) //if the file is not completely read.
        {
            fields.clear();
            words.clear();
            //We split the file into lines.
            getline(infile, line);
            //We split the lines into words.
            split1(line, words);
            //At each iteration, we create a process, p, and push it into Processes vector.
            for(int k = 0; k < words.size(); k++){
                if(words[k] == " "){
                    words.erase(words.begin()+k);
                }
            }
            if(words.size()!=0){
                if(words[1] == "type"){     //Type operation
                    if(words[0] == "create"){
                        myCatalog.createType(words[2], stoi(words[3]));
                    }
                    else if(words[0] == "delete"){
                        myCatalog.deleteType(words[2]);
                    }
                    else if(words[0] == "list"){
                        myVec = myCatalog.listAllTypes();
                        for(int p = 0; p < myVec.size(); p++){
                            output.push_back(myVec[p]);
                        }
                    }
                    else{
                        cout << "bad input" << endl;
                    }
                }
                else if(words[1] == "record"){
                    if(words[0] == "create"){
                        for(int k = 3; k < words.size(); k++){
                            fields.push_back(stoi(words[k]));
                        }
                        myCatalog.createRecord(words[2], fields);
                    }
                    else if(words[0] == "delete"){
                        myCatalog.deleteRecord(words[2], stoi(words[3]));
                    }
                    else if(words[0] == "update"){
                        for(int k = 3; k < words.size(); k++){
                            fields.push_back(stoi(words[k]));
                        }
                        myCatalog.updateRecord(words[2], stoi(words[3]), fields);
                    }
                    else if(words[0] == "search"){
                        string myString;
                        myString = myCatalog.searchRecord(words[2], stoi(words[3]));
                        if(myString != ""){
                            output.push_back(myString);
                        }
                    }
                    else if(words[0] == "list"){
                        yourVector=myCatalog.listRecordsOfType(words[2]);
                        for(int m = 0; m < yourVector.size(); m++){
                            output.push_back(yourVector[m]);
                        }
                    }
                }
            }
        }

        infile.close(); //we close the file at copmletion.
    }
    ofstream offile; //offile is the name of the output file.
    offile.open(argv[2]); //name of the output file is output.txt
    //First we print the output1 vector in a for loop.
    for(int w = 0; w < output.size(); w++){
        cout << output[w] << endl;
        offile << output[w] << endl;
    }
}