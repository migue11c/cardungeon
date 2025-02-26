#include <stdio.h>

#ifdef _WIN32
	#include <conio.h>
#else
	#include <termios.h>
	#include <unistd.h>
#endif

// needs to either be reformatted into one deck, called upon the start of the game or will stay like this for each new game added
// will also be really nice to add ANSI to each card

// full ANSI formatted deck of 54 cards
// 31 red
// 33 yellow
// 35 magenta
// 36 cyan
// static char* templatedeck[54] = {"\033[36mAC\033m[0m","\033[36m2C\033m[0m","\033[36m3C\033m[0m","\033[36m4C\033m[0m","\033[36m5C\033m[0m","\033[36m6C\033m[0m","\033[36m7C\033m[0m","\033[36m8C\033m[0m","\033[36m9C\033m[0m","\033[36mXC\033m[0m","\033[36mJC\033m[0m","\033[36mQC\033m[0m","\033[36mKC\033m[0m","\033[35mAS\033m[0m","\033[35m2S\033m[0m","\033[35m3S\033m[0m","\033[35m4S\033m[0m","\033[35m5S\033m[0m","\033[35m6S\033m[0m","\033[35m7S\033m[0m","\033[35m8S\033m[0m","\033[35m9S\033m[0m","\033[35mXS\033m[0m","\033[35mJS\033m[0m","\033[35mQS\033m[0m","\033[35mKS\033m[0m","\033[33mAD\033m[0m","\033[33m2D\033m[0m","\033[33m3D\033m[0m","\033[33m4D\033m[0m","\033[33m5D\033m[0m","\033[33m6D\033m[0m","\033[33m7D\033m[0m","\033[33m8D\033m[0m","\033[33m9D\033m[0m","\033[33mXD\033m[0m","\033[33mJD\033m[0m","\033[33mQD\033m[0m","\033[33mKD\033m[0m","\033[31mAH\033m[0m","\033[31m2H\033m[0m","\033[31m3H\033m[0m","\033[31m4H\033m[0m","\033[31m5H\033m[0m","\033[31m6H\033m[0m","\033[31m7H\033m[0m","\033[31m8H\033m[0m","\033[31m9H\033m[0m","\033[31mXH\033m[0m","\033[31mJH\033m[0m","\033[31mQH\033m[0m","\033[31mKH\033m[0m","\033[31mjR\033m[0m","\033[35mjB\033m[0m"};

void shuffle(int size){

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
    char* scoundeck[44] = {"\033[36mAC\033m[0m","\033[36m2C\033m[0m","\033[36m3C\033m[0m","\033[36m4C\033m[0m","\033[36m5C\033m[0m","\033[36m6C\033m[0m","\033[36m7C\033m[0m","\033[36m8C\033m[0m","\033[36m9C\033m[0m","\033[36mXC\033m[0m","\033[36mJC\033m[0m","\033[36mQC\033m[0m","\033[36mKC\033m[0m","\033[35mAS\033m[0m","\033[35m2S\033m[0m","\033[35m3S\033m[0m","\033[35m4S\033m[0m","\033[35m5S\033m[0m","\033[35m6S\033m[0m","\033[35m7S\033m[0m","\033[35m8S\033m[0m","\033[35m9S\033m[0m","\033[35mXS\033m[0m","\033[35mJS\033m[0m","\033[35mQS\033m[0m","\033[35mKS\033m[0m","\033[33m2D\033m[0m","\033[33m3D\033m[0m","\033[33m4D\033m[0m","\033[33m5D\033m[0m","\033[33m6D\033m[0m","\033[33m7D\033m[0m","\033[33m8D\033m[0m","\033[33m9D\033m[0m","\033[33mXD\033m[0m","\033[31m2H\033m[0m","\033[31m3H\033m[0m","\033[31m4H\033m[0m","\033[31m5H\033m[0m","\033[31m6H\033m[0m","\033[31m7H\033m[0m","\033[31m8H\033m[0m","\033[31m9H\033m[0m","\033[31mXH\033m[0m"};
	// shuffle deck


    while (1) {
	   return;
	}
}

