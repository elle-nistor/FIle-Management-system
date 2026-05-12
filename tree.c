#include "search_index.h"

NTree Init_Tree(){
	NTree aux = malloc(sizeof(TreeNode));
	aux->file_reference = NULL;
	aux->info = '\0';
	aux->left_child = NULL;
	aux->right_sibling = NULL;
	return aux;
}

void Add_Word_To_Tree(NTree *root, char *word, LFile file){
	
	NTree p = (*root)->left_child;
	NTree prev = (*root);

	int lenght = strlen(word);

	for (int i = 0; i < lenght; i++){
            
		if (p == NULL){
                // allocate tree node
                p = malloc(sizeof(TreeNode));
                p->file_reference = NULL;
                p->left_child = NULL;
                p->right_sibling = NULL;

                // add current character
                p->info = word[i];
                
				// if p was NULL, prev had no child
                prev->left_child = p;
            }

            if (p->info != word[i]){
                while (p->right_sibling)
                {
                    p = p->right_sibling;
                    if (p->info == word[i])
                        break;
                }
            }
            
            // if we still haven't found the char, we need a add it in a new node
            if (p->info != word[i]){
                NTree aux = malloc(sizeof(TreeNode));
                aux->info = word[i];
                aux->file_reference = NULL;
                aux->left_child = NULL;
                aux->right_sibling = NULL;

                p->right_sibling = aux; // it becomes the new sibling of current node
                p = aux; // new node becomes current node
            }

            prev = p;
            p = p->left_child; // move to next level
        }
	
	// create ref node
	LReference new = malloc(sizeof(Reference));
	new->file = file;
	new->next = NULL;

	// i use prev, since at last iteartion p = p->next ( p == NULL);

	// if list is null, insert as first element
	if (prev->file_reference == NULL){
		prev->file_reference = new;
	}

	// adding new in lexicografic order

	// else iterate throught list, move forward until  new ID is
	// > than current ID, the insert it in the list
	else {
		LReference current_ref = prev->file_reference;
		LReference prev_ref = NULL;

		// if current ID < new ID
		while (current_ref){
			if (strcmp(current_ref->file->id, new->file->id) < 0)
				{
					prev_ref = current_ref;
					current_ref = current_ref->next;
				} else break;
		}	

		// insert it at the begginig of the list
		if (prev_ref == NULL){
			new->next = current_ref;
			prev->file_reference = new;

		// else insert it between prev and current
		} else {
			new->next = current_ref;
			prev_ref->next = new;
		}
	}
}

int Count_File_Refs(NTree leaf){
	int cnt = 0;

		LReference current = leaf->file_reference;

		while(current){
			cnt++;
			current = current->next;
		}

	return cnt;
}

void Print(FILE *fout, NTree root, int level){

	// i will use it to store chars forming a word
	char buffer[100] = {'\0'};

	NTree current = root->left_child;

	if (!current){
		return;
	}

	buffer[level] = current->info;

	// if its the last letter of a word (it has a refference)
	if (current->file_reference != NULL){

		buffer[level + 1] = '\0';

		// print keyword
		for (int i = 0; i <= level; i++){
			fprintf(fout, "%c", buffer[i]);
		}

		// print number of files it has been reffered to
		fprintf(fout, "\n%d\n", Count_File_Refs(current));

		// print files id in lexicografic order
		// since I previously inserted them in lexicografic order, i just print the list now

		LReference f = current->file_reference;

			while (f){
				printf("%s ", f->file->id);
			}
	}
}