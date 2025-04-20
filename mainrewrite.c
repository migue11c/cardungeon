#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define _DEBUG

#ifdef _WIN32
  #include <windows.h>
  #include <conio.h>
#else
  #include <termios.h>
  #include <unistd.h>
#endif

// ANSI color format:
// 31 red
// 33 yellow
// 35 magenta
// 36 cyan

// takes keyboard input without echo, can be handled later

char getkey() {
#ifdef _WIN32
  // windows terminal input noeecho
  return _getch();
#else
  // linux terminal input noecho
  struct termios oldt, newt;
  char ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO); // flag for disabling echo
  tcsetattr(STDIN_FILENO, TCSANOW, &newt); // applying the flag
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // reenabling echo
  return ch;
#endif
}

typedef struct {
  char *name; // formatted string meant for card display
  char value; // this can also be int, needs to be defined in each game
  char suit; // ambiguous regarding usable jokers. need extra func
} card;

typedef struct {
  card *items;
  size_t count;
  size_t start;
  size_t capacity;
  // can add bool held;
} deck;

// switch statement that determines value of the card based on the game
// games supported: scoundrel, donsol, regicide

int getValue(char rank, char game){
  switch (rank) {
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'X': return 10;
    case 'J':
      switch (game) {
	case 's': return 11;
	case 'r': return 10;
	default: return 11;
      }
    case 'Q':
      switch (game) {
	case 's': return 11; // WARN: this needs checking
	case 'r': return 15;
	case 'd': return 13;
	default: return 12;
      }
    case 'K':
      switch (game) {
	case 's': return 11; // WARN: this needs checking
	case 'r': return 20;
	case 'd': return 15;
	default: return 13;
      }
    case 'A':
      switch (game) {
	case 's': return 11; // WARN: this needs checking
	case 'r': return 1;
	case 'd': return 17;
	default: return 1;
      }
    case 'j':
      switch (game) {
	case 'd': return 21;
	default: return 0;
      }
    default: return 0;
  }
}


// initializes memory for the deck
// start is at 8 to offset putting cards in the front
// and to encourage better code
// usage: initDeck(deck castle);

void initDeck(deck *tgt) {
  tgt->count=0;
  tgt->start=8;
  tgt->capacity=64;
  tgt->items=malloc(tgt->capacity*sizeof(card));
}

//#define initDeck(T)\
  do {\
    T.count=0;\
    T.start=8;\
    T.count=64;\
    T.items=malloc(T.capacity*sizeof(card));\
    if(!T.items) {perror("malloc failed"); exit(1);}\
  } while(0);


// freeing the deck memory.
// it also sets T.items to NULL to make sure its safe
// usage: clearDeck(deck castle);

void clearDeck(deck *tgt) {
  free(tgt->items);
  tgt->items=NULL;
}

//#define clearDeck(T)\
  do {\
    free(T.items);\
    T.items = NULL;\
  } while(0);

// moves the cards from c element to s element of the deck
// usage: arrangeDeck(castle, 8, castle.start);

void arrangeDeck(deck *tgt, const unsigned int new, const unsigned int current) {
  memmove(&tgt->items[new], &tgt->items[current], tgt->count*sizeof(card));
  tgt->start=new;
}

//#define arrangeDeck(Z,s,c)\
  do {\
    memmove(&Z.items[s], &Z.items[c], Z.count*sizeof(card));\
    Z.start=s;\
  } while(0);


// swaps two cards
// NOTE: This is without bitwise ops.

void cardswap(card *a, card *b) {
  card temp = *a;
  *a = *b;
  *b = temp;
}

// takes the top item off of the deck, memory of it is still accessible
// no need to set it to null tho, but we can do it for fun
// usage: deckTrim(deck castle);
// or: card next = deckTrim(deck castle);
// WARN: This needs to be tested

card deckTrim(deck *tgt) {
  card temp;
  if (0<tgt->count--) {
    temp = tgt->items[tgt->start++];
  }
  return temp;
}

//#define deckTrim(T) ({card retval = NULL; if (0<T.count--) {retval = T.items[T.start]; T.items[T.start++] = NULL;} retval;})


// appends an item to the bottom of the deck
// usage: deckAppend(&castle, item);

void deckAppend(deck *tgt, const card crd) {
  if (tgt->count+tgt->start >= tgt->capacity) arrangeDeck(tgt,8,tgt->start);
}

//#define deckAppend(T, I)\
  do {\
    if (T.count+T.start >= T.capacity) arrangeDeck(T,8,T.start);\
    T.items[T.start+T.count++] = I;\
  } while(0);


// fills the deck with a previously provided array
// usage: fillDeck(&deck, array, length);

void deckFill(deck *tgt, const card *array, const unsigned int length) {
  for (int i=0; i<length; i++) deckAppend(tgt, array[i]);
}

//#define deckFill(T, A, S)\
  do {\
    for (int i=0; i<S; i++) deckAppend(T,A[i]);\
  } while(0);


// shuffles the card contents of the deck
// usage: deckShuffle(&deck);
// NOTE: This needs cardswap() to be tested properly if decided on bitwise.

void deckShuffle(deck *tgt) {
  srand(time(NULL));
  for (int i=tgt->count-1; i>0; i--) {
    int j=rand()%(i+1);
    cardswap(&tgt->items[tgt->start+i], &tgt->items[tgt->start+j]);
  }
}

// #define deckShuffle(T)\
  do {\
    srand(time(NULL));\
    for (int i = T.count-1; i>0; i--) {\
      int j = rand() % (i+1);\
      cardswap(&T.items[T.start+i], &T.items[T.start+j]);\
    };\
  } while(0);

// prints ALL items of the deck, even the null ones
// NOTE: This is typically for debug purposes

void fullyPrintDeck(const deck D) {
  for (int h=0;h<D.capacity;h++) {
    if (D.items[h].name != NULL) printf("%d:%s ",h,D.items[h].name); 
    else if (h>99) printf(" %d:EM ",h);
    else printf("%d:EM ",h);
    if (h%10==9) printf("\n");
  }
}

// prints only the usable elements of the deck
// number determines how many cards will be printed
// if at 0, it will print the entire usable deck

void printDeck(const deck D, int num) {
  if (num == 0) num=D.count;
  for (int h=0;h<num;h++) {
    printf("%s ",D.items[h+D.start].name);
    if (h%10==9) printf("\n");
  }
}


// common print function
void goback() {
  printf("\033[36mPress any key to go back...\033[0m");
  getkey();
  printf("\n");
}

void menu() {
  printf("\nWelcome to cardungeon!\nPlease press one of the following keys:\nc: Basic controls\n");
  printf("s: Play Scoundrel\nS: Scoundrel rules\nd: Play Donsol\nD: Donsol rules\nr: Play Regicide\nR: Regicide rules\nx: Stats\nq: Quit\n");
  // ANSI escape codes do underline and red color
  // 0m resets the text formatting
}

// NOTE: SCOUNDREL SECTION
// TODO: Logic for Scoundrel

void scoundrel() {
  goback();
  return;
}


// NOTE: DONSOL SECTION
// TODO: Logic for Donsol

void donsol() {
  goback();
  return;
}


// NOTE: REGICIDE SECTION
// TODO: Rewrite for regicide using multiple functions

typedef struct {
 
} regienemy;

void regishuffle(deck *tgt) {
  deck temp; temp.count=4; temp.capacity=4;
  temp.items=malloc(4*sizeof(card));
  int iter=0;
  for (int i=0; i<tgt->count; i++) {
    temp.items[i%4]=tgt->items[tgt->start+i];
    if(i%4==3) {
      deckShuffle(&temp);
      for (int j=0; j<4; j++) {
	tgt->items[tgt->start+iter]=temp.items[j];
	iter++;
      }
    }
  }
}

//#define regishuffle(T)\
  do {\
    deck temp; temp.count = 4; temp.capacity = 4;\
    temp.items = malloc(4*sizeof(card));\
    int iter=0;\
    for (int i=0; i<T.count; i++) {\
      temp.items[i%4] = T.items[T.start+i];\
      if (i%4==3) {\
	deckShuffle(temp);\
	for(int j=0; j<4; j++) {\
	  T.items[T.start+iter] = temp.items[j];\
	  iter++;\
	}\
      }\
    }\
  } while(0);

void regicide() {
  card regipldeck[40] = {
    {"\033[36mA\u2663\033[39m",'A','C'},
    {"\033[36m2\u2663\033[39m",'2','C'},
    {"\033[36m3\u2663\033[39m",'3','C'},
    {"\033[36m4\u2663\033[39m",'4','C'},
    {"\033[36m5\u2663\033[39m",'5','C'},
    {"\033[36m6\u2663\033[39m",'6','C'},
    {"\033[36m7\u2663\033[39m",'7','C'},
    {"\033[36m8\u2663\033[39m",'8','C'},
    {"\033[36m9\u2663\033[39m",'9','C'},
    {"\033[36mX\u2663\033[39m",'X','C'},
    {"\033[35mA\u2660\033[39m",'A','S'},
    {"\033[35m2\u2660\033[39m",'2','S'},
    {"\033[35m3\u2660\033[39m",'3','S'},
    {"\033[35m4\u2660\033[39m",'4','S'},
    {"\033[35m5\u2660\033[39m",'5','S'},
    {"\033[35m6\u2660\033[39m",'6','S'},
    {"\033[35m7\u2660\033[39m",'7','S'},
    {"\033[35m8\u2660\033[39m",'8','S'},
    {"\033[35m9\u2660\033[39m",'9','S'},
    {"\033[35mX\u2660\033[39m",'X','S'},
    {"\033[33mA\u2666\033[39m",'A','D'},
    {"\033[33m2\u2666\033[39m",'2','D'},
    {"\033[33m3\u2666\033[39m",'3','D'},
    {"\033[33m4\u2666\033[39m",'4','D'},
    {"\033[33m5\u2666\033[39m",'5','D'},
    {"\033[33m6\u2666\033[39m",'6','D'},
    {"\033[33m7\u2666\033[39m",'7','D'},
    {"\033[33m8\u2666\033[39m",'8','D'},
    {"\033[33m9\u2666\033[39m",'9','D'},
    {"\033[33mX\u2666\033[39m",'X','D'},
    {"\033[31mA\u2665\033[39m",'A','H'},
    {"\033[31m2\u2665\033[39m",'2','H'},
    {"\033[31m3\u2665\033[39m",'3','H'},
    {"\033[31m4\u2665\033[39m",'4','H'},
    {"\033[31m5\u2665\033[39m",'5','H'},
    {"\033[31m6\u2665\033[39m",'6','H'},
    {"\033[31m7\u2665\033[39m",'7','H'},
    {"\033[31m8\u2665\033[39m",'8','H'},
    {"\033[31m9\u2665\033[39m",'9','H'},
    {"\033[31mX\u2665\033[39m",'X','H'}
  };
  card regiendeck[12] = {
    {"\033[36mJ\u2663\033[39m",'J','C'},
    {"\033[35mJ\u2660\033[39m",'J','S'},
    {"\033[33mJ\u2666\033[39m",'J','D'},
    {"\033[31mJ\u2665\033[39m",'J','H'},
    {"\033[36mQ\u2663\033[39m",'Q','C'},
    {"\033[35mQ\u2660\033[39m",'Q','S'},
    {"\033[33mQ\u2666\033[39m",'Q','D'},
    {"\033[31mQ\u2665\033[39m",'Q','H'},
    {"\033[36mK\u2663\033[39m",'K','C'},
    {"\033[35mK\u2660\033[39m",'K','S'},
    {"\033[33mK\u2666\033[39m",'K','D'},
    {"\033[31mK\u2665\033[39m",'K','H'}
  };
  deck castle, tavern, discard, hand;
  initDeck(&castle);
  initDeck(&tavern);
  initDeck(&discard);
  initDeck(&hand);
  deckFill(&castle, regiendeck, 12);
  deckFill(&tavern, regipldeck, 40);
  deckShuffle(&tavern); // shuffles player deck
  regishuffle(&castle); // special algo for shuffling enemy deck

  // pre-game setup
  bool perfect = 0;
  int jokers = 2;
  regienemy en = {};
  int enemy = 0;
  bool newen = 1;
  int missing = 0;
  int dmg = 0;
  bool held[8] = {0,0,0,0,0,0,0,0};
  bool empty[8] = {0,0,0,0,0,0,0,0};
  bool confirm = false;
  
  // draw hand of 8 (remove 8 from tavern)
  for (int i = 0; i<8; i++){
    deckAppend(&hand, deckTrim(&tavern));
  }
  printDeck(castle, 0);
  goback();
  return;
}

// \u2663 clubs
// \u2660 spades
// \u2666 diamonds
// \u2665 hearts

//static card detailedtemplatedeck[52] = {
// {"\033[36mA\u2663\033[0m",'A','C'},
// {"\033[36m2\u2663\033[0m",'2','C'},
// {"\033[36m3\u2663\033[0m",'3','C'},
// {"\033[36m4\u2663\033[0m",'4','C'},
// {"\033[36m5\u2663\033[0m",'5','C'},
// {"\033[36m6\u2663\033[0m",'6','C'},
// {"\033[36m7\u2663\033[0m",'7','C'},
// {"\033[36m8\u2663\033[0m",'8','C'},
// {"\033[36m9\u2663\033[0m",'9','C'},
// {"\033[36mX\u2663\033[0m",'X','C'},
// {"\033[36mJ\u2663\033[0m",'J','C'},
// {"\033[36mQ\u2663\033[0m",'Q','C'},
// {"\033[36mK\u2663\033[0m",'K','C'},
// {"\033[35mA\u2660\033[0m",'A','S'},
// {"\033[35m2\u2660\033[0m",'2','S'},
// {"\033[35m3\u2660\033[0m",'3','S'},
// {"\033[35m4\u2660\033[0m",'4','S'},
// {"\033[35m5\u2660\033[0m",'5','S'},
// {"\033[35m6\u2660\033[0m",'6','S'},
// {"\033[35m7\u2660\033[0m",'7','S'},
// {"\033[35m8\u2660\033[0m",'8','S'},
// {"\033[35m9\u2660\033[0m",'9','S'},
// {"\033[35mX\u2660\033[0m",'X','S'},
// {"\033[35mJ\u2660\033[0m",'J','S'},
// {"\033[35mQ\u2660\033[0m",'Q','S'},
// {"\033[35mK\u2660\033[0m",'K','S'},
// {"\033[33mA\u2666\033[0m",'A','D'},
// {"\033[33m2\u2666\033[0m",'2','D'},
// {"\033[33m3\u2666\033[0m",'3','D'},
// {"\033[33m4\u2666\033[0m",'4','D'},
// {"\033[33m5\u2666\033[0m",'5','D'},
// {"\033[33m6\u2666\033[0m",'6','D'},
// {"\033[33m7\u2666\033[0m",'7','D'},
// {"\033[33m8\u2666\033[0m",'8','D'},
// {"\033[33m9\u2666\033[0m",'9','D'},
// {"\033[33mX\u2666\033[0m",'X','D'},
// {"\033[33mJ\u2666\033[0m",'J','D'},
// {"\033[33mQ\u2666\033[0m",'Q','D'},
// {"\033[33mK\u2666\033[0m",'K','D'},
// {"\033[31mA\u2665\033[0m",'A','H'},
// {"\033[31m2\u2665\033[0m",'2','H'},
// {"\033[31m3\u2665\033[0m",'3','H'},
// {"\033[31m4\u2665\033[0m",'4','H'},
// {"\033[31m5\u2665\033[0m",'5','H'},
// {"\033[31m6\u2665\033[0m",'6','H'},
// {"\033[31m7\u2665\033[0m",'7','H'},
// {"\033[31m8\u2665\033[0m",'8','H'},
// {"\033[31m9\u2665\033[0m",'9','H'},
// {"\033[31mX\u2665\033[0m",'X','H'},
// {"\033[31mJ\u2665\033[0m",'J','H'},
// {"\033[31mQ\u2665\033[0m",'Q','H'},
// {"\033[31mK\u2665\033[0m",'Y','H'}
//};


void stats() {} // TODO: This system needs to get implemented.

int main() {
  regicide();
  getkey();
  return 0;
}
