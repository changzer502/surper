// ************************************************************************
//  文件名：dibapi.cpp
//
//  DIB(Independent Bitmap) API函数库：
//
//  PaintDIB()          - 绘制DIB对象
//  CreateDIBPalette()  - 创建DIB对象调色板
//  FindDIBBits()       - 返回DIB图像象素起始位置
//  DIBWidth()          - 返回DIB宽度
//  DIBHeight()         - 返回DIB高度
//  PaletteSize()       - 返回DIB调色板大小
//  DIBNumColors()      - 计算DIB调色板颜色数目
//  CopyHandle()        - 拷贝内存块
//
//  SaveDIB()           - 将DIB保存到指定文件中
//  ReadDIBFile()       - 重指定文件中读取DIB对象
//
// ************************************************************************

#include "stdafx.h"
#include "dibapi.h"
#include <io.h>
#include <errno.h>

#include <math.h>
#include <direct.h>

/*
 * Dib文件头标志（字符串"BM"，写DIB时用到该常数）
 */
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

/*************************************************************************
 *
 * 函数名称：
 *   PaintDIB()
 *
 * 参数:
 *   HDC hDC            - 输出设备DC
 *   LPRECT lpDCRect    - 绘制矩形区域
 *   HDIB hDIB          - 指向DIB对象的指针
 *   LPRECT lpDIBRect   - 要输出的DIB区域
 *   CPalette* pPal     - 指向DIB对象调色板的指针
 *
 * 返回值:
 *   BOOL               - 绘制成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数主要用来绘制DIB对象。其中调用了StretchDIBits()或者
 * SetDIBitsToDevice()来绘制DIB对象。输出的设备由由参数hDC指
 * 定；绘制的矩形区域由参数lpDCRect指定；输出DIB的区域由参数
 * lpDIBRect指定。
 *
 ************************************************************************/

BOOL WINAPI PaintDIB(HDC     hDC,
					LPRECT  lpDCRect,
					HDIB    hDIB,
					LPRECT  lpDIBRect,
					CPalette* pPal)
{
	LPSTR    lpDIBHdr;            // BITMAPINFOHEADER指针
	LPSTR    lpDIBBits;           // DIB象素指针
	BOOL     bSuccess=FALSE;      // 成功标志
	HPALETTE hPal=NULL;           // DIB调色板
	HPALETTE hOldPal=NULL;        // 以前的调色板

	// 判断DIB对象是否为空
	if (hDIB == NULL)
	{
		// 返回
		return FALSE;
	}

	// 锁定DIB
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIBHdr);

	// 获取DIB调色板，并选中它
	if (pPal != NULL)
	{
		hPal = (HPALETTE) pPal->m_hObject;

		// 选中调色板
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}

	// 设置显示模式
	::SetStretchBltMode(hDC, COLORONCOLOR);

	// 判断是调用StretchDIBits()还是SetDIBitsToDevice()来绘制DIB对象
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
	{
		// 原始大小，不用拉伸。
		bSuccess = ::SetDIBitsToDevice(hDC,                    // hDC
								   lpDCRect->left,             // DestX
								   lpDCRect->top,              // DestY
								   RECTWIDTH(lpDCRect),        // nDestWidth
								   RECTHEIGHT(lpDCRect),       // nDestHeight
								   lpDIBRect->left,            // SrcX
								   (int)DIBHeight(lpDIBHdr) -
									  lpDIBRect->top -
									  RECTHEIGHT(lpDIBRect),   // SrcY
								   0,                          // nStartScan
								   (WORD)DIBHeight(lpDIBHdr),  // nNumScans
								   lpDIBBits,                  // lpBits
								   (LPBITMAPINFO)lpDIBHdr,     // lpBitsInfo
								   DIB_RGB_COLORS);            // wUsage
	}
    else
	{
		// 非原始大小，拉伸。
		bSuccess = ::StretchDIBits(hDC,                          // hDC
							   lpDCRect->left,                 // DestX
							   lpDCRect->top,                  // DestY
							   RECTWIDTH(lpDCRect),            // nDestWidth
							   RECTHEIGHT(lpDCRect),           // nDestHeight
							   lpDIBRect->left,                // SrcX
							   lpDIBRect->top,                 // SrcY
							   RECTWIDTH(lpDIBRect),           // wSrcWidth
							   RECTHEIGHT(lpDIBRect),          // wSrcHeight
							   lpDIBBits,                      // lpBits
							   (LPBITMAPINFO)lpDIBHdr,         // lpBitsInfo
							   DIB_RGB_COLORS,                 // wUsage
							   SRCCOPY);                       // dwROP
	}
	
    // 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);
	
	// 恢复以前的调色板
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}
	
	// 返回
	return bSuccess;
}

