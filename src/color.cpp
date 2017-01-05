#include "color.h"
#ifdef WIN32
#include <windows.h>
#endif

/** @brief This function sets the given stream to the given color, YELLOW is default.
*		   This is a legacy function, which is not recommended to use any more.
*
*  @param  out  The ostream to be changed, defines which stream you want to set
*  @param  color The Color to set. YELLOW by default.
*/
#if WIN32
void setColor(std::ostream& out, int choice) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (choice) {
		case RED: // red
			SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case YELLOW: // yellow
			SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case GREEN: // green
			SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case BLUE: // blue
			SetConsoleTextAttribute(hOut, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		//case WHITE: // white
		case NORMAL: // normal
			SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
	}
}

#elif (linux || __MACH__)
void setColor(std::ostream& out, int choice) {
	switch (choice) {
		case RED: // red
			out << "\e[31m";
			break;
		case YELLOW: // yellow
			out << "\e[33m";
			break;
		case GREEN: // green
			out << "\e[32m";
			break;
		case BLUE: // blue
			out << "\e[34m";
			break;
		//case WHITE: // white
		case NORMAL: // normal
			out << "\e[0m";
			break;
		case BOLD: // bold 
			out << "\e[1m";
			break;
		case UNDERLINE: // bold 
			out << "\e[4m";
			break;
	}	
}
#endif

std::ostream& operator << (std::ostream& out, Color c) {
#ifndef SCRIPT
	setColor(out, c);
#endif
	return out;
}

