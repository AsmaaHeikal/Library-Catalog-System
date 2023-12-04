#include <bits/stdc++.h>

using namespace std;

#ifndef LIBRARY_CATALOG_SYSTEM_LIBRARY_CATALOG_H
#define LIBRARY_CATALOG_SYSTEM_LIBRARY_CATALOG_H

class DB {
private:
    struct Author {
        char authorID[15];
        char authorName[30];
        char address[30];
    };
    struct Book {
        char ISBN[15];
        char bookTitle[30];
        char authorID[15];
    };


public:
    void updatePrimaryIndex(const string &file, const string &index) {

        ifstream dataFile(file);
        ofstream indexFile(index);

        //check that datafile is full
        if (!dataFile.is_open() || !indexFile.is_open()) {
            cerr << "Error opening files." << endl;
            return;
        }

        //declaration
        vector<pair<string, int>> primaryIndex;
        string id;
        pair<string, int> p;
        int bitOffset = 2;

        //set the cursor at the beginning of the file skipping first 2 char
        dataFile.seekg(bitOffset, ios::beg);

        //getting the length indicator of the record
        char lengthIndicator[2];
        dataFile.read(lengthIndicator, 2);
        int recLen = stoi(lengthIndicator);
        bitOffset += 2;

        while (!dataFile.eof()) {

            getline(dataFile, id, '|');


            //adding the id and bit offset of the record to the vector
            p.first = id;
            p.second = bitOffset;
            primaryIndex.push_back(p);

            //adding length of record to the bit offset
            bitOffset += 2;
            bitOffset += recLen;

            //getting the length indicator of the next record
            dataFile.seekg((recLen - 2), ios::cur);
            dataFile.read(lengthIndicator, 2);
            recLen = stoi(lengthIndicator);

        }

        //filling the index file
        for (const auto &entry: primaryIndex) {
            indexFile << entry.first << '|' << entry.second << "\n";
        }

        dataFile.close();
        indexFile.close();
    }

    void search(string id, const string &file){

        ifstream dataFile(file);

        //check that datafile is full
        if (!dataFile.is_open()) {
            cerr << "Error opening files." << endl;
            return;
        }

        int bitOffset = 2;

        //set the cursor at the beginning of the file skipping first 2 char
        dataFile.seekg(bitOffset, ios::beg);

        //getting the length indicator of the record
        char lengthIndicator[2];
        dataFile.read(lengthIndicator, 2);
        int recLen = stoi(lengthIndicator);
        bitOffset += 2;

        while (!dataFile.eof()) {

            string str,rec;
            getline(dataFile, str, '|');
            if(str == id){
                rec = id+'|';
                for(int i = 0; i<recLen-2;i++){
                    rec+=dataFile.get();
                }
                cout<<rec;
                return;
            }
            //adding length of record to the bit offset
            bitOffset += 2;
            bitOffset += recLen;

            //getting the length indicator of the next record
            dataFile.seekg((recLen - 2), ios::cur);
            dataFile.read(lengthIndicator, 2);
            recLen = stoi(lengthIndicator);

        }
        dataFile.close();
    }

    bool binarySearch(const std::vector<pair<string, int>>& index, const std::string& id) {
        int low = 0;
        int high = index.size() - 1;

        while (low <= high) {
            int mid = (low + high) / 2;
            if (index[mid].first == id) {
                return true;
            } else if (index[mid].first < id) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }

    void searchPI(string id, const string &file, const string &file2) {
        ifstream indexFile(file);
        ifstream File(file2);

        //check that datafile is full
        if (!indexFile.is_open()) {
            cerr << "Error opening files." << endl;
            return;
        }

        indexFile.seekg(0, ios::beg);
        while (!indexFile.eof()) {

            vector<pair<string, int>> indx;
            while (!indexFile.eof()) {
                pair<string,int> entry;
                std::getline(indexFile, entry.first, '|');
                indexFile >> entry.second;
                indx.push_back(entry);
                indexFile.ignore();  // Ignore the newline character
            }
            if(binarySearch(indx,id)){
                cout << "Found:";
                search(id,file2);
                return;
            }else cout<<"not found";
        }

        indexFile.close();
        File.close();

    }


};


#endif //LIBRARY_CATALOG_SYSTEM_LIBRARY_CATALOG_H
