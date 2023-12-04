#include "Library_catalog.h"
int main() {
    DB db;
    db.updatePrimaryIndex("authors.txt", "authorsPI.txt");
    db.updatePrimaryIndex("books.txt", "booksPI.txt");

    string id;
    cin>>id;
    db.searchPI(id,"authorsPI.txt","authors.txt");

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

