#include "search_index.h"

void Swap_Files(LFile *a, LFile *b){
    LFile temp = *a;
    *a = *b;
    *b = temp;
}

// comparing function (after score and file id)
int Rel_Max_Heap_Files(LFile a, LFile b){
    if (a->score != b->score) {
        // return 1 if first file (a) has higher score then (b)
        return (a->score > b->score); 
    }
    
    // in case scores are equal, compare lexicografic after the id
    return (strcmp(a->id, b->id) < 0); 
}

TFileHeap* Allocate_File_Heap(int nrMax, compareFiles comp){
    TFileHeap* h = malloc(sizeof(TFileHeap));
    if (!h) 
        return NULL;

    // allocate array of file pointers
    h->v = malloc(nrMax * sizeof(LFile));
    if (!h->v) {
        free(h);
        return NULL;
    }

    // initialize heap based on given parametres
    h->nrMax = nrMax;
    h->nrElem = 0;
    h->comp = comp; // comparing function (Rel_Max_Heap_Files)

    return h;
}

void Free_File_Heap(TFileHeap **h){
    free((*h)->v);
    free(*h);
    *h = NULL; 
}

// insert files in heap
void Insert_File_Heap(TFileHeap* h, LFile file){
    if (h == NULL || h->nrElem >= h->nrMax) return;

    int current = h->nrElem;
    int parrent = (current - 1)/2;
    
    // insert file on its position
    h->v[h->nrElem] = file;
    h->nrElem++;

    // apply comparing function and swap files to mantain score priority
    while(current != 0 && h->comp(h->v[current], h->v[parrent])){
        Swap_Files(&h->v[current], &h->v[parrent]);
        current = parrent;
        parrent = (current - 1)/2;
    }
}

// Extract file with the highest score (top of the heap)
LFile Extract_File_Heap(TFileHeap* h){

    // extract file from top (highest priority)
    LFile top = h->v[0];

    // add last element from the right to top (temporary)
    Swap_Files(&h->v[0], &h->v[h->nrElem - 1]);

    h->nrElem--; // we just extracted first element

    // start iterating from top to restore heap property
    int current = 0;

    while(1){
        // index will hold the 'best' element among current and its children
        int index = current;

        int left = 2 * index + 1; // find left kid
        int right = 2 * index + 2; // find right kid

        // if there's a left child, compare it with its parent
        if (left < h->nrElem && h->comp(h->v[left], h->v[index]))
            index = left;

        // if there's a right child, compare it with previous winner
        if (right < h->nrElem && h->comp(h->v[right], h->v[index]))
            index = right;
            
        // if index has't changed, it means the heap is already sorted
        if (index == current)
            break;

        // 'winner' takes the higher position (current)
        Swap_Files(&h->v[current], &h->v[index]);

        // continue iterating
        current = index;
    }
    return top;
}

// function to find a keyword in the tree, and print its top k file refs
void TopK(FILE *fin, FILE *fout, NTree *current){

    char keyword[100];
    int k;
    
    fscanf(fin, "%s %d", keyword, &k);

    int index = 0;
    int lenght = strlen(keyword);
    
    NTree p = (*current)->left_child;

    // find terminating node of the word
    while (index < lenght) {
        if (p != NULL && p->info != keyword[index]) {
            while(p != NULL && p->info != keyword[index])
                p = p->right_sibling;
        }

        // in case keyword cannot be found in the tree
        if (p == NULL) {
            fprintf(fout, "EMPTY\n");
            return;
        }

        // if we iterated through all chars of the keyword
        if (index == lenght - 1) break;

        p = p->left_child;
        index++;
    }

    // if the kw doesnt exist or it has no refs, print EMPTY
    if (p == NULL || p->file_reference == NULL) {
        fprintf(fout, "EMPTY\n");
        return;
    }

    // count references and allocate File Heap
    int cnt = Count_File_Refs(p);
    TFileHeap *h = Allocate_File_Heap(cnt, Rel_Max_Heap_Files);

    // add all the references with the kw in the heap
    LReference ref = p->file_reference;
    while (ref) {
        Insert_File_Heap(h, ref->file);
        ref = ref->next;
    }

    // extract all k references

    // in case there are less then k refs, extract all possible refs
    if (cnt < k)
        k = cnt;

    // print number of references for the keyword
    fprintf(fout, "%d ", k);

    for (int i = 0; i < k; i++) {
        LFile file = Extract_File_Heap(h);
        fprintf(fout, "%s ", file->id);
    }
    fprintf(fout, "\n");

    // Free heap
    Free_File_Heap(&h);
}
