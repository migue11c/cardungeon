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

// static char* templatedeck[54] = {
// "\033[36mAC\033[0m","\033[36m2C\033[0m","\033[36m3C\033[0m","\033[36m4C\033[0m",
// "\033[36m5C\033[0m","\033[36m6C\033[0m","\033[36m7C\033[0m","\033[36m8C\033[0m",
// "\033[36m9C\033[0m","\033[36mXC\033[0m","\033[36mJC\033[0m","\033[36mQC\033[0m",
// "\033[36mKC\033[0m","\033[35mAS\033[0m","\033[35m2S\033[0m","\033[35m3S\033[0m",
// "\033[35m4S\033[0m","\033[35m5S\033[0m","\033[35m6S\033[0m","\033[35m7S\033[0m",
// "\033[35m8S\033[0m","\033[35m9S\033[0m","\033[35mXS\033[0m","\033[35mJS\033[0m",
// "\033[35mQS\033[0m","\033[35mKS\033[0m","\033[33mAD\033[0m","\033[33m2D\033[0m",
// "\033[33m3D\033[0m","\033[33m4D\033[0m","\033[33m5D\033[0m","\033[33m6D\033[0m",
// "\033[33m7D\033[0m","\033[33m8D\033[0m","\033[33m9D\033[0m","\033[33mXD\033[0m",
// "\033[33mJD\033[0m","\033[33mQD\033[0m","\033[33mKD\033[0m","\033[31mAH\033[0m",
// "\033[31m2H\033[0m","\033[31m3H\033[0m","\033[31m4H\033[0m","\033[31m5H\033[0m",
// "\033[31m6H\033[0m","\033[31m7H\033[0m","\033[31m8H\033[0m","\033[31m9H\033[0m",
// "\033[31mXH\033[0m","\033[31mJH\033[0m","\033[31mQH\033[0m","\033[31mKH\033[0m",
// "\033[31mjR\033[0m","\033[35mjB\033[0m"};

struct card{
    char* name;
    char value;
    char color;
};

//static struct card detailedtemplatedeck[52] = {
//    {"\033[36mAC\033[0m",'A','C'},{"\033[36m2C\033[0m",'2','C'},{"\033[36m3C\033[0m",'3','C'},{"\033[36m4C\033[0m",'4','C'},
//    {"\033[36m5C\033[0m",'5','C'},{"\033[36m6C\033[0m",'6','C'},{"\033[36m7C\033[0m",'7','C'},{"\033[36m8C\033[0m",'8','C'},
//    {"\033[36m9C\033[0m",'9','C'},{"\033[36mXC\033[0m",'X','C'},{"\033[36mJC\033[0m",'J','C'},{"\033[36mQC\033[0m",'Q','C'},
//    {"\033[36mKC\033[0m",'K','C'},{"\033[35mAS\033[0m",'A','S'},{"\033[35m2S\033[0m",'2','S'},{"\033[35m3S\033[0m",'3','S'},
//    {"\033[35m4S\033[0m",'4','S'},{"\033[35m5S\033[0m",'5','S'},{"\033[35m6S\033[0m",'6','S'},{"\033[35m7S\033[0m",'7','S'},
//    {"\033[35m8S\033[0m",'8','S'},{"\033[35m9S\033[0m",'9','S'},{"\033[35mXS\033[0m",'X','S'},{"\033[35mJS\033[0m",'J','S'},
//    {"\033[35mQS\033[0m",'Q','S'},{"\033[35mKS\033[0m",'K','S'},{"\033[33mAD\033[0m",'A','D'},{"\033[33m2D\033[0m",'2','D'},
//    {"\033[33m3D\033[0m",'3','D'},{"\033[33m4D\033[0m",'4','D'},{"\033[33m5D\033[0m",'5','D'},{"\033[33m6D\033[0m",'6','D'},
//    {"\033[33m7D\033[0m",'7','D'},{"\033[33m8D\033[0m",'8','D'},{"\033[33m9D\033[0m",'9','D'},{"\033[33mXD\033[0m",'X','D'},
//    {"\033[33mJD\033[0m",'J','D'},{"\033[33mQD\033[0m",'Q','D'},{"\033[33mKD\033[0m",'K','D'},{"\033[31mAH\033[0m",'A','H'},
//    {"\033[31m2H\033[0m",'2','H'},{"\033[31m3H\033[0m",'3','H'},{"\033[31m4H\033[0m",'4','H'},{"\033[31m5H\033[0m",'5','H'},
//    {"\033[31m6H\033[0m",'6','H'},{"\033[31m7H\033[0m",'7','H'},{"\033[31m8H\033[0m",'8','H'},{"\033[31m9H\033[0m",'9','H'},
//    {"\033[31mXH\033[0m",'X','H'},{"\033[31mJH\033[0m",'J','H'},{"\033[31mQH\033[0m",'Q','H'},{"\033[31mKH\033[0m",'Y','H'}
//};