/*************************************************************************
 *
 * 函数名称：
 *   CreateDIBPalette()
 *
 * 参数:
 *   HDIB hDIB          - 指向DIB对象的指针
 *   CPalette* pPal     - 指向DIB对象调色板的指针
 *
 * 返回值:
 *   BOOL               - 创建成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数按照DIB创建一个逻辑调色板，从DIB中读取颜色表并存到调色板中，
 * 最后按照该逻辑调色板创建一个新的调色板，并返回该调色板的句柄。这样
 * 可以用最好的颜色来显示DIB图像。
 *
 ************************************************************************/


BOOL WINAPI CreateDIBPalette(HDIB hDIB, CPalette* pPal)
{
	// 指向逻辑调色板的指针
	LPLOGPALETTE lpPal;
	
	// 逻辑调色板的句柄
	HANDLE hLogPal;
	
	// 调色板的句柄
	HPALETTE hPal = NULL;
	
	// 循环变量
	int i;
	
	// 颜色表中的颜色数目
	WORD wNumColors;
	
	// 指向DIB的指针
	LPSTR lpbi;
	
	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFO lpbmi;
	
	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREINFO lpbmc;
	
	// 表明是否是Win3.0 DIB的标记
	BOOL bWinStyleDIB;
	
	// 创建结果
	BOOL bResult = FALSE;
	
	// 判断DIB是否为空
	if (hDIB == NULL)
	{
		// 返回FALSE
		return FALSE;
	}
	
	// 锁定DIB
	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	
	// 获取指向BITMAPINFO结构的指针（Win3.0）
	lpbmi = (LPBITMAPINFO)lpbi;
	
	// 获取指向BITMAPCOREINFO结构的指针
	lpbmc = (LPBITMAPCOREINFO)lpbi;
	
	// 获取DIB中颜色表中的颜色数目
	wNumColors = ::DIBNumColors(lpbi);
	
	if (wNumColors != 0)
	{
		// 分配为逻辑调色板内存
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)
									+ sizeof(PALETTEENTRY)
									* wNumColors);
		
		// 如果内存不足，退出
		if (hLogPal == 0)
		{
			// 解除锁定
			::GlobalUnlock((HGLOBAL) hDIB);
			
			// 返回FALSE
			return FALSE;
		}
		
		lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);
		
		// 设置版本号
		lpPal->palVersion = PALVERSION;
		
		// 设置颜色数目
		lpPal->palNumEntries = (WORD)wNumColors;
		
		// 判断是否是WIN3.0的DIB
		bWinStyleDIB = IS_WIN30_DIB(lpbi);

		// 读取调色板
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				// 读取红色分量
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
				
				// 读取绿色分量
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
				
				// 读取蓝色分量
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
				
				// 保留位
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				// 读取红色分量
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
				
				// 读取绿色分量
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
				
				// 读取红色分量
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
				
				// 保留位
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}
		
		// 按照逻辑调色板创建调色板，并返回指针
		bResult = pPal->CreatePalette(lpPal);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) hLogPal);
		
		// 释放逻辑调色板
		::GlobalFree((HGLOBAL) hLogPal);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);
	
	// 返回结果
	return bResult;
}

/*************************************************************************
 *
 * 函数名称：
 *   FindDIBBits()
 *
 * 参数:
 *   LPSTR lpbi         - 指向DIB对象的指针
 *
 * 返回值:
 *   LPSTR              - 指向DIB图像象素起始位置
 *
 * 说明:
 *   该函数计算DIB中图像象素的起始位置，并返回指向它的指针。
 *
 ************************************************************************/


LPSTR WINAPI FindDIBBits(LPSTR lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + ::PaletteSize(lpbi));
}


/*************************************************************************
 *
 * 函数名称：
 *   DIBWidth()
 *
 * 参数:
 *   LPSTR lpbi         - 指向DIB对象的指针
 *
 * 返回值:
 *   DWORD              - DIB中图像的宽度
 *
 * 说明:
 *   该函数返回DIB中图像的宽度。对于Windows 3.0 DIB，返回BITMAPINFOHEADER
 * 中的biWidth值；对于其它返回BITMAPCOREHEADER中的bcWidth值。
 *
 ************************************************************************/


DWORD WINAPI DIBWidth(LPSTR lpDIB)
{
	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFOHEADER lpbmi;
	
	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREHEADER lpbmc;

	// 获取指针
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// 返回DIB中图像的宽度
	if (IS_WIN30_DIB(lpDIB))
	{
		// 对于Windows 3.0 DIB，返回lpbmi->biWidth
		return lpbmi->biWidth;
	}
	else
	{
		// 对于其它格式的DIB，返回lpbmc->bcWidth
		return (DWORD)lpbmc->bcWidth;
	}
}


