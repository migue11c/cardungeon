#include <stdio.h>

#ifdef _WIN32
	#include <conio.h>
#else
	#include <termios.h>
	#include <unistd.h>
#endif

// needs to either be reformatted into one deck, called upon the start of the game or will stay like this for each new game added

static int ingame = 0;
static char* scoundeck[44] = {"AC","2C","3C","4C","5C","6C","7C","8C","9C","XC","JC","QC","KC","AS","2S","3S","4S","5S","6S","7S","8S","9S","XS","JS","QS","KS","2D","3D","4D","5D","6D","7D","8D","9D","XD","2H","3H","4H","5H","6H","7H","8H","9H","XH"};
static char* donsdeck[54] = {"AC","2C","3C","4C","5C","6C","7C","8C","9C","XC","JC","QC","KC","AS","2S","3S","4S","5S","6S","7S","8S","9S","XS","JS","QS","KS","AD","2D","3D","4D","5D","6D","7D","8D","9D","XD","JD","QD","KD","AH","2H","3H","4H","5H","6H","7H","8H","9H","XH","JH","QH","KH","jR","jB"};
static char* regipldeck[40] = {"AC","2C","3C","4C","5C","6C","7C","8C","9C","XC","AS","2S","3S","4S","5S","6S","7S","8S","9S","XS","AD","2D","3D","4D","5D","6D","7D","8D","9D","XD","AH","2H","3H","4H","5H","6H","7H","8H","9H","XH"};
static char* regiendeck[12] = {"JC","QC","KC","JS","QS","KS","JD","QD","KD","JH","QH","KH"};

char getkey() {
	#ifdef _WIN32
		// windows terminal input noecho
		char ch = _getch();
		return ch; // maybe can just return _getch() instead?
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

void menu(){
	printf("Hello welcome to cardungeon!\nPlease press one of the following keys:\ns: Play Scoundrel\nS: Scoundrel rules\nd: Play Donsol\nD: Donsol rules\nr: Play Regicide\nR: Regicide rules\nq: Quit\n");
	// ANSI escape codes do underline and red color
	// 0m resets the text formatting
	printf("\033[4;31mYou may use your keyboard now.\033[0m\n");
}

void scoundrel() {
	ingame = 1;
	while (ingame) {
		
	}
}

void donsol() {
	printf("donsol\n");
}

void regicide() {
	printf("regicide\n");
}

int main() {
	char ch;
	int gamerunning = 1;
	// create an introduction
	while(gamerunning){
		menu();
input:
		switch (getkey()) {
			case 's':
				break;
			case '5':
				break;
			case 'd':
				donsol();
				break;
			case 'D':
				break;
			case 'r':
				regicide();
				break;
			case 'R':
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