void cardswap(struct card* a, struct card* b){
    struct card temp = *a;
    *a = *b;
    *b = temp;
}

void deckshuffle(struct card arr[], int size){
    srand(time(NULL));
    for (int i = size-1; i>0; i--){
        int j = rand() % (i+1);
        cardswap(&arr[i], &arr[j]);
    }
}


// DEPRECATED

//void swap(char** a, char** b) {
//    char* temp = *a;
//    *a = *b;
//    *b = temp;
//}

//void shuffle (char* arr[], int size){
//    srand(time(NULL));
//    for (int i = size-1; i > 0; i--){
//        int j = rand() % (i+1);
//        swap(&arr[i], &arr[j]);
//    }
//}

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
    struct card scoundeck[44] = {
        {"\033[36mAC\033[0m",'A','C'},{"\033[36m2C\033[0m",'2','C'},{"\033[36m3C\033[0m",'3','C'},{"\033[36m4C\033[0m",'4','C'},
        {"\033[36m5C\033[0m",'5','C'},{"\033[36m6C\033[0m",'6','C'},{"\033[36m7C\033[0m",'7','C'},{"\033[36m8C\033[0m",'8','C'},
        {"\033[36m9C\033[0m",'9','C'},{"\033[36mXC\033[0m",'X','C'},{"\033[36mJC\033[0m",'J','C'},{"\033[36mQC\033[0m",'Q','C'},
        {"\033[36mKC\033[0m",'K','C'},{"\033[35mAS\033[0m",'A','S'},{"\033[35m2S\033[0m",'2','S'},{"\033[35m3S\033[0m",'3','S'},
        {"\033[35m4S\033[0m",'4','S'},{"\033[35m5S\033[0m",'5','S'},{"\033[35m6S\033[0m",'6','S'},{"\033[35m7S\033[0m",'7','S'},
        {"\033[35m8S\033[0m",'8','S'},{"\033[35m9S\033[0m",'9','S'},{"\033[35mXS\033[0m",'X','S'},{"\033[35mJS\033[0m",'J','S'},
        {"\033[35mQS\033[0m",'Q','S'},{"\033[35mKS\033[0m",'K','S'},{"\033[33m2D\033[0m",'2','D'},{"\033[33m3D\033[0m",'3','D'},
        {"\033[33m4D\033[0m",'4','D'},{"\033[33m5D\033[0m",'5','D'},{"\033[33m6D\033[0m",'6','D'},{"\033[33m7D\033[0m",'7','D'},
        {"\033[33m8D\033[0m",'8','D'},{"\033[33m9D\033[0m",'9','D'},{"\033[33mXD\033[0m",'X','D'},{"\033[31m2H\033[0m",'2','H'},
        {"\033[31m3H\033[0m",'3','H'},{"\033[31m4H\033[0m",'4','H'},{"\033[31m5H\033[0m",'5','H'},{"\033[31m6H\033[0m",'6','H'},
        {"\033[31m7H\033[0m",'7','H'},{"\033[31m8H\033[0m",'8','H'},{"\033[31m9H\033[0m",'9','H'},{"\033[31mXH\033[0m",'X','H'}
    };
	// shuffle deck

    while (1) {
	   return;
	}
}

void donsol() {
    struct card donsoldeck[54] = {
        {"\033[36mAC\033[0m",'A','C'},{"\033[36m2C\033[0m",'2','C'},{"\033[36m3C\033[0m",'3','C'},{"\033[36m4C\033[0m",'4','C'},
        {"\033[36m5C\033[0m",'5','C'},{"\033[36m6C\033[0m",'6','C'},{"\033[36m7C\033[0m",'7','C'},{"\033[36m8C\033[0m",'8','C'},
        {"\033[36m9C\033[0m",'9','C'},{"\033[36mXC\033[0m",'X','C'},{"\033[36mJC\033[0m",'J','C'},{"\033[36mQC\033[0m",'Q','C'},
        {"\033[36mKC\033[0m",'K','C'},{"\033[35mAS\033[0m",'A','S'},{"\033[35m2S\033[0m",'2','S'},{"\033[35m3S\033[0m",'3','S'},
        {"\033[35m4S\033[0m",'4','S'},{"\033[35m5S\033[0m",'5','S'},{"\033[35m6S\033[0m",'6','S'},{"\033[35m7S\033[0m",'7','S'},
        {"\033[35m8S\033[0m",'8','S'},{"\033[35m9S\033[0m",'9','S'},{"\033[35mXS\033[0m",'X','S'},{"\033[35mJS\033[0m",'J','S'},
        {"\033[35mQS\033[0m",'Q','S'},{"\033[35mKS\033[0m",'K','S'},{"\033[33mAD\033[0m",'A','D'},{"\033[33m2D\033[0m",'2','D'},
        {"\033[33m3D\033[0m",'3','D'},{"\033[33m4D\033[0m",'4','D'},{"\033[33m5D\033[0m",'5','D'},{"\033[33m6D\033[0m",'6','D'},
        {"\033[33m7D\033[0m",'7','D'},{"\033[33m8D\033[0m",'8','D'},{"\033[33m9D\033[0m",'9','D'},{"\033[33mXD\033[0m",'X','D'},
        {"\033[33mJD\033[0m",'J','D'},{"\033[33mQD\033[0m",'Q','D'},{"\033[33mKD\033[0m",'K','D'},{"\033[31mAH\033[0m",'A','H'},
        {"\033[31m2H\033[0m",'2','H'},{"\033[31m3H\033[0m",'3','H'},{"\033[31m4H\033[0m",'4','H'},{"\033[31m5H\033[0m",'5','H'},
        {"\033[31m6H\033[0m",'6','H'},{"\033[31m7H\033[0m",'7','H'},{"\033[31m8H\033[0m",'8','H'},{"\033[31m9H\033[0m",'9','H'},
        {"\033[31mXH\033[0m",'X','H'},{"\033[31mJH\033[0m",'J','H'},{"\033[31mQH\033[0m",'Q','H'},{"\033[31mKH\033[0m",'Y','H'},
        {"\033[31mjR\033[0m",'j','R'},{"\033[35mjB\033[0m",'j','B'}
    };
    deckshuffle(donsoldeck, 54);
    // all 54 cards with ANSI values
	printf("donsol\n");
	// shuffle deck

	while (1) {
	    return;
	}
}

void regishuffle(struct card arr[]){
    struct card temp[4] = {};
    int iter = 0;
    for (int i = 0; i<12; i++){
        temp[i%4] = arr[i];
        if (i%4==3){
            deckshuffle(temp, 4);
            for (int j = 0; j<4;j++){
                arr[iter] = temp[j];
                iter++;
            }
        }
    }
}

void regidraw(struct card deck[], struct card hand[], int amt){

}

