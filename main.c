#include <locale.h>
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

void clearDeck(deck dec) { // entirely deletes the deck
    free(dec.items);
    dec.count = 0;
    dec.capacity = 64;
}

int getSize(deck dec) {
    return dec.count;
}

void deckTrim(deck dec) { // trims the deck from the end
    // reallocates memory if needed, but reduces the item count at the start of the check
    if (--dec.count == (dec.capacity/2)-1 && dec.count > 64) {
        dec.capacity /= 2;
        dec.items = realloc(dec.items, dec.capacity*sizeof(*dec.items));
    }
}

void deckDel(deck dec) { // deletes items from the front
    // you dont need this michael
    // cause a memory leak michael
}

void deckAppend(deck dec, card crd) { // pushes an element to the back of the deck
    // this part expands the allocation if necessary
    if (dec.count >= dec.capacity) {
        if (dec.capacity == 0) dec.capacity = 64;
        dec.capacity *= 2;
        dec.items = realloc(dec.items, dec.capacity*sizeof(*dec.items));
    }
    dec.items[dec.count++] = crd;
}

void initDeck(deck dec, card arr[], int size){
    dec.capacity = 64;
    dec.count = 0;
    dec.items = malloc(dec.capacity*sizeof(*dec.items));
    for (int i=0; i<size;i++){
        deckAppend(dec, arr[i]);
    };
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

void stats() {

}

void cardswap(card *a, card *b){
    card temp = *a;
    *a = *b;
    *b = temp;
}

void deckshuffle(deck dec){
    srand(time(NULL));
    for (int i = dec.count-1; i>0; i--){
        int j = rand() % (i+1);
        cardswap(&dec.items[i], &dec.items[j]);
    }
}

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
    initDeck(dungeon, scoundeck, 44);

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
        {"\033[33mA\u2666\033[0m",'A','D'},
        {"\033[33m2\u2666\033[0m",'2','D'},
        {"\033[33m3\u2666\033[0m",'3','D'},
        {"\033[33m4\u2666\033[0m",'4','D'},
        {"\033[33m5\u2666\033[0m",'5','D'},
        {"\033[33m6\u2666\033[0m",'6','D'},
        {"\033[33m7\u2666\033[0m",'7','D'},
        {"\033[33m8\u2666\033[0m",'8','D'},
        {"\033[33m9\u2666\033[0m",'9','D'},
        {"\033[33mX\u2666\033[0m",'X','D'},
        {"\033[33mJ\u2666\033[0m",'J','D'},
        {"\033[33mQ\u2666\033[0m",'Q','D'},
        {"\033[33mK\u2666\033[0m",'K','D'},
        {"\033[31mA\u2665\033[0m",'A','H'},
        {"\033[31m2\u2665\033[0m",'2','H'},
        {"\033[31m3\u2665\033[0m",'3','H'},
        {"\033[31m4\u2665\033[0m",'4','H'},
        {"\033[31m5\u2665\033[0m",'5','H'},
        {"\033[31m6\u2665\033[0m",'6','H'},
        {"\033[31m7\u2665\033[0m",'7','H'},
        {"\033[31m8\u2665\033[0m",'8','H'},
        {"\033[31m9\u2665\033[0m",'9','H'},
        {"\033[31mX\u2665\033[0m",'X','H'},
        {"\033[31mJ\u2665\033[0m",'J','H'},
        {"\033[31mQ\u2665\033[0m",'Q','H'},
        {"\033[31mK\u2665\033[0m",'Y','H'},
        {"\033[31mjR\033[0m",'j','R'},
        {"\033[35mjB\033[0m",'j','B'}
    };

    deck dungeon;
    initDeck(dungeon,donsoldeck,54);
    deckshuffle(dungeon);
    // all 54 cards with ANSI values
	printf("donsol\n");
	// shuffle deck

	while (1) {
	    return;
	}
}

struct reginemy {
    card enemy;
    int hp;
    int atk;
};

void regishuffle(deck dec){
    deck temp;
    temp.count = 4;
    temp.capacity = 64;
    temp.items = malloc(4*sizeof(*temp.items));
    int iter = 0;
    for (int i = 0; i<12; i++){
        printf("test0\n"); // there is an issue here
        temp.items[i%4] = dec.items[i];
        printf("test1\n");
        if (i%4==3){
            deckshuffle(temp);
            printf("test2\n");
            for (int j = 0; j<4;j++){
                dec.items[iter] = temp.items[j];
                iter++;
            }
        }
    }
}

