#include <stdio.h>
#include <stdlib.h>


typedef struct {
    char* name;
    int value;
} card;

typedef struct {
    card* items;
    size_t count;
    size_t capacity;
} deck;

void append(deck dec, card item){
    dec.items[dec.count++] = item; // this doesn't work when adding more than 1 element
}

int main() {
    deck dec1, dec2;
    dec1.items = malloc(4*sizeof(*dec1.items));
    dec2.items = malloc(4*sizeof(*dec2.items));\
    dec1.count = 0;
    dec2.count = 0;
    card item = {"card1",1};
    card ite = {"card2",2};
    append(dec1, item);
    append(dec2, dec1.items[0]);
    printf("%s\n",dec2.items[0].name);
    append(dec1, ite);
    card temp;
    temp = dec1.items[1];
    printf("something is done %s\n",temp.name);
    dec2.items[1] = temp;
    printf("%s",dec2.items[1].name);
    return 0;
}
