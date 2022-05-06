
#ifndef _INC_SuanfaAPI
#define _INC_SuanfaAPI
BOOL WINAPI ContourDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI Template(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
					 int iTempH, int iTempW, 
					 int iTempMX, int iTempMY,
					 FLOAT * fpArray, FLOAT fCoef);
#endif
