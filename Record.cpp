//
// Created by MacBook on 7.04.2019.
//

#include "Record.h"
#include <iostream>
//These are two constructors for class Record.
Record::Record(string typeName, int numberOfFields) {
    this->fields = fields;
    this->numberOfFields = numberOfFields;
    this->typeName = typeName;
}
Record::Record(const Record &obj) {
    this->numberOfFields = obj.numberOfFields;
    this->typeName = obj.typeName;
    for(int i = 0; i < obj.fields.size(); i++){
        this->fields.push_back(obj.fields[i]);
    }
}