/*************************************************************************
 *
 * 函数名称：
 *   DIBHeight()
 *
 * 参数:
 *   LPSTR lpDIB        - 指向DIB对象的指针
 *
 * 返回值:
 *   DWORD              - DIB中图像的高度
 *
 * 说明:
 *   该函数返回DIB中图像的高度。对于Windows 3.0 DIB，返回BITMAPINFOHEADER
 * 中的biHeight值；对于其它返回BITMAPCOREHEADER中的bcHeight值。
 *
 ************************************************************************/


DWORD WINAPI DIBHeight(LPSTR lpDIB)
{
	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFOHEADER lpbmi;
	
	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREHEADER lpbmc;

	// 获取指针
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// 返回DIB中图像的宽度
	if (IS_WIN30_DIB(lpDIB))
	{
		// 对于Windows 3.0 DIB，返回lpbmi->biHeight
		return lpbmi->biHeight;
	}
	else
	{
		// 对于其它格式的DIB，返回lpbmc->bcHeight
		return (DWORD)lpbmc->bcHeight;
	}
}


/*************************************************************************
 *
 * 函数名称：
 *   PaletteSize()
 *
 * 参数:
 *   LPSTR lpbi         - 指向DIB对象的指针
 *
 * 返回值:
 *   WORD               - DIB中调色板的大小
 *
 * 说明:
 *   该函数返回DIB中调色板的大小。对于Windows 3.0 DIB，返回颜色数目×
 * RGBQUAD的大小；对于其它返回颜色数目×RGBTRIPLE的大小。
 *
 ************************************************************************/


WORD WINAPI PaletteSize(LPSTR lpbi)
{
	// 计算DIB中调色板的大小
	if (IS_WIN30_DIB (lpbi))
	{
		//返回颜色数目×RGBQUAD的大小
		return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBQUAD));
	}
	else
	{
		//返回颜色数目×RGBTRIPLE的大小
		return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
	}
}


/*************************************************************************
 *
 * 函数名称：
 *   DIBNumColors()
 *
 * 参数:
 *   LPSTR lpbi         - 指向DIB对象的指针
 *
 * 返回值:
 *   WORD               - 返回调色板中颜色的种数
 *
 * 说明:
 *   该函数返回DIB中调色板的颜色的种数。对于单色位图，返回2，
 * 对于16色位图，返回16，对于256色位图，返回256；对于真彩色
 * 位图（24位），没有调色板，返回0。
 *
 ************************************************************************/
WORD WINAPI DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;

	// 对于Windows的DIB, 实际颜色的数目可以比象素的位数要少。
	// 对于这种情况，则返回一个近似的数值。
	
	// 判断是否是WIN3.0 DIB
	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;
		
		// 读取dwClrUsed值
		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		
		if (dwClrUsed != 0)
		{
			// 如果dwClrUsed（实际用到的颜色数）不为0，直接返回该值
			return (WORD)dwClrUsed;
		}
	}

	// 读取象素的位数
	if (IS_WIN30_DIB(lpbi))
	{
		// 读取biBitCount值
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	}
	else
	{
		// 读取biBitCount值
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
	}
	
	// 按照象素的位数计算颜色数目
	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}


/*************************************************************************
 *
 * 函数名称：
 *   DIBBitCount()
 *
 * 参数:
 *   LPSTR lpbi         - 指向DIB对象的指针
 *
 * 返回值:
 *   WORD               - 返回调色板中颜色的种数
 *
 * 说明:
 *   该函数返回DIBBitCount。
 *
 ************************************************************************/
WORD WINAPI DIBBitCount(LPSTR lpbi)
{
	WORD wBitCount;

	// 读取象素的位数
	if (IS_WIN30_DIB(lpbi))
	{
		// 读取biBitCount值
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	}
	else
	{
		// 读取biBitCount值
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
	}
	
	// 返回wBitCount
	return wBitCount;
}

/*************************************************************************
 *
 * 函数名称：
 *   CopyHandle()
 *
 * 参数:
 *   HGLOBAL h          - 要复制的内存区域
 *
 * 返回值:
 *   HGLOBAL            - 复制后的新内存区域
 *
 * 说明:
 *   该函数复制指定的内存区域。返回复制后的新内存区域，出错时返回0。
 *
 ************************************************************************/

HGLOBAL WINAPI CopyHandle (HGLOBAL h)
{
	if (h == NULL)
		return NULL;

	// 获取指定内存区域大小
	DWORD dwLen = ::GlobalSize((HGLOBAL) h);
	
	// 分配新内存空间
	HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);
	
	// 判断分配是否成功
	if (hCopy != NULL)
	{
		// 锁定
		void* lpCopy = ::GlobalLock((HGLOBAL) hCopy);
		void* lp     = ::GlobalLock((HGLOBAL) h);
		
		// 复制
		memcpy(lpCopy, lp, dwLen);
		
		// 解除锁定
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}






