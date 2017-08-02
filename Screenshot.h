#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <stdio.h>
#include <GL/glut.h>
#include <WinGDI.h> /* windows only :( */

#define SCREENSHOT_ERROR	-1
#define SCREENSHOT_SUCCESS	 0

char *SCREENSHOT_FILENAME = "screenshot.bmp";

int ScreenshotCapture() {
	GLint iViewPortDataArray[4] = { 0 };
	GLint iWidth = 0;
	GLint iHeight = 0;
	GLenum elastBuffer = 0;
	GLubyte *pImageData = NULL;
	unsigned long ulIimageSize;
	BITMAPFILEHEADER stBmpFileHdr = { 0 };
	BITMAPINFOHEADER stBmpInfoHdr = { 0 };
	FILE *fdWritingFile = NULL;
	
	glGetIntegerv(GL_VIEWPORT, iViewPortDataArray);
	/*
	view port parameters not used in this example
	View Port X = iViewPortDataArray[0];
	View Port Y = iViewPortDataArray[1];
	*/
	iWidth = iViewPortDataArray[2];
	iHeight = iViewPortDataArray[3];

	/* setup the header */
	stBmpFileHdr.bfType = 0x4D42;
	stBmpFileHdr.bfReserved1 = 0;
	stBmpFileHdr.bfReserved2 = 0;
	stBmpFileHdr.bfOffBits = sizeof(stBmpFileHdr) + sizeof(stBmpInfoHdr);
	ulIimageSize = ((iWidth + ((4 - (iWidth % 4)) % 4)) * iHeight * 3) + 2;
	stBmpFileHdr.bfSize = ulIimageSize + sizeof(stBmpFileHdr) + sizeof(stBmpInfoHdr);
	pImageData = (GLubyte *)malloc(ulIimageSize);

	if (pImageData == NULL)
		return SCREENSHOT_ERROR;

	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_PACK_SWAP_BYTES, 1);
	glGetIntegerv(GL_READ_BUFFER, (GLint *)&elastBuffer);
	glReadBuffer(GL_FRONT);

	glReadPixels(0, 0, iWidth, iHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pImageData);
	pImageData[ulIimageSize - 1] = 0;
	pImageData[ulIimageSize - 2] = 0;
	glReadBuffer(elastBuffer);

	fdWritingFile = fopen(SCREENSHOT_FILENAME, "wb");
	if (fdWritingFile == NULL)
	{
		free(pImageData);
		return SCREENSHOT_ERROR;
	}

	stBmpInfoHdr.biSize = sizeof(stBmpInfoHdr);
	stBmpInfoHdr.biWidth = iWidth;
	stBmpInfoHdr.biHeight = iHeight;
	stBmpInfoHdr.biPlanes = 1;
	stBmpInfoHdr.biBitCount = 24;
	stBmpInfoHdr.biCompression = 0;
	stBmpInfoHdr.biSizeImage = ulIimageSize;
	stBmpInfoHdr.biXPelsPerMeter = 2835;
	stBmpInfoHdr.biYPelsPerMeter = 2835;
	stBmpInfoHdr.biClrUsed = 0;
	stBmpInfoHdr.biClrImportant = 0;

	fwrite(&stBmpFileHdr, sizeof(stBmpFileHdr), 1, fdWritingFile);
	fwrite(&stBmpInfoHdr, sizeof(stBmpInfoHdr), 1, fdWritingFile);
	fwrite(pImageData, ulIimageSize, 1, fdWritingFile);

	fclose(fdWritingFile);
	free(pImageData);

	return SCREENSHOT_SUCCESS;
}



#endif