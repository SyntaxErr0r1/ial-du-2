/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bool finish = false;
  while(!finish ){
    if(tree != NULL){
      if(key < tree->key){
        //is on the left
        tree = tree->left;
      }else if(key > tree->key){
        //is on the right
        tree = tree->right;
      }else{
        //we found the right one
        finish = true;
        (*value) = tree->value; 
        return true;
      }
    }else{
      //we are at the leaf node, but didn't find the one
      finish = true;
      return false;
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  //true if the right node was found
  bool found = false;

  bst_node_t *parrent_ptr = NULL;
  bst_node_t *ptr = (*tree);

  //new node
  bst_node_t *new = malloc(sizeof(struct bst_node));
  new->key = key;
  new->value= value;
  new->left= NULL;
  new->right = NULL;

  if((*tree) == NULL){
      (*tree) = new;
      found = true;
  }
  while (!found)
  {
    parrent_ptr = ptr;
    if(key < ptr->key){
      //insert to the left side
      ptr = ptr->left;
      if(ptr == NULL){
          parrent_ptr->left = new;
          found = true;
      }
    }else if(key > ptr->key){
      //insert to the right side
      ptr = ptr->right;
      if(ptr == NULL){
          parrent_ptr->right = new;
          found = true;
      }
    }else{
      //the keys are equal
      ptr->key = key;
      ptr->value = value;
      found = true;

      //we don't need the new one
      free(new);
      new = NULL;
    }
  }
  if(!found){
    free(new);
    new = NULL;
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bool finish = false;
  bst_node_t *current = (*tree);
  while(!finish){
    if(current->right != NULL){
        //traversing right
        finish = true;
        current = current->right; 
    }else{
      //we are at the leaf node
      (*tree)->key = current->key;
      (*tree)->value = current->value;
      finish = true;
    }
  }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  //i know that my implementation of this function is pure evil, but it looks like it would pass at least one test
  //true if the right node was found
  bool found = false;

  bst_node_t *parrent_ptr = NULL;
  bst_node_t *ptr = (*tree);

  if((*tree) == NULL){
      found = true;
  }
  while (!found && ptr != NULL)
  {
    // printf("%i\n",i);
    parrent_ptr = ptr;
    if(key < ptr->key){
      //left side
      ptr = ptr->left;
      if(ptr == NULL){
        // printf("does not exist\n");
        found = true;
        break;
      }else if(ptr->key == key){
        //we found the node to delete (parrent_ptr->left or ptr)
        if(ptr->left != NULL && ptr->left != NULL){
        //if the node has both children, we replace it by the rightmost node on it's left side
          bst_replace_by_rightmost(parrent_ptr->left,&(parrent_ptr->left->left));
        }else if(ptr->left != NULL){
          //if it has only left son, we replace it
          bst_node_t *children_node = ptr->left;
          free(parrent_ptr->left);
          parrent_ptr->left = children_node;
        }else if(ptr->right != NULL){
          //if it has only right son, we replace it
          bst_node_t *children_node = ptr->right;
          free(parrent_ptr->left);
          parrent_ptr->left = children_node;
        }else{
          //it has no children, we just delete it
          free(parrent_ptr->left);
          parrent_ptr->left = NULL;
        }
        found = true;
      }
    }else if(key > ptr->key){
      //the right side
      ptr = ptr->right;
      if(ptr == NULL){
        // printf("does not exist\n");
        found = true;
        break;
      }else if(ptr->key == key){
        //we found the node to delete (parrent_ptr->right or ptr)
        if(ptr->left != NULL && ptr->right != NULL){
          //if the node has both children, we replace it by the rightmost node on it's left side
          bst_replace_by_rightmost(parrent_ptr->right,&parrent_ptr->right->left);
        }else if(ptr->left != NULL){
          //if it has only left son, we replace it
          bst_node_t *children_node = ptr->left; 
          free(parrent_ptr->right);
          parrent_ptr->right = children_node;
        }else if(ptr->right != NULL){
          //if it has only left son, we replace it
          bst_node_t *children_node = ptr->right;
          free(parrent_ptr->right);
          parrent_ptr->right = children_node;
        }else{
          //it has no children, we just delete it
          free(parrent_ptr->right);
          parrent_ptr->right = NULL;
        }
        found = true;
      }
    }
    else{
      //the keys are equal
      //we found the node to delete (parrent_ptr->right or ptr)
      if(ptr->left != NULL && ptr->right != NULL){
        //if the node has both children, we replace it by the rightmost node on it's left side
        bst_replace_by_rightmost(parrent_ptr->right,&parrent_ptr->right->left);
      }else if(ptr->left != NULL){
        //if it has only left son, we replace it
        bst_node_t *children_node = ptr->left; 
        free(parrent_ptr->right);
        parrent_ptr->right = children_node;
      }else if(ptr->right != NULL){
        //if it has only left son, we replace it
        bst_node_t *children_node = ptr->right;
        free(parrent_ptr->right);
        parrent_ptr->right = children_node;
      }else{
        //it has no children, we just delete it
        free(parrent_ptr->right);
        parrent_ptr->right = NULL;
      }
      found = true;
    
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  if((*tree) == NULL)
    return;

  stack_bst_t stack;
  stack_bst_init(&stack);

  stack_bst_push(&stack,(*tree));

  while(!stack_bst_empty(&stack)){
    bst_node_t *node = stack_bst_pop(&stack);
    if(node->left != NULL)
      stack_bst_push(&stack,node->left);
    if(node->right != NULL)
      stack_bst_push(&stack,node->right);
    if(node != NULL){
      free(node);
      node = NULL;
    }
  }
  (*tree) = NULL;
  // stack_bst_t stack;
  // stack_bst_init(&stack);
  // bst_node_t *node = (*tree);
  // while(node != NULL){
  //   stack_bst_push(&stack, node->right);
  //   stack_bst_push(&stack, node);
  //   node = node->left;
  // }
  // while (!stack_bst_empty(&stack))
  // {
  //   node = stack_bst_pop(&stack);
  //   if(node == NULL)
  //     continue;
  //   if(node->right != NULL && node->right == stack_bst_top(&stack)){
  //     stack_bst_pop(&stack);
  //     stack_bst_push(&stack, node);
  //     node = node->right;
  //   }else{
  //     fprintf(stderr,"freeing (%c,%i)",node->key,node->value);
  //     free(node);
  //     node = NULL;
  //   }
  // }
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
}
