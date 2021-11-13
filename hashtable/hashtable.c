/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
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
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  for (int i = 0; i < HT_SIZE; i++)
  {
    (*table)[i] = NULL;
  } 
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    ht_item_t* item = (*table)[get_hash(key)];
    while(item != NULL)
    {
        if(key == item->key)
            return item;
        item = item->next;
    }
    return item;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  ht_item_t * item = ht_search(table, key);
  if(item == NULL){
    //item is non existent so we create and insert a new one
    ht_item_t *new = (ht_item_t *)malloc(sizeof(ht_item_t));
    new->next=(*table)[get_hash(key)];
    new->key=key;
    new->value = value;
    (*table)[get_hash(key)] = new;
  }else{
    //the item is already inside the table, we just update it
    item->value=value;
  }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *item = ht_search(table, key);
  if(item == NULL)
    return NULL;
  return &(item->value);
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  ht_item_t *parent = NULL;
  ht_item_t *item = (*table)[get_hash(key)];

  bool finished = false;
  while(item != NULL && !finished){
    if(item->key == key){
      if(parent == NULL){
        //we are deleting the first item
        (*table)[get_hash(key)] = item->next;
      }else{
        //we are deleting an item, which is the middle of the List or at the end
        parent->next = item->next;
      }
      free(item);
      item = NULL;
      finished = true;
      continue;
    }
    parent = item;
    item = item->next;
  }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  for (int i = 0; i < MAX_HT_SIZE; i++)
  {
    ht_item_t *item = (*table)[i];
    // int j = 0;

    while(item != NULL){
      //fprintf(stderr,"deleting %s at index %i\n",item->key,i);
      ht_item_t *next  = item->next;
      if(item->value != -1){
        free(item);
      }
      item = next;
    }
    (*table)[i] = NULL;
  }
}
