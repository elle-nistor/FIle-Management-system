#include "search_index.h"

void Manage_Commands(FILE *fin, FILE *fout, FList *file_list,  NTree root){
    int q;
    char command[10];
    char buffer[100] = {'\0'};

    fscanf(fin, "%d", &q);

        for (int i = 0; i < q; i++){
            fscanf(fin, "%s", command);

            if (strcmp(command, "ADD") == 0){
                Add_File(fin, fout, file_list, root);

            } else if (strcmp(command, "DEL") == 0){
                Delete_File(fin, fout, file_list,root);

            } else if (strcmp(command, "ADDKW") == 0){
                AddKw(fin, fout, file_list, root);

            } else if (strcmp(command, "DELKW") == 0){
                DelKw(fin, fout, file_list, root);
                
            } else if (strcmp(command, "FIND") == 0){
                Find_Keyword(fin, fout, &root);

            } else if (strcmp(command, "TOPK") == 0){
                TopK(fin, fout, &root);

            } else if (strcmp(command, "PRINT") == 0){
                if (Tree_Has_Keywords(root))
                    Print(fout, root->left_child, buffer, 0);

                else fprintf(fout,"EMPTY\n");
            } 
        }
}
