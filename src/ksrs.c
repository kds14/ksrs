#include <stdio.h>
#include <termios.h>

char getch()
{
	struct termios old, new;

	tcgetattr(0, &old);
	new = old;
	new.c_lflag &= ~ICANON;
	new.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &new);

	char ch = getchar();

	tcsetattr(0, TCSANOW, &old);

	return ch;
}

int main()
{
	printf("Press any key\n");
	char c = getch();
	printf("%c\n", c);
	return 0;
}
