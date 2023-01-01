#include <stdio.h>
#include "./CInterface.h"

int main(void){
  setDecorChosen("", "", GREEN_B);
  setDecorHead("<- ", " ->", YELLOW);
  setColor(NULL, WHITE_B, WHITE_B, NULL);

  char *header = "What do you like";

  char *footer = "This is the end";

  char *option[2][5] = {
    {"Something ?", "Money", "Power", "Good Sleep", "Something again"},
    {"Yup", "NO", "I don't care", "Okay", "Seems good"}
  };

  char *description[2][5] = {
    {"This is comment", "Yeah I know", "Yup", NULL, NULL},
    {NULL, NULL, NULL, NULL, "Something"}
  };

  char *field[] = {
    "Your age", "My dog's name", "Country"
  };

  const int ROW = 2;
  const int COL = 5;
  const int FIELD = 0;

  basicTUI* test = initBasicTUI_Arr(header, footer, ROW, COL, FIELD, option, NO_DESCRIPTION, NO_FIELD);

  char* ret = runBasicTUI_radio(test);

  printf("%s\n", ret);

  destroyTUI(test);
  return 0;
}