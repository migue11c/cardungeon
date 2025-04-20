#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
  char *name;
  char value;
} card;

typedef struct {
  card *items;
  size_t count;
  size_t start;
  size_t capacity;
} deck;

void initDeck(deck *tgt) {
  tgt->count = 0;
  tgt->start = 8;
  tgt->capacity = 64;
  tgt->items = malloc(tgt->capacity*sizeof(card));
}

void addToDeck(deck *tgt, const unsigned int amt, const unsigned int n, const card *crd) {
  for (int i=0; i<amt; i++) tgt->items[tgt->start+n+i]=crd[i];
  tgt->count++;
}

int main() {
  deck castle;
  initDeck(&castle);
  card shit[2] = {{"shitcard",'x'},{"goodcard",'y'}};
  unsigned int n = 3;
  int length = sizeof(shit)/sizeof(card);
  addToDeck(&castle, length, n, shit);
  printf("%s",castle.items[castle.start+n].name);
  printf("%s",castle.items[castle.start+n+1].name);
  return 0;
}
