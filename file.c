#include "search_index.h"

void Manage_Commands(FILE *fin, FILE *fout, LFile file){
    int q;
    char command[10];

    fscanf(fin, "%d", &q);

        for (int i = 0; i < q; i++){
            fscanf(fin, "%s", command);

            if (strcmp(command, "ADD") == 0){

            } else if (strcmp(command, "DEL") == 0){

            } else if (strcmp(command, "ADDKW") == 0){

            } else if (strcmp(command, "DELKW") == 0){

            } else if (strcmp(command, "FIND") == 0){

            } else if (strcmp(command, "TOPK") == 0){

            } else if (strcmp(command, "PRINT") == 0){

            } 
        }
}
