#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <termios.h>            
#include <unistd.h> 
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>

#include "./CInterface.h"

#include "./const.h"

static size_t __GLOBAL_BUFFER_SIZE = BUFFER_MEDIUM;
static size_t __GLOBAL_INDENT_SIZE = 40;
extern int errno;

/* Coloring */
char* DECOR_HEAD_COLOR = WHITE;
char* DECOR_OPTION_COLOR = WHITE;
char* DECOR_OPTION_CHOSEN_COLOR = WHITE_B;
char* DECOR_END_COLOR = WHITE;

/* Formatting */
char* DECOR_HEAD_START = "[== ";
char* DECOR_HEAD_END = " ==]";

char* DECOR_OPTION_START = "< ";
char* DECOR_OPTION_END = " >";

char* DECOR_OPTION_CHOSEN_START = "<= ";
char* DECOR_OPTION_CHOSEN_END = " =>";

char* DECOR_END_START = "[== ";
char* DECOR_END_END = " ==]";

void setDecorChosen(char* start, char* end, char* color){
  if (start != NULL)
    DECOR_OPTION_CHOSEN_START = start;
  
  if (end != NULL)
    DECOR_OPTION_CHOSEN_END = end;

  if (color != NULL)
    DECOR_OPTION_CHOSEN_COLOR = color;
}

void setDecorOption(char* start, char* end, char* color){
  if (start != NULL)
    DECOR_OPTION_START = start;
  
  if (end != NULL)
    DECOR_OPTION_END = end;

  if (color != NULL)
    DECOR_OPTION_COLOR = color;
}

void setDecorEnd(char* start, char* end, char* color){
  if (start != NULL)
    DECOR_END_START = start;
  
  if (end != NULL)
    DECOR_END_END = end;

  if (color != NULL)
    DECOR_END_COLOR = color;
}

void setDecorHead(char* start, char* end, char* color){
  if (start != NULL)
    DECOR_HEAD_START = start;
  
  if (end != NULL)
    DECOR_HEAD_END = end;

  if (color != NULL)
    DECOR_HEAD_COLOR = color;
}

void setColor(char* head, char* footer, char* option, char* chosenOption){
  if (head != NULL)
    DECOR_HEAD_COLOR = head;
  
  if (footer != NULL)
    DECOR_END_COLOR = footer;

  if (option != NULL)
    DECOR_OPTION_COLOR = option;
  
  if (chosenOption != NULL)
    DECOR_OPTION_CHOSEN_COLOR = chosenOption;
}
/**
 * @brief This function will initilize a basicTUI to be used
 * 
 * @param title The title line
 * @param footer The footer line
 * @param no_of_option_per_panel Number of option per panel (AKA COLUMNS)
 * @param no_panel The total number of panel (AKA ROWS)
 * @param no_field The total number of field (optional field, use macro NO_FIELD to left blank)
 * @param option The pointer to option 
 * @param description The pointer to description
 * @param field The pointer to field
 * @return basicTUI* A malloced pointer to basicTUI instance
 */
basicTUI* initBasicTUI_Arr(char* title, char* footer, size_t no_of_option_per_panel, size_t no_panel, size_t no_field, char* option[no_panel][no_of_option_per_panel], char* description[no_panel][no_of_option_per_panel], char* field[]){
  basicTUI* returnTUI = malloc(sizeof(basicTUI));

  if (!returnTUI) goto ERROR_NULL_MALLOC;

  /*Allocating the description and option */
  returnTUI->option = malloc(sizeof(char**) * no_panel);
  if (!returnTUI->option) goto ERROR_NULL_MALLOC;

  for (size_t i = 0; i < no_panel; i++){
    returnTUI->option[i] = malloc(sizeof(char*) * no_of_option_per_panel);
    if (!returnTUI->option[i]) goto ERROR_NULL_MALLOC;
  }

  returnTUI->description = malloc(sizeof(char**) * no_panel);
  if (!returnTUI->description) goto ERROR_NULL_MALLOC;

  for (size_t i = 0; i < no_panel; i++){
    returnTUI->description[i] = malloc(sizeof(char*) * no_of_option_per_panel);
    if (!returnTUI->description[i]) goto ERROR_NULL_MALLOC;
  }

  /*Assigning the head*/
  returnTUI->head = malloc(strlen(title) + 1);
  if (!returnTUI->head) goto ERROR_NULL_MALLOC;
  strncpy(returnTUI->head, title, strlen(title) + 1);

  /*Adding the number of panel*/
  returnTUI->no_panels = no_panel;

  /*Adding the number of option per panel*/
  returnTUI->no_options_per_panel = no_of_option_per_panel;

  /*Adding the number of field*/
  returnTUI->no_field = no_field;

  /*Adding the option & description*/
  for (int i = 0; i < no_panel; i++){
    for (int j = 0; j < no_of_option_per_panel; j++){
      returnTUI->option[i][j] = malloc(strlen(option[i][j]) + 1);
      if (!returnTUI->option[i][j]) goto ERROR_NULL_MALLOC;
      strncpy(returnTUI->option[i][j], option[i][j], strlen(option[i][j]) + 1);

      if (description == NO_DESCRIPTION)
        continue;

      if (description[i][j] == NULL)
        continue;

      returnTUI->description[i][j] = malloc(strlen(description[i][j]) + 1);
      if (!returnTUI->description[i][j]) goto ERROR_NULL_MALLOC;
      strncpy(returnTUI->description[i][j], description[i][j], strlen(description[i][j]) + 1);
    }
  }

  if (description == NO_DESCRIPTION)
    returnTUI->description[0][0] = NULL;

  if (field != NO_FIELD){
    returnTUI->field = malloc(sizeof(char*) * no_field);
    if (!returnTUI->field) goto ERROR_NULL_MALLOC;

    for (int i = 0; i < no_field; i++){
      returnTUI->field[i] = malloc(strlen(field[i]) + 1);
      if (!returnTUI->field[i]) goto ERROR_NULL_MALLOC;
      strncpy(returnTUI->field[i], field[i], strlen(field[i]) + 1);
    }
  }else
    returnTUI->field[0] = NULL;

  /*The end*/
  returnTUI->footer = malloc(strlen(footer) + 1);
  if (!returnTUI->footer) goto ERROR_NULL_MALLOC;
  strncpy(returnTUI->footer, footer, strlen(footer) + 1);

  return returnTUI;

  ERROR_NULL_MALLOC:
    perror("Memory can't be allocated");
    return NULL;
}

char** runBasicTUI_checkboxes(basicTUI* TUI, char* submitKey){
  if (*submitKey == 13 /*Enter*/ || *submitKey == '[' || *submitKey == 27 /* ESC key */) goto INVALID_SUBMIT_KEY;

  static struct termios oldt, newt;

  //Store the old attribute
  tcgetattr( STDIN_FILENO, &oldt);

  //Set the new attribute, revert the flag
  newt = oldt;
  newt.c_lflag &= ~(ICANON);         

  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  uint64_t optionCursorX = 0, optionCursorY = 0;
  char  **optionArray = malloc(sizeof(char*) * TUI->no_panels);

  for (int i = 0; i < TUI->no_panels; i++)
    optionArray[i] = calloc(TUI->no_options_per_panel, 1);
  
  optionArray[optionCursorY][optionCursorX] = 1;

  char InputReceiver = -1;

  #define ENTER 10
  #define ESC 27
  do {
    __CLEAR_SCREEN;
    /* Logic flow of the selection */
    switch(InputReceiver){
      case KEY_UP:
        if (optionCursorY > 0)
          optionCursorY--;
        break;

      case KEY_DOWN:
        if (optionCursorY < TUI->no_panels - 1)
          optionCursorY++;
          break;
      
      case KEY_RIGHT:
        if (optionCursorX > 0)
          optionCursorX--;
          break;

      case KEY_LEFT:
        if (optionCursorX < TUI->no_options_per_panel - 1)
          optionCursorX++;
          break;

      /* Register option */
      case ENTER:
        optionArray[optionCursorY][optionCursorX] = 1 - optionArray[optionCursorY][optionCursorX];
        break;

      case ESC:;
        fd_set set;
        struct timeval timeout;
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        int retR = select(1, &set, NULL, NULL, &timeout);

        if (retR != 1 && retR != -1)
            goto ACTION_CANCELLED;
      break;  
      default: break;
    }
  #undef ENTER
  #undef ESC

    char* optionModeStart;
    char* optionModeEnd;
    char* constructBuffer = malloc(BUFFER_MEDIUM);
    memset(constructBuffer, 0, BUFFER_MEDIUM);

    /* Print the head */
    color(DECOR_HEAD_COLOR);

    strncat(constructBuffer, DECOR_HEAD_START, strlen(DECOR_HEAD_START));
    strncat(constructBuffer, TUI->head, strlen(TUI->head));
    strncat(constructBuffer, DECOR_HEAD_END, strlen(DECOR_HEAD_END));

    printf("%s\n\n", constructBuffer);
    fflush(stdin);
    free(constructBuffer);

    constructBuffer = NULL;

    /* Print the options */
    for (int i = 0; i < TUI->no_panels; i++){
        for (int j = 0; j < TUI->no_options_per_panel; j++){
          constructBuffer = calloc(__GLOBAL_BUFFER_SIZE, 1);

          optionModeStart = (optionArray[i][j]) ? DECOR_OPTION_CHOSEN_START : DECOR_OPTION_START;
          optionModeEnd =   (optionArray[i][j]) ? DECOR_OPTION_CHOSEN_END : DECOR_OPTION_END;

          color(optionArray[i][j] ? DECOR_OPTION_CHOSEN_COLOR : DECOR_OPTION_COLOR); //Set the corresponding color

          strncpy(constructBuffer, optionModeStart, strlen(optionModeStart));
          strncat(constructBuffer, TUI->option[i][j], strlen(TUI->option[i][j]));
          strncat(constructBuffer, optionModeEnd, strlen(optionModeEnd));

          //Give the extra notifier at first
          if (i == optionCursorY && j == optionCursorX){color(WHITE_B); printf(">  ");  color(optionArray[i][j] ? DECOR_OPTION_CHOSEN_COLOR : DECOR_OPTION_COLOR);}
          printf("%-*s", (i == optionCursorY && j == optionCursorX) ? __GLOBAL_INDENT_SIZE -3 : __GLOBAL_INDENT_SIZE,  constructBuffer);
          
          free(constructBuffer);
          constructBuffer = NULL; 
        }
        printf("\n");
    }

    if (TUI->description[optionCursorY][optionCursorX] != NULL){
            //In case there is a description

            size_t line_length = strlen(DECOR_HEAD_START) + strlen(DECOR_HEAD_END) + strlen(TUI->head) + 2;

            char* line = malloc(line_length);
            memset(line, '-', line_length - 2);
            line[line_length - 1] = '\0';

            color(WHITE_B); //Should have be set to custom color for description
            printf("\n%s\n", line);
            free(line);
            
            printf("Description:\n");
            printf("%s\n", TUI->description[optionCursorY][optionCursorX]);
    }

    constructBuffer = NULL;

    /* Print the footer */

    constructBuffer = calloc(__GLOBAL_BUFFER_SIZE, 1);
    color(DECOR_END_COLOR);

    strncpy(constructBuffer, DECOR_END_START, strlen(DECOR_END_START));
    strncat(constructBuffer, TUI->footer, strlen(TUI->footer));
    strncat(constructBuffer, DECOR_END_END, strlen(DECOR_END_END));

    printf("\n%s\n",constructBuffer);

    free(constructBuffer);

    if (InputReceiver == *submitKey /* Enter key */)
      break;

    //Just in case the program get cancel out
    color(WHITE);  
    
    read(STDIN_FILENO, &InputReceiver, 1);
  }while (1);
  
  /*restore the old settings*/
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  color(WHITE);

  return optionArray;

  ACTION_CANCELLED:
  INVALID_SUBMIT_KEY:
    return NULL;
}

/**
 * @brief 
 * 
 * @param TUI 
 * @param submitKey 
 * @return size_t 
 */

char* runBasicTUI_radio(basicTUI* TUI){
  static struct termios oldt, newt;

  //Store the old attribute
  tcgetattr( STDIN_FILENO, &oldt);

  //Set the new attribute, revert the flag
  newt = oldt;
  newt.c_lflag &= ~(ICANON);         

  tcsetattr( STDIN_FILENO, TCSANOW, &newt);

  uint64_t optionCursorX = 0, optionCursorY = 0;
  char InputReceiver = -1;
  #define ESC 27
  do {
    __CLEAR_SCREEN;
    /* Login flow of the selection */
    switch(InputReceiver){
      case KEY_UP:
        if (optionCursorY > 0)
          optionCursorY--;
        break;

      case KEY_DOWN:
        if (optionCursorY < TUI->no_panels - 1)
          optionCursorY++;
          break;

      case KEY_RIGHT:
        if (optionCursorX > 0)
          optionCursorX--;
          break;

      case KEY_LEFT:
        if (optionCursorX < TUI->no_options_per_panel - 1)
          optionCursorX++;
          break;

      case ESC:;
            fd_set set;
            struct timeval timeout;
            FD_ZERO(&set);
            FD_SET(STDIN_FILENO, &set);
            timeout.tv_sec = 0;
            timeout.tv_usec = 0;
            int retR = select(1, &set, NULL, NULL, &timeout);

            if (retR != 1 && retR != -1)
                goto ACTION_CANCELLED;
      break;

      default: break;
    }
  #undef ESC

    char* optionModeStart;
    char* optionModeEnd;
    char* constructBuffer = calloc(__GLOBAL_BUFFER_SIZE, 1);

    /* Print the head */
    color(DECOR_HEAD_COLOR);

    strncat(constructBuffer, DECOR_HEAD_START, strlen(DECOR_HEAD_START));
    strncat(constructBuffer, TUI->head, strlen(TUI->head));
    strncat(constructBuffer, DECOR_HEAD_END, strlen(DECOR_HEAD_END));

    printf("%s\n\n", constructBuffer);
    
    fflush(stdin);
    free(constructBuffer);

    constructBuffer = NULL;

    /* Print the options */
    for (int i = 0; i < TUI->no_panels; i++){
      for (int j = 0; j < TUI->no_options_per_panel; j++){
        constructBuffer = calloc(__GLOBAL_BUFFER_SIZE, 1);

        optionModeStart = (i == optionCursorY && j == optionCursorX) ? DECOR_OPTION_CHOSEN_START : DECOR_OPTION_START;
        optionModeEnd = (i == optionCursorY && j == optionCursorX) ? DECOR_OPTION_CHOSEN_END : DECOR_OPTION_END;

        color(i == optionCursorY && j == optionCursorX ? DECOR_OPTION_CHOSEN_COLOR : DECOR_OPTION_COLOR); //Set the corresponding color

        strncpy(constructBuffer, optionModeStart, strlen(optionModeStart));
        strncat(constructBuffer, TUI->option[i][j], strlen(TUI->option[i][j]));
        strncat(constructBuffer, optionModeEnd, strlen(optionModeEnd));

        printf("%-*s", __GLOBAL_INDENT_SIZE , constructBuffer);
        
        free(constructBuffer);
        constructBuffer = NULL;
      }
      printf("\n");
    }
    /* Print the description */
    if (TUI->description[optionCursorY][optionCursorX] != NULL){
      //In case there is a description

      size_t line_length = strlen(DECOR_HEAD_START) + strlen(DECOR_HEAD_END) + strlen(TUI->head) + 2;

      char* line = malloc(line_length);
      memset(line, '-', line_length - 2);
      line[line_length - 1] = '\0';

      color(WHITE_B); //Should have be set to custom color for description
      printf("\n%s\n", line);
      free(line);
      
      printf("Description:\n");
      printf("%s\n", TUI->description[optionCursorY][optionCursorX]);
    }
    
    constructBuffer = NULL;

    /* Print the footer */

    constructBuffer = calloc(__GLOBAL_BUFFER_SIZE, 1);
    color(DECOR_END_COLOR);

    strncpy(constructBuffer, DECOR_END_START, strlen(DECOR_END_START));
    strncat(constructBuffer, TUI->footer, strlen(TUI->footer));
    strncat(constructBuffer, DECOR_END_END, strlen(DECOR_END_END));

    //printf("\n%s\n",constructBuffer);

    free(constructBuffer);

    if (InputReceiver == 10 /* Enter key */)
      break;

    //Just in case the program get cancel out
    color(WHITE);  
    
    read(STDIN_FILENO, &InputReceiver, 1);

  }while (1);


  /*restore the old settings*/
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  color(WHITE);

  char* returnBuffer = NULL;

  if (TUI->field[0] != NULL){
    /*Print the input field */
    size_t line_length = strlen(DECOR_HEAD_START) + strlen(DECOR_HEAD_END) + strlen(TUI->head) + 2;

    char* line = malloc(line_length);
    memset(line, '-', line_length - 2);
    line[line_length - 1] = '\0';

    color(WHITE); //Should have be set to custom color for description
    printf("%s\n", line);
    free(line);

    returnBuffer = calloc(__GLOBAL_BUFFER_SIZE, 1);
    sprintf(returnBuffer, "%d;%d ", optionCursorY, optionCursorX);
    char* tempBuffer = malloc(BUFFER_SMALL);

    for (int i = 0; i < TUI->no_field; i++){
      printf("%s: ", TUI->field[i]);
      fgets(tempBuffer, BUFFER_SMALL, stdin);
      strncat(returnBuffer, tempBuffer, strlen(tempBuffer) - 1);

      if (i == TUI->no_field - 1)
       continue;

      strcat(returnBuffer, ",");
    }
    free(tempBuffer);
  }
    
  return returnBuffer;

  ACTION_CANCELLED:
    return NULL;
}

void destroyTUI(basicTUI* TUI){
  /* Free the option and description matrice */
  for (size_t i = 0; i < TUI->no_panels; i++){
    for (size_t j = 0; j < TUI->no_options_per_panel; j++){
      free(TUI->option[i][j]);
      free(TUI->description[i][j]);
    }
    free(TUI->option[i]);
    free(TUI->description[i]);
  }
  /* Free the field table */
  for (size_t i = 0; i < TUI->no_field; i++)
    free(TUI->field[i]);
  
  /* Head and footer */
  free(TUI->head);
  free(TUI->footer);

  /*Overall TUI free*/
  free(TUI);

  TUI = NULL;
}