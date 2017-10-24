/*
 * lcd.c
 *
 *  Created on: 19/06/2017
 *      Author: Fl�vio Soares
 */

#include "lcd.h"


/*
  LCDout_Clear();
  LCDout_GotoXY(1, 1);
  LCDout_WriteString("GOODSTRACKER");
  LCDout_GotoXY(2, 6);
  LCDout_WriteString("TESTE...123");
 */

int LCDInit()
{
	LCDout_Clear();
	LCDout_GotoXY(1, 1);

	return 0;
}

void LCDWrite(char ch)
{
	LCDout_Write(ch);
}

void LCDWriteLn(void)
{
	LCDout_WriteLn();
}

void LCDWriteLineStr(byte line, char *str)
{
	LCDout_WriteLineStr(line, str);
}

void LCDWriteString(char *str)
{
	LCDout_WriteString(str);
}

void LCDShiftLeft(void)
{
	LCDout_ShiftLeft();
}

void LCDShiftRight(void)
{
	LCDout_ShiftRight();
}

void LCDGotoXY(byte line, byte column)
{
	LCDout_GotoXY(line, column);
}

void LCDSetEntryMode(bool increment, bool shiftLeft)
{
	LCDout_SetEntryMode(increment, shiftLeft);
}

void LCDCursorShiftRight(void)
{
	LCDout_CursorShiftRight();
}

void LCDCursorShiftLeft(void)
{
	LCDout_CursorShiftLeft();
}

void LCDHome(void)
{
	LCDout_Home();
}

void LCDLine(byte line)
{
	LCDout_Line(line);
}

void LCDClear(void)
{
	LCDout_Clear();
}

