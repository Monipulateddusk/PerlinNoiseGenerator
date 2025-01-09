#include "BaseUserInterfaceElement.h"

std::list<BaseUserInterfaceElement*> BaseUserInterfaceElement::elements;

BaseUserInterfaceElement::BaseUserInterfaceElement(int pX, int pY, int w, int h)
{
	elements.push_back(this);

	posX = pX;
	posY = pY;
	width = w;
	height = h;
	isMouseInside = false;

	initFont();
}

BaseUserInterfaceElement::~BaseUserInterfaceElement()
{
	elements.remove(this);
}

bool BaseUserInterfaceElement::updateUI(MouseState& state, int screenHeight)
{
	int x = state.mouseXPos;
	int y = screenHeight - state.mouseYPos;

	isMouseInside = false;

	// Check to see if the mouse's cursor is inside the element, if so, set the inside to true
	if (x >= posX && x <= posX + width &&
		y >= posY && y <= posY + height)
	{
		isMouseInside = true;
	} 
	return false;
}

void BaseUserInterfaceElement::initFont()
{
	unsigned char* fontBuffer;
	long size;

	// Load the font file and then init into the stb_trueType
	// Wasteful to do it for every class that inherits but for now that isn't too problematic
	FILE* fontFile = fopen("..\\res\\8-bit-hud.ttf", "rb");
	fseek(fontFile, 0, SEEK_END);
	size = ftell(fontFile); // Determining file length
	fseek(fontFile, 0, SEEK_SET);

	fontBuffer = (unsigned char*)malloc(size);

	if (fontBuffer != 0) {
		fread(fontBuffer, size, 1, fontFile);
	}
	else {
		// If the font buffer is 0 for any number of reasons, close the program before continuing
		assert("FONT BUFFER IS 0.");
	}
	fclose(fontFile);

	// Prep the font
	if (!stbtt_InitFont(&fontInfo, fontBuffer, 0)) {
		assert("INIT FONT FAILED.");
	}
}
/// <summary>
/// Ordinarally this generates a single channel of data. By multiplying the bitmap width and height in the calloc by 4, we can store RGBA values for transparency
/// </summary>
/// <param name="text"></param>
/// <param name="b_w"></param>
/// <param name="b_h"></param>
/// <param name="l_h"></param>
/// <returns></returns>
BitmapInfo BaseUserInterfaceElement::writeText(const char* text, int b_w, int b_h, int l_h)
{
	unsigned char* bitmap = (unsigned char*)calloc(b_w * b_h * 4, sizeof(unsigned char));

	// Font scaling calcs
	float scale = stbtt_ScaleForPixelHeight(&fontInfo, l_h);

	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);
	ascent = roundf(ascent * scale);
	descent = roundf(descent * scale);

	int x = 0;
	for (int i = 0; i < strlen(text); ++i) {
		// Calculate how wide the character is
		int ax, lsb; //advance width, leftside bearing
		stbtt_GetCodepointHMetrics(&fontInfo, text[i], &ax, &lsb);

		// If we cannot get a character as it is not in our ttf file/not supported, skip it (don't render)
		if (text[i] == ' ') {
			x += roundf(ax * scale);
			continue;
		}


		// Get bounding box for character. Sometimes offsets for characters that dip above or below the line (g, y for example)
		int c_x1, c_y1, c_x2, c_y2; // Character bounds
		stbtt_GetCodepointBitmapBox(&fontInfo, text[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

		// Calc the character's height - not the same always 
		int y = ascent + c_y1;

		// Render character
		int charWidth = (c_x2 - c_x1);
		int charHeight = (c_y2 - c_y1);

		if (charWidth <= 0 || charHeight <= 0) {
			continue; // Skip invalid character
		}

		unsigned char* charBitmap = (unsigned char*)malloc(charWidth * charHeight);
		stbtt_MakeCodepointBitmap(&fontInfo, charBitmap, charWidth, charHeight, charWidth, scale, scale, text[i]);

		// Open AI aided with this for loop. Was unsure how to process the raw data to output the desired
		for (int cy = 0; cy < charHeight; ++cy)
		{
			for (int cx = 0; cx < charWidth; ++cx)
			{
				int gsBitmapIndex = cy * charWidth + cx; // Grayscale bitmap index
				int rGBAIndex = (x + cx + (y + cy) * b_w) * 4;
				unsigned char alphaValue = charBitmap[gsBitmapIndex];	// This warning is confusing. We are assuming that the malloc worked to correctly intitalise the data
				bitmap[rGBAIndex] = 0;    // Red
				bitmap[rGBAIndex + 1] = 0; // Green
				bitmap[rGBAIndex + 2] = 0; // Blue
				bitmap[rGBAIndex + 3] = alphaValue; // Alpha

			}
		}
		free(charBitmap); // As we used malloc to assign the pointer, we must use free to free the memory - delete doesn't work

		x += roundf(ax * scale);

		// add kerning to the width
		int kern;
		kern = stbtt_GetCodepointKernAdvance(&fontInfo, text[i], text[i + 1]);
		x += roundf(kern * scale);
	}

	return BitmapInfo(bitmap, b_w, b_h, l_h);
}

BaseUserInterfaceElement* addElement(BaseUserInterfaceElement* element)
{
	static int lastX = 5;
	static int lastY = 5;

	element->setPosition(lastX, lastY);
	lastY += element->getHeight() + 5;
	return element;
}
