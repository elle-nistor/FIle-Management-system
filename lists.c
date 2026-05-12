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

        // ad keyword to end of the list
        current->next = new;
    }
}

void Add_File(FILE *fin, FILE *fout, FList *f){
    int score, t;
    char id[50], word[100];

    fscanf(fin, "%s %d %d", id, &score, &t);

    // check is there no file with same id already
    LFile current = (*f)->head;

        while(current){
            if (strcmp(current->id, id) == 0){
                fprintf(fout, "EXISTS");
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

    // add each keyword to keywors list
        for(int i = 0; i < t; i++){
            fscanf(fin, "%s", word);
            Add_Keyword(word, new);
        }

    // insert node at the end of the list, and it becomes the new tail
    (*f)->tail->next = new;
    new->next = NULL;
    new->prev = (*f)->tail;
    (*f)->tail = new;

    fprintf(fout, "OK");
}