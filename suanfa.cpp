#include "stdafx.h"
#include "suanfa.h"
#include "DIBAPI.h"

#include <math.h>
#include <direct.h>
BOOL WINAPI ContourDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
		// 指向源图像的指针
	LPSTR	lpSrc;
	
	// 指向缓存图像的指针
	LPSTR	lpDst;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIB;

	//循环变量
	long i;
	long j;
	unsigned char n,e,s,w,ne,se,nw,sw;

	//像素值
	unsigned char pixel;

	// 暂时分配内存，以保存新图像
	hNewDIB = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIB == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIB);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);
	for(j = 1; j <lHeight-1; j++)
	{
		for(i = 1;i <lWidth-1; i++)
		{
			
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lWidth * j + i;
			
			// 指向目标图像倒数第j行，第i个象素的指针			
			lpDst = (char *)lpNewDIBBits + lWidth * j + i;
			
			//取得当前指针处的像素值，注意要转换为unsigned char型
			pixel = (unsigned char)*lpSrc;

			//目标图像中含有0和255外的其它灰度值
	//		if(pixel != 255 && pixel != 0)
	//		return FALSE;
			if(pixel == 0)
			{
				*lpDst = (unsigned char)0;
				nw = (unsigned char)*(lpSrc + lWidth -1);
				n  = (unsigned char)*(lpSrc + lWidth );
				ne = (unsigned char)*(lpSrc + lWidth +1);
				w = (unsigned char)*(lpSrc -1);
				e = (unsigned char)*(lpSrc +1);
				sw = (unsigned char)*(lpSrc - lWidth -1);
				s  = (unsigned char)*(lpSrc - lWidth );
				se = (unsigned char)*(lpSrc - lWidth +1);
				//如果相邻的八个点都是黑点
				if(nw+n+ne+w+e+sw+s+se==0)
				{
					*lpDst = (unsigned char)255;
				}
			}
		}
	}

	// 复制腐蚀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIB);
	LocalFree(hNewDIB);

	// 返回
	return TRUE;
}
/************************************************************************************/
BOOL WINAPI Template(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
					 int iTempH, int iTempW, 
					 int iTempMX, int iTempMY,
					 FLOAT * fpArray, FLOAT fCoef)
{
	// 指向复制图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 指向要复制区域的指针
	unsigned char*	lpDst;
	
	// 循环变量
	LONG	i;
	LONG	j;
	LONG	k;
	LONG	l;
	
	// 计算结果
	FLOAT	fResult;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	
	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);
	
	// 行(除去边缘几行)
	for(i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		// 列(除去边缘几列)
		for(j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			fResult = 0;
			
			// 计算
			for (k = 0; k < iTempH; k++)
			{
				for (l = 0; l < iTempW; l++)
				{
					// 指向DIB第i - iTempMY + k行，第j - iTempMX + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
						+ j - iTempMX + l;
					
					// 保存象素值
					fResult += (* lpSrc) * fpArray[k * iTempW + l];
				}
			}
			
			// 乘上系数
			fResult *= fCoef;
			
			// 取绝对值
			fResult = (FLOAT ) fabs(fResult);
			
			// 判断是否超过255
			if(fResult > 255)
			{
				// 直接赋值为255
				* lpDst = 255;
			}
			else
			{
				// 赋值
				* lpDst = (unsigned char) (fResult + 0.5);
			}
			
		}
	}
	
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	// 返回
	return TRUE;

}