void regicide() {
    struct card regipldeck[40] = {
        {"\033[36mAC\033[0m",'A','C'},{"\033[36m2C\033[0m",'2','C'},{"\033[36m3C\033[0m",'3','C'},{"\033[36m4C\033[0m",'4','C'},
        {"\033[36m5C\033[0m",'5','C'},{"\033[36m6C\033[0m",'6','C'},{"\033[36m7C\033[0m",'7','C'},{"\033[36m8C\033[0m",'8','C'},
        {"\033[36m9C\033[0m",'9','C'},{"\033[36mXC\033[0m",'X','C'},{"\033[35mAS\033[0m",'A','S'},{"\033[35m2S\033[0m",'2','S'},
        {"\033[35m3S\033[0m",'3','S'},{"\033[35m4S\033[0m",'4','S'},{"\033[35m5S\033[0m",'5','S'},{"\033[35m6S\033[0m",'6','S'},
        {"\033[35m7S\033[0m",'7','S'},{"\033[35m8S\033[0m",'8','S'},{"\033[35m9S\033[0m",'9','S'},{"\033[35mXS\033[0m",'X','S'},
        {"\033[33mAD\033[0m",'A','D'},{"\033[33m2D\033[0m",'2','D'},{"\033[33m3D\033[0m",'3','D'},{"\033[33m4D\033[0m",'4','D'},
        {"\033[33m5D\033[0m",'5','D'},{"\033[33m6D\033[0m",'6','D'},{"\033[33m7D\033[0m",'7','D'},{"\033[33m8D\033[0m",'8','D'},
        {"\033[33m9D\033[0m",'9','D'},{"\033[33mXD\033[0m",'X','D'},{"\033[31mAH\033[0m",'A','H'},{"\033[31m2H\033[0m",'2','H'},
        {"\033[31m3H\033[0m",'3','H'},{"\033[31m4H\033[0m",'4','H'},{"\033[31m5H\033[0m",'5','H'},{"\033[31m6H\033[0m",'6','H'},
        {"\033[31m7H\033[0m",'7','H'},{"\033[31m8H\033[0m",'8','H'},{"\033[31m9H\033[0m",'9','H'},{"\033[31mXH\033[0m",'X','H'}
    };
    struct card regiendeck[12] = {
        {"\033[36mJC\033[0m",'J','C'},{"\033[35mJS\033[0m",'J','S'},{"\033[33mJD\033[0m",'J','D'},{"\033[31mJH\033[0m",'J','H'},
        {"\033[36mQC\033[0m",'Q','C'},{"\033[35mQS\033[0m",'Q','S'},{"\033[33mQD\033[0m",'Q','D'},{"\033[31mQH\033[0m",'Q','H'},
        {"\033[36mKC\033[0m",'K','C'},{"\033[35mKS\033[0m",'K','S'},{"\033[33mKD\033[0m",'K','D'},{"\033[31mKH\033[0m",'K','H'}
    };
    struct card hand[8] = {NULL};

	deckshuffle(regipldeck, 40); // shuffles player deck

	regishuffle(regiendeck); // shuffles enemy deck

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
		printf("%s ",regipldeck[i].name);
		if (i%10==9){
			printf("\n");
		}
	}
	printf("\n");
	for (int i=0;i<12;i++){
		printf("%s ",regiendeck[i].name);
		if (i%6==5){
		    printf("\n");
		}
	}
	printf("\n");
	#endif

	int enemy = 0; // which enemy in a row are you fighting
	int enhp = 0; // enemy hp
	int todraw = 8; // how many cards you need to draw on action
	int enatk = 0;
	int newen = 1; // bool to check if there is a new enemy
	while (enemy!=12) { // very bad approach if facing 2 enemies
	    if (newen){ // difficulty goes up the more enemies you beat
			if (enemy == 0){ regidraw(regipldeck, hand, todraw); } // lacks support for more than 1 enemy
			if (enemy<4){ enhp = 20; enatk = 10; }
			else if (enemy<8) { enhp = 30; enatk = 15; }
			else { enhp = 40; enatk = 20; }
			// lacks actual gameplay loop
		}
	}
}

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);  // For Windows
	#endif
    char ch;
	int gamerunning = 1;
	// create an introduction
	printf("\n\n\033[35m\u250F\u2501\u2501\u2501\u2513\n\u2503A  \u2503\n\u2503 S \u2503\n\u2503  A\u2503\n\u2517\u2501\u2501\u2501\u251b\033[0m\n\nThe ace of spades!\n");
	while(gamerunning){
		menu();
input:
		switch (getkey()) {
		    case 'c':
				printf("\nCards are displayed as a value and suit.\nValues are displayed as: A,2,3,4,5,6,7,8,9,X,J,Q,K, X is 10 but as a single character.\nSuits are displayed as: \033[35mS\033[0mpades,\033[36mC\033[0mlubs,\033[33mD\033[0miamonds,\033[31mH\033[0mearts\nExample: \033[31mAH\033[0m, \033[36m9C\033[0m\nJokers are displayed as: \033[31mjR\033[0m and \033[35mjB\033[0m\nAll relevant controls will be shown on the screen during each turn.\n\n");
				goback();
				break;
			case 's':
			    // everything is done within scoundrel();
			    scoundrel();
				break;
			case 'S':
			    printf("\nScoundrel tutorial\n");
                goback();
				break;
			case 'd':
				donsol();
				break;
			case 'D':
                printf("\nDonsol tutorial\n");
                goback();
				break;
			case 'r':
				regicide();
				break;
			case 'R':
                printf("\nRegicide tutorial\n");
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
