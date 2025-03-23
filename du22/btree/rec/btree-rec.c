/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
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
  *tree = NULL; // simply initialize it as NULL
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  if (!tree){ // basecase if the tree is empty
    return false;
  }
  
  if (tree->key == key){ // if we found the key -> assign the addres of nodes content to value ptr
    *value = &tree->content;
    return true;
  }
  else if(key < tree->key){ // If the key is less then we search left subtree
    if (tree->left == NULL){
      return false;
    }
    return bst_search(tree->left, key, value);
  }
  else{ // else we search right subtree
    if(tree->right == NULL){
      return false;
    }
    return bst_search(tree->right, key, value);
  }
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíčem už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  if (*tree == NULL){ // basecase: inserting a new node
    *tree = malloc(sizeof(bst_node_t)); // alloc a new node
    if (*tree == NULL){ 
      exit(EXIT_FAILURE); // error handling
    }
    (*tree)->key = key;
    (*tree)->content = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    return;
  }

  if ((*tree)->key == key){ // if key already exists -> replace with new value
    if((*tree)->content.value != NULL){
      free((*tree)->content.value);
      (*tree)->content.value = NULL;
    }
    (*tree)->content = value;
  }
  else if (key < (*tree)->key){ // we look in the left subtree
    bst_insert(&((*tree)->left), key, value);
  }
  else{ // else we look in the right subtree
    bst_insert(&((*tree)->right), key, value);
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  // if tree is null or empty
  if (!tree || *tree == NULL){
    return;
  }
  
  if ((*tree)->right != NULL){ // find the rightmost node
    bst_replace_by_rightmost(target, &((*tree)->right));
  }else{
    if (target->content.value != NULL){
      free(target->content.value);
    }
    // temporary node to delete
    bst_node_t *temp_node = *tree;
    // replace the key, content
    target->content = (*tree)->content;
    (*tree)->content.value = NULL;
    target->key = (*tree)->key;
    
    // deleting the rigtmost node
    *tree = (*tree)->left;
    free(temp_node);
  }
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
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  if (*tree == NULL) {
    return; // key not found
  }

  // search left subtree
  if (key < (*tree)->key) {
    bst_delete(&((*tree)->left), key);
  }
  // search right subtree
  else if (key > (*tree)->key) {
    bst_delete(&((*tree)->right), key);
  }
  else { // key found
    if ((*tree)->left == NULL && (*tree)->right == NULL) { // leaf node
      free((*tree)->content.value); // free the content
      free(*tree);
      *tree = NULL;
    }
    else if ((*tree)->left == NULL && (*tree)->right != NULL) { // only right child
      bst_node_t *temp_node = *tree;
      *tree = (*tree)->right;
      free(temp_node->content.value); // free the content
      free(temp_node);
    }
    else if ((*tree)->right == NULL && (*tree)->left != NULL) { // only left child
      bst_node_t *temp_node = *tree;
      *tree = (*tree)->left;
      free(temp_node->content.value); // free the content
      free(temp_node);
    }
    else { // both children
      if ((*tree)->left != NULL){
        bst_replace_by_rightmost(*tree, &((*tree)->left));
      }
    }
  }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  if(*tree != NULL){
    // dispose left and right trees by recursion
    bst_dispose(&((*tree)->left));
    bst_dispose(&((*tree)->right));

    // freeing the content
    if((*tree)->content.value != NULL){
      free((*tree)->content.value);
      (*tree)->content.value = NULL;
    }

    // freeing the node
    bst_node_t *temp_node = *tree;
    free(temp_node);
    *tree = NULL; // set to null
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  // if the tree is not empty
  if (tree != NULL){
    bst_add_node_to_items(tree, items);
    bst_preorder(tree->left, items);
    bst_preorder(tree->right, items);
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  // if the tree is not empty
  if (tree != NULL){
    bst_inorder(tree->left, items);
    bst_add_node_to_items(tree, items);
    bst_inorder(tree->right, items);
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  // if the tree is not empty
  if (tree != NULL){
    bst_postorder(tree->left, items);
    bst_postorder(tree->right, items);
    bst_add_node_to_items(tree, items);
  }
}