void regicide() {
    card regipldeck[40] = {
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
        {"\033[33mA\u2666\033[0m",'A','D'},
        {"\033[33m2\u2666\033[0m",'2','D'},
        {"\033[33m3\u2666\033[0m",'3','D'},
        {"\033[33m4\u2666\033[0m",'4','D'},
        {"\033[33m5\u2666\033[0m",'5','D'},
        {"\033[33m6\u2666\033[0m",'6','D'},
        {"\033[33m7\u2666\033[0m",'7','D'},
        {"\033[33m8\u2666\033[0m",'8','D'},
        {"\033[33m9\u2666\033[0m",'9','D'},
        {"\033[33mX\u2666\033[0m",'X','D'},
        {"\033[31mA\u2665\033[0m",'A','H'},
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
    card regiendeck[12] = {
        {"\033[36mJ\u2663\033[0m",'J','C'},
        {"\033[35mJ\u2660\033[0m",'J','S'},
        {"\033[33mJ\u2666\033[0m",'J','D'},
        {"\033[31mJ\u2665\033[0m",'J','H'},
        {"\033[36mQ\u2663\033[0m",'Q','C'},
        {"\033[35mQ\u2660\033[0m",'Q','S'},
        {"\033[33mQ\u2666\033[0m",'Q','D'},
        {"\033[31mQ\u2665\033[0m",'Q','H'},
        {"\033[36mK\u2663\033[0m",'K','C'},
        {"\033[35mK\u2660\033[0m",'K','S'},
        {"\033[33mK\u2666\033[0m",'K','D'},
        {"\033[31mK\u2665\033[0m",'K','H'}
    };

    deck castle, tavern, discard;
	initDeck(castle, regiendeck, 12);

	initDeck(tavern, regipldeck, 40);
	deckshuffle(tavern); // shuffles player deck
	regishuffle(castle); // special algo for shuffling enemy deck

	// there is something wrong with regishuffle

	// notes to keep in mind
	//
	// stats: atk/hp
	// J 10/20
	// Q 15/30
	// K 20/40
	// this can be easily managed
	//
	// first jacks then queens than kings

	#ifdef _DEBUG
	printf("\n");
	for (int i=0;i<40;i++){
		printf("%s ",tavern.items[i].name);
		if (i%10==9){
			printf("\n");
		}
	}
	printf("\n");
	for (int i=0;i<12;i++){
		printf("%s ",castle.items[i].name);
		if (i%6==5){
		    printf("\n");
		}
	}
	printf("\n");
	#endif

	// game logic needs to go here
	// first initialize enemy
	// then draw a full hand (8)
	//
	// hearts: heal from discard
	// diamonds: draw from deck
	// clubs: deal double damage
	// spades: reduce enemy attack
	//
	// turn order:
	// 1. Play a card or yield
	// 2. Activate the suit power
	// 3. Deal damage to the enemy
	// 4. Suffer damage from the enemy (pick cards to discard)
	struct reginemy en = {};
	int enemy = 0;
	int newen = 1;
	int owned = 8;
	while (enemy < 12 || owned == 0) {
	    if (newen == 1) {
			en.enemy = regiendeck[enemy];
			switch (regiendeck[enemy].value) { // assigns stats to enemy based on card value
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
			newen = 0; // initialized
		}

		// end of func
		if (en.hp == 0){
		    newen = 1; // needs a new enemy if the one is dead
		}
	}
	if (owned == 0){
	   printf("you lose\n");
	}
	else {
	   printf("killed everyone congrats\n");
	}
}

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);  // For Windows
	#endif
    char ch;
	int gamerunning = 1;
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
			case 's':
			    scoundrel(); // starts scoundrel
				break;
			case 'S': // scoundrel rules
			    printf("\nScoundrel tutorial\n");
				break;
			case 'd':
				donsol(); // starts donsol (first difficulty picker, quits if specified)
				break;
			case 'D': // donsol rules
                printf("\nDonsol tutorial\n");
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
			case 'q':
				gamerunning = 0;
				break;
			default:
				goto input;
			goback();
		}
	}
	printf("byebye\n");
	return 0;
}
