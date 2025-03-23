/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte triviální funkci letter_count. Všimněte si, že výstupní strom může být značně degradovaný 
 * (až na úroveň lineárního seznamu). Jako typ hodnoty v uzlu stromu využijte 'INTEGER'.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je 
 * uložen ve stromu (klíč vždy lowercase).
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    // init tree
    bst_init(tree);

    // go through each character in the input
    for (int index = 0; input[index] != '\0'; index++) {
        char current_char = input[index];
        char norm_char;

        // normalize the key to lowercase and handle special characters
        if (current_char >= 'A' && current_char <= 'Z') {
            char offset = 'a' - 'A'; // offset for converting to lowercase
            norm_char = current_char + offset; // convert lowercase
        }
        else if (current_char >= 'a' && current_char <= 'z') { 
            norm_char = current_char; 
        }
        else if (current_char == ' ') { // space
            norm_char = ' ';
        }
        else {
            norm_char = '_'; // other characters
        }

        bst_node_content_t *node_content = NULL;

        // serach for the node
        bool found_node = bst_search(*tree, norm_char, &node_content);

        if (found_node && node_content != NULL) {
            // if node exists, increment the count
            if (node_content->value != NULL && node_content->type == INTEGER) {
                (*(int *)(node_content->value)) += 1;
            }
            else {
                // if the value is not an integer, set it to 1
                int *new_count  = malloc(sizeof(int));
                if (!new_count) {
                    exit(EXIT_FAILURE); 
                }
                *new_count  = 1;
                node_content->value = new_count;
                node_content->type = INTEGER;
            }
        }
        else {
            // if the node does not exist, create a new one
            int *count = malloc(sizeof(int));
            if (!count) {
                exit(EXIT_FAILURE);
            }
            *count = 1; 

            // create new content
            bst_node_content_t new_content_value;
            new_content_value.type = INTEGER;
            new_content_value.value = count;

            // insert the new node
            bst_insert(tree, norm_char, new_content_value);
        }
    }
}