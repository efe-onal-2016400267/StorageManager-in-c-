//
// Created by MacBook on 7.04.2019.
//

#include "Systemcatalog.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
using namespace std;
//Constructor for system catalog.
Systemcatalog::Systemcatalog() {
    this->numberOfTypes = numberOfTypes;
    this->Types = Types;
    numberOfTypes = 0;
}

//This struct is used to customizes built-in sort() function for custom vectors.
struct less_than_key
{
    inline bool operator() (const Type& struct1, const Type& struct2)
    {
        return (struct1.typeName < struct2.typeName);
    }
};
//This function calls list record function of associated type.
vector<string> Systemcatalog::listRecordsOfType(string typeName) {
    vector<string> myVector;
    myVector.clear();
    for(int i = 0; i < this->Types.size(); i++){
        if(typeName == this->Types[i].typeName){
            return this->Types[i].listRecords();
        }
    }
    return myVector;
}
//This function calls update record function of associated type.
int Systemcatalog::updateRecord(string typeName, int primaryKey, vector<int> fields) {
    for(int i = 0; i < this->Types.size(); i++){
        if(typeName == this->Types[i].typeName){
            Types[i].updateRecord(fields, primaryKey);
            updateSystemCatalog();
            return 0;
        }
    }
    cout << "no such type"<< endl;
    return -1;
}

//This function creates a file with typeName and adds creates a new type instance putting it to the Types vector.
int Systemcatalog::createType(string typeName, int numOfFields) {
    for(int i = 0; i < Types.size(); i++){
        if(typeName == Types[i].typeName){
            return -1;
        }
    }
    Type newType = Type(typeName, numOfFields);
    newType.sizeOfOneRecord = 4 * numOfFields;
    Types.push_back(newType);
    ofstream outfile(typeName + ".txt");
    outfile.close();
    this->numberOfTypes++;
    this->updateSystemCatalog();
    return 0;
}

//This function is used to add the types that were created during previous executions of the program.
//It is called at the top of the main function.
int Systemcatalog::createTypeAgain(string typeName, int numOfFields) {
    for(int i = 0; i < Types.size(); i++){
        if(typeName == Types[i].typeName){
            return -1;
        }
    }
    Type newType = Type(typeName, numOfFields);
    newType.sizeOfOneRecord = 4 * numOfFields;
    Types.push_back(newType);
    //fstream outfile(typeName + ".txt");
    //outfile.close();
    //this->updateSystemCatalog();
    return 0;
}
//This function puts the names of all types in a vector myVector which is then returned.
vector<string> Systemcatalog::listAllTypes() {
    vector<string> myVector;
    myVector.clear();
    sort(this->Types.begin(), this->Types.end(), less_than_key());
    for(int i = 0; i < Types.size(); i++){
        myVector.push_back(this->Types[i].typeName);
        cout << this->Types[i].typeName<< endl;
    }
    return myVector;
}
//This function calls create record function of associated type.
int Systemcatalog::createRecord(string typeName, vector<int> fields) {
    for(int i = 0; i < this->Types.size(); i++){
        if(typeName == this->Types[i].typeName){
            this->Types[i].createRecord(fields);
            this->updateSystemCatalog();
            return 0;
        }
    }
    cout << "No  such type!!!" << endl;
    return -1;
}
//This function calls search record function of associated type.
string Systemcatalog::searchRecord(string typeName, int primaryKey) {
    string myString = "";
    for(int i = 0; i < this->Types.size(); i++){
        if(typeName == this->Types[i].typeName){
            return this->Types[i].searchRecord(primaryKey);
            //return 0;
        }
    }
    return myString;
}
//This function updates the system catalog after every write operation to any of the files.
//It fills the file by making use of the Types vector which holds numOfRecords, numOfPages, typeName, and numOfFields.
int Systemcatalog::updateSystemCatalog() {
    remove("SystemCatalog.txt");
    ofstream ofile;
    ofile.open("SystemCatalog.txt");
    ofile << to_string(this->numberOfTypes) << endl;
    for(int i = 0; i < numberOfTypes; i++){
        ofile << Types[i].typeName << " " << Types[i].typeName + ".txt" << " " << to_string(Types[i].numOfRecords) << " " << to_string(Types[i].numOfFields) << " " << to_string(Types[i].numOfPages) << endl;
    }
    return 0;
}
//This function deletes the file of a type, deleting all of its records and decrements numberOfTypes.
int Systemcatalog::deleteType(string typeName) {
    for(int i = 0; i < this->Types.size(); i++){
        if(this->Types[i].typeName == typeName){
            string filename = this->Types[i].typeName + ".txt" ;
            remove(filename.c_str());
            this->Types.erase(this->Types.begin() + i);
            this->numberOfTypes--;
            this->updateSystemCatalog();
            return 0;
        }
    }
    return 0;
}
//This function calls delete record function of associated type.
int Systemcatalog::deleteRecord(string typeName, int primaryKey) {
    for(int i = 0; i < this->Types.size(); i++){
        if(typeName == this->Types[i].typeName){
            this->Types[i].deleteRecord(primaryKey);
            updateSystemCatalog();
            return 0;
        }
    }
    cout << "no such record"<< endl;
    return -1;
}
int Systemcatalog::restoreSytemsType(int numOfFields, string typeName) {
    createTypeAgain(typeName, numOfFields);
    return 0;
}
