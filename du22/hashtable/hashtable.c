/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  if (table == NULL){ // if the table is NULL
    return;
  }
  // set everything in the array to NULL
  for (int i = 0; i < HT_SIZE; i++) {
    (*table)[i] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  if (table == NULL || key == NULL){ // if the table or key are NULL
    return NULL;
  }

  int index = get_hash(key); // use the hash function for the index
  
  ht_item_t *active_item = (*table)[index];
  while (active_item != NULL){ // go through the linked list
    if (strcmp(active_item->key, key) == 0){
      return active_item; // if found
    }
    active_item = active_item->next; // else go to next element
  }
  // if not found
  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  if (table == NULL || key == NULL){ // if the table or key are NULL
    return;
  }

  ht_item_t *found_item = NULL;
  found_item = ht_search(table, key); // search for the key

  // if exists
  if (found_item){
    found_item->value = value; // if found -> replace the value
    return;
  }

  // if doesnt exist
  int index = get_hash(key); // get the index
  ht_item_t *new_item = malloc(sizeof(ht_item_t)); // alloc new item
  if (!new_item){
    exit(EXIT_FAILURE); // error handling
  }
  new_item->key = malloc(strlen(key) + 1); // alloc memory for the key  
  if (!new_item->key){
    free(new_item);
    exit(EXIT_FAILURE); // error handling
  }
  strcpy(new_item->key, key); // copy the key
  // init the new item
  new_item->value = value;
  new_item->next = NULL;
  if((*table)[index] != NULL){
    new_item->next = (*table)[index]; // insert the new item to the beginning of the list
  }
  (*table)[index] = new_item;
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if (table == NULL || key == NULL){ // if the table or key are NULL
    return NULL;
  }
  ht_item_t *found_item = NULL;
  found_item = ht_search(table, key); // search for the key

  if (found_item){
    return &found_item->value; // if found return the value
  }
  // if not found
  return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  if (table == NULL || key == NULL){ // if the table or key are NULL
    return;
  }
  int index = get_hash(key); // get index
  ht_item_t *active_item = (*table)[index];
  ht_item_t *prev_item = NULL;
  
  // go thorugh the linked list
  while(active_item != NULL && strcmp(active_item->key, key) != 0){
    prev_item = active_item; // save the previous element
    active_item = active_item->next;
  }

  // if not found
  if (!active_item){
    return;
  }

  // if the element is first
  if (!prev_item){
    (*table)[index] = active_item->next;
  }else{
    prev_item->next = active_item->next;
  }

  // free
  if (active_item->key){
    free(active_item->key);
  }
  free(active_item);
  active_item = NULL; // just to be sure
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  if (table == NULL){ // if the table is NULL
    return;
  }
  // go through the array
  for (int i = 0; i < HT_SIZE; i++){
    ht_item_t *active_item = (*table)[i];
    // go through the linked list
    while (active_item != NULL){ 
      ht_item_t *next = active_item->next;
      // free the key and the item
      free(active_item->key);
      free(active_item);
      active_item = next;
    }
    // set the array to NULL
    (*table)[i] = NULL;
  }
}
