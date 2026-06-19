# File Indexing System - Nistor Daniela 314CB

# 1. Introduction
A C-based simplified file indexing system. This project manages a collection of files and their keywords, utilizing doubly linked lists, multi-way search trees, and max-heaps to efficiently search, update, and prioritize files based on their relevance score.

# 2. Input file
The program scans from the input file (`indexare.in`):
-> number of operations
-> scans operations line by line, followed by their parametres, and calls the specific function based on all the commands available (`ADD`, `DEL`, `ADDKW`, `DELKW`, `FIND`, `TOPK`, `PRINT`).

# 3. Data Structures

* **Doubly linked lists**
The Files are stored into a doubly linked list (with head and tail pointers) to facilitate easy insertion at the end and safe O(1) removal from any position when a file is deleted.

* **Multi-way Tree (Trie)**
Used to store the keywords letter by letter. The terminal nodes of the tree store a linked list of references (pointers) to the actual files that contain that specific keyword. 

* **Max-Heap**
Dynamically allocated during the `TOPK` operation to ensure files are extracted based on priority. The priority is dictated by the highest relevance score.

# 4. Output file
The output / result of the input commands (e.g., `OK`, `EXISTS`, `NOT FOUND`, `EMPTY`), the search results for specific keywords, or the full tree print, written in `indexare.out`.

# 5. Notable Execution Logic
* **Lexicographic Order:** When inserting a new word into the tree, the program compares the current char with siblings to ensure alphabetical order. The same is done for the file references list. Inserting them directly in the correct position is optimal, as it avoids an unnecesary sorting step later.

* **Recursive Tree Functions:** Tree operations (printing, freeing memory, and deleting keywords) are implemented recursively. Because a multi-way tree is inherently a recursive data structure, this approach natively tracks the traversal path (backtracking) via the call stack. It allows the system to elegantly evaluate and delete dead branches from the bottom up without the need for complex, manual stack structures.

* **Tree Garbage Collection:** During `DEL` and `DELKW`, a recursive function checks the tree nodes. If a node loses its last file reference and has no other children, it is completely removed and freed. The function safely goes back up to clean any other nodes that became useless, saving memory.

* **Custom Heap Comparator:** The max-heap uses a custom comparing function to sort the files. It first checks the integer score, and if there is a tie, it falls back to a lexicographical check (`strcmp`) on the file ID.

* **Comments** I used comments to explain important steps and for visual coherence. The mostly show the intuitive thinking, therefore I used a less formal language then in the Readme.