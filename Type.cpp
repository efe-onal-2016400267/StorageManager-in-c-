//
// Created by MacBook on 7.04.2019.
//

#include "Type.h"
#include <fstream>
#include <stdio.h>
using namespace std;

//This struct is used to customizes built-in sort() function for custom vectors.
struct less_than_key1
{
    inline bool operator() (const string struct1, const string struct2)
    {
        int i, j;
        string myString;
        string yourString;
        for(int p = 0; p < struct1.length();  p++){
            if(&struct1.at(p) == " " || p == struct1.length()-1){
                i = stoi(myString);
                break;
            }
            myString = myString + struct1.at(p);
        }
        for(int p = 0; p < struct2.length();  p++){
            if(&struct2.at(p) == " " || p == struct2.length()-1){
                j = stoi(yourString);
                break;
            }
            yourString = yourString + struct2.at(p);
        }
        return (i < j);
    }
};
//Constructor for type.
Type::Type(string typeName, int numOfFields) {
    this->typeName = typeName;
    this->fields = fields;
    this->numOfFields = numOfFields;
    this->numOfRecords = numOfRecords;
    this->numOfPages  = numOfPages;
    this->sizeOfOneRecord = sizeOfOneRecord;
    numOfPages = 0;
    numOfRecords = 0;
}

//Here we read every page of a type one by one (by making use of record number, first byte of a page).
//While doing so we fill our records vector which is soon to be cleared.
//Finally, we make an output vector of strings. The strings contain each field of a record.
vector<string> Type::listRecords() {
    vector<string> output;
    output.clear();
    string line = "";
    int whichPage = 0;
    int pivot = 1;
    vector<int> myVector;
    int recordNumberOfPage;
    Record myRecord(this->typeName, this->numOfFields);
    for(int i = 0; i < this->numOfPages; i++){
        myVector = readPage(i);
        recordNumberOfPage = myVector[0];
        for(int k = 0; k < recordNumberOfPage; k++){
            for(int p = 0; p < this->numOfFields; p++){
                myRecord.fields.push_back(myVector[pivot]);
                pivot++;
            }
            this->records.push_back(myRecord);
            myRecord.fields.clear();
        }
        for(int y = 0; y < records.size(); y++){
            for(int x = 0; x < this->numOfFields; x++){
                line = line + to_string(this->records[y].fields[x] )+ " ";
            }
            //if(line != ""){
            output.push_back(line);
            //}

            line = "";
        }
        pivot = 1;
        this->records.clear();
    }

    //Here we sort the output vector.
    sort(output.begin(), output.end(), less_than_key1());
    return output;
}

//This function writes a full page 1025 bytes every time it is called. First a buffer is filled with integer input to be
//written. Then the  actual write happens by write((char*)&buffer, sizeof(buffer)) function. Seekp(position) is a pivot
//used to specify the write byte.
int Type::writePage(int pageNumber) {
    Page myPage;
    myPage.pageID = pageNumber;
    vector<int> myVector = readPage(myPage.pageID);
    myPage.numOfRecordsP = this->records.size();
    int field;
    int buffer[PAGESIZE/4];
    int writePosition  = myPage.pageID*1024;
    fstream myFile (this->typeName+".txt", ios::in | ios::out | ios::binary);

    int j = 0;
    buffer[0] = myPage.numOfRecordsP;
    for(int i = 0; i < myPage.numOfRecordsP; i++){

        // for(j; j < myPage.recordsP[i].fields.size()*myPage.numOfRecordsP;  j++){
        for(int k = 0; k < this->numOfFields; k++){
            field = this->records[i].fields[k];
            buffer[j+1] = field;
            j++;
            //   }

            //outfile.write((char*)&field, sizeof(field));
        }
    }
    myFile.seekp (writePosition);
    myFile.write((char*)&buffer, sizeof(buffer));
    myFile.close();
    return 0;
}
//This function reads 1024 bytes (size of a page) from a file by making use of read((char*)&buffer, sizeof(buffer).
//Seekg is a  pivot used to specify the read position in the file. Read data is held in a buffer which is then put
//in a vecor, myVector which is then outputted.
vector<int> Type::readPage(int pageNumber) {
    int i = 0;
    int readPos = pageNumber * 1024;
    int buffer[PAGESIZE/4];
    vector<int> myVector;
    fstream myFile (this->typeName+".txt", ios::in | ios::out | ios::binary);
    myFile.seekg(readPos);
    myFile.read((char*)&buffer, sizeof(buffer));
    //this->pages[0].numOfRecordsP = buffer[i];
    //i++;
    while(i < PAGESIZE/4){
        myVector.push_back(buffer[i]);
        i++;
    }
    //cout << "reading page: " << pageNumber  << endl;
    myFile.close();
    return myVector;
}

