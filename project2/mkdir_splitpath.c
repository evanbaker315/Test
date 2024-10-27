

#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

struct NODE* navigateToNode(char* dirName);
//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW

    if (strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char dirName[64], baseName[64];
    struct NODE* parent = splitPath(pathName, baseName, dirName);

    if (parent == NULL) {
        return;
    }
    // Check if the directory already exists
    struct NODE* temp = parent->childPtr;
    while (temp != NULL) {
        if (strcmp(temp->name, baseName) == 0) {
            printf("MKDIR ERROR: directory %s already exists\n", baseName);
            return;
        }
        temp = temp->siblingPtr;
    }

    // Create and initialize the new directory node
    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';
    newNode->parentPtr = parent;
    newNode->childPtr = newNode->siblingPtr = NULL;

    // Insert into the parent's child/sibling list
    if (parent->childPtr == NULL) {
        parent->childPtr = newNode;
    } else {
        struct NODE* sibling = parent->childPtr;
        while (sibling->siblingPtr != NULL) {
            sibling = sibling->siblingPtr;
        }
        sibling->siblingPtr = newNode;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    // TO BE IMPLEMENTED
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

    // YOUR CODE HERE
    //
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    char pathCopy[64];
    strcpy(pathCopy, pathName);

    char* lastSlash = strrchr(pathCopy, '/');
    if (lastSlash == NULL) {
        strcpy(dirName, "");
        strcpy(baseName, pathName);
        return cwd;
    }

    *lastSlash = '\0';  // Split the path
    strcpy(dirName, pathCopy);
    strcpy(baseName, lastSlash + 1);

    struct NODE* parent = navigateToNode(dirName);
    if (parent == NULL) {
        printf("ERROR: directory %s does not exist\n", dirName);
    }
    return parent;
}

struct NODE* navigateToNode(char* dirName) {
    if (strcmp(dirName, "/") == 0) return root;

    struct NODE* current = cwd;
    char* token = strtok(dirName, "/");

    while (token != NULL) {
        struct NODE* temp = current->childPtr;
        while (temp != NULL && strcmp(temp->name, token) != 0) {
            temp = temp->siblingPtr;
        }

        if (temp == NULL) return NULL;  // Directory doesn't exist
        current = temp;
        token = strtok(NULL, "/");
    }

    return current;
}