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
        return 2;
    }

    // initialize file list
    FList file_list = Init_File_List();
    NTree root = Init_Tree();

    // manage commands -> execute commands from input file
    Manage_Commands(fin, fout, &file_list, root);

    fclose(fin);
    fclose(fout);

    Free_Tree(root);
    Free_File_List(file_list);

    return 0;
}