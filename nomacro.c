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
  tgt->items=malloc(tgt->capacity*sizeof(*tgt->items));
}

// freeing the deck memory.
// it also sets T.items to NULL to make sure its safe
// usage: clearDeck(deck castle);

void clearDeck(deck *tgt) {
  free(tgt->items);
  tgt->items=NULL;
}

// moves the cards from c element to s element of the deck
// usage: arrangeDeck(castle, 8, castle.start);

void arrangeDeck(deck *tgt, const unsigned int new, const unsigned int current) {
  memmove(&tgt->items[new], &tgt->items[current], tgt->count*sizeof(*tgt->items));
  tgt->start=new;
}

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


// appends an item to the bottom of the deck
// usage: deckAppend(&castle, item);

void deckAppend(deck *tgt, const card crd) {
  if (tgt->count+tgt->start >= tgt->capacity) arrangeDeck(tgt,8,tgt->start);
  tgt->items[tgt->start+tgt->count++] = crd;
}

// fills the deck with a previously provided array
// usage: fillDeck(&deck, array, length);

void deckFill(deck *tgt, const card *array, const unsigned int length) {
  for (int i=0; i<length; i++) deckAppend(tgt, array[i]);
}

// shuffles the card contents of the deck
// usage: deckShuffle(&deck);
// NOTE: This needs cardswap() to be tested properly if decided on bitwise.

void deckShuffle(deck *tgt) {
  for (int i=tgt->count-1; i>0; i--) {
    int j=rand()%(i+1);
    cardswap(&tgt->items[tgt->start+i], &tgt->items[tgt->start+j]);
  }
}

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
    if (h%16==15) printf("\n");
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
  printf("s: Play Scoundrel\nS: Scound rules\nd: Play Donsol\nD: Donsol rules\nr: Play Regicide\nR: Regicide rules\nx: Stats\nq: Quit\n");
  // ANSI escape codes do underline and red color
  // 0m resets the text formatting
}

// NOTE: SCOUNDREL SECTION
// TODO: Logic for Scoundrel

void scoundrel() {
  card scoundeck[44] = {
    {"\033[36mA\u2663\033[0m",'A','C'},
    {"\033[36m2\u2663\033[0m",'2','C'},
    {"\033[36m3\u2663\033[0m",'3','C'},
    {"\033[36m4\u2663\033[0m",'4','C'},
    {"\033[36m5\u2663\033[0m",'5','C'},
    {"\033[36m6\u2663\033[0m",'6','C'},
    {"\033[36m7\u2663\033[0m",'7','C'},
    {"\033[36m8\u2663\033[0m",'8','C'},
    {"\033[36m9\u2663\033[0m",'9','C'},
    {"\033[36mX\u2663\033[0m",'X','C'},
    {"\033[36mJ\u2663\033[0m",'J','C'},
    {"\033[36mQ\u2663\033[0m",'Q','C'},
    {"\033[36mK\u2663\033[0m",'K','C'},
    {"\033[35mA\u2660\033[0m",'A','S'},
    {"\033[35m2\u2660\033[0m",'2','S'},
    {"\033[35m3\u2660\033[0m",'3','S'},
    {"\033[35m4\u2660\033[0m",'4','S'},
    {"\033[35m5\u2660\033[0m",'5','S'},
    {"\033[35m6\u2660\033[0m",'6','S'},
    {"\033[35m7\u2660\033[0m",'7','S'},
    {"\033[35m8\u2660\033[0m",'8','S'},
    {"\033[35m9\u2660\033[0m",'9','S'},
    {"\033[35mX\u2660\033[0m",'X','S'},
    {"\033[35mJ\u2660\033[0m",'J','S'},
    {"\033[35mQ\u2660\033[0m",'Q','S'},
    {"\033[35mK\u2660\033[0m",'K','S'},
    {"\033[33m2\u2666\033[0m",'2','D'},
    {"\033[33m3\u2666\033[0m",'3','D'},
    {"\033[33m4\u2666\033[0m",'4','D'},
    {"\033[33m5\u2666\033[0m",'5','D'},
    {"\033[33m6\u2666\033[0m",'6','D'},
    {"\033[33m7\u2666\033[0m",'7','D'},
    {"\033[33m8\u2666\033[0m",'8','D'},
    {"\033[33m9\u2666\033[0m",'9','D'},
    {"\033[33mX\u2666\033[0m",'X','D'},
    {"\033[31m2\u2665\033[0m",'2','H'},
    {"\033[31m3\u2665\033[0m",'3','H'},
    {"\033[31m4\u2665\033[0m",'4','H'},
    {"\033[31m5\u2665\033[0m",'5','H'},
    {"\033[31m6\u2665\033[0m",'6','H'},
    {"\033[31m7\u2665\033[0m",'7','H'},
    {"\033[31m8\u2665\033[0m",'8','H'},
    {"\033[31m9\u2665\033[0m",'9','H'},
    {"\033[31mX\u2665\033[0m",'X','H'}
  };
  deck dungeon;
  initDeck(&dungeon);
  clearDeck(&dungeon);
  goback();
  return;
}


// NOTE: DONSOL SECTION
// TODO: Logic for Donsol

void donsol() {
  card scoundeck[44] = {
    {"\033[36mA\u2663\033[0m",'A','C'},
    {"\033[36m2\u2663\033[0m",'2','C'},
    {"\033[36m3\u2663\033[0m",'3','C'},
    {"\033[36m4\u2663\033[0m",'4','C'},
    {"\033[36m5\u2663\033[0m",'5','C'},
    {"\033[36m6\u2663\033[0m",'6','C'},
    {"\033[36m7\u2663\033[0m",'7','C'},
    {"\033[36m8\u2663\033[0m",'8','C'},
    {"\033[36m9\u2663\033[0m",'9','C'},
    {"\033[36mX\u2663\033[0m",'X','C'},
    {"\033[36mJ\u2663\033[0m",'J','C'},
    {"\033[36mQ\u2663\033[0m",'Q','C'},
    {"\033[36mK\u2663\033[0m",'K','C'},
    {"\033[35mA\u2660\033[0m",'A','S'},
    {"\033[35m2\u2660\033[0m",'2','S'},
    {"\033[35m3\u2660\033[0m",'3','S'},
    {"\033[35m4\u2660\033[0m",'4','S'},
    {"\033[35m5\u2660\033[0m",'5','S'},
    {"\033[35m6\u2660\033[0m",'6','S'},
    {"\033[35m7\u2660\033[0m",'7','S'},
    {"\033[35m8\u2660\033[0m",'8','S'},
    {"\033[35m9\u2660\033[0m",'9','S'},
    {"\033[35mX\u2660\033[0m",'X','S'},
    {"\033[35mJ\u2660\033[0m",'J','S'},
    {"\033[35mQ\u2660\033[0m",'Q','S'},
    {"\033[35mK\u2660\033[0m",'K','S'},
    {"\033[33m2\u2666\033[0m",'2','D'},
    {"\033[33m3\u2666\033[0m",'3','D'},
    {"\033[33m4\u2666\033[0m",'4','D'},
    {"\033[33m5\u2666\033[0m",'5','D'},
    {"\033[33m6\u2666\033[0m",'6','D'},
    {"\033[33m7\u2666\033[0m",'7','D'},
    {"\033[33m8\u2666\033[0m",'8','D'},
    {"\033[33m9\u2666\033[0m",'9','D'},
    {"\033[33mX\u2666\033[0m",'X','D'},
    {"\033[31m2\u2665\033[0m",'2','H'},
    {"\033[31m3\u2665\033[0m",'3','H'},
    {"\033[31m4\u2665\033[0m",'4','H'},
    {"\033[31m5\u2665\033[0m",'5','H'},
    {"\033[31m6\u2665\033[0m",'6','H'},
    {"\033[31m7\u2665\033[0m",'7','H'},
    {"\033[31m8\u2665\033[0m",'8','H'},
    {"\033[31m9\u2665\033[0m",'9','H'},
    {"\033[31mX\u2665\033[0m",'X','H'}
  };
  deck dungeon;
  initDeck(&dungeon);
  clearDeck(&dungeon);
  goback();
  return;
}


// NOTE: REGICIDE SECTION
// TODO: Rewrite for regicide using multiple functions

typedef struct {
  card enemy;
  int hp;
  int atk;
} regienemy;

void regishuffle(deck *tgt) {
  deck temp; temp.count=4; temp.capacity=4; temp.start=0;
  // this instead of initDeck(temp);
  temp.items=malloc(4*sizeof(*temp.items));
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

void regiAddEnemy(bool *new, regienemy *en, const deck castle, const int num) {
  if(new) {
    en->enemy = castle.items[num+castle.start];
    switch (en->enemy.value) {
      case 'J':
	en->hp = 20;
	en->atk = 10;
	break;
      case 'Q':
	en->hp = 30;
	en->atk = 15;
	break;
      case 'K':
	en->hp = 40;
	en->atk = 20;
	break;
      default:
	printf("invalid value\n");
	break;
    }
    new = false;
  }
}

// error codes

// 0 hand is fine
// 1 combo with A has more than 2 cards
// 2 matched combo without A or Regi has more than 10 value
// 3 combo has unmatched cards
// 4 no selected cards
//
// WARN: This logic needs to be checked.
// Everything works fine but there needs to be some unit tests.

int regiHandCheck(const deck hand, const bool held[]) {
  int value,first,cardnum,temp;
  value=0; first=0; cardnum=0;
  bool containsA,matched,containsRegi;
  containsA=false; matched=false; containsRegi=false;
  for (int i=0; i<8; i++) {
    if(held[i]) {
      temp = getValue(hand.items[i].value, 'r');
      if (temp==1) containsA=true;
      if (temp>10) containsRegi=true;
      switch (++cardnum) {
	case 1: if (!containsA) first=temp; break;
	case 2:
	  if (!containsA) {
	    matched = (temp==first);
	    if (!matched) return 3;
	  }
	  break;
	default:
	  if (containsA) return 1;
	  else if (matched) {
	    matched = (temp==first);
	    if(!matched) return 3;
	  }
	  break;
      }
      value+=temp;
    }
  }
  if (!containsA && !containsRegi && value>10) return 2;
  if (cardnum==0) return 4;
  return 0;
}

// NOTE: This needs to be checked

int getRegiDmg (const deck used, const bool held[]) {
  int atk=0;
  for (int i=0; i<8; i++) {
    if (held[i]) atk+=getValue(used.items[i].value, 'r');
  }
  return atk;
}

// a bit redundant function for checking defensive state, just to type less

bool regiDmgCheck (const deck used, const bool held[], const int atk) {
  if (getRegiDmg(used, held) >= atk) return true;
  else return false;
}

int getUsedCards (const bool held[]) {
  int amt=0;
  for (int i=0; i<8; i++) {if (held[i]) amt++;}
  return amt;
}

void regiHandleInput(const bool empty[], bool held[]) {
regs1in:
  switch (getkey()) {
    case '1':
      if(empty[0]) goto regs1in;
      held[0] = !held[0];
      break;
    case '2':
      if(empty[1]) goto regs1in;
      held[1] = !held[1];
      break;
    case '3':
      if(empty[2]) goto regs1in;
      held[2] = !held[2];
      break;
    case '4':
      if(empty[3]) goto regs1in;
      held[3] = !held[3];
      break;
    case '5':
      if(empty[4]) goto regs1in;
      held[4] = !held[4];
      break;
    case '6':
      if(empty[5]) goto regs1in;
      held[5] = !held[5];
      break;
    case '7':
      if(empty[6]) goto regs1in;
      held[6] = !held[6];
      break;
    case '8':
      if(empty[7]) goto regs1in;
      held[7] = !held[7];
      break;
  }
}

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
  srand(time(NULL)); // this does not need to be reinitialized
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
  int foes = 0;
  bool newen = true;
  int missing = 0;
  int dmg = 0;
  bool held[8] = {0,0,0,0,0,0,0,0};
  bool empty[8] = {0,0,0,0,0,0,0,0};
  bool confirm = false;
  bool suits[4] = {0,0,0,0};
  
  // draw hand of 8 (remove 8 from tavern)
  for (int i = 0; i<8; i++){
    deckAppend(&hand, deckTrim(&tavern));
  }

  while (foes<12 && missing<8) {
regstart:
    // NOTE: STEP 0:
    // this is only for testing
    regiAddEnemy(&newen,&en,castle,foes);
    foes++;
    printDeck(castle, foes);
    newen=true;
    printf("\n");
    // TODO: instead of step 1 and step 3, do a flip switch between atk and def
    // and merge it all into one function, then you can call it twice
    

  }

  clearDeck(&castle);
  clearDeck(&tavern);
  clearDeck(&discard);
  clearDeck(&hand);
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
