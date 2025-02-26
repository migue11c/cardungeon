#include <stdio.h>

#ifdef _WIN32
	#include <conio.h>
#else
	#include <termios.h>
	#include <unistd.h>
#endif

enum State{
	MENU = 0,
	SCOUNDREL,
	DONSOLE,
	DONSOLM,
	DONSOLH,
	SRULES,
	DRULES,
	QUIT
};

static enum State gamestate = MENU;

char getkey() {
	#ifdef _WIN32
	   char ch = _getch();
	   return ch;
	#else
		struct termios oldt, newt;
		char ch;
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		ch = getchar();
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		return ch;
	#endif
}

void menu(){
	printf("Hello welcome to cardungeon!\nPlease press one of the following keys:\n1: Play Scoundrel\n2: Play Donsol (Easy)\n3: Play Donsol (Medium)\n4: Play Donsol (Hard)\nS: Scoundrel rules\nD: Donsol rules\nQ: Quit\n");
	// ANSI escape codes do underline and red color
	// 0m resets the text formatting
	printf("\033[4;31mYou may use your keyboard now.\033[0m\n");
}

int main() {
	char ch;
	int gamerunning = 1;
	// create an introduction
	while(gamerunning){
		switch (gamestate) {
			case MENU:
				menu();
input:
				switch (getkey()) {
					case '1':
						gamestate = SCOUNDREL;
						break;
					case '2':
						gamestate = DONSOLE;
						break;
					case '4':
						gamestate = DONSOLH;
						break;
					case 's':
						gamestate = SRULES;
						break;
					case 'd':
						gamestate = DRULES;
						break;
					case 'q':
						gamerunning = 0;
						break;
					case '5':
						gamestate = SRULES;
						break;
					case 'D':
						gamestate = DRULES;
						break;
					case 'Q':
						gamerunning = 0;
						break;
					default:
						goto input;
				}
				break;
			default:
				break;
		}
	}
	printf("byebye");
	return 0;
}
