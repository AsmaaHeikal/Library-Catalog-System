#include <iostream>
#include <fstream>
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
// ==================== delete ========================
void deleteRecord(char id[], string filename){
    fstream file(filename, ios::out | ios::in | ios::binary);
    if(!file.is_open()){
        std::cout << "Deletion failed -> fail to open file\n";
        return;
    }

    // call search function to return the offset of that record if exists
    short offset = 28; // add here search call instead

    if (offset == -1) {
        std::cout << "Deletion failed -> id not found\n";
        return;
    }

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
// =================================================



// only for testing purposes
void writeAuthors(){ // to add records to data file to test my functions
    fstream file("authors.txt", ios::binary|ios::in|ios::out);
    if(!file.is_open()){
        cout << "Error: opening file failed\n";
        return;
    }

    Author data[] = {
            {"1", "AA1 A2", "222 Dokki St"},   // offset = 4
            {"2", "B1 B2", "23 Dokki St"},     // offset = 28
            {"3", "C111 C2", "2 Dokki St"},    // offset = 50
            {"4", "D31 D2", "2566 Dokki St"},  // offset = 73
            {"5", "E5551 E2", "24446 Dokki St"}// offset = 98
    };

    short header = -1;
    file.write((char*)&header, sizeof(header)); // header

    for(int i = 0; i <  5; i++){
        short size = strlen(data[i].authorID) + strlen(data[i].authorName) + strlen(data[i].address)+3;
        file.write((char*)&size, sizeof(size));
        file.write(data[i].authorID, strlen(data[i].authorID));
        file.write("|", 1);
        file.write(data[i].authorName, strlen(data[i].authorName));
        file.write("|", 1);
        file.write(data[i].address, strlen(data[i].address));
        file.write("|", 1);
    }
    cout << "records added successfully\n";
    file.close();
}

// only for testing purposes
void writeBooks(){
}

void readAuthors() {
    ifstream file("authors.txt", ios::binary|ios::in|ios::out);

    if (!file.is_open()) {
        cout << "Error: opening file failed\n";
        return;
    }

    short header;
    file.read((char*)&header, sizeof(header));
    cout << "Header: "<< header << endl;

    short size;
    file.read((char*)&size, sizeof(size));

    while (file) {
        char record[size];
        file.read(record, size);
        char mark = record[0];

        if (mark != '*') {

           string authorID, authorName, address;

            int fieldCounter = 0;
            for(int i  = 0; i < size;i++){
                if (record[i]=='|') {
                    fieldCounter++;
                    continue;
                }
                if (fieldCounter==0){
                    authorID+=record[i];
                }
                if(fieldCounter==1){
                    authorName+=record[i];
                }
                if(fieldCounter==2){
                    address+=record[i];
                }
            }
            cout << authorID << "|" << authorName << "|" << address << endl;
        }
        file.read((char*)&size, sizeof(size));

    }

    file.close();
}

int main() {
    writeAuthors();
    readAuthors();
    deleteRecord("1", "authors.txt");
    readAuthors();
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
//                deleteRecord("1","books.txt");
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

