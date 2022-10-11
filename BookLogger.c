#include "BookLogger.h"

Screen currentScreen;
Collection* collection;
FILE* fptr;
char filePath[50];

int main(int argc, char const *argv[])
{
    //CreateFilePath("Doggy.txt");
    DisplayInitMenu();
    return 0;
}

// Handle the choice of the user depending on what screen theyre currently on. 
void HandleChoice(int choice){
    switch(currentScreen){
        case INIT:
            InitScreenOptions(choice);
            break;
        case MAIN:
            MainScreenOptions(choice);
            break;
        default:
            printf("Option Selected.");
    }
}

void InitScreenOptions(int choice){
    switch(choice){
        case 0:
            exit(0);
        case 1:
            CreateCollection();
            break;
        case 2:
            LoadCollection();
            break;
        default:
            printf("Invalid selection...");
            system("cls");
            DisplayInitMenu();
    }
}

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
            AddToCollection(CreateBook());
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
        default:
            printf("Invalid Selection");
            system("cls");
            DisplayMainMenu();
    }
}

// Get an existing file path from the user. File must exist in the system.
void LoadCollection(){
    printf("Enter your (.txt) file name ({drive}\\\\{directory}\\\\{filename}): ");
    scanf("%s", &filePath);
    getchar();
    fptr = fopen(filePath, "r");
    if(fptr == 0){
        printf("Error loading file. Please enter a valid file path...\n");
        LoadCollection();
        return;
    } else {
        printf("File loaded successfully.");
    }
}

void CreateCollection(){
    collection = malloc(sizeof(collection));
    if(!collection){
        printf("Failed to create collection...");
        return;
    }
    char titleLoc[20];
    char* title = titleLoc;
    printf("Collection Title (20 Characters Maximum - No Spaces): ");
    scanf("%s", title);
    collection->title = title;
    collection->size = 0;
    collection->collection = NULL;
    CreateAndOpenFile(title);
    DisplayMainMenu();
}

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
    DisplayMainMenu();
}

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
    DisplayMainMenu();
}

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
    DisplayMainMenu();
}

void SaveCollection(){
    fptr = fopen(filePath, "w");
    if(fptr == 0)
        CreateAndOpenFile(collection->title);
    
    fprintf(fptr, "Collection: %s\n", collection->title);
    printf("Collection saved to: %s", filePath);
    //TODO:- Loop through logged books and add them to the file.
    DisplayMainMenu();
}

FILE* CreateAndOpenFile(char* title){
    strcpy(filePath, CreateFilePath(title));
    return fopen(filePath, "w");
}

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
    DisplayInitMenu();
}

void SearchCollection(){
    printf("Searching Collection...\n");
    DisplayMainMenu();
}

Book* CreateBook(){
    Book* newBook = malloc(sizeof(Book));
    printf("Book Title: ");
    fgets(newBook->title, 35, stdin);
    printf("Author: ");
    fgets(newBook->author, 25, stdin);
    newBook->id = collection->size+1;
    collection->size += 1;
    printf("Adding Book:\n\t%s\t%s", newBook->title, newBook->author);
    return newBook;
}
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
    printf("Select Option [0-5]: ");
    scanf(" %d", &choice);
    getchar();
    system("cls");
    HandleChoice(choice);
}
char* CreateFilePath(char* name){
    char* fullName;
    fullName = malloc(sizeof(char)*50);
    char* ext = ".txt";
    char* dir = ".\\Collections\\";
    strcpy(fullName, dir);
    strcat(fullName, name);
    strcat(fullName, ext);
    printf("%s", fullName);
    return fullName;
}