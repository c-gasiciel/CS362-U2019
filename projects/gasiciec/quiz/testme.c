#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define STR_MAX 6

char inputChar()
{
    // TODO: rewrite this function
    char lower = ' ';
    char upper = '}';

    //Randomly generate character from '(' to 'z'
    char c = (rand() % (upper - lower + 1)) + lower;
    printf("Random char is: %c\n", c);
    return c;
}

char *inputString()
{
    // TODO: rewrite this function
    //Declare string
    char* randStr = malloc(STR_MAX * sizeof(char));

    if(randStr == 0)
    {
      printf("malloc() failed\n");
    }

    //Fill string with random characters from a-z or /0 if last char
    int i,
        lower = 97,
        upper = 122;

    for(i = 0; i < STR_MAX; i++)
    {
      //Fill with random char a-z if not the end of the string
      if(i < 5)
      {
        int valForStr = (rand() % (upper - lower + 1)) + lower;
        randStr[i] = valForStr;
      }

      //End of string must have null terminator
      else
      {
        randStr[i] = '\0';
      }
    }

    printf("Random string is: %s\n", randStr);
    return randStr;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