//This function creates a new page by writing a blank page 1024 bytes except for the first 4 bytes, which are 0 since
//it is the number of records of a newly created page.
int Type::createNewPage(){

    Page myPage;
    myPage.pageID=this->numOfPages;
    int writePosition  = myPage.pageID*1024;

    int buffer[PAGESIZE];
    buffer[0] = 0;
    fstream myFile(this->typeName + ".txt", ios::in | ios::out | ios::binary);
    myFile.seekp (writePosition);
    myFile.write((char*)&buffer, PAGESIZE);
    myPage.numOfRecordsP = 0;
    myPage.pageID = this->numOfPages;
    //this->pages.push_back(myPage);
    this->numOfPages++;
    return 0;
}
//This function reads all pages of a file in a for loop in which if number of records of a page is less than
//maximum records of a page, page is chosen as the page to put the record on.
//If the page is full, we read the next page. If all pages are full or this is the first record, a new page is created.
int Type::createRecord(vector<int> fields) {
    vector<int> myVector;
    Record myRecord(this->typeName, this->numOfFields);
    for(int i = 0; i < this->numOfPages; i++){
        int pivot = 0;
        myVector = readPage(i);
        if(myVector[0] < (1024/this->numOfFields)/4){
            for(int k = 0; k < myVector[0]; k++){
                for(int j = 0; j < this->numOfFields; j++){
                    myRecord.fields.push_back(myVector[pivot + 1]);
                    pivot++;
                }
                this->records.push_back(myRecord);
                myRecord.fields.clear();
            }
            for(int m = 0; m < this->numOfFields; m++){
                myRecord.fields.push_back(fields[m]);
            }
            this->records.push_back(myRecord);
            writePage(i);
            this->numOfRecords++;
            this->records.clear();
            return 0;
        }
    }
    for(int i = 0; i < this->numOfFields; i++){
        myRecord.fields.push_back(fields[i]);
    }
    createNewPage();
    this->records.push_back(myRecord);
    writePage(this->numOfPages-1);
    this->numOfRecords++;
    this->records.clear();
    return 0;
}
//This function reads all pages in a for loop and for each page fills and clears the records vector. While doing so, it searches
//for the record. If the record is found, it is then updated by changing the record in reccords vector and writing the whole
//page again.
int Type::updateRecord(vector<int> fields, int primaryKey) {
    int whichPage = 0;
    int pivot = 1;
    vector<int> myVector;
    int recordNumberOfPage;
    Record myRecord(this->typeName, this->numOfFields);
    for(int i = 0; i < this->numOfPages; i++){
        myVector = readPage(i);
        recordNumberOfPage = myVector[0];
        for(int k = 0; k < recordNumberOfPage; k++){
            for(int p = 0; p < this->numOfFields; p++){
                myRecord.fields.push_back(myVector[pivot]);
                pivot++;
            }
            this->records.push_back(myRecord);
            myRecord.fields.clear();
        }
        for(int l = 0; l < this->records.size(); l++){
            if(records[l].fields[0] == primaryKey){
                records[l].fields.clear();
                for(int q = 0; q < fields.size(); q++){
                    this->records[l].fields.push_back(fields[q]);
                }
                break;
            }
        }
        writePage(i);
        this->records.clear();
        return 0;
    }
    cout << "Record not existent." << endl;
    return 0;
}
//This function searches for a record by primary key in all pages. If the record is found, it is erased from the records vector
//decrementing the first byte of the page which esures when a new record is created it is going to fill the first avaliable page.
//There is no output if the record does not exist.
int Type::deleteRecord(int primaryKey) {
    int whichPage = 0;
    int pivot = 1;
    vector<int> myVector;
    int recordNumberOfPage;
    Record myRecord(this->typeName, this->numOfFields);
    for(int i = 0; i < this->numOfPages; i++){
        myVector = readPage(i);
        recordNumberOfPage = myVector[0];
        for(int k = 0; k < recordNumberOfPage; k++){
            for(int p = 0; p < this->numOfFields; p++){
                myRecord.fields.push_back(myVector[pivot]);
                pivot++;
            }
            this->records.push_back(myRecord);
            myRecord.fields.clear();
        }
        for(int l = 0; l < this->records.size(); l++){
            if(records[l].fields[0] == primaryKey){
                this->records.erase(this->records.begin()+l);
                this->numOfRecords--;
                break;
            }
        }
        writePage(i);
        this->records.clear();
        return 0;
    }
    cout << "Record not existent." << endl;
    return 0;
}
//This function searches for a records in all pages of a file. If the record is found, it's fields are outputted in the
//output vector. If the records is not found, empty vector is returned.
string Type::searchRecord(int recordID) {
    string output = "";
    int i = 0;
    vector<int> myVector;
    Record myRecord(this->typeName, this->numOfFields);
    for(int j = 0; j < this->numOfPages; j++){
        myVector = readPage(j);
        for(int k = 1; k < myVector[0]+1; k++){
            for(int l = 0; l < this->numOfFields; l++){
                myRecord.fields.push_back(myVector[(k-1)*this->numOfFields+1+l]);
            }
            this->records.push_back(myRecord);
            myRecord.fields.clear();
        }
        for(int m = 0; m < this->records.size(); m++){
            if(this->records[m].fields[0] == recordID){
                for(int f = 0; f < this->records[m].fields.size(); f++){
                    output = output + to_string(this->records[m].fields[f]) + " ";
                }
            }
        }
        if(output != ""){
            cout << output << endl;
            this->records.clear();
            return output;
        }
        this->records.clear();
    }
    cout << "Record is not in the table!";
    return output;
}
