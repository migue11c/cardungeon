#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
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
// static char* templatedeck[54] = {"\033[36mAC\033[0m","\033[36m2C\033[0m","\033[36m3C\033[0m","\033[36m4C\033[0m","\033[36m5C\033[0m","\033[36m6C\033[0m","\033[36m7C\033[0m","\033[36m8C\033[0m","\033[36m9C\033[0m","\033[36mXC\033[0m","\033[36mJC\033[0m","\033[36mQC\033[0m","\033[36mKC\033[0m","\033[35mAS\033[0m","\033[35m2S\033[0m","\033[35m3S\033[0m","\033[35m4S\033[0m","\033[35m5S\033[0m","\033[35m6S\033[0m","\033[35m7S\033[0m","\033[35m8S\033[0m","\033[35m9S\033[0m","\033[35mXS\033[0m","\033[35mJS\033[0m","\033[35mQS\033[0m","\033[35mKS\033[0m","\033[33mAD\033[0m","\033[33m2D\033[0m","\033[33m3D\033[0m","\033[33m4D\033[0m","\033[33m5D\033[0m","\033[33m6D\033[0m","\033[33m7D\033[0m","\033[33m8D\033[0m","\033[33m9D\033[0m","\033[33mXD\033[0m","\033[33mJD\033[0m","\033[33mQD\033[0m","\033[33mKD\033[0m","\033[31mAH\033[0m","\033[31m2H\033[0m","\033[31m3H\033[0m","\033[31m4H\033[0m","\033[31m5H\033[0m","\033[31m6H\033[0m","\033[31m7H\033[0m","\033[31m8H\033[0m","\033[31m9H\033[0m","\033[31mXH\033[0m","\033[31mJH\033[0m","\033[31mQH\033[0m","\033[31mKH\033[0m","\033[31mjR\033[0m","\033[35mjB\033[0m"};

