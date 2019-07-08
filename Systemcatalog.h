//
// Created by MacBook on 7.04.2019.
//

#ifndef STORAGEMANAGER_SYSTEMCATALOG_H
#define STORAGEMANAGER_SYSTEMCATALOG_H


#include <iostream>
#include <vector>
#include "Type.h"


using namespace std;

class Systemcatalog {
public:
    Systemcatalog();    //Default constructor
    int numberOfTypes;  //How many different types/files we have?
    vector<Type> Types; //This vector will hold the types so that they can be outputted easily as a list.
    int createType(string typeName, int numOfFields);   //This function is used to create a type. It creates a file with the same name as the type.
    int deleteType(string typeName);    //This function deletes a type, meaning deletes the file of the type.
    vector<string> listAllTypes();     //This function lists all types.
    int createRecord(string typeName, vector<int> fields);  //This function calls the createRecord function of associated type.
    string searchRecord(string typeName, int primaryKey);   //This function calls the search records function of associated type.
    int updateSystemCatalog();  //This function updated the system for every change in every table.
    vector<string> listRecordsOfType(string typeName);  //This function calls list record function of associated type.
    int updateRecord(string typeName, int primaryKey, vector<int> fields);//This function calls update record function of associated type.
    int deleteRecord(string typeName, int primaryKey);//This function calls delete record function of associated type.
    int createTypeAgain(string typeName, int numOfFields);//This function is used to restore the system after previous executions.
    int restoreSytemsType(int numOfFields, string typeName);
};


#endif //STORAGEMANAGER_SYSTEMCATALOG_H
