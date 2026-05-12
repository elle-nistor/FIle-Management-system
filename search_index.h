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

// --- Function Prototypes ---

// file.c
FList Init_File_List();
void Add_Keyword(char *word, LFile file);
void Add_File(FILE *fin, FILE *fout, FList *f);
void Manage_Commands(FILE *fin, FILE *fout, LFile file);

// tree.c
NTree Init_Tree();
void Add_Word_To_Tree(NTree *root, char *word, LFile file);
void Print(FILE *fout, NTree root, int level);
int Count_File_Refs(NTree leaf);



