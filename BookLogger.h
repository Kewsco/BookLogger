#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test
// Macbook Test...
// Enumerator to keep track of which screen the user is currently on. 
typedef enum {
    INIT, MAIN, OTHER
} Screen;

// Struct to represent one book.
typedef struct {
    int id;
    char title[35];
    char author[25];
} Book;

// Struct to hold the data of one book, with a pointer to the next book node.
struct BookNode{
    Book book;
    struct BookNode* next;
};

// Struct to hold the data regarding the users current loaded collection.
typedef struct {
    char* title;
    struct BookNode* collection;
    int size;
} Collection;

//------ Menu Displaying Methods..
void DisplayInitMenu();
void DisplayMainMenu();

//------Option Handling Methods..
void HandleChoice(int choice);
void InitScreenOptions(int choice);
void MainScreenOptions(int choice);

//------ Collection Based Methods..
void SaveCollection();
void AddToCollection();
void RemoveFromCollection();
void PrintCollection();
void SaveCollection();
void DeleteCollection();
void SearchCollection();
void UpdateIDs();
void OpenCollection(int i);
int ReadCollectionData(char* title);

//Creation Methods..
Book* CreateBook();
Book* CreateBookFromFile(char* title, char* author);
FILE* CreateAndOrOpenFile();
char* CreateFilePath(char* name);