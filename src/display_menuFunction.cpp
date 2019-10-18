#include "display_menuFunction.h"
#include <LiquidCrystal_I2C.h>
#include <LCDMenuLib2.h>
#include <TimeLib.h>

#define MAX_SIZE_STRING_DATETIME 20

extern LiquidCrystal_I2C lcd;
extern LCDMenuLib2 LCDML;

/* ===================================================================== *
 *                                                                       *
 * Menu Callback Function                                                *
 *                                                                       *
 * ===================================================================== *
 *
 * EXAMPLE CODE:

// *********************************************************************
void your_function_name(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // setup
    // is called only if it is started

    // starts a trigger event for the loop function every 100 milliseconds
    LCDML.FUNC_setLoopInterval(100);
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop
    // is called when it is triggered
    // - with LCDML_DISP_triggerMenu( milliseconds )
    // - with every button status change

    // check if any button is pressed (enter, up, down, left, right)
    if(LCDML.BT_checkAny()) {
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // loop end
    // you can here reset some global vars or delete it
  }
}


 * ===================================================================== *
 */


// *********************************************************************
void mFunc_information(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // setup function
    lcd.setCursor(0, 0);
    lcd.print(F("To close this"));
    lcd.setCursor(0, 1);
    lcd.print(F("function press"));
    lcd.setCursor(0, 2);
    lcd.print(F("any button or use"));
    lcd.setCursor(0, 3);
    lcd.print(F("back button"));
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if(LCDML.BT_checkAny()) { // check if any button is pressed (enter, up, down, left, right)
      // LCDML_goToMenu stops a running menu function and goes to the menu
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}


// *********************************************************************
uint8_t g_func_timer_info = 0;  // time counter (global variable)
unsigned long g_timer_1 = 0;    // timer variable (global variable)
void mFunc_timer_info(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    lcd.print(F("20 sec wait")); // print some content on first row
    g_func_timer_info = 20;       // reset and set timer
    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds

    LCDML.TIMER_msReset(g_timer_1);
  }


  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function

    // reset screensaver timer
    LCDML.SCREEN_resetTimer();

    // this function is called every 100 milliseconds

    // this method checks every 1000 milliseconds if it is called
    if(LCDML.TIMER_ms(g_timer_1, 1000)) {
      g_func_timer_info--;                // increment the value every second
      lcd.setCursor(0, 0);                // set cursor pos
      lcd.print(F("  "));
      lcd.setCursor(0, 0);                // set cursor pos
      lcd.print(g_func_timer_info);       // print the time counter value
    }

    // this function can only be ended when quit button is pressed or the time is over
    // check if the function ends normally
    if (g_func_timer_info <= 0)
    {
      // leave this function
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}


// *********************************************************************
uint8_t g_button_value = 0; // button value counter (global variable)
uint8_t g_posCursor = 0; // cursor position
boolean g_editingCursor = false; // true if cursor is in editing mode
uint8_t g_tmp_value = 0; // temp variable to store decimal value
char g_strDateTmp[]="01/01/2001"; // string with temporal date
void mFunc_p2(uint8_t param)
// *********************************************************************
{
  char buffer[20];
  char strMaskInput[]="NN/NN/NNNN";
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // setup function
    // print LCD content
    //sprintf(strDate,"%02u/01/2001",g_tmp_day);
    lcd.setCursor(0, 0);
    lcd.print(F("Configurar Fecha"));
    lcd.setCursor(0, 1);
    lcd.print(g_strDateTmp);
    lcd.setCursor(0, 1);
    lcd.cursor();
    // Reset Button Value
    g_button_value = 0;
    g_posCursor = 0;
    g_editingCursor = false;

    // Disable the screensaver for this function until it is closed
    LCDML.FUNC_disableScreensaver();

  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      if (LCDML.BT_checkEnter()) {
        if (strMaskInput[g_posCursor]=='N') {
          if (g_editingCursor) {
            sprintf(buffer, "%u", g_tmp_value);
            g_strDateTmp[g_posCursor]=buffer[0];
            Serial.print("Tmp value: ");Serial.println(buffer);
            Serial.println(g_strDateTmp);
            lcd.noBlink();
          } else {
            char tempChar = g_strDateTmp[g_posCursor];
            g_tmp_value = (uint8_t)tempChar - 48; // converting char to uint using ascii value
            //Serial.print(F("ascii: "));Serial.println((uint8_t)tempChar);
            //Serial.print(F("g_tmp_value: ")); Serial.println(g_tmp_value);
            lcd.blink();
          }
          g_editingCursor=!g_editingCursor;
        }
      }
      if (LCDML.BT_checkLeft() || LCDML.BT_checkUp()) {// check if button left is pressed
        LCDML.BT_resetLeft(); // reset the left button
        LCDML.BT_resetUp(); // reset the left button
        if (g_editingCursor){
          if (strMaskInput[g_posCursor]=='N') {
            g_tmp_value = ((g_tmp_value>0)?g_tmp_value-1:0);
          }
          lcd.setCursor(g_posCursor,1);
          lcd.print(g_tmp_value);
          lcd.setCursor(g_posCursor,1);
        } else {
          g_posCursor = ((g_posCursor>0)?g_posCursor-1:0);
          lcd.setCursor(g_posCursor, 1); // set cursor
          //lcd.print(g_button_value); // print change content
          Serial.println(F("Izquierda"));
        }
      }
      if (LCDML.BT_checkRight() || LCDML.BT_checkDown()) {// check if button right is pressed
        LCDML.BT_resetRight(); // reset the left button
        LCDML.BT_resetDown(); // reset the left button
        if (g_editingCursor){
          g_tmp_value = ((g_tmp_value<9)?g_tmp_value+1:9);
          lcd.setCursor(g_posCursor,1);
          lcd.print(g_tmp_value);
          lcd.setCursor(g_posCursor,1);
        } else {
          g_posCursor = ((g_posCursor<9)?g_posCursor+1:9);
          lcd.setCursor(g_posCursor, 1); // set cursor
          //lcd.print(g_button_value); // print change content
          Serial.println(F("Derecha"));
        }
      }
      Serial.print(F("Posicion cursor: "));
      Serial.println(g_posCursor);
    }

    
    // check if button count is three
    if (g_button_value >= 3) {
      LCDML.FUNC_goBackToMenu();      // leave this function
    }
  }

  if(LCDML.FUNC_close()) {    // ****** STABLE END *********
    lcd.noCursor();
    // you can here reset some global vars or do nothing
  }
}

#define SCREENSAVER_CHECK_BUTTONS_TIME 100
#define SCREENSAVER_REFRESH_TIME 1000
#define SCREENSAVER_FADEOUT 30000
#define SCREENSAVER_COUNT_DISPLAY_CLOCK (SCREENSAVER_REFRESH_TIME / SCREENSAVER_CHECK_BUTTONS_TIME) // be.: 10 * 100ms of loop interval => 1seg refresh screensaver.
#define SCREENSAVER_COUNT_FADEOUT (SCREENSAVER_FADEOUT / SCREENSAVER_CHECK_BUTTONS_TIME)


int countDisplayClock, countFadeout;
boolean LCD_backlight;

char* strDateTime(char* dateString) {
    snprintf_P(dateString, 
            MAX_SIZE_STRING_DATETIME,
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            day(),
            month(),
            year(),
            hour(),
            minute(),
            second() );
    return dateString;
  }
void updateLCDScreen() {
  char dateString[MAX_SIZE_STRING_DATETIME];

  // update LCD content
  lcd.setCursor(0, 0); // set cursor
  lcd.print(strDateTime(dateString));
  Serial.print(F("Hora :"));
  Serial.println(dateString);
  lcd.setCursor(0, 1);
  lcd.print(F("P1-- P2*M P3-- P4--"));
  lcd.setCursor(0, 2);
  lcd.print(F("P5*A P6-- P7-- P8-- "));
  lcd.setCursor(0, 3);
  lcd.print(F("  [Menu] - Pulsar   "));
  
}

// *********************************************************************
void mFunc_screensaver(uint8_t param)
// *********************************************************************
{  
  if(LCDML.FUNC_setup()) {         // ****** SETUP *********
    countDisplayClock = SCREENSAVER_COUNT_DISPLAY_CLOCK; 
    countFadeout = SCREENSAVER_COUNT_FADEOUT;
    LCD_backlight = true;

    updateLCDScreen(); // update LCD content
      
    LCDML.FUNC_setLoopInterval(SCREENSAVER_CHECK_BUTTONS_TIME);  // starts a trigger event for the loop function every 100 milliseconds
  }

  if(LCDML.FUNC_loop()) {
    if (LCDML.BT_checkAny()) {// check if any button is pressed (enter, up, down, left, right)
      if (countFadeout) {
        Serial.println(F("Vuelta al menu"));
        LCDML.FUNC_goBackToMenu();  // leave this function
      } else {
        Serial.println(F("Desactivando FADEOUT"));
        countFadeout = SCREENSAVER_COUNT_FADEOUT;
        LCD_backlight = true;
        updateLCDScreen();
        lcd.backlight();
      }
      
    } else {  
      if (countFadeout) {
        countFadeout--;
        if (countDisplayClock) {
          countDisplayClock--;
        } else {
          updateLCDScreen(); // update LCD content
          countDisplayClock=SCREENSAVER_COUNT_DISPLAY_CLOCK;
        } 
      } else {
        if (LCD_backlight) {
          LCD_backlight = false;
          Serial.println(F("Activando FADEOUT"));
          lcd.clear();
          lcd.noBacklight();
        }        
      }
    }
  }

  if(LCDML.FUNC_close())  {
    Serial.println(F("Desactivando Screensaver"));
    // The screensaver go to the root menu
    LCDML.MENU_goRoot();
  }
}



// *********************************************************************
void mFunc_back(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // end function and go an layer back
    LCDML.FUNC_goBackToMenu(1);      // leave this function and go a layer back
  }
}


// *********************************************************************
void mFunc_goToRootMenu(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // go to root and display menu
    LCDML.MENU_goRoot();
  }
}


// *********************************************************************
void mFunc_jumpTo_timer_info(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // Jump to main screen
    LCDML.OTHER_jumpToFunc(mFunc_timer_info);
  }
}


/* ===================================================================== *
 *                                                                       *
 * Dynamic content                                                       *
 *                                                                       *
 * ===================================================================== *
 */


uint8_t g_dynParam = 100; // when this value comes from an EEPROM, load it in setup
                          // at the moment here is no setup function (To-Do)
void mDyn_para(uint8_t line)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    g_dynParam=50;
  }
  // check if this function is active (cursor stands on this line)
  if (line == LCDML.MENU_getCursorPos())
  {
    // make only an action when the cursor stands on this menu item
    //check Button
    if(LCDML.BT_checkAny()) {
      if(LCDML.BT_checkEnter()) {
        // this function checks returns the scroll disable status (0 = menu scrolling enabled, 1 = menu scrolling disabled)
        if(LCDML.MENU_getScrollDisableStatus() == 0) {
          // disable the menu scroll function to catch the cursor on this point
          // now it is possible to work with BT_checkUp and BT_checkDown in this function
          // this function can only be called in a menu, not in a menu function
          LCDML.MENU_disScroll();
        }
        else {
          // enable the normal menu scroll function
          LCDML.MENU_enScroll();
        }
        // do something
        // ...
      }
      // This check have only an effect when MENU_disScroll is set
      if(LCDML.BT_checkUp()) {
        g_dynParam--;
      }

      // This check have only an effect when MENU_disScroll is set
      if(LCDML.BT_checkDown()) {
        g_dynParam++;
      }

      if(LCDML.BT_checkLeft()) {
        g_dynParam--;
      }
      if(LCDML.BT_checkRight()) {
        g_dynParam++;
      }
    }
  }

  char buf[20];
  sprintf (buf, "dynValue: %d", g_dynParam);

  // use the line from function parameters
  lcd.setCursor(1, line);
  lcd.print(buf);

}