void swap(char** a, char** b) {
    char* temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle (char* arr[], int size){
    srand(time(NULL));
    for (int i = size-1; i > 0; i--){
        int j = rand() % (i+1);
        swap(&arr[i], &arr[j]);
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

void goback(){
    printf(
        "\033[36mPress any key to go back...\033[0m\n"
    );
	getkey();
}

void menu(){
	printf(
	   "\nHello welcome to cardungeon!\nPlease press one of the following keys:\nc: Basic controls\ns: Play Scoundrel\nS: Scoundrel rules\nd: Play Donsol\nD: Donsol rules\nr: Play Regicide\nR: Regicide rules\nq: Quit\n"
	);
	// ANSI escape codes do underline and red color
	// 0m resets the text formatting
}

void scoundrel() {
    char* scoundeck[44] = {"\033[36mAC\033[0m","\033[36m2C\033[0m","\033[36m3C\033[0m","\033[36m4C\033[0m","\033[36m5C\033[0m","\033[36m6C\033[0m","\033[36m7C\033[0m","\033[36m8C\033[0m","\033[36m9C\033[0m","\033[36mXC\033[0m","\033[36mJC\033[0m","\033[36mQC\033[0m","\033[36mKC\033[0m","\033[35mAS\033[0m","\033[35m2S\033[0m","\033[35m3S\033[0m","\033[35m4S\033[0m","\033[35m5S\033[0m","\033[35m6S\033[0m","\033[35m7S\033[0m","\033[35m8S\033[0m","\033[35m9S\033[0m","\033[35mXS\033[0m","\033[35mJS\033[0m","\033[35mQS\033[0m","\033[35mKS\033[0m","\033[33m2D\033[0m","\033[33m3D\033[0m","\033[33m4D\033[0m","\033[33m5D\033[0m","\033[33m6D\033[0m","\033[33m7D\033[0m","\033[33m8D\033[0m","\033[33m9D\033[0m","\033[33mXD\033[0m","\033[31m2H\033[0m","\033[31m3H\033[0m","\033[31m4H\033[0m","\033[31m5H\033[0m","\033[31m6H\033[0m","\033[31m7H\033[0m","\033[31m8H\033[0m","\033[31m9H\033[0m","\033[31mXH\033[0m"};
	// shuffle deck

    while (1) {
	   return;
	}
}

void donsol() {
    // all 54 cards with ANSI values
    char* donsdeck[54] = {"\033[36mAC\033[0m","\033[36m2C\033[0m","\033[36m3C\033[0m","\033[36m4C\033[0m","\033[36m5C\033[0m","\033[36m6C\033[0m","\033[36m7C\033[0m","\033[36m8C\033[0m","\033[36m9C\033[0m","\033[36mXC\033[0m","\033[36mJC\033[0m","\033[36mQC\033[0m","\033[36mKC\033[0m","\033[35mAS\033[0m","\033[35m2S\033[0m","\033[35m3S\033[0m","\033[35m4S\033[0m","\033[35m5S\033[0m","\033[35m6S\033[0m","\033[35m7S\033[0m","\033[35m8S\033[0m","\033[35m9S\033[0m","\033[35mXS\033[0m","\033[35mJS\033[0m","\033[35mQS\033[0m","\033[35mKS\033[0m","\033[33mAD\033[0m","\033[33m2D\033[0m","\033[33m3D\033[0m","\033[33m4D\033[0m","\033[33m5D\033[0m","\033[33m6D\033[0m","\033[33m7D\033[0m","\033[33m8D\033[0m","\033[33m9D\033[0m","\033[33mXD\033[0m","\033[33mJD\033[0m","\033[33mQD\033[0m","\033[33mKD\033[0m","\033[31mAH\033[0m","\033[31m2H\033[0m","\033[31m3H\033[0m","\033[31m4H\033[0m","\033[31m5H\033[0m","\033[31m6H\033[0m","\033[31m7H\033[0m","\033[31m8H\033[0m","\033[31m9H\033[0m","\033[31mXH\033[0m","\033[31mJH\033[0m","\033[31mQH\033[0m","\033[31mKH\033[0m","\033[31mjR\033[0m","\033[35mjB\033[0m"};
	printf("donsol\n");
	// shuffle deck

	while (1) {
	   return;
	}
}

void regicide() {
    char* regipldeck[40] = {"\033[36mAC\033[0m","\033[36m2C\033[0m","\033[36m3C\033[0m","\033[36m4C\033[0m","\033[36m5C\033[0m","\033[36m6C\033[0m","\033[36m7C\033[0m","\033[36m8C\033[0m","\033[36m9C\033[0m","\033[36mXC\033[0m","\033[35mAS\033[0m","\033[35m2S\033[0m","\033[35m3S\033[0m","\033[35m4S\033[0m","\033[35m5S\033[0m","\033[35m6S\033[0m","\033[35m7S\033[0m","\033[35m8S\033[0m","\033[35m9S\033[0m","\033[35mXS\033[0m","\033[33mAD\033[0m","\033[33m2D\033[0m","\033[33m3D\033[0m","\033[33m4D\033[0m","\033[33m5D\033[0m","\033[33m6D\033[0m","\033[33m7D\033[0m","\033[33m8D\033[0m","\033[33m9D\033[0m","\033[33mXD\033[0m","\033[31mAH\033[0m","\033[31m2H\033[0m","\033[31m3H\033[0m","\033[31m4H\033[0m","\033[31m5H\033[0m","\033[31m6H\033[0m","\033[31m7H\033[0m","\033[31m8H\033[0m","\033[31m9H\033[0m","\033[31mXH\033[0m"};
    char* regiendeck[12] = {"\033[36mJC\033[0m","\033[36mQC\033[0m","\033[36mKC\033[0m","\033[35mJS\033[0m","\033[35mQS\033[0m","\033[35mKS\033[0m","\033[33mJD\033[0m","\033[33mQD\033[0m","\033[33mKD\033[0m","\033[31mJH\033[0m","\033[31mQH\033[0m","\033[31mKH\033[0m"};
    char* hand[8] = {NULL};

	shuffle(regipldeck, 40);
	shuffle(regiendeck, 12);

	#ifdef _DEBUG
	printf("\n");
	for (int i=0;i<40;i++){
		printf("%s ",regipldeck[i]);
		if (i%10==9){
			printf("\n");
		}
	}
	printf("\n");
	for (int i=0;i<12;i++){
		printf("%s ",regiendeck[i]);
		if (i%6==5){
		    printf("\n");
		}
	}
	printf("\n");
	#endif

	while (1) {
        return;
	}
}

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);  // For Windows
	#endif
    char ch;
	int gamerunning = 1;
	// create an introduction
	printf("\n\n\033[35m\u250F\u2501\u2501\u2501\u2513\n\u2503A  \u2503\n\u2503 S \u2503\n\u2503  A\u2503\n\u2517\u2501\u2501\u2501\u2518\033[0m\n\nThe ace of spades!\n");
	while(gamerunning){
		menu();
input:
		switch (getkey()) {
		    case 'c':
				printf("\nCards are displayed as a value and suit.\nValues are displayed as: A,2,3,4,5,6,7,8,9,X,J,Q,K, X is 10 but as a single character.\nSuits are displayed as: \033[35mS\033[0mpades,\033[36mC\033[0mlubs,\033[33mD\033[0miamonds,\033[31mH\033[0mearts\nExample: \033[31mAH\033[0m, \033[36m9C\033[0m\nJokers are displayed as: \033[31mjR\033[0m and \033[35mjB\033[0m\nAll relevant controls will be shown on the screen during each turn.\n\n");
				goback();
				break;
			case 's':
			    scoundrel();
				break;
			case 'S':
                goback();
				break;
			case 'd':
				donsol();
				break;
			case 'D':
                goback();
				break;
			case 'r':
				regicide();
				break;
			case 'R':
                goback();
				break;
			case 'q':
				gamerunning = 0;
				break;
			default:
				goto input;
		}
	}
	printf("byebye");
	return 0;
}
