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
            
        // if tree is empty
        // add the word char by char

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

        // if current is < than current node
        // we need to ad a new node as the first left child of current node

        else if (word[i] < p->info) {

            NTree aux = malloc(sizeof(TreeNode));
            aux->info = word[i];
            aux->file_reference = NULL;
            aux->left_child = NULL;
            
            aux->right_sibling = p; 
            prev->left_child = aux; 
            p = aux; // new node becomes current node
        }

        // if current char is > than current node char
        // we need to find the right position for it in the siblings list

        else if (p->info != word[i]){
            while (p->right_sibling && p->right_sibling->info < word[i])
                p = p->right_sibling;

            // if we found the char in the siblings list, we just move to that node
            if (p->right_sibling && p->right_sibling->info == word[i])
                p = p->right_sibling;
            
            else {
                // if we still haven't found the char, we need a add it in a new node
                NTree aux = malloc(sizeof(TreeNode));
                aux->info = word[i];
                aux->file_reference = NULL;
                aux->left_child = NULL;

                aux->right_sibling = p->right_sibling; 
                p->right_sibling = aux; // it becomes the new sibling of current node
                p = aux; // new node becomes current node
            }
        }

        // move to next level
        prev = p;
        p = p->left_child; 
    }
    
    //  --------- adding new in lexicografic order -------------
    
    // create ref node
    LReference new = malloc(sizeof(Reference));
    new->file = file;
    new->next = NULL;

    // i use prev, since at last iteartion p = p->next ( p == NULL);

    // if list is null, insert as first element
    if (prev->file_reference == NULL){
        prev->file_reference = new;
    }

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

int Tree_Has_Keywords(NTree current) {
    if (!current) {
        return 0;
    }
    
    // Return 1 because we found a valid word
    if (current->file_reference != NULL) {
        return 1;
    }
    
    // continue searching in left child and right sibling
    if (Tree_Has_Keywords(current->left_child)) return 1;
    if (Tree_Has_Keywords(current->right_sibling)) return 1;
    
    return 0; 
}

// recursive function to print the tree
void Print(FILE *fout, NTree current, char *buffer, int level){

	if (!current){
		return;
	}

	buffer[level] = current->info;

	// if current node has file references
    // we print the keyword and its refs

	if (current->file_reference != NULL){

		buffer[level + 1] = '\0';

		// print keyword
		for (int i = 0; i <= level; i++){
			fprintf(fout, "%c", buffer[i]);
		}

		// print number of files it has been reffered to
		fprintf(fout, " %d ", Count_File_Refs(current));

		// print files id in lexicografic order
		// since I previously inserted them in lexicografic order, i just print the list now

		LReference f = current->file_reference;

			while (f){
				fprintf(fout, "%s ", f->file->id);
				f = f->next;
			}
            fprintf(fout, "\n");
	}

	// recursive call for left child and right sibling (to print all the keywords))
	Print(fout, current->left_child, buffer, level + 1);
    Print(fout, current->right_sibling, buffer, level);
}

// recursive function to free the tree
void Free_Tree(NTree current){
    if (!current){
        return;
    }

    Free_Tree(current->left_child);
    Free_Tree(current->right_sibling);

    LReference ref = current->file_reference;
    while(ref){
        LReference aux = ref;
        ref = ref->next;
        free(aux);
    }

    free(current);
}
// recursive function that returns the updated node
NTree Recursive_Delete_Node(NTree node, char *word, int index, char *file_id) {
    if (node == NULL) 
        return NULL;

    // if current char is different from current node's info, search in the right siblings list
    if (node->info != word[index]) {
        node->right_sibling = Recursive_Delete_Node(node->right_sibling, word, index, file_id);
        return node;
    }

    // if we found the char, check if we reached the end of the word
    if (index < strlen(word) - 1) {
        // move to next level
        node->left_child = Recursive_Delete_Node(node->left_child, word, index + 1, file_id);
        
    } else {
        // if it's the last char, find and remove the specific file reference
        LReference curr = node->file_reference, prev = NULL;
        
        while (curr && strcmp(curr->file->id, file_id) != 0) {
            prev = curr; 
            curr = curr->next;
        }
        
        if (curr) {
            // in case it's not the first node (has prev)
            if (prev) prev->next = curr->next;
            // in case it's the first
            else node->file_reference = curr->next;
            
            free(curr);
        }
    }

    //  if node has no references and no children, delete it
    if (node->file_reference == NULL && node->left_child == NULL) {
        NTree sibling = node->right_sibling; // save sibling
        free(node); // free current node
        return sibling; // sibling takes its place
    }

    return node;
}

void Delete_Word_From_Tree(NTree *current, LFile file, char *word) {
    // if current is null, or word is null, or we reached the end of the tree, we return
    if (current && word) {
        *current = Recursive_Delete_Node(*current, word, 0, file->id);
    }
}


// function to find a keyword in the tree, and print its refs
void Find_Keyword(FILE *fin, FILE *fout, NTree *current){

    char keyword[100];
    fscanf(fin, "%s", keyword);

    int index = 0;
    int lenght = strlen(keyword);

    NTree p = (*current)->left_child;

    // while we haven't iterated throught all the chars of the keyword, we move down the tree
    while (index < lenght){
        if (p != NULL && p->info != keyword[index]){
            while(p != NULL && p->info != keyword[index])
                p = p->right_sibling;
        }

        // if we haven't found the char, it means that the keyword doesn't exist in the tree
        if (p == NULL) {
                    fprintf(fout, "EMPTY\n");
            return;
        }

        // if we are at the last char of the keyword, we print the keyword and its refs
        if (index == lenght - 1) {
            if (p->file_reference != NULL) {
                fprintf(fout, "%d ", Count_File_Refs(p));

                // print files id in lexicografic order
                LReference ref = p->file_reference;
                while (ref) {
                    fprintf(fout, "%s ", ref->file->id);
                    ref = ref->next;
                }
                fprintf(fout, "\n");
            } else             
            fprintf(fout, "EMPTY\n");
            break;
        }

        // move to next child
        p = p->left_child;
        index++;
    }
}

