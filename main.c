#include "search_index.h"

int main(){

    // input output file management
    FILE *fin, *fout;

    fin = fopen("indexare.in", "r");

    if (fin == NULL){
        printf("Could not open input file");
        return 2;
    }

    fout = fopen("indexare.out", "w");

    if (fout == NULL){
        printf("Could not create output file");
    }

    // initialize file list
    FList file_list = Init_File_List();
    NTree root = Init_Tree();

    Manage_Commands(fin, fout, file_list->head);


    fclose(fin);
    fclose(fout);
    
    return 0;
}