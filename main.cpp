#include <bits/stdc++.h>
using namespace std;

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

void updatePI(const string &file, const string &index);

void updateBooksSI();

void updateAuthorsSI();

pair<bool, pair<string, string>> binarySearch(const vector<pair<string, string>> &index, const string &id);

void search(int bitOffset, const string &file);

void searchPI(string id, const string &file, const string &file2);

void searchSI(string id, const string &SIfile, const string &file, const string &PIfile);


int main() {

//    updatePrimaryIndex("authors.txt", "authorsPI.txt");
//    updatePrimaryIndex("books.txt", "booksPI.txt");

//    string id;
//    getline(cin,id);
//    searchSI(id,"booksSI.txt","books.txt","booksPI.txt");
//    db.searchPI(id,"booksPI.txt","books.txt");
//    db. updateBooksSI();
//    db.updateAuthorsSI();
3
//    while (true) {
//        cout << "Welcome to the Library Catalog System\n";
//        cout << "1. Add New Author\n";
//        cout << "2. Add New Book\n";
//        cout << "3. Update Author Name (Author ID)\n";
//        cout << "4. Update Book Title (ISBN)\n";
//        cout << "5. Delete Book (ISBN)\n";
//        cout << "6. Delete Author (Author ID)\n";
//        cout << "7. Print Author (Author ID)\n";
//        cout << "8. Print Book (ISBN)\n";
//        cout << "9. Write Query\n";
//        cout << "10. Exit\n";
//
//        int choice;
//        cout << "Enter your choice: ";
//        cin >> choice;
//
//        switch (choice) {
//            case 1: {
//                break;
//            }
//            case 2: {
//
//                break;
//            }
//            case 3: {
//
//                break;
//            }
//            case 4: {
//
//                break;
//            }
//            case 5: {
//
//                break;
//            }
//            case 6: {
//
//                break;
//            }
//            case 7: {
//
//                break;
//            }
//            case 8: {
//
//                break;
//            }
//            case 9: {
//
//                break;
//            }
//            case 10: {
//                return 0;
//            }
//            default: {
//                cout << "Invalid choice. Please try again.\n";
//                break;
//            }
//        }
//    }
}





pair<bool, pair<string, string>> binarySearch(const vector<pair<string, string>> &index, const string &id) {
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
            return result;
        } else if (stoi(index[mid].first) < ID) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

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

void searchPI(string id, const string &file, const string &file2) {
    ifstream indexFile(file);
    ifstream File(file2);

    //check that datafile is full
    if (!indexFile.is_open()) {
        cerr << "Error opening files." << endl;
        return;
    }

    indexFile.seekg(0, ios::beg);
    vector<pair<string, string>> indx;
    pair<string, string> entry;
    while (!indexFile.eof()) {

        getline(indexFile, entry.first, '|');
        getline(indexFile, entry.second);
        indx.push_back(entry);
    }
    if (binarySearch(indx, id).first) {
        cout << "Found: ";
        int bitoffset = stoi(binarySearch(indx, id).second.second);
        search(bitoffset, file2);
        return;
    } else cout << "not found";


    indexFile.close();
    File.close();

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
    char lengthIndicator[2];
    dataFile.read(lengthIndicator, 2);
    int recLen = stoi(lengthIndicator);
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
