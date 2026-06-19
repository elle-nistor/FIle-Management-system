#include "search_index.h"
FList Init_File_List(){
    FList aux = (FList)malloc(sizeof(FileList));
    aux->head = NULL;
    aux->tail = NULL;
    return aux;
}

void Add_Keyword(char *word, LFile file){
    LKeyword new = malloc(sizeof(Keyword));
    if (!new)
        return;

    strcpy(new->keyword, word);
    new->next = NULL;

    // if keywords list is empty
    if (!file->keyword_list){
        file->keyword_list = new;
    } else {
        LKeyword current = file->keyword_list;
        
        // get to the end of the keywords list
            while(current->next){
                current = current->next;
            }

        // add keyword to end of the list
        current->next = new;
    }
}

// to avoid adding duplicates, this function checks if a specific kw already exists
int Check_if_Keyword_exists(char *word, LFile file){
    if (!file) return 0;

    LKeyword k = file->keyword_list;

    // return 1 if keyword exists in the list
    while(k){
        if (strcmp(k->keyword, word) == 0){
            return 1;
        }
        k = k->next;
    }

    return 0;
}

void Add_File(FILE *fin, FILE *fout, FList *f, NTree root){
    int score, t;
    char id[50], word[100];

    fscanf(fin, "%s %d %d", id, &score, &t);

    // check is there no file with same id already
    LFile current = (*f)->head;

        while(current){
            if (strcmp(current->id, id) == 0){
                fprintf(fout, "EXISTS\n");

            // read all keywords to "clean" the input for next operations
                 for(int i = 0; i < t; i++){
                     fscanf(fin, "%s", word);
                }
                return;
            }
            current = current->next;
        }

    // create new list node
    LFile new = malloc(sizeof(File));
        if (!new) return;
    
    // add file data
    strcpy(new->id, id);
    new->score = score;
    new->keyword_list = NULL;
    new->next = NULL;
    new->prev = NULL;

    // add each keyword to keywords list and to the tree
        for(int i = 0; i < t; i++){
            fscanf(fin, "%s", word);

             if (!Check_if_Keyword_exists(word, new)){
                Add_Keyword(word, new);
                Add_Word_To_Tree(&root, word, new);
            }
        }

    // insert node at the end of the list, and it becomes the new tail
        
    // if list is empty
    if (!(*f)->head){
        (*f)->head = new;
        (*f)->tail = new;

    // if list has already items, add at the end and update tail
    } else {
        (*f)->tail->next = new;
        new->prev = (*f)->tail;
        (*f)->tail = new;
    }

    fprintf(fout, "OK\n");
}

void Free_File_List(FList file_list){
    LFile current = file_list->head;

    while(current){
        LFile aux = current;
        current = current->next;
        
        // free keywords list
        LKeyword k = aux->keyword_list;
        
            while (k) {
                LKeyword temp = k;
                k = k->next;
                free(temp);
            }
        
        free(aux);
    }

    free(file_list);
}

// function to check if a file exists in the list, and return a pointer to it
int Scan_and_Search_if_File_Exists(FILE *fin, FILE *fout, FList *f, LFile *specific_file){

    char id[50];

    fscanf(fin, " %s", id);

    // check is this file exists in the file list
    LFile current = (*f)->head;

        while(current){
            if (strcmp(current->id, id) == 0){
                *specific_file = current;
                break;
            }
            current = current->next;
        }

    if (!(*specific_file)){
        fprintf(fout,"NOT FOUND\n");
        return 0;
    }

    return 1;

}

void Delete_File(FILE *fin, FILE *fout, FList *f, NTree root){

    LFile to_delete = NULL;

    if (!Scan_and_Search_if_File_Exists(fin, fout,f, &to_delete)){
        return;
    }

    LKeyword k = to_delete->keyword_list;

    // iterate to eliminate (if needed) keyword from tree
        while (k){
            Delete_Word_From_Tree(&(root->left_child), to_delete, k->keyword);
            k = k->next;
        }

    // reset k and iterate again to free keyword list
    k = to_delete->keyword_list;

    // free keyword list
    while (k) {
        LKeyword temp = k;
        k = k->next;
        free(temp);
    }

    // prepare list links to free (to_delete)

    if (to_delete->prev != NULL) {
        to_delete->prev->next = to_delete->next;
    }

    if (to_delete->next != NULL) {
        to_delete->next->prev = to_delete->prev;
    }

    if (to_delete == (*f)->head) {
        (*f)->head = to_delete->next;
    }

    if (to_delete == (*f)->tail) {
        (*f)->tail = to_delete->prev;
    }

    free(to_delete);
    fprintf(fout, "OK\n");
}

// add keyword to a file
void AddKw(FILE *fin, FILE *fout, FList *f, NTree root){

    char keyword[100];
    LFile to_add = NULL;

    if (!Scan_and_Search_if_File_Exists(fin, fout,f, &to_add)){

        // clean input for next operations
        fscanf(fin, "%s", keyword);
        return;
    }

    fscanf(fin, "%s", keyword);
    if (!Check_if_Keyword_exists(keyword, to_add)){
        Add_Keyword(keyword, to_add);
        Add_Word_To_Tree(&root, keyword, to_add);
}  
    // even if the kw exists - printf OK
    fprintf(fout, "OK\n");
}

// delete a keyword from a file
void Delete_Kw_From_File(FList *f, LFile file, char *word){
    LKeyword k = file->keyword_list, prev = NULL;

    // iterate until finding the keyword
    while (k && strcmp(k->keyword, word) != 0) {
        prev = k;
        k = k->next;
    }

    // using its prev, free the keyword
    if (k) {
        if (prev) 
            prev->next = k->next;
        else 
            file->keyword_list = k->next;
        
        free(k);
    }

    // check if file has no more keywords - in this case -> delete it
    if (!file->keyword_list) {

        // in case its not the first file in the list
        if (file->prev) 
            file->prev->next = file->next;

        // in case its the first
        else (*f)->head = file->next;

        // in case its not the last 
        if (file->next) 
            file->next->prev = file->prev;

        // in case its the last
        else 
            (*f)->tail = file->prev;

        free(file);
    }
}

// delete a keyword from a file
void DelKw(FILE *fin, FILE *fout, FList *f, NTree root){

    char keyword[100];
    LFile to_delete = NULL;

    if (!Scan_and_Search_if_File_Exists(fin, fout,f, &to_delete)){

        // clean input for next operations
        fscanf(fin, "%s", keyword);
        return;
    }

    fscanf(fin, "%s", keyword);
    Delete_Word_From_Tree(&root->left_child, to_delete, keyword);
    Delete_Kw_From_File(f, to_delete, keyword);
    fprintf(fout, "OK\n");
}


