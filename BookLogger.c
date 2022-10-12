#include "BookLogger.h"

Screen currentScreen;
Collection* collection;
FILE* fptr;
char filePath[50];

int main(int argc, char const *argv[])
{
    while(1){
        switch(currentScreen){
            case INIT:
                DisplayInitMenu();
                break;
            case MAIN:
                DisplayMainMenu();
                break;
            case INSERTION:
                DisplayInsertionMenu();
                break;
        }
    }
    return 0;
}

// Open a collection with 2 modes. (0 - create a new collection. 1 - Load collection from existing file.)
void OpenCollection(int mode){
    collection = malloc(sizeof(collection));
    if(!collection){
        printf("Collection memory allocation error.\n");
    }
    printf("Enter Collection Name (Max 20 Characters): ");
    char titleLoc[20];
    char* title = titleLoc;
    scanf("%s", title);
    getchar();
    collection->title = title;
    collection->size = 0;
    collection->collection = NULL;
    if(mode == 0){
        // Create new collection file.
        fptr = CreateAndOrOpenFile(title);
    } else {
        // Load from existing file.
        int check = ReadCollectionData(title);
        if(check == 0)
            return;
    }
    DisplayMainMenu();
}

// Read the data of a provided txt file using the given name.
int ReadCollectionData(char* title){
    strcpy(filePath, CreateFilePath(title));
    fptr = fopen(filePath, "r");
    if(!fptr){
        printf("File Not Found...\n");
        return 0;
    } else {
        
        while(!feof(fptr)){
            char id[3];
            char title[35];
            char author[25];
            fgets(id, 3, fptr);
            fgets(title, 35, fptr);
            fgets(author, 25, fptr);
            if(!feof(fptr))
                AddToCollection(CreateBookFromFile(title, author));
        }
        fclose(fptr);
        return 1;
    }
}

// Add a book to the end of the collection.
void AddToCollection(Book* book){
    struct BookNode* bn;
    bn = (struct BookNode *)calloc(1, sizeof(struct BookNode));
    bn->book = *book;
    bn->next == NULL;
    if(collection->collection == NULL){
        collection->collection = bn;
    } else {
        struct BookNode* last = collection->collection;
        while(last->next != NULL)
            last = last->next;
        last->next = bn;
    }
}

// Prepend a new book to the beginning of a collection.
void AddToStartOfCollection(struct BookNode** first, Book* book){
    struct BookNode* bn;
    bn = (struct BookNode*)calloc(1, sizeof(struct BookNode));
    bn->book = *book;
    bn->next = (*first);
    (*first) = bn;
    UpdateIDs();
}

// Add a new book at a specific index within the collection. 
void AddAtSpecificIndex(struct BookNode** first, Book* book){
    int index;
    printf("Enter index between 1-%d:", collection->size);
    scanf(" %d", &index);
    getchar();
    if(index < 1 || index > collection->size){
        printf("Invalid Index entered...\n");
    } else if(index == 1){
        AddToStartOfCollection(&collection->collection, book);
    } else if(index == collection->size){
        AddToCollection(book);
    } else {
        struct BookNode* tmp = *first;
        struct BookNode* bn = (struct BookNode*)calloc(1, sizeof(struct BookNode));
        bn->book = *book;
        for(int i = 1; i < index-1; i++){
            tmp = tmp->next;
        }
        bn->next = tmp->next;
        tmp->next = bn;
        UpdateIDs();
    }
}

// Remove a book from the collection. Finally free up the used memory.
void RemoveFromCollection(){  
    int choice;
    printf("What entry would you like to delete? (Integer Value): ");
    scanf(" %d", &choice);
    getchar();
    struct BookNode* tmp = collection->collection;
    if(choice == 1){
        collection->collection = collection->collection->next;
        tmp->next = NULL;
        free(tmp);
    } else {
        for(int i = 1; i < choice - 1; i++){
            tmp = tmp->next;
        }
        struct BookNode* toDel = tmp->next;
        tmp->next = tmp->next->next;
        toDel->next = NULL;
        free(toDel);
    }
    UpdateIDs();
}

// Loop through the collection and print the details of each book.
void PrintCollection(){
    printf("Printing Collection: %s\n", collection->title);
    if(collection->collection == NULL){
        printf("Collection is empty...\n");
    } else {
        struct BookNode* bn = collection->collection;
        while(bn){
            printf("[%d]......\n  Title : %s  Author: %s",
                bn->book.id,
                bn->book.title,
                bn->book.author);
            bn = bn->next;
        }  
        free(bn);
    }
    printf("\nPress Enter to return to the main menu...");
    while(getchar() != '\n');
}

// Write the contents of the collection to a text file.
void SaveCollection(){
    fptr = fopen(filePath, "w");
    if(fptr == 0)
        CreateAndOrOpenFile(collection->title);
    //fprintf(fptr, "Collection: %s\n", collection->title);
    struct BookNode* bn = collection->collection;
    while(bn){
        fprintf(fptr, "%d\n", bn->book.id);
        fprintf(fptr, "%s", bn->book.title);
        fprintf(fptr, "%s", bn->book.author);
        bn = bn->next;
    }

    fclose(fptr);
    printf("Collection saved to: %s\n", filePath);
}

// Create/Open a file given the title of the collection chosen by the user.
FILE* CreateAndOrOpenFile(char* title){
    strcpy(filePath, CreateFilePath(title));
    return fopen(filePath, "w");
}

// Delete the collection, freeing up all of the used memory in the process. Also deleted the txt file.
void DeleteCollection(){
    struct BookNode* bn = collection->collection;
    while(bn){
        free(bn);
        bn = bn->next;
    }
    collection->collection = NULL;
    collection = NULL;
    free(collection);
    free(bn);
    fclose(fptr);
    if(remove(filePath) == 0){
        printf("Collection Deleted.\n");
    } else {
        printf("Could Not Delete Collection.\n");
    }
    printf("Press Enter to Continue");
    while(getchar() != '\n');
    DisplayInitMenu();
}

// Search the collection for a specific book. (Not Implemented)
void SearchCollection(){
    printf("Searching Collection...\n");
    // TODO:- Search by Author...
}

// Prompts the user to enter details regarding a book. Creates book with said details and returns it.
Book* CreateBook(){
    Book* newBook = malloc(sizeof(Book));
    printf("Book Title: ");
    fgets(newBook->title, 35, stdin);
    printf("Author: ");
    fgets(newBook->author, 25, stdin);
    newBook->id = collection->size+1;
    collection->size++;
    printf("Adding Book:\n\t%s\t%s", newBook->title, newBook->author);
    return newBook;
}

// Creates and returns a book using data provided by a txt file.
Book* CreateBookFromFile(char* title, char* author){
    Book* nb = malloc(sizeof(Book));
    nb->id = collection->size +1;
    strcpy(nb->title, title);
    strcpy(nb->author, author);
    collection->size++;
    return nb;
}

// Update IDs of all books once one has been removed from the collection.
void UpdateIDs(){
    collection->size = 0;
    struct BookNode* bn = collection->collection;
    while (bn)
    {
        bn->book.id = collection->size + 1;
        collection->size++;
        bn = bn->next;
    }
    free(bn);
}

// Displays the Initialisation Menu to the user (Create/Load collection or quit.)
void DisplayInitMenu(){
    currentScreen = INIT;
    int choice;
    printf(" ---------- Book Logger ---------- \n");
    printf("[1] - Create New Collection.\n");
    printf("[2] - Load Collection.\n");
    printf("[0] - Quit.\n");
    printf("Select Option [0-2]: ");
    scanf(" %d", &choice);
    getchar();
    system("cls");
    HandleChoice(choice);
}

// Display the Main Menu to the user (Print/Search/Add/Remove/Save/Delete/Quit)
void DisplayMainMenu(){
    currentScreen = MAIN;
    int choice;
    printf("------ Main Menu ------\n");
    printf("[1] - Print collection\n");
    printf("[2] - Search Collection\n");
    printf("[3] - Add to collection\n");
    printf("[4] - Remove from collection\n");
    printf("[5] - Save collection\n");
    printf("[6] - Delete Collection\n");
    printf("[0] - Quit\n");
    printf("Select Option [0-6]: ");
    scanf(" %d", &choice);
    getchar();
    system("cls");
    HandleChoice(choice);
}

// Display the insertion menu to the user (Prepend/Insert/Append)
void DisplayInsertionMenu(){
    currentScreen = INSERTION;
    int choice;
    printf("------ Book Insertion ------\n");
    printf("[1] - Add to beginning\n");
    printf("[2] - Insert at specific index\n");
    printf("[3] - Add to end\n");
    printf("[4] - Return to main menu.\n");
    printf("Select Option [1-4]: ");
    scanf(" %d", &choice);
    getchar();
    system("cls");
    HandleChoice(choice);
;}

// Handle the choice of the user depending on what screen theyre currently on. 
void HandleChoice(int choice){
    switch(currentScreen){
        case INIT:
            InitScreenOptions(choice);
            break;
        case MAIN:
            MainScreenOptions(choice);
            break;
        case INSERTION:
            InsertionOptions(choice);
            break;
    }
}

// Handles Init menu options.
void InitScreenOptions(int choice){
    switch(choice){
        case 0:
            exit(0);
        case 1:
            OpenCollection(0);
            break;
        case 2:
            OpenCollection(1);
            break;
    }
}

// Handles Main menu options.
void MainScreenOptions(int choice){
    switch(choice){
        case 0:
            exit(0);
        case 1:
            PrintCollection();
            break;
        case 2:
            SearchCollection();
            break;
        case 3:
            DisplayInsertionMenu();
            break;
        case 4:
            RemoveFromCollection();
            break;
        case 5:
            SaveCollection();
            break;
        case 6:
            DeleteCollection();
            break;
    }
}

// Handles the insertion menu options.
void InsertionOptions(int choice){
    Book* book;
    switch(choice){
        case 1:
            book = CreateBook();
            AddToStartOfCollection(&collection->collection, book);
            break;
        case 2:
            book = CreateBook();
            AddAtSpecificIndex(&collection->collection, book);
            break;
        case 3:
            book = CreateBook();
            AddToCollection(book);
            break;
        case 4:
            DisplayMainMenu();
            break;
    }
}

// Takes the users chosen collection title, then creates a valid file path to the Collection folder, then returns it.
char* CreateFilePath(char* name){
    char* fullName;
    fullName = malloc(sizeof(char)*50);
    char* ext = ".txt";
    char* dir = ".\\Collections\\";
    strcpy(fullName, dir);
    strcat(fullName, name);
    strcat(fullName, ext);
    return fullName;
}