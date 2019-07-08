//
// Created by MacBook on 7.04.2019.
//

#ifndef STORAGEMANAGER_RECORD_H
#define STORAGEMANAGER_RECORD_H


#include <iostream>
#include <vector>
using namespace std;

class Record {
public:
    Record();   //Default constructor
    Record(string typeName, int numberOfFields);   //Contructor.
    Record (const Record &obj);
    vector<int> fields;
    int numberOfFields;
    string typeName;
    unsigned int recordSize;
    int getRecordSize();

};


#endif //STORAGEMANAGER_RECORD_H
