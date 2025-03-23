/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree)
{
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  bst_node_t *active_node = tree;
  while(active_node != NULL){ // traversing the tree
    if (active_node->key == key){
      *value = &active_node->content; // found
      return true;
    }
    else if (key < active_node->key){
      active_node = active_node->left; // moving to left subtree
    }
    else{
      active_node = active_node->right; // moving to right
    }
  }

  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  bst_node_t *active = *tree;
  bst_node_t *parent = NULL;

  while(active != NULL){ // traversing the tree
    parent = active;
    if(key == active->key){
      if (active->content.value != NULL){
        free(active->content.value);
      }
      active->content = value;
      return;
    }
    // moving to subtrees
    else if (key < active->key){
      active = active->left;
    }
    else{
      active = active->right;
    }
  }

  // if key doesnt exist then we insert new node
  bst_node_t *new_node = malloc(sizeof(bst_node_t));
  if (new_node == NULL){
    exit(EXIT_FAILURE); // error handling
  }
  // init stuff
  new_node->key = key;
  new_node->content = value;
  new_node->left = NULL;
  new_node->right = NULL;

  if (parent == NULL){ // if tree was empty then the new node becomes the root
    *tree = new_node;
  }
  // traversing tree
  else if(key < parent->key){
    parent->left = new_node;
  }
  else{
    parent->right = new_node;
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  if (tree == NULL || *tree == NULL) {
    return;
  }

  bst_node_t *active_node = *tree;
  bst_node_t *parent = NULL;

  // we find the rightmost node
  while (active_node->right != NULL) {
    parent = active_node;
    active_node = active_node->right;
  }

  // free the rightmost nodes content
  if (target->content.value != NULL) {
    free(target->content.value);
    target->content.value = NULL;
  }

  // copy the key and content
  target->key = active_node->key;
  target->content = active_node->content;

  // delete the rightmost node
  active_node->content.value = NULL;
  if (parent != NULL){
    parent->right = active_node->left;
  }
  else{
    // if the rightmost node is the root
    *tree = active_node->left;
  }

  free(active_node);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t **active_node_ptr = tree; // to keep track of the parent
  bst_node_t *active_node = *tree;

  // find the node to delete
  while (active_node != NULL && active_node->key != key) {
    if (key < active_node->key) {
      active_node_ptr = &(active_node->left); // the link should point to left child
      active_node = active_node->left;
    } else {
      active_node_ptr = &(active_node->right); // the link should point to right child
      active_node = active_node->right;
    }
  }

  if (!active_node) {
    return; // no key found
  }

  // // if the node has only one subtree
  if (active_node->left == NULL || active_node->right == NULL) {
    bst_node_t *child_node = NULL;
    if (active_node->left != NULL){
      child_node = active_node->left;
    }
    else{
      child_node = active_node->right;
    }

    *active_node_ptr = child_node; // replace the node with the child

    // free the content
    if (active_node->content.value != NULL) {
      free(active_node->content.value);
    }
    free(active_node);
  }
  // if the node has both subtrees
  else {
    // replace the node with the rightmost node of the left subtree to keep the tree balanced
    bst_replace_by_rightmost(active_node, &(active_node->left));
  }
}


/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  // if the tree is empty
  if (!tree || *tree == NULL) {
    return;
  }

  // init stack
  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bst_push(&stack, *tree); // push the root onto the stack

  // traverse the tree 
  while (!stack_bst_empty(&stack)) { // go through the stack
    bst_node_t *new_node = stack_bst_pop(&stack); // pop the node
    // if the node has left or right child -> we push them onto the stack
    if (new_node->left != NULL) {
      stack_bst_push(&stack, new_node->left);
    }
    if (new_node->right != NULL) {
      stack_bst_push(&stack, new_node->right);
    }

    // free the content
    if (new_node->content.value != NULL) {
      free(new_node->content.value);
      new_node->content.value = NULL;
    }

    // free the node
    free(new_node);
    new_node = NULL; // just trying to find the mem leaks
  }

  // set tree to NULL
  *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{
  if (tree == NULL) { // if the tree is empty
    return;
  }
  // if the tree is empty
  bst_node_t *active_node = tree;
  while (active_node != NULL) { // go through tree
    bst_add_node_to_items(active_node, items); 
    if (active_node->right != NULL) {
      stack_bst_push(to_visit, active_node->right); // push right child onto the stack
    }
    active_node = active_node->left; // move the left child
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  // init stack
  stack_bst_t stack;
  stack_bst_init(&stack);

  // traverse the tree
  bst_leftmost_preorder(tree, &stack, items);

  // go through the stack
  while (!stack_bst_empty(&stack)) {
    bst_node_t *active_node = stack_bst_pop(&stack); // pop the node
    bst_leftmost_preorder(active_node, &stack, items);
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  // if the tree is empty
  bst_node_t *active_node = tree;
  while (active_node != NULL) { // go through the tree
    stack_bst_push(to_visit, active_node); // push the node onto the stack
    active_node = active_node->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  // init stack
  stack_bst_t stack;
  stack_bst_init(&stack);

  // traverse the tree
  bst_leftmost_inorder(tree, &stack);

  // go through the stack
  while (!stack_bst_empty(&stack)) {
    bst_node_t *active_node = stack_bst_pop(&stack);
    bst_add_node_to_items(active_node, items); // visit the node

    // if the node has right child
    if (active_node->right != NULL) {
      bst_leftmost_inorder(active_node->right, &stack);
    }
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
  // if the tree is empty
  bst_node_t *active_node = tree;
  while (active_node != NULL) {
    stack_bst_push(to_visit, active_node); // push the node onto the stack
    stack_bool_push(first_visit, true); // this is the first visit
    active_node = active_node->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  // init stack
  stack_bst_t to_visit_stack;
  stack_bst_init(&to_visit_stack);

  stack_bool_t first_visit; // stack for bool values
  stack_bool_init(&first_visit);

  // traverse the tree
  bst_leftmost_postorder(tree, &to_visit_stack, &first_visit);

  // go through the stack
  while (!stack_bst_empty(&to_visit_stack)) {
    bst_node_t *active_node = stack_bst_top(&to_visit_stack);
    bool first = stack_bool_top(&first_visit);
    stack_bool_pop(&first_visit);

    // if the node was visited
    if (first) { 
      stack_bool_push(&first_visit, false); // mark the node as visited
      if (active_node->right != NULL) {
        bst_leftmost_postorder(active_node->right, &to_visit_stack, &first_visit);
      }
    } else {
      bst_add_node_to_items(active_node, items); // visit the node
      stack_bst_pop(&to_visit_stack);
    }
  }
}
