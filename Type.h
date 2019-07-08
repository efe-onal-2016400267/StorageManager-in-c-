//
// Created by MacBook on 7.04.2019.
//

#ifndef STORAGEMANAGER_TYPE_H
#define STORAGEMANAGER_TYPE_H


#include <iostream>
#include <vector>
#include "Record.h"
using namespace std;

#define PAGESIZE 1024


typedef struct _Page
{
    int pageID;            //ID number of the page
    int numOfRecordsP;      //Number of records in the page
    //unsigned int numOfDeletedRecords;
    int maxNumOfRecords;    //maxNumOfRecords
    //int maxNumOfRecordsFunc();      //This function finds the maximum number of records in a page by simply dividing 1024 by record size.
    vector<Record> recordsP;
}Page;

class Type {
public:
    Type();     //Default constructor
    Type(string typeName, int numOfFields);     //Constructor
    vector<int> fields;     //We are going to hold the fields in an integer vector.
    string typeName;        //Type name must be specified so a file of the same name can be opened.
    int numOfRecords;       //Number of records is held in this integer.
    int numOfFields;        //Number fields is held in this integer.
    int numOfPages;         //This int holds the number of pages.
    int createRecord(vector<int> fields);   //This function will create a new record.
    int sizeOfOneRecord;    //What is the size of one record of the type.
    int createNewPage();    //This function created a new page.
    int writePage(int pageNumber);  //Write a page to a file.
    vector<int> readPage(int pageNumber);   //Read a page of a file.
    string searchRecord(int recordID); //This function searches for a record.
    vector<Record> records; //Records in the file.
    vector<Page> pages;     //Pages in the file.
    int deleteType();       //Function for delete type.
    vector<string> listRecords();   //This function lists all records.
    int updateRecord(vector<int> fields, int primaryKey);//This function updates a record.
    int deleteRecord(int primaryKey);//This function  deletes a record.
    int restoreType(string typeName, int numberOfPages1, int numberOfFields, int numberOfRecords);
};


#endif //STORAGEMANAGER_TYPE_H
