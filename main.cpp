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

short cntID=0, CntNameLL = 0 , CntNameSec = 0;

int main() {
    while (true) {
        cout << "Welcome to the Library Catalog System\n";
        cout << "1. Add New Author\n";
        cout << "2. Add New Book\n";
        cout << "3. Update Author Name (Author ID)\n";
        cout << "4. Update Book Title (ISBN)\n";
        cout << "5. Delete Book (ISBN)\n";
        cout << "6. Delete Author (Author ID)\n";
        cout << "7. Print Author (Author ID)\n";
        cout << "8. Print Book (ISBN)\n";
        cout << "9. Write Query\n";
        cout << "10. Exit\n";

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                break;
            }
            case 2: {

                break;
            }
            case 3: {

                break;
            }
            case 4: {

                break;
            }
            case 5: {

                break;
            }
            case 6: {

                break;
            }
            case 7: {

                break;
            }
            case 8: {

                break;
            }
            case 9: {

                break;
            }
            case 10: {
                return 0;
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }
}

void insertAuthorInPrimary(const char id[],short offset){
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

void insertName(char name[30],char id[15]){
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
                    offPlace=authorSI.tellg()
                }
            }
        }
    }
}

void Add(Author author){
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