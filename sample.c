#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./CInterface.h"

int main(void){
  setDecorChosen("", "", GREEN_B);
  setDecorHead("<- ", " ->", YELLOW);
  setColor(NULL, WHITE_B, WHITE_B, NULL);

  char *header = "Choose a sort method: ";

  char *footer = "End";

  char *option[2][5] = {
    {"Bubble Sort", "Selection Sort", "Insertion Sort", "Quick Sort", "Count sort"},
    {"Radix Sort",  "Merge Sort",    "Bogo Sort", "Bucket Sort",       "Shell Sort"}
  };

  char *description[2][5] = {
    {"Add description here", "Leave NULL for no description in that option", "Use macro NO_DESCRIPTION to indicate no description for the whole panel", NULL, NULL},
    {NULL,              NULL,          NULL,  NULL, "Lorem Ipsum"}
  };

  char *field[] = {
    "The field input are treated as a string (type something)", "You can get multiple input by typing in and left a trailing space"
  };

  const int ROW = 2;
  const int COL = 5;
  const int FIELD = 2;

  basicTUI* test = initBasicTUI_Arr(header, footer, COL, ROW, FIELD, option, description, field);

  char* ret;
  ret  = runBasicTUI_radio(test);

  char *coordinate = strtok(ret, ";");

  int x = atoi(coordinate);
  coordinate = strtok(NULL, ",");
  int y = atoi(coordinate);

  printf("The chosen sort method is: %s, the input are: ", option[x][y]);

  
  while ((coordinate = strtok(NULL, " ")) != NULL){
    printf("[%s]\n", coordinate);
  }


  destroyTUI(test);
  return 0;
}