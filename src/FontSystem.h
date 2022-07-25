#ifndef FONTSYS_H
#define FONTSYS_H

#include <iostream>
#include <string>
#include <map>


#include <ft2build.h>
#include FT_FREETYPE_H 


class FontSystem{
public:
	FontSystem(	const char* fontFilePath,
				const int fontSize,
				const std::string& bitmapFilename);
	~FontSystem();

	void generateFontAtlas();


	// -- Glyph struct to store Glyph information.
	struct Glyph {
		int width;
		int height;
		int bearingX;
		int bearingY;
		int advance;

		// -- texture Coords of char in the Atlas
		float atlasLeft;
		float atlasRight;
		float atlasTop;
		float atlasBottom;
	};

	// -- O(log(n)) lookup
	Glyph getCharacterInformation(char c);
	int getAtlasHeight();
	int getAtlasWidth();
	unsigned char* getAtlasData();
	unsigned char* m_atlasData;


private:
	// -- Map to store each char
	std::map<char, Glyph> characters;

	// -- Member attributes
	int m_atlasWidth;
	int m_atlasHeight;
	const int m_fontSize;
	const std::string m_fileName;
	const char* m_filePath;
	//unsigned char* m_atlasData;
	int m_altasDataSize;


	// -- Disable Struct padding
	#pragma pack(push, 1) 

	// -- See https://en.wikipedia.org/wiki/BMP_file_format for more information.
	struct BitmapFileHeader {
		short identity;		// -- BM, BA, CI ... etc, in hex.
		int size;			// -- In bytes.
		short reserved1;
		short reserved2;
		int offset;			// -- Starting address of the pixel array.
	};

	// -- See https://en.wikipedia.org/wiki/BMP_file_format for more information.
	struct BitmapInfoHeader {
		int headerSize;
		int bitmapWidth;
		int bitmapHeight;
		short colorPlanes;		// -- Must be 1
		short bitsPerPixel;		// -- 1,4,8,16,24,32.
		int compressionFlag;
		int imageSize;
		int horizontalRes;		// -- Horizontal pixel per metre.
		int verticalRes;		// -- Vertical pixel per metre.
		int colors;				// -- 0 or size 2^n.
		int importantColors;	// -- 0 if every color is important.
	};

	// -- Re-Enable Struct padding
	#pragma pack(pop)

};


#endif // !FONTSYS_H
