#include <locale.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
	#include <conio.h>
#else
	#include <termios.h>
	#include <unistd.h>
#endif

#define _DEBUG

// needs to either be reformatted into one deck, called upon the start of the game or will stay like this for each new game added
// will also be really nice to add ANSI to each card

// full ANSI formatted deck of 54 cards
// 31 red
// 33 yellow
// 35 magenta
// 36 cyan

typedef struct{
    char *name; // formatted string meant for card display
    char value; // this can be int, needs to be defined by each game
    char suit; // ambiguous regarding jokers in games, but can pass
} card;

typedef struct {
    card *items; // pointer to array of items
    size_t count; // how many elements are in the deck
    size_t start; // marks the start of the vector
    size_t capacity; // how many elements CAN be in the deck (modified by append and trim)
} deck;

// gigantic fucking switch statement lol
// retrieves the value from the char of the card based on the game
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
                case 'x': return 11;
                case 'r': return 10;
                default: return 11;
            }
        case 'Q':
            switch (game) {
                case 'x': return 11;
                case 'r': return 15;
                case 'd': return 13;
                default: return 12;
            }
        case 'K':
            switch (game) {
                case 'x': return 11;
                case 'r': return 20;
                case 'd': return 15;
                default: return 13;
            }
        case 'A':
            switch (game) {
                case 'x': return 11;
                case 'r': return 1;
                case 'd': return 17;
                default: return 11;
            }
        case 'j':
            switch (game) {
                case 'd': return 21;
                default: return 0;
            }
        default: return 0;
    }
}

// frees the memory
#define clearDeck(T)\
    do {\
        free(T.items);\
        T.count = 0;\
        T.start = 0;\
        T.capacity = 64;\
    } while(0)

// all of these need to be macros

#define initDeck(T)\
    do {\
        T.count = 0;\
        T.start = 0;\
        T.capacity = 64;\
        T.items = malloc(T.capacity*sizeof(*T.items));\
    } while(0);

// trims the deck from behind, memory can still be accessed for leftover cards but it will be changed after appending.
// no need to set the value to null when we have array size
#define deckTrim(T)\
    do {\
        if (--T.count+T.start == (T.capacity/2)-1 && T.count+T.start > 64) {\
        T.capacity /= 2;\
        T.items = realloc(T.items, T.capacity*sizeof(*T.items));\
        }\
    } while(0);

// void deckDel(deck dec) { // deletes items from the front
//     // you dont need this michael
//     // cause a memory leak michael
// }

#define deckAppend(T, I)\
    do {\
        if (T.count+T.start >= T.capacity) {\
            if (T.capacity == 0) T.capacity = 64;\
            T.capacity *= 2;\
            T.items = realloc(T.items, T.capacity*sizeof(*T.items));\
        }\
        T.items[T.start+T.count++] = I;\
    } while(0);


#define fillDeck(T, A, S)\
    do {\
        for (int i = 0; i<S; i++){\
            deckAppend(T,A[i]);\
        };\
    } while(0);

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

void stats() {
    // needs to parse a file containing plaintext of 5 of each game's scores
}

void cardswap(card *a, card *b){ // this works cuz it's a pointer to a type?
    card temp = *a;
    *a = *b;
    *b = temp;
}

#define drawLogic(T) ({T.count--; card retval = T.items[T.start++]; retval;})
// usage: deckAppend(deck output, drawLogic(deck input));
// adds the first card of the input deck to the last card of the output deck

#define deckShuffle(T)\
    do {\
        srand(time(NULL));\
        for (int i = T.count-1; i>0; i--){\
            int j = rand() % (i+1);\
            cardswap(&T.items[i], &T.items[j]);\
        };\
    } while(0);
// shuffles the cards around

char getkey() {
	#ifdef _WIN32
		// windows terminal input noecho
		return _getch(); // maybe can just return _getch() instead?
	#else
		// linux terminal inout noecho
		struct termios oldt, newt;
		char ch;
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO); // flag for disabling echo
		tcsetattr(STDIN_FILENO, TCSANOW, &newt); // disables echo
		ch = getchar();
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // enables echo
		return ch;
	#endif
}

void debug(){

};

void goback(){ // returns to the menu screen
    printf("\033[36mPress any key to go back...\033[0m");
	getkey();
	printf("\n");
}

