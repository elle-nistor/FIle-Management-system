#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Keyword List ---
typedef struct Keyword{
	char keyword[100];
	struct Keyword *next;
} Keyword, *LKeyword;

// ---- File List ----
typedef struct File
{   char id[50];
	int score;
	LKeyword keyword_list;
	struct File *next;
	struct File *prev;
} File, *LFile;

typedef struct FileList{
	LFile head;
	LFile tail;
} FileList, *FList;

// --- Reference List ---
typedef struct Reference{
	LFile file;
	struct Reference *next;
}Reference, *LReference;

// ---- Char Tree ------
typedef struct TreeNode{
	LReference file_reference;
	char info;
	struct TreeNode *left_child;
	struct TreeNode *right_sibling;
} TreeNode, *NTree;

// ---- Max-Heap -------
typedef int (*compareFiles)(LFile, LFile);

typedef struct FileHeap {
    int nrMax;
    int nrElem;
    LFile *v;
    compareFiles comp;
} TFileHeap;

// -------- Function Prototypes --------

// file.c
void Manage_Commands(FILE *fin, FILE *fout, FList *file_list,  NTree root);

// list.c
FList Init_File_List();
void Add_Keyword(char *word, LFile file);
void Add_File(FILE *fin, FILE *fout, FList *f, NTree root);
void Free_File_List(FList f);
int Scan_and_Search_if_File_Exists(FILE *fin, FILE *fout, FList *f, LFile *specific_file);
void Delete_File(FILE *fin, FILE *fout, FList *f, NTree root);
void AddKw(FILE *fin, FILE *fout, FList *f, NTree root);
void DelKw(FILE *fin, FILE *fout, FList *f, NTree root);
int Check_if_Keyword_exists(char *word, LFile file);
void Delete_Kw_From_File(FList *f, LFile file, char *word);

// tree.c
NTree Init_Tree();
void Add_Word_To_Tree(NTree *root, char *word, LFile file);
void Print(FILE *fout, NTree current, char *buffer, int level);
int Count_File_Refs(NTree leaf);
void Free_Tree(NTree current);
NTree Recursive_Delete_Node(NTree node, char *word, int index, char *file_id);
void Delete_Word_From_Tree(NTree *current, LFile file, char *word);
int Tree_Has_Keywords(NTree current);
void Find_Keyword(FILE *fin, FILE *fout, NTree *current);

// heap.c
void Swap_Files(LFile *a, LFile *b);
int Rel_Max_Heap_Files(LFile a, LFile b);
TFileHeap* Allocate_File_Heap(int nrMax, compareFiles comp);
void Free_File_Heap(TFileHeap **h);
void Insert_File_Heap(TFileHeap* h, LFile file);
LFile Extract_File_Heap(TFileHeap* h);
void TopK(FILE *fin, FILE *fout, NTree *current);