void donsol() {
    // all 54 cards with ANSI values
    char* donsdeck[54] = {"\033[36mAC\033m[0m","\033[36m2C\033m[0m","\033[36m3C\033m[0m","\033[36m4C\033m[0m","\033[36m5C\033m[0m","\033[36m6C\033m[0m","\033[36m7C\033m[0m","\033[36m8C\033m[0m","\033[36m9C\033m[0m","\033[36mXC\033m[0m","\033[36mJC\033m[0m","\033[36mQC\033m[0m","\033[36mKC\033m[0m","\033[35mAS\033m[0m","\033[35m2S\033m[0m","\033[35m3S\033m[0m","\033[35m4S\033m[0m","\033[35m5S\033m[0m","\033[35m6S\033m[0m","\033[35m7S\033m[0m","\033[35m8S\033m[0m","\033[35m9S\033m[0m","\033[35mXS\033m[0m","\033[35mJS\033m[0m","\033[35mQS\033m[0m","\033[35mKS\033m[0m","\033[33mAD\033m[0m","\033[33m2D\033m[0m","\033[33m3D\033m[0m","\033[33m4D\033m[0m","\033[33m5D\033m[0m","\033[33m6D\033m[0m","\033[33m7D\033m[0m","\033[33m8D\033m[0m","\033[33m9D\033m[0m","\033[33mXD\033m[0m","\033[33mJD\033m[0m","\033[33mQD\033m[0m","\033[33mKD\033m[0m","\033[31mAH\033m[0m","\033[31m2H\033m[0m","\033[31m3H\033m[0m","\033[31m4H\033m[0m","\033[31m5H\033m[0m","\033[31m6H\033m[0m","\033[31m7H\033m[0m","\033[31m8H\033m[0m","\033[31m9H\033m[0m","\033[31mXH\033m[0m","\033[31mJH\033m[0m","\033[31mQH\033m[0m","\033[31mKH\033m[0m","\033[31mjR\033m[0m","\033[35mjB\033m[0m"};
	printf("donsol\n");
	while (1) {
	   return;
	}
}

void regicide() {
    char* regipldeck[40] = {"\033[36mAC\033m[0m","\033[36m2C\033m[0m","\033[36m3C\033m[0m","\033[36m4C\033m[0m","\033[36m5C\033m[0m","\033[36m6C\033m[0m","\033[36m7C\033m[0m","\033[36m8C\033m[0m","\033[36m9C\033m[0m","\033[36mXC\033m[0m","\033[35mAS\033m[0m","\033[35m2S\033m[0m","\033[35m3S\033m[0m","\033[35m4S\033m[0m","\033[35m5S\033m[0m","\033[35m6S\033m[0m","\033[35m7S\033m[0m","\033[35m8S\033m[0m","\033[35m9S\033m[0m","\033[35mXS\033m[0m","\033[33mAD\033m[0m","\033[33m2D\033m[0m","\033[33m3D\033m[0m","\033[33m4D\033m[0m","\033[33m5D\033m[0m","\033[33m6D\033m[0m","\033[33m7D\033m[0m","\033[33m8D\033m[0m","\033[33m9D\033m[0m","\033[33mXD\033m[0m","\033[31mAH\033m[0m","\033[31m2H\033m[0m","\033[31m3H\033m[0m","\033[31m4H\033m[0m","\033[31m5H\033m[0m","\033[31m6H\033m[0m","\033[31m7H\033m[0m","\033[31m8H\033m[0m","\033[31m9H\033m[0m","\033[31mXH\033m[0m"};
    char* regiendeck[12] = {"\033[36mJC\033[0m","\033[36mQC\033[0m","\033[36mKC\033[0m","\033[35mJS\033[0m","\033[35mQS\033[0m","\033[35mKS\033[0m","\033[33mJD\033[0m","\033[33mQD\033[0m","\033[33mKD\033[0m","\033[31mJH\033[0m","\033[31mQH\033[0m","\033[31mKH\033[0m"};
	printf("regicide\n");
	while (1) {
    return;
	}
}

int main() {
	char ch;
	int gamerunning = 1;
	// create an introduction
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
