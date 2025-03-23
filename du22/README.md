# Data Structures Implementation Project (IAL Course)

- **Author:** Martin Ševčík (xsevcim00)
- **Institution:** Faculty of Information Technology, Brno University of Technology (FIT VUT Brno)
- **Date:** 2024/2025

This project implements several fundamental data structures and algorithms related to binary search trees and hash tables. The implementations are part of a school assignment where I had to complete the following components:

## Project Components

### 1. Binary Search Tree - Recursive Implementation (`btree/rec/btree-rec.c`)

A binary search tree implementation using recursive functions for all operations:
- Tree initialization
- Search operations
- Node insertion
- Node deletion
- Tree traversal (preorder, inorder, postorder)
- Memory management and tree disposal

### 2. Binary Search Tree - Iterative Implementation (`btree/iter/btree-iter.c`)

The same binary search tree functionality implemented iteratively using stacks:
- All operations are implemented without recursion
- Uses a custom stack implementation (`stack.c` and stack.h) for tree traversal
- Implements all standard BST operations (init, search, insert, delete, disposal)
- Traversals using preorder, inorder, and postorder methods

### 3. Letter Frequency Counter (`btree/exa/btree-exa.c`)

A practical application of the binary search tree that counts letter frequencies in a string:
- Counts occurrences of letters (a-z, case insensitive)
- Tracks space characters separately
- Groups all other characters under a '_' key
- Uses the BST implementations to store and retrieve frequency data

### 4. Hash Table Implementation (`hashtable/hashtable.c`)

A hash table with chaining to handle collisions:
- Implements open hashing with linked lists for collision resolution
- Supports operations like initialization, search, insertion, and deletion
- Memory management for proper cleanup of dynamically allocated resources
- Uses a basic hash function that can be improved for better performance

## Implementation Details

### Binary Search Tree

Both implementations (recursive and iterative) offer the same functionality:

```
void bst_init(bst_node_t **tree)               // Initialize an empty tree
bool bst_search(...)                            // Search for a key in the tree
void bst_insert(...)                            // Insert a new key-value pair
void bst_delete(...)                            // Delete a node by key
void bst_dispose(...)                           // Free all memory used by the tree
void bst_preorder/inorder/postorder(...)        // Tree traversal methods
```

The key difference is in the implementation approach:
- The recursive version uses natural recursion for simplicity
- The iterative version uses explicit stacks to manage traversal state

### Hash Table

The hash table implementation features:

```
void ht_init(ht_table_t *table)                 // Initialize the hash table
ht_item_t *ht_search(...)                       // Search for a key in the table
void ht_insert(...)                             // Insert or update a key-value pair
float *ht_get(...)                              // Get a value for a key
void ht_delete(...)                             // Delete an entry by key
void ht_delete_all(...)                         // Clear the entire table
```

## Building and Testing

Each component has its own Makefile for easy compilation:

```bash
# To compile and run the recursive BST implementation
cd btree/rec
make
./test

# To compile and run the iterative BST implementation
cd btree/iter
make
./test

# To compile and run the letter counting example
cd btree/exa
make
./test_rec    # Uses recursive implementation
./test_iter   # Uses iterative implementation

# To compile and run the hash table implementation
cd hashtable
make
./test
```

## Project Structure

```
du22/
├── btree/                      # Binary Search Tree implementations
│   ├── btree.c                 # Common BST functionality
│   ├── btree.h                 # BST interface definitions
│   ├── character.c             # Character data type support
│   ├── character.h             # Character type definitions
│   ├── test_util.c             # Testing utilities
│   ├── test_util.h             # Testing interface
│   ├── test.c                  # Main test file
│   ├── exa/                    # Example application
│   │   ├── btree-exa.c         # Letter frequency counter
│   │   └── Makefile            # Build script
│   ├── iter/                   # Iterative implementation
│   │   ├── btree-iter.c        # Iterative BST implementation
│   │   ├── stack.c             # Stack implementation
│   │   ├── stack.h             # Stack interface
│   │   └── Makefile            # Build script
│   └── rec/                    # Recursive implementation
│       ├── btree-rec.c         # Recursive BST implementation
│       └── Makefile            # Build script
├── hashtable/                  # Hash table implementation
│   ├── hashtable.c             # Hash table implementation
│   ├── hashtable.h             # Hash table interface
│   ├── test_util.c             # Testing utilities
│   ├── test_util.h             # Testing interface
│   ├── test.c                  # Main test file
│   └── Makefile                # Build script
└── README                      # Project documentation
```

## Learning Outcomes

This project demonstrates:
- Understanding of fundamental data structures (BST and hash tables)
- Different implementation approaches (recursive vs. iterative)
- Memory management in C
- Proper testing of data structure operations
- Practical application of data structures for solving problems

The implementations focus on correctness and proper memory management rather than optimizing for performance.