/*************************************************************************
 *
 * 函数名称：
 *   SaveDIB()
 *
 * 参数:
 *   HDIB hDib          - 要保存的DIB
 *   CFile& file        - 保存文件CFile
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE或者CFileException
 *
 * 说明:
 *   该函数将指定的DIB对象保存到指定的CFile中。该CFile由调用程序打开和关闭。
 *
 *************************************************************************/


BOOL WINAPI SaveDIB(HDIB hDib, CFile& file)
{
	// Bitmap文件头
	BITMAPFILEHEADER bmfHdr;
	
	// 指向BITMAPINFOHEADER的指针
	LPBITMAPINFOHEADER lpBI;
	
	// DIB大小
	DWORD dwDIBSize;

	if (hDib == NULL)
	{
		// 如果DIB为空，返回FALSE
		return FALSE;
	}

	// 读取BITMAPINFO结构，并锁定
	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);
	
	if (lpBI == NULL)
	{
		// 为空，返回FALSE
		return FALSE;
	}
	
	// 判断是否是WIN3.0 DIB
	if (!IS_WIN30_DIB(lpBI))
	{
		// 不支持其它类型的DIB保存
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) hDib);
		
		// 返回FALSE
		return FALSE;
	}

	// 填充文件头

	// 文件类型"BM"
	bmfHdr.bfType = DIB_HEADER_MARKER;

	// 计算DIB大小时，最简单的方法是调用GlobalSize()函数。但是全局内存大小并
	// 不是DIB真正的大小，它总是多几个字节。这样就需要计算一下DIB的真实大小。
	
	// 文件头大小＋颜色表大小
	// （BITMAPINFOHEADER和BITMAPCOREHEADER结构的第一个DWORD都是该结构的大小）
	dwDIBSize = *(LPDWORD)lpBI + ::PaletteSize((LPSTR)lpBI);
	
	// 计算图像大小
	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// 对于RLE位图，没法计算大小，只能信任biSizeImage内的值
		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		// 象素的大小
		DWORD dwBmBitsSize;

		// 大小为Width * Height
		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
		
		// 计算出DIB真正的大小
		dwDIBSize += dwBmBitsSize;

		// 更新biSizeImage（很多BMP文件头中biSizeImage的值是错误的）
		lpBI->biSizeImage = dwBmBitsSize;
	}


	// 计算文件大小：DIB大小＋BITMAPFILEHEADER结构大小
	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	
	// 两个保留字
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	// 计算偏移量bfOffBits，它的大小为Bitmap文件头大小＋DIB头大小＋颜色表大小
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
											  + PaletteSize((LPSTR)lpBI);
	// 尝试写文件
	TRY
	{
		// 写文件头
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		
		// 写DIB头和象素
		file.WriteHuge(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
		// 解除锁定
		::GlobalUnlock((HGLOBAL) hDib);
		
		// 抛出异常
		THROW_LAST();
	}
	END_CATCH
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDib);
	
	// 返回TRUE
	return TRUE;
}


/*************************************************************************
 *
 * 函数名称：
 *   ReadDIBFile()
 *
 * 参数:
 *   CFile& file        - 要读取得文件文件CFile
 *
 * 返回值:
 *   HDIB               - 成功返回DIB的句柄，否则返回NULL。
 *
 * 说明:
 *   该函数将指定的文件中的DIB对象读到指定的内存区域中。除BITMAPFILEHEADER
 * 外的内容都将被读入内存。
 *
 *************************************************************************/

HDIB WINAPI ReadDIBFile(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	DWORD dwBitsSize;
	HDIB hDIB;
	LPSTR pDIB;

	// 获取DIB（文件）长度（字节）
	dwBitsSize = file.GetLength();

	// 尝试读取DIB文件头  
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
	{
		// 大小不对，返回NULL。
		return NULL;
	}

	// 判断是否是DIB对象，检查头两个字节是否是"BM"
	if (bmfHeader.bfType != DIB_HEADER_MARKER)
	{
		// 非DIB对象，返回NULL。
		return NULL;
	}

	// 为DIB分配内存
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB == 0)
	{
		// 内存分配失败，返回NULL。
		return NULL;
	}
	
	// 锁定
	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	// 读象素  文件接着刚刚读过的地方读取（BITMAPFILEHEADER后）
	if (file.ReadHuge(pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) !=
		dwBitsSize - sizeof(BITMAPFILEHEADER) )
	{
		// 大小不对。
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// 释放内存
		::GlobalFree((HGLOBAL) hDIB);
		
		// 返回NULL。
		return NULL;
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);
	
	// 返回DIB句柄
	return hDIB;
}



