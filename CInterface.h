/******************************************************************************
  * @file           : Cinterface.h
  * @brief          : The header file of Cinterface
  ******************************************************************************
  * @attention
  * @author MagicExists 
  * @anchor https://github.com/gugugiyu
  * Copyright (c) 
  * All rights reserved.
  *
  ******************************************************************************
*/

#ifndef __C_INTERFACE_H_
#define __C_INTERFACE_H_

/* External inclusion */
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif


/* Provided Color */
#define BLACK    "\033[0;30m"
#define BLACK_B  "\033[1;30m"
#define RED      "\033[0;31m"
#define RED_B    "\033[1;31m"
#define GREEN    "\033[0;32m"
#define GREEN_B  "\033[1;32m"
#define YELLOW   "\033[0;33m"
#define YELLOW_B "\033[1;33m"
#define BLUE     "\033[0;34m"
#define BLUE_B   "\033[1;34m"
#define PURPLE   "\033[0;35m"
#define PURPLE_B "\033[1;35m"
#define CYAN     "\033[0;36m"
#define CYAN_B   "\033[1;36m"
#define WHITE    "\033[0;37m"
#define WHITE_B  "\033[1;37m"

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_LEFT 67
#define KEY_RIGHT 68

#define NO_DESCRIPTION NULL
#define NO_FIELD NULL

/* Sen pen color */
#define color(__COLOR__) printf(__COLOR__);

/*Clear screen*/
#define __CLEAR_SCREEN printf("\e[1;1H\e[2J")

/* Coloring */
extern char* DECOR_HEAD_COLOR;
extern char* DECOR_OPTION_COLOR;
extern char* DECOR_OPTION_CHOSEN_COLOR;
extern char* DECOR_END_COLOR;

/* Formatting */
extern char* DECOR_HEAD_START;
extern char* DECOR_HEAD_END;

extern char* DECOR_OPTION_START;
extern char* DECOR_OPTION_END;

extern char* DECOR_OPTION_CHOSEN_START;
extern char* DECOR_OPTION_CHOSEN_END;

extern char* DECOR_END_START;
extern char* DECOR_END_END;

/*Providing interface*/
typedef struct basicTUI{
  size_t no_panels; // The number of panels

  size_t no_options_per_panel; //The number of the option per panel, as we displaying a matrix choice or not

  size_t no_field; //Number of field

  char *head; // The title of the current TUI

  char *footer; //The closing string of the current UI

  char ***option; //Panel of options, each char** pointer is a panel
  
  char ***description; //The description

  char **field; //Extra input field
}basicTUI;

typedef struct tabTUI{
  basicTUI* option_per_tab;

  size_t no_tab; //The total number of tabs

  char* tab[]; //The pointer to the tab
}tabTUI;

/* Color & Decor getters, setters */
void setDecorChosen(char* start, char* end, char* color);
void setDecorOption(char* start, char* end, char* color);
void setDecorEnd(char* start, char* end, char* color);
void setDecorHead(char* start, char* end, char* color);
void setColor(char* head, char* footer, char* option, char* chosenOption);

/*Initialize Interface*/
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
basicTUI* initBasicTUI_Arr(char* title, char* footer, size_t no_of_option_per_panel, size_t no_panel, size_t no_field, char* option[no_panel][no_of_option_per_panel], char* description[no_panel][no_of_option_per_panel], char* field[]);

/*Running Interface (radio & checkboxes)*/
char** runBasicTUI_checkboxes(basicTUI* TUI, char* submitKey); //Reimplementing
char* runBasicTUI_radio(basicTUI* TUI);

/*Destroy Interface*/
void destroyTUI(basicTUI* TUI);

#endif /* __C_INTERFACE_H_ */