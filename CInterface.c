#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <termios.h>            
#include <unistd.h> 
#include <errno.h>

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
 * @param title 
 * @param footer 
 * @param no_of_option_per_panel 
 * @param no_panel 
 * @param no_field 
 * @param option 
 * @param description 
 * @param field 
 * @return basicTUI* 
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
    /*Adding the field*/
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

// /**
//  * @brief Initialization function for descriptionTUI that using a double pointer to the string
//  * 
//  * @param title The given title for this current TUI
//  * @param footer The given footer for this current TUI
//  * @param no_of_option The total number of option for this current TUI
//  * @param content The option (must match the no_of_option parameter)
//  * @param description The description for the following option (Left NULL if don't have)
//  * @return basicTUI* The initialized framework
//  */

// uint8_t* runBasicTUI_checkboxes(basicTUI* TUI, char* submitKey){
//   if (*submitKey == 13 /*Enter*/ || *submitKey == '[') return NULL;

//   static struct termios oldt, newt;

//   //Store the old attribute
//   tcgetattr( STDIN_FILENO, &oldt);

//   //Set the new attribute, revert the flag
//   newt = oldt;
//   newt.c_lflag &= ~(ICANON);         

//   tcsetattr(STDIN_FILENO, TCSANOW, &newt);

//   uint64_t optionCursor = 0;
//   uint8_t *optionArray = malloc(sizeof(uint64_t) * TUI->no_options);
//   memset(optionArray, 0, sizeof(uint64_t) * TUI->no_options);
//   //optionArray[0] = 1;

//   int InputReceiver = -1;

//   #define ENTER 10
//   do {
//     __CLEAR_SCREEN;
//     /* Logic flow of the selection */
//     switch(InputReceiver){
//       case KEY_UP:
//         if (optionCursor > 0)
//           optionCursor--;
//         break;

//       case KEY_DOWN:
//         if (optionCursor < TUI->no_options - 1)
//           optionCursor++;
//           break;

//       /* Register option */
//       case ENTER:
//         optionArray[optionCursor] = 1 - optionArray[optionCursor];
          
//         break;
//       default: break;
//     }
//   #undef ENTER

//     char* optionModeStart;
//     char* optionModeEnd;
//     char* constructBuffer = malloc(BUFFER_MEDIUM);
//     memset(constructBuffer, 0, BUFFER_MEDIUM);

//     if (InputReceiver == 27 /* \033 value*/ || InputReceiver == '[')
//       continue;
    
//     /* Print the head */
//     color(DECOR_HEAD_COLOR);

//     strncat(constructBuffer, DECOR_HEAD_START, strlen(DECOR_HEAD_START));
//     strncat(constructBuffer, TUI->head, strlen(TUI->head));
//     strncat(constructBuffer, DECOR_HEAD_END, strlen(DECOR_HEAD_END));

//     printf("%s\n\n", constructBuffer);
//     fflush(stdin);
//     free(constructBuffer);

//     constructBuffer = NULL;

//     /* Print the options */
//     for (int i = 0; i < TUI->no_options; i++){
//       constructBuffer = malloc(BUFFER_MEDIUM);
//       memset(constructBuffer, 0, BUFFER_MEDIUM);

//       optionModeStart = (optionArray[i]) ? DECOR_OPTION_CHOSEN_START : DECOR_OPTION_START;
//       optionModeEnd =   (optionArray[i]) ? DECOR_OPTION_CHOSEN_END : DECOR_OPTION_END;

//       color(optionArray[i] ? DECOR_OPTION_CHOSEN_COLOR : DECOR_OPTION_COLOR); //Set the corresponding color

//       strncpy(constructBuffer, optionModeStart, strlen(optionModeStart));
//       strncat(constructBuffer, TUI->option[i], strlen(TUI->option[i]));
//       strncat(constructBuffer, optionModeEnd, strlen(optionModeEnd));

//       //Give the extra notifier at first
//       if (i == optionCursor){color(WHITE_B); printf(">  ");  color(optionArray[i] ? DECOR_OPTION_CHOSEN_COLOR : DECOR_OPTION_COLOR);}
//       printf("%s\n", constructBuffer);
      
//       free(constructBuffer);
//       constructBuffer = NULL;
//     }
//     /* Print the footer */

//     constructBuffer = malloc(BUFFER_MEDIUM);
//     memset(constructBuffer, 0, BUFFER_MEDIUM);
//     color(DECOR_END_COLOR);

//     strncpy(constructBuffer, DECOR_END_START, strlen(DECOR_END_START));
//     strncat(constructBuffer, TUI->footer, strlen(TUI->footer));
//     strncat(constructBuffer, DECOR_END_END, strlen(DECOR_END_END));
    
//     printf("\n%s\n", constructBuffer);

//     free(constructBuffer);
//     constructBuffer = NULL;

//     //Just in case the program get cancel out
//     color(WHITE);
//   }while ((InputReceiver = getchar()) != *submitKey);
  
//   /*restore the old settings*/
//   tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//   color(WHITE);

//   return optionArray;
// }

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
  int InputReceiver = -1;

  do {
    __CLEAR_SCREEN;
    /* Login flow of the selection */
    switch(InputReceiver){
      case KEY_UP:
        if (optionCursorX > 0)
          optionCursorX--;
        break;

      case KEY_DOWN:
        if (optionCursorX < TUI->no_panels - 1)
          optionCursorX++;
          break;

      case KEY_RIGHT:
        if (optionCursorY > 0)
          optionCursorY--;
          break;

      case KEY_LEFT:
        if (optionCursorY < TUI->no_options_per_panel - 1)
          optionCursorY++;
          break;

      default: break;
    }

    char* optionModeStart;
    char* optionModeEnd;
    char* constructBuffer = malloc(__GLOBAL_BUFFER_SIZE);
    memset(constructBuffer, 0, __GLOBAL_BUFFER_SIZE);

    if (InputReceiver == 27 /* \033 value*/ || InputReceiver == '[')
      continue;
    
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
        constructBuffer = malloc(__GLOBAL_BUFFER_SIZE);
        memset(constructBuffer, 0, __GLOBAL_BUFFER_SIZE);

        optionModeStart = (i == optionCursorX && j == optionCursorY) ? DECOR_OPTION_CHOSEN_START : DECOR_OPTION_START;
        optionModeEnd = (i == optionCursorX && j == optionCursorY) ? DECOR_OPTION_CHOSEN_END : DECOR_OPTION_END;

        color(i == optionCursorX && j == optionCursorY ? DECOR_OPTION_CHOSEN_COLOR : DECOR_OPTION_COLOR); //Set the corresponding color

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
    if (TUI->description[optionCursorX][optionCursorY] != NULL){
      //In case there is a description

      size_t line_length = strlen(DECOR_HEAD_START) + strlen(DECOR_HEAD_END) + strlen(TUI->head) + 2;

      char* line = malloc(line_length);
      memset(line, '-', line_length - 2);
      line[line_length - 1] = '\0';

      color(WHITE_B); //Should have be set to custom color for description
      printf("\n%s\n", line);
      free(line);
      
      printf("Description:\n");
      printf("%s\n", TUI->head);
    }
      
    /* Print the footer */

    constructBuffer = malloc(__GLOBAL_BUFFER_SIZE);
    memset(constructBuffer, 0, __GLOBAL_BUFFER_SIZE);
    color(DECOR_END_COLOR);

    strncpy(constructBuffer, DECOR_END_START, strlen(DECOR_END_START));
    strncat(constructBuffer, TUI->footer, strlen(TUI->footer));
    strncat(constructBuffer, DECOR_END_END, strlen(DECOR_END_END));
    
    printf("\n%s\n", constructBuffer);

    free(constructBuffer);
    constructBuffer = NULL;

    //Just in case the program get cancel out
    color(WHITE);
  }while ((InputReceiver = getchar()) != 10 /* Enter key */);


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

    returnBuffer = malloc(__GLOBAL_BUFFER_SIZE);
    char* tempBuffer = malloc(BUFFER_SMALL);

    for (int i = 0; i < TUI->no_field; i++){
      printf("%s: ", TUI->field[i]);
      fgets(tempBuffer, BUFFER_SMALL, stdin);
      strncat(returnBuffer, tempBuffer, strlen(tempBuffer) - 1);
      strcat(returnBuffer, ",");
    }
    free(tempBuffer);
  }

  return returnBuffer;
}

void destroyTUI(basicTUI* TUI){
  /* Free the option and description matrice */
  printf("%p\n", TUI->option[2][3]);
  for (size_t i = 0; i < TUI->no_panels; i++){
    for (size_t j = 0; j < TUI->no_options_per_panel; i++){
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