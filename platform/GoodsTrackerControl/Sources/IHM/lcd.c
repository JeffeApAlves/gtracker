/*
 * lcd.c
 *
 *  Created on: 19/06/2017
 *      Author: Fl�vio Soares
 */

#include "LCDout.h"
#include "lcd.h"

inline void LCDInit(){

	LCDout_Init();
	LCDout_Clear();
	LCDout_GotoXY(1, 1);
}
//----------------------------------------------------------------------------------

inline void LCDWrite(char ch){

	LCDout_Write(ch);
}
//----------------------------------------------------------------------------------

inline void LCDWriteLn(void){

	LCDout_WriteLn();
}
//----------------------------------------------------------------------------------

inline void LCDWriteLineStr(byte line, char *str){

	LCDout_WriteLineStr(line, str);
}
//----------------------------------------------------------------------------------

inline void LCDWriteString(char *str){

	LCDout_WriteString(str);
}
//----------------------------------------------------------------------------------

inline void LCDShiftLeft(void){

	LCDout_ShiftLeft();
}
//----------------------------------------------------------------------------------

inline void LCDShiftRight(void){

	LCDout_ShiftRight();
}
//----------------------------------------------------------------------------------

inline void LCDGotoXY(byte line, byte column){

	LCDout_GotoXY(line, column);
}
//----------------------------------------------------------------------------------

inline void LCDSetEntryMode(bool increment, bool shiftLeft){

	LCDout_SetEntryMode(increment, shiftLeft);
}
//----------------------------------------------------------------------------------

inline void LCDCursorShiftRight(void){

	LCDout_CursorShiftRight();
}
//----------------------------------------------------------------------------------

inline void LCDCursorShiftLeft(void){

	LCDout_CursorShiftLeft();
}
//----------------------------------------------------------------------------------

inline void LCDHome(void){

	LCDout_Home();
}
//----------------------------------------------------------------------------------

inline void LCDLine(byte line){

	LCDout_Line(line);
}
//----------------------------------------------------------------------------------

inline void LCDClear(void){

	LCDout_Clear();
}
//----------------------------------------------------------------------------------