void menu(){
	printf("\nWelcome to cardungeon!\nPlease press one of the following keys:\nc: Basic controls\n");
	printf("s: Play Scoundrel\nS: Scoundrel rules\nd: Play Donsol\nD: Donsol rules\nr: Play Regicide\nR: Regicide rules\nx: Stats\nq: Quit\n");
	// ANSI escape codes do underline and red color
	// 0m resets the text formatting
}

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
    initDeck(dungeon);

	// shuffle deck
	//
	// turn order:
	// 1: deal hand based on missing cards
	// 2: discard or play 3 cards
	//     2.1: cannot discard if last play was a discard, can only discard at the start of the turn
	//     2.5: if discarded, shuffle hand and append to end of deck array

    while (1) {
	   return;
	}
}

void donsol() {
    card donsoldeck[54] = {
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
        {"\033[36mJ\u2663\033[39m",'J','C'},
        {"\033[36mQ\u2663\033[39m",'Q','C'},
        {"\033[36mK\u2663\033[39m",'K','C'},
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
        {"\033[35mJ\u2660\033[39m",'J','S'},
        {"\033[35mQ\u2660\033[39m",'Q','S'},
        {"\033[35mK\u2660\033[39m",'K','S'},
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
        {"\033[33mJ\u2666\033[39m",'J','D'},
        {"\033[33mQ\u2666\033[39m",'Q','D'},
        {"\033[33mK\u2666\033[39m",'K','D'},
        {"\033[31mA\u2665\033[39m",'A','H'},
        {"\033[31m2\u2665\033[39m",'2','H'},
        {"\033[31m3\u2665\033[39m",'3','H'},
        {"\033[31m4\u2665\033[39m",'4','H'},
        {"\033[31m5\u2665\033[39m",'5','H'},
        {"\033[31m6\u2665\033[39m",'6','H'},
        {"\033[31m7\u2665\033[39m",'7','H'},
        {"\033[31m8\u2665\033[39m",'8','H'},
        {"\033[31m9\u2665\033[39m",'9','H'},
        {"\033[31mX\u2665\033[39m",'X','H'},
        {"\033[31mJ\u2665\033[39m",'J','H'},
        {"\033[31mQ\u2665\033[39m",'Q','H'},
        {"\033[31mK\u2665\033[39m",'Y','H'},
        {"\033[31mjR\033[39m",'j','R'},
        {"\033[35mjB\033[39m",'j','B'}
    };

    deck dungeon;
    int diff = 0;
    bool cleared = false;
    int missing = 4;
	bool empty[4]={0,0,0,0};
	int difficulty;
	int hp = 20;

	deck playing;
    initDeck(playing);
    initDeck(dungeon);
	fillDeck(dungeon, donsoldeck, 54);
    deckShuffle(dungeon);

	while (1) {
dondiffin:
    // difficulty selection
		switch (getkey()){
			case '1': diff = 0; break;
			case '2': diff = 1; break;
			case '3': diff = 2; break;
			case 'q': printf("exiting...\n"); return;
			default: goto dondiffin;
		}
	}
	// the actual game
	while (dungeon.count > 0 && hp > 0) {
	    if (playing.count <= 1) {
			while (playing.count < 4) {
			    deckAppend(playing, dungeon.items[dungeon.start]);
				dungeon.start++;
				dungeon.count--;
			}

		}
donsolin:
        // play the actual game
		switch (getkey()) {
		    case '1':
			case '2':
			case '3':
			case '4':
			case 'e':
			    // easy: all monsters dead or previous room fully cleared
				// medium: previous room fully cleared
				// hard: all monsters dead
				switch (diff) {
                    case 0:
                    case 1:
                    case 2:
                    default: printf("difficulty error\n"); break;
				}
			    break;
			case 'q': printf("quitting\n"); return; break;
			default: goto donsolin;
		}
	}
	// results:
	if (hp <= 0) {
	    printf("you lost\n");
	}
	else {
	    printf("you won\n");
	}
}

typedef struct{
    card enemy;
    int hp;
    int atk;
} regienemy;

#define regishuffle(T)\
    do {\
        deck temp; temp.count = 4; temp.capacity = 64;\
        temp.items = malloc(4*sizeof(*temp.items));\
        int iter = 0;\
        for (int i = 0; i<T.count; i++){\
            temp.items[i%4] = T.items[i];\
            if (i%4==3){\
                deckShuffle(temp);\
                for (int j = 0; j<4;j++){\
                    T.items[iter] = temp.items[j];\
                    iter++;\
                }\
            }\
        }\
    } while(0);

// 0 hand is fine
// 1 combo with A has more than 2 cards
// 2 matched combo without A or Regi has more than 10 value
// 3 combo has unmatched cards
// 4 no selected cards

int regiHandCheck(deck hand, bool held[]) {
    int value = 0;
    int first = 0;
    bool containsA = false;
    bool matched = false;
    bool containsRegi = false;
    int cardnum = 0;
    int temp;
    for (int i = 0;i<8;i++){
        if (held[i]) {
            temp = getValue(hand.items[i].value, 'r');
            if (temp == 1){ //these stay true permanently
                containsA = true;
            }
            if (temp > 10){
                containsRegi = true;
            }
            switch (++cardnum) {
                case 1:
                    if (!containsA){
                        first = temp;
                    }
                    break;
                case 2:
                    if (!containsA) {
                        matched = (temp == first);
                        if (!matched) {
                            return 3; // unmatched 2 cards without A
                        }
                    }
                    break;
                default:
                    if (containsA){
                        return 1; // 3 cards in A combo
                    }
                    else if (matched) {
                        matched = (temp == first);
                        if (!matched) {
                            return 3; // unmatched 3 cards withut A
                        }
                    }
                    break;
            }
            value += temp;
        }
    }
    if (!containsA && !containsRegi && value > 10) {
        return 2; // bad sum
    }
    if (cardnum == 0) {
        return 4; // no cards
    }
    return 0; //otherwise valid
}

bool regiDmgCheck (deck dec, bool held[], int atk){
    int total=0;
    for (int i = 0; i<8;i++){
        if(held[i]){
            total += getValue(dec.items[i].value, 'r');
        }
    }
    if (total >= atk) return true;
    else return false;
}

#define getRegiValue(D,H) ({int retval=0; for (int i=0;i<D.count;i++){if(H[i]){retval+=getValue(D.items[i].value,'r');};}; retval;});

#define getUsedCards(D,H) ({int retval=0; for (int i=0;i<D.count;i++){if(H[i]){retval++;};}; retval;});

void printDeck(deck D) {
    for (int h=0;h<D.count;h++){\
        printf("%s ",D.items[h+D.start].name);
        if(h%10==9){
            printf("\n");
        }
    }
}

void regiPrint(deck X,deck Y,deck Z){
        printf("\nTavern\n");
        printDeck(X);
        printf("\nDiscard\n");
        printDeck(Y);
        printf("\nCastle\n");
        printDeck(Z);
        printf("\n");
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

    deck castle, tavern, discard, hand;
	initDeck(castle);
	initDeck(tavern);
	initDeck(discard);
	initDeck(hand);
	fillDeck(castle, regiendeck, 12);
	fillDeck(tavern, regipldeck, 40);
	deckShuffle(tavern); // shuffles player deck
	regishuffle(castle); // special algo for shuffling enemy deck

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
	    deckAppend(hand, drawLogic(tavern));
		//tavern.start++;
		//tavern.count--;
	}

	while (enemy < 12 && missing < 8) {
regstart:
	    if (newen) { // STEP 0 draws the new enemy from castle
			en.enemy = castle.items[enemy];
			switch (castle.items[enemy].value) { // assigns stats to enemy based on card value
			    case 'J':
					en.hp = 20;
					en.atk = 10;
					break;
				case 'Q':
				    en.hp = 30;
					en.atk = 15;
					break;
				case 'K':
				    en.hp = 40;
					en.atk = 20;
					break;
				default:
				    printf("invalid value");
				    break;
			}
			newen = false; // initialized
		}

		// STEP 1 getkey switch for playing the game
regs1:
        while (!confirm) {
            if (missing == 8) {
                goto regs2;
            }
            printf("\n ---------------------------------------------------------------\n\n Attack!");
            if (perfect) {printf("   Perfect kill!"); perfect = 0;}
            printf("\n");
            // needs to display enemy, atk, hp and your hand along with selected cards
            #ifdef _DEBUG
            regiPrint(tavern, discard, castle);
            #endif
            printf("\n Enemy:%s(%d)  HP:%d  ATK:%d\n\n Hand:%d | Tavern:%lu | Discard:%lu | Jokers:%d\n",en.enemy.name, enemy, en.hp, en.atk, 8-missing, (unsigned long)tavern.count, (unsigned long)discard.count, jokers);
            printf(" \u250F\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2513\n");
            printf(" \u2503");
            for (int i = 0; i<8; i++){ // player hand
                if (held[i]) printf("\033[47m");
                if (!empty[i]) printf(" %s \033[49m\u2503",hand.items[i].name);
                else printf("    \u2503");
            }
            printf("\n \u2517\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u251b\n 1-8: Select/deselect a card");
            if (jokers > 0) printf("   j: Play a joker");
            printf("   Enter: confirm   q: Quit\n");
regs1in:
            switch (getkey()) {
                case '1':
                    if (empty[0]) goto regs1in;
                    held[0] = !held[0];
                    break;
                case '2':
              		if (empty[1]) goto regs1in;
              		held[1] = !held[1];
                    break;
                case '3':
              		if (empty[2]) goto regs1in;
               	    held[2] = !held[2];
                    break;
                case '4':
                    if (empty[3]) goto regs1in;
                    held[3] = !held[3];
                    break;
                case '5':
                    if (empty[4]) goto regs1in;
                    held[4] = !held[4];
                    break;
                case '6':
                    if (empty[5]) goto regs1in;
                    held[5] = !held[5];
                    break;
                case '7':
                    if (empty[6]) goto regs1in;
                    held[6] = !held[6];
                    break;
                case '8':
                    if (empty[7]) goto regs1in;
                    held[7] = !held[7];
                    break;
                case 'j':
                    if (jokers > 0) {
                        for (int i=0;i<8;i++){
                            if (held[i]) {
                                held[i] = false;
                            }
                            if (!empty[i]) {
                                deckAppend(discard, hand.items[i]);
                                missing++;
                            }
                            empty[i] = false;
                            if (tavern.count>0) {
                                hand.items[i] = tavern.items[tavern.start];
                                tavern.start++; tavern.count--;
                                missing--;
                            }
                        }
                        jokers --;
                    }
                    else {
                        goto regs1in;
                    }
                    // needs to empty hand to discard, draw new cards from tavern and disable one joker use
                    break;
                case '\r': confirm = true; break;
                case '\n': confirm = true; break;
                case 'q': printf("quitting...\n"); return;
				default: goto regs1in;
            }
        }
        confirm = false; // resets value of confirm bool

        // step 2 add values and check hand

        switch (regiHandCheck(hand, held)) {
            case 0: break; // hand is good
            case 1: goto regs1in; // cannot pair more than 2 aces;
            case 2: goto regs1in; // paired cards add up to higher than 10
            case 3: goto regs1in; // bad pair
            case 4: goto regs1in; // no card selected
            // needs more cases to explain what's bad
            default: printf(" invalid hand\n"); goto regs1;
                // goto regs1 if bad hand;
        }

        missing += getUsedCards(hand, held); // add 'missing' counter
        dmg = getRegiValue(hand, held); // add cards to value
        // this is kryptonite to everyone who is aware of dynamic programming
        // however as of now i am not aware of a better solution and do not wish
        // to spend too much of my time looking for one

        bool suits[4]= {0,0,0,0}; // spades, clubs, diamonds, hearts in that order
        for (int i = 0; i<8;i++){ // check for suits
            if (held[i]) {
                switch (hand.items[i].suit) {
                    case 'S': suits[0]=true; break;
                    case 'C': suits[1]=true; break;
                    case 'D': suits[2]=true; break;
                    case 'H': suits[3]=true; break;
                    default: printf("invalid behavior"); break;
                }
            }
        }
        switch (en.enemy.suit) { // disables the suit
            case 'S': suits[0]=false; break;
            case 'C': suits[1]=false; break;
            case 'D': suits[2]=false; break;
            case 'H': suits[3]=false; break;
            default: printf("invalid behavior"); break;
        }
        #ifdef _DEBUG
        printf("\nS:%d, C:%d, D:%d, H:%d\n",suits[0],suits[1],suits[2],suits[3]);
        printf("\ndmg: %d\n",dmg);
        #endif
        for (int i = 0; i<8;i++){ // empty hands
            if (!empty[i]) {
                if (held[i]) {
                    empty[i] = true;
                    deckAppend(discard, hand.items[i]);
                    held[i] = false;
                }
            }
        }
        // step 3 activate powers
        // order matters
        if (suits[3]) { // hearts shuffle discard and then put num of cards into bottom of deck
            deckShuffle(discard);
            int temp = discard.count;
            for (int i=0; i<temp && i<dmg;i++){
                deckAppend(tavern, discard.items[i]);
                discard.count--;
            }
        };
        if (suits[2]) { // draws cards from the deck
            int drawn=0;
            for (int i=0; i<8 && drawn<dmg && tavern.count>0; i++) {
                if (empty[i]) {
                    hand.items[i] = tavern.items[tavern.start];
                    tavern.start++; tavern.count--;
                    empty[i] = false;
                    missing--;
                    drawn++;
                }
            }
        }
        if (suits[0]) { // reduces atk
            en.atk -= dmg;
            if (en.atk < 0) {
                en.atk = 0;
            }
        }
        if (suits[1]) dmg *= 2; // doubles damage

        // step 3.1 enemy takes damage
        en.hp -= dmg;

        // step 3.5 check if enemy is alive
		if (en.hp <= 0){
		    if (en.hp == 0){
				perfect = 1;
				tavern.items[tavern.start-1] = castle.items[enemy]; tavern.start--; tavern.count++;
			} else deckAppend(discard, castle.items[enemy]);
		    enemy++; newen = true; // needs a new enemy if the one is dead
			goto regstart;
		}
regs2:

        // step 4 getkey to pick damage taken
        if (en.atk == 0) goto regs1;

		while (!confirm && missing<8) {
		    printf("\n ---------------------------------------------------------------\n\n Defend!\n");
            // needs to display enemy, atk, hp and your hand along with selected cards
            #ifdef _DEBUG
            regiPrint(tavern, discard, castle);
            #endif
            printf("\n Enemy:%s(%d)  HP:%d  ATK:%d\n\n Hand:%d | Tavern:%lu | Discard:%lu | Jokers:%d\n",en.enemy.name, enemy, en.hp, en.atk, 8-missing, (unsigned long)tavern.count, (unsigned long)discard.count, jokers);
            printf(" \u250F\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2513\n");
            printf(" \u2503");
            for (int i = 0; i<8; i++){ // player hand
                if (held[i]) printf("\033[47m");
                if (!empty[i]) printf(" %s \033[49m\u2503",hand.items[i].name);
                else printf("    \u2503");
            }
            printf("\n \u2517\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u251b\n");
            printf(" 1-8: Select/deselect a card   Enter: confirm   q: Quit\n");
regs2in:
		    switch (getkey()) {
				case '1':
                    if (empty[0]) goto regs2in;
                    held[0] = !held[0];
                    break;
                case '2':
              		if (empty[1]) goto regs2in;
              		held[1] = !held[1];
                    break;
                case '3':
              		if (empty[2]) goto regs2in;
               	    held[2] = !held[2];
                    break;
                case '4':
                    if (empty[3]) goto regs2in;
                    held[3] = !held[3];
                    break;
                case '5':
                    if (empty[4]) goto regs2in;
                    held[4] = !held[4];
                    break;
                case '6':
                    if (empty[5]) goto regs2in;
                    held[5] = !held[5];
                    break;
                case '7':
                    if (empty[6]) goto regs2in;
                    held[6] = !held[6];
                    break;
                case '8':
                    if (empty[7]) goto regs2in;
                    held[7] = !held[7];
                    break;
                case '\r': confirm = true; break;
                case '\n': confirm = true; break;
                case 'q': printf("quitting...\n"); return;
                default:
                    goto regs2in;
			}
		}
		confirm = false;
		if (!regiDmgCheck(hand, held, en.atk)){
		    int use = getUsedCards(hand, held); // checks if not enough cards are left
			if (use == 8-missing) {
			    missing = 8;
			    goto noplayreg;
			}
		    goto regs2in;
		}

		missing += getUsedCards(hand, held); // add 'missing' counter

		for (int i = 0; i<8;i++){ // empty hands
            if (!empty[i]) {
                if (held[i]) {
                    empty[i] = true;
                    deckAppend(discard, hand.items[i]);
                    held[i] = false;
                }
            }
        }
	}
	noplayreg:
	if (missing >= 8){
	   printf("\nyou lose\n");
	}
	else {
	   printf("\nkilled everyone congrats\n");
	}
	clearDeck(tavern);
	clearDeck(castle);
	clearDeck(hand);
	clearDeck(discard);
}

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);  // For Windows
	#endif
    char ch;
	bool gamerunning = true;
	// welcome message
	//printf("\n\n\033[35;49m\u250F\u2501\u2501\u2501\u2513\n\u2503\033[45;30mA  \033[35;49m\u2503\n\u2503\033[45;30m \u2660 \033[35;49m\u2503\n\u2503\033[45;30m  A\033[35;49m\u2503\n\u2517\u2501\u2501\u2501\u251b\033[0m\n\nThe ace of spades!\n");
	printf("\n\n\033[35;49m\u250F\u2501\u2501\u2501\u2513\n\u2503A  \u2503\n\u2503 \u2660 \u2503\n\u2503  A\u2503\n\u2517\u2501\u2501\u2501\u251b\033[0m\n\nThe ace of spades!\n");
	//printf("\n\n\033[30;45mA  \033[0m\n\033[30;45m \u2660 \033[0m\n\033[30;45m  A\033[0m\n");
	while(gamerunning){
		menu();
input:
		switch (getkey()) {
		    case 'c':
				printf("\nCards are displayed as a value and suit.\n");
				printf("Values are displayed as: A,2,3,4,5,6,7,8,9,X,J,Q,K, X is 10 but as a single character.\n");
				printf("Suits are displayed as: \033[35mS\033[0mpades,\033[36mC\033[0mlubs,\033[33mD\033[0miamonds,\033[31mH\033[0mearts\n");
				printf("Example: \033[31mAH\033[0m, \033[36m9C\033[0m\n");
				printf("Jokers are displayed as: \033[31mjR\033[0m and \033[35mjB\033[0m\n");
				printf("All relevant controls will be shown on the screen during each turn.\n\n");
				break;
			case 'x':
			    stats();
				break;
#ifdef _DEBUG
			case '.':
			    debug();
				break;
#endif
			case 's':
				printf("COMING SOON\n");
			    scoundrel(); // starts scoundrel
				break;
			case 'S': // scoundrel rules
			    printf("\nScoundrel tutorial\n");
				printf("COMING SOON\n");
				break;
			case 'd':
				printf("COMING SOON\n");
				donsol(); // starts donsol (first difficulty picker, quits if specified)
				break;
			case 'D': // donsol rules
                printf("\nDonsol tutorial\n");
				printf("COMING SOON\n");
				break;
			case 'r':
				regicide(); // starts regicide
				break;
			case 'R': // regicide rules
                printf("\nRegicide is a modern playing card game. Solo rules apply.\n");
                printf("You win the game by defeating the entire castle deck, composed of Jacks, Queens and Kings (in that order).\n");
                printf("You lose the game by having an empty hand.\n");
                printf("You have 8 cards in hand and 2 jokers that allow you to discard your entire hand and pull 8 more from the tavern.\n");
                printf("Every used card and overkilled enemy goes into discard deck,\n");
                printf("if enemies are defeated with exact amount of damage they go on top of tavern.\n");
                printf("Jacks have 20hp and 10atk, Queens have 30hp and 15atk, Kings have 40hp and 20atk.\n");
                printf("Each suit has its' own power:\n\n   Spades permanently reduce enemy atk by value,\n   Clubs deal double damage,\n");
                printf("   Diamonds draw cards from tavern deck equal to value,\n");
                printf("   \033[31mHearts\033[0m take cards from discard (after shuffling discard) into tavern.\n\n");
                printf("Depending on enemy suit, that suit will be disabled for the encounter.\n");
                printf("Value is equal to all played cards summed together.\n");
                printf("A has a value of 1, all enemy cards have same values as their atk.\n");
                printf("You can play more than one card if the cards are the same value and their combined value does not exceed 10.\n");
                printf("A is also an animal companion and can be paired with any card or sets of cards.\n");
                printf("\nTurn order:\n\n   1. Player plays a card.\n   2. Player activates avaliable suit powers.\n");
                printf("   3. Player deals damage to the enemy.\n   4. Player suffers damage from the enemy, if the enemy isn't defated.\n\n");
				break;
			case 'q': gamerunning = false; printf("byebye\n"); return 0;
			default:
				goto input;
		}
		goback();
	}
}
