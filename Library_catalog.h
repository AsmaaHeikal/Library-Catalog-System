#include <iostream>
#include <fstream>
#include <vector>
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
    pair<bool, pair<string, string>> binarySearch(const std::vector<pair<string, string>> &index, const std::string &id);

    void updatePI(const string &file, const string &index) {

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
        short lengthIndicator;
        dataFile.read((char*)&lengthIndicator, 2);
        int recLen = lengthIndicator;
        bitOffset += 2;

        while (!dataFile.eof()) {

            getline(dataFile, id, '|');


            //adding the id and the bit offset of the record to the vector
            p.first = id;
            p.second = bitOffset;
            primaryIndex.push_back(p);

            //adding length of record to the bit offset
            bitOffset += 2;
            bitOffset += recLen;

            //getting the length indicator of the next record
            dataFile.seekg((recLen - 2), ios::cur);
            dataFile.read((char*)&lengthIndicator, 2);
            recLen = lengthIndicator;

        }

        //filling the index file
        for (const auto &entry: primaryIndex) {
            indexFile << entry.first << '|' << entry.second << "\n";
        }

        dataFile.close();
        indexFile.close();
    }

    void updateBooksSI() {

        ifstream dataFile("books.txt");
        ofstream indexFile("booksSI.txt");

        //check that datafile is full
        if (!dataFile.is_open() || !indexFile.is_open()) {
            cerr << "Error opening files." << endl;
            return;
        }

        //declaration
        vector<pair<string, string>> secondaryIndex;
        string id, bookTitle;
        pair<string, string> p;



        //set the cursor at the beginning of the file skipping first 2 char
        dataFile.seekg(2, ios::beg);

        //getting the length indicator of the record
        char lengthIndicator[2];
        dataFile.read(lengthIndicator, 2);
        int recLen = stoi(lengthIndicator);


        while (!dataFile.eof()) {
            int bitoffset = recLen;
            getline(dataFile, id, '|');
            getline(dataFile, bookTitle, '|');
            bitoffset -= id.length();
            bitoffset -= bookTitle.length();
            bitoffset -= 2;
            char buffer[bitoffset];
            dataFile.read(buffer, bitoffset);
            string authorID(buffer);

            //adding the id and the bit offset of the record to the vector

            bool added = false;
            for(auto &i : secondaryIndex){
                if( authorID == i.first){
                    i.second+=',';
                    i.second+=id;
                    added = true;
                }
            }

            if(!added){
                p.first = authorID;
                p.second = id;
                secondaryIndex.push_back(p);
            }

            //adding length of record to the bit offset


            //getting the length indicator of the next record
            dataFile.read(lengthIndicator, 2);
            recLen = stoi(lengthIndicator);

        }


        //filling the index file
        for (const auto &entry: secondaryIndex) {
            indexFile << entry.first << '|' << entry.second << "\n";
        }

        dataFile.close();
        indexFile.close();
    }

    void updateAuthorsSI() {

        ifstream dataFile("authors.txt");
        ofstream indexFile("authorSI.txt");

        //check that datafile is full
        if (!dataFile.is_open() || !indexFile.is_open()) {
            cerr << "Error opening files." << endl;
            return;
        }

        //declaration
        vector<pair<string, string>> secondaryIndex;
        string id, authorName;
        pair<string, string> p;



        //set the cursor at the beginning of the file skipping first 2 char
        dataFile.seekg(2, ios::beg);

        //getting the length indicator of the record
        char lengthIndicator[2];
        dataFile.read(lengthIndicator, 2);
        int recLen = stoi(lengthIndicator);


        while (!dataFile.eof()) {

            getline(dataFile, id, '|');
            getline(dataFile, authorName, '|');


            //adding the id and the bit offset of the record to the vector

            bool added = false;
            for(auto &i : secondaryIndex){
                if( authorName == i.first){
                    i.second+=',';
                    i.second+=id;
                    added = true;
                }
            }

            if(!added){
                p.first = authorName;
                p.second = id;
                secondaryIndex.push_back(p);
            }

            //adding length of record to the bit offset


            //getting the length indicator of the next record
            dataFile.seekg(recLen-3-authorName.length(),ios::cur);
            dataFile.read(lengthIndicator, 2);
            recLen = stoi(lengthIndicator);

        }

        //filling the index file
        for (const auto &entry: secondaryIndex) {
            indexFile << entry.first << '|' << entry.second << "\n";
        }

        dataFile.close();
        indexFile.close();
    }

    void search(int bitOffset, const string &file) {

        ifstream dataFile(file);

        //check that datafile is full
        if (!dataFile.is_open()) {
            cerr << "Error opening files." << endl;
            return;
        }

        //set the cursor at the beginning of the record and subtract 2 to get the length indicator
        dataFile.seekg(bitOffset - 2, ios::beg);

        //getting the length indicator of the record
        char lengthIndicator[2];
        dataFile.read(lengthIndicator, 2);
        int recLen = stoi(lengthIndicator);

        string rec;
        for (int i = 0; i < recLen; i++) {
            rec += dataFile.get();
        }
        cout << rec<< endl;
        dataFile.close();
    }

    int searchPI(string id, const string &file, const string &file2) {
        ifstream indexFile(file);
        ifstream File(file2);

        //check that datafile is full
        if (!indexFile.is_open()) {
            cerr << "Error opening files." << endl;
            return -1;
        }

        indexFile.seekg(0, ios::beg);
        vector<pair<string, string>> indx;
        pair<string, string> entry;
        while (!indexFile.eof()) {

            getline(indexFile, entry.first, '|');
            getline(indexFile, entry.second);
            indx.push_back(entry);
        }
        int bitoffset=-1;
        if (binarySearch(indx, id).first) {
            cout << "Found: ";
            bitoffset = stoi(binarySearch(indx, id).second.second);
            cout << bitoffset << endl;
            search(bitoffset, file2);
        } else {
            cout << "not found\n";
        }


        indexFile.close();
        File.close();
        return bitoffset;
    }

    void searchSI(string id, const string &SIfile, const string &file, const string &PIfile) {
        ifstream SIFile(SIfile);
        ifstream PIFile(PIfile);
        ifstream File(file);

        //check that datafile is full
        if (!SIFile.is_open()) {
            cerr << "Error opening files." << endl;
            return;
        }

        SIFile.seekg(0, ios::beg);
        vector<pair<string, string>> indx;
        pair<string, string> entry;
        while (!SIFile.eof()) {

            std::getline(SIFile, entry.first, '|');
            SIFile >> entry.second;
            indx.push_back(entry);
            SIFile.ignore();  // Ignore the newline character
        }
        if (binarySearch(indx, id).first) {
            istringstream values ;
            values.str(binarySearch(indx,id).second.second);
            values.seekg(0,ios::beg);
            string val ;

            while(!values.eof()){

                getline(values,val,',');
                searchPI(val,PIfile,file) ;
                
            }



        } else cout << "not found";


        PIFile.close();
        SIFile.close();
        File.close();

    }

    // ============= delete ================
    bool deleteRecordPI(string id, string filename){
        // load primary index on vector of pairs <id, offset>
        fstream file(filename, ios::in|ios::out|ios::binary);
        vector<pair<string,  string> > primaryIndex;
        pair<string, string> entry;

        while (getline(file, entry.first, '|') && getline(file,entry.second)) {
            primaryIndex.push_back(entry);
        }

        file.close();

        // search using binary search
        int low = 0, high = primaryIndex.size() - 1, mid=-1;
        while(low <= high){
            mid = (high + low) / 2;
            if(primaryIndex[mid].first < id){
                low = mid + 1;
            } else if (primaryIndex[mid].first > id){
                high = mid - 1;
            } else {
                break;
            }
        }

        if(low > high ) return false;

        // once found authorId delete entry
        primaryIndex.erase(primaryIndex.begin() + mid);

        // rewrite index to file
        file.open(filename, ios::trunc|ios::out);
        if(!file.is_open()){
            cout << "Error: failed to open index file\n";
            return false;
        }

        for(int i = 0; i < primaryIndex.size(); i++){
            // if we want our index file fixed length record
//        file.write((char*)&primaryIndex[i].first, 15);
//        file.write((char*)&primaryIndex[i].second, sizeof(primaryIndex[i].second));
            file << primaryIndex[i].first << '|' << primaryIndex[i].second;
            if(i < primaryIndex.size()-1 )
                file << "\n";
        }

        file.close();
        return true;
    }


    void deleteRecord(char id[], string filename){
        fstream file(filename, ios::out | ios::in | ios::binary);
        if(!file.is_open()){
            std::cout << "Deletion failed -> fail to open file\n";
            return;
        }

        // call search function to return the offset of that record if exists
        short offset = searchPI(id, "authorsPI.txt", "authors.txt"); // add here search call instead
//    cout << "from delete "<<offset <<endl;
        if (offset == -1) {
            std::cout << "Deletion failed -> id not found\n";
            return;
        }
        deleteRecordPI(id, filename);
        // back to Header to find the start of available list
        short header, deletedRecordSize, currSize;
        file.seekg(0, ios::beg);
        file.read((char*)&header, sizeof(header));

        file.seekg(offset-2, ios :: beg); // length indicator of deleted record
        file.read((char*)&deletedRecordSize, sizeof(deletedRecordSize));

        // not the first record to delete from file
        if(header != -1)
        {
            file.seekg(header - 2, ios::beg); // extract size of first record in avail list (size of first record in avail list (worst fit): largest size)
            file.read((char*)&currSize, sizeof(currSize));
        }

        // empty avail list or deleted record size is the largest
        // empty => header = deletedRecordOffset and deletedRecord: overwrite the first 3 bytes by *-1 (header = -1)
        // currSize <= deletedRecordSize => header = deletedRecordOffset and deletedRecord: overwrite the first 3 bytes by *currOffset as (header = currOffset)
        if(header == -1 or currSize<=deletedRecordSize){
            file.seekp(offset, ios::beg);
            file.write("*", 1);
            file.write((char*)&header, sizeof(header)); // let the deleted record points to the second largest
            file.seekg(0, ios::beg);
            file.write((char*)&offset, sizeof(offset));
            return;
        }

        short currOffset = header, nextOffset, nextSize;
        file.seekg(currOffset+1 , ios::beg); // skip * to read the next offset
        file.read((char*)&nextOffset , sizeof(short));

        // only one item in avail list and its size is >= deleted item size
        // let curr on avail list points to deleted item
        if(nextOffset == -1 and currSize >= deletedRecordSize){
            file.seekp(currOffset+1, ios::beg); // skip * to read the next offset
            file.write((char*)&offset, sizeof(offset));
            file.seekp(offset, ios::beg);
            file.write("*", 1);
            file.write((char*)&nextOffset, sizeof(nextOffset));
            return;
        }

        // only one item in avail list and its size is < deleted item size
        // let deleted item  points to curr item on avail list
        if(nextOffset == -1 ){
            file.seekp(offset, ios::beg);
            file.write("*", 1);
            file.write((char*)&currOffset, sizeof(currOffset));
            file.seekp(currOffset+1, ios::beg);
            file.write("-1", sizeof(short));
            return;
        }

        file.seekg(nextOffset-2, ios::beg);   // extract size of next item
        file.read((char*)&nextSize , sizeof(nextSize));

        while(true){
            // right place for deleted item is found
            if(nextSize <= deletedRecordSize){
                file.seekp(currOffset+1, ios::beg); // skip * to write the next offset of currOffset
                file.write((char*)&offset, sizeof(offset));
                file.seekp(offset, ios::beg);
                file.write("*", 1); // overwrite one byte by *
                file.write((char*)&nextOffset, sizeof(nextOffset)); // overwrite 2 bytes by next offset of deletedOffset
                return;
            }
            else
            {
                currOffset = nextOffset; // advance the curr to next
                file.seekp(nextOffset+1, ios::beg);
                file.read((char*)&nextOffset , sizeof(nextOffset));

                if(nextOffset == -1){ // reach the end of avail list
                    file.seekp(currOffset + 1, ios::beg); // skip * to write the next offset of currOffset
                    file.write((char*)&offset, sizeof(offset));
                    file.seekp(offset, ios::beg);
                    file.write("*", 1);
                    file.write((char*)&nextOffset, sizeof(nextOffset));
                    break;
                }

                file.seekg(nextOffset-2, ios::beg); // extract size of next
                file.read((char*)&nextSize , sizeof(nextSize));
            }
        }
        file.close();
    }

}

};

pair<bool, pair<string, string>> DB::binarySearch(const vector<pair<string, string>> &index, const string &id) {
    int ID = stoi(id);
    int low = 0;
    int high = index.size() - 1;
    pair<bool, pair<string, string>> result;
    result.first = false;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (stoi(index[mid].first) == ID) {
            result.first = true;
            result.second = index[mid];
        } else if (stoi(index[mid].first) < ID) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
        return result;

    }

#endif //LIBRARY_CATALOG_SYSTEM_LIBRARY_CATALOG_H
