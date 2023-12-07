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

void insertAuthorInPrimary(const char id[],short offset);

void insertName(char name[30],char id[15]);

void Add(Author author);

pair<bool, pair<string, string>> binarySearch(const vector<pair<string, string>> &index, const int &id);

pair<bool, pair<string, string>> binarySearch(const vector<pair<string, string>> &index, const string &id);

void search(int bitOffset, const string &file);

void searchPI(string id, const string &file, const string &file2);

void searchSI(string id, const string &SIfile, const string &file, const string &PIfile);


short cntID=0, CntNameLL = 0 , CntNameSec = 0;

int main() {

//    updatePI("authors.txt", "authorsPI.txt");
//    updatePI("books.txt", "booksPI.txt");
//    updateBooksSI();
//    updateAuthorsSI();
    string id;
    getline(cin,id);
    searchSI(id,"authorSI.txt","authors.txt","authorsPI.txt");
//    searchSI(id,"booksSI.txt","books.txt","booksPI.txt");
//    searchPI(id,"authorsPI.txt","authors.txt");
//    searchPI(id,"booksPI.txt","books.txt");

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






void updatePI(const string &file, const string &index) {

    ifstream dataFile(file);
    ofstream indexFile(index);

    //check that datafile is full
    if (!dataFile.is_open() || !indexFile.is_open()) {
        cerr << "Error opening files." << endl;
        return;
    }

    //declaration
    vector<pair<int, int>> primaryIndex;
    string id;
    pair<int, int> p;
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
        p.first = stoi(id);
        p.second = bitOffset;
        primaryIndex.push_back(p);

        //adding length of record to the bit offset
        bitOffset += 2;
        bitOffset += recLen;

        //getting the length indicator of the next record
        dataFile.seekg((recLen - id.length()-1), ios::cur);
        dataFile.read(lengthIndicator, 2);
        recLen = stoi(lengthIndicator);

    }

    sort(primaryIndex.begin(), primaryIndex.end());
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
    vector<pair<int, string>> secondaryIndex;
    string id, bookTitle;
    pair<int, string> p;



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
            if( authorID == to_string(i.first)){
                i.second+=',';
                i.second+=id;
                added = true;
            }
        }

        if(!added){
            p.first = stoi(authorID);
            p.second = id;
            secondaryIndex.push_back(p);
        }

        //adding length of record to the bit offset


        //getting the length indicator of the next record
        dataFile.read(lengthIndicator, 2);
        recLen = stoi(lengthIndicator);

    }

    sort(secondaryIndex.begin(), secondaryIndex.end());
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

    sort(secondaryIndex.begin(), secondaryIndex.end());
    //filling the index file
    for (const auto &entry: secondaryIndex) {
        indexFile << entry.first << '|' << entry.second << "\n";
    }

    dataFile.close();
    indexFile.close();
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
    auto x =binarySearch(indx, id);
    if (x.first) {
        istringstream values ;
        values.str(x.second.second);
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
    auto x = binarySearch(indx, stoi(id));
    if (x.first) {
        cout << "Found: ";

        int bitoffset = stoi(x.second.second);
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

void Add(Author author) {
    fstream out("authors.txt",ios::in|ios::out|ios::app);
    int RecordSize,IDSize,NameSize, AddressSize;
    short header;
    out.seekg(0,ios::beg);
    out.read((char*)&header,sizeof(header));
    NameSize=strlen(author.authorName);
    AddressSize=strlen(author.address);
    IDSize=strlen(author.authorID);
    RecordSize=IDSize+NameSize+AddressSize +2;
    insertName(author.authorName,author.authorID);
    if(header==-1){
        out.seekp(0,ios::end);
        out.write((char*)&RecordSize,sizeof(RecordSize));
        short end1=out.tellp();

    }
}

void insertName(char *name, char *id) {
    fstream authorSI("authorsSI.txt",ios::in|ios::out);
    short first=0;
    short last=CntNameSec-1;
    short mid;
    bool found=false;
    char tmp[30];
    while(first<=last){
        mid=(first+last)/2;
        authorSI.seekg(mid*32,ios::beg);
        authorSI.read((char*)&tmp,sizeof(tmp));
        if(strcmp(tmp,name)==0){
            found=true;
            break;
        }
        else if(strcmp(tmp,name)<0){
            first=mid+1;
        }
        else{
            last=mid-1;
        }
    }
    authorSI.close();
    if(!found){
        if(CntNameLL==0){
            authorSI.open("authorsSI.txt",ios::in|ios::out);
            authorSI.seekg(0,ios::end);
            authorSI.write((char*)&name,30);
            authorSI.write((char*)&CntNameLL,sizeof(CntNameLL));
            CntNameSec++;
            authorSI.close();
            fstream nameLL("nameLL.txt",ios::in|ios::out);
            nameLL.seekg(0,ios::end);
            nameLL.write(name,30);
            nameLL.write(id,15);
            short nega=-1;
            nameLL.write((char*)&nega,sizeof(nega));
            CntNameLL++;
            nameLL.close();
        }
        else{
            authorSI.open("authorsSI.txt",ios::in|ios::out);
            short offPlace=-1;
            authorSI.seekg(0,ios::beg);
            int i=0;
            while(i<CntNameSec){
                char s[30];
                authorSI.read((char*)&s,sizeof(s));
                if(strcmp(name,s) < 0){
                    offPlace=authorSI.tellg();
                }
            }
        }
    }
}

void insertAuthorInPrimary(const char *id, short offset) {
    fstream authorsPI("authorsPI.txt",ios::in|ios::out);
    int x=0;
    for(int i=0;id[i]!='\0';i++){
        x*=10;
        x+=id[i]-'0';
    }
    int tmp=0;
    short of=0;
    bool hi=false;
    if(cntID==0){
        authorsPI.write((char*)&x,sizeof(x));
        authorsPI.write((char*)&offset,sizeof(offset));
        cntID++;
        return;
    }
    authorsPI.read((char*)&tmp,sizeof(tmp));
    while(authorsPI.good()){
        if(tmp>x){
            hi=true;
            authorsPI.seekg(-4,ios::cur);
            of=authorsPI.tellg();
            break;
        }
        authorsPI.seekg(2,ios::cur);
        authorsPI.read((char*)&tmp,sizeof(tmp));
    }
    authorsPI.close();
    authorsPI.open("authorsPI.txt",ios::in|ios::out);
    if(!hi){
        authorsPI.seekg(cntID*6,ios::beg);
        authorsPI.write((char*)&x,sizeof(x));
        authorsPI.write((char*)&offset,sizeof(offset));
        cntID++;
    }
    else{
        authorsPI.seekg((cntID - 1) * 6, ios::beg);
        int numend;
        short ofend;
        authorsPI.read((char*)& numend, sizeof(numend));
        authorsPI.read((char*)& ofend, sizeof(ofend));

        authorsPI.seekg(of, ios::beg);
        while(authorsPI.good()){
            int tmpnum; short tmpof;
            int tmpnum1; short tmpof1;
            authorsPI.read((char*)& tmpnum, sizeof(tmpnum));
            authorsPI.read((char*)& tmpof, sizeof(tmpof));
            authorsPI.read((char*)& tmpnum1, sizeof(tmpnum1));
            authorsPI.read((char*)& tmpof1, sizeof(tmpof1));
            authorsPI.seekg(-6, ios::cur);
            authorsPI.write((char*)& tmpnum, sizeof(tmpnum));
            authorsPI.write((char*)& tmpof, sizeof(tmpof));
        }
        authorsPI.close();
        authorsPI.open("PrimaryIndex.txt", ios::out | ios::in | ios::binary);
        authorsPI.seekg(0, ios::end);

        authorsPI.write((char*)& numend, sizeof(numend));
        authorsPI.write((char*)& ofend, sizeof(ofend));
        authorsPI.seekg(of, ios::beg);
        authorsPI.write((char*)& x, sizeof(x));
        authorsPI.write((char*)& offset, sizeof(of));
        cntID++;

    }
    authorsPI.close();
}

pair<bool, pair<string, string>> binarySearch(const vector<pair<string, string>> &index, const int &id) {

    int low = 0;
    int high = index.size() - 1;
    pair<bool, pair<string, string>> result;
    result.first = false;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (stoi(index[mid].first) == id) {
            result.first = true;
            result.second = index[mid];
            return result;
        } else if (stoi(index[mid].first) < id) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

}

pair<bool, pair<string, string>> binarySearch(const vector<pair<string, string>> &index, const string &id) {

    int low = 0;
    int high = index.size() - 1;
    pair<bool, pair<string, string>> result;
    result.first = false;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (index[mid].first == id) {
            result.first = true;
            result.second = index[mid];
            return result;
        } else if (index[mid].first < id) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

}