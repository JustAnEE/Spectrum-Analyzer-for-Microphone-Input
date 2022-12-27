#include "../Hpp/FontSystem.hpp"

FontSystem::FontSystem(	const char* fontFilePath,
						const int fontSize,
						const std::string& fileName )
	: m_fileName(fileName), m_fontSize(fontSize)
{

	m_filePath = fontFilePath;

	// -- 16 x 8 BitMap initialization.
	m_atlasWidth = fontSize * 16;
	m_atlasHeight = fontSize * 8;

	// -- Each set of 4 bytes is 1 pixel.
	m_altasDataSize = m_atlasWidth * m_atlasHeight * 4;
	m_atlasData = (unsigned char*)calloc(m_altasDataSize, sizeof(unsigned char));

	if (m_atlasData == nullptr) {
		std::cout << "ERROR in FontSystem::FontSystem.  m_atlasData calloc failure.";
		exit(-100);
	}
}

FontSystem::~FontSystem(){
	free(m_atlasData);
}


FontSystem::Glyph FontSystem::getCharacterInformation(char c) {
	return characters[c];
}

int FontSystem::getAtlasWidth() {
	return m_atlasWidth;
}

int FontSystem::getAtlasHeight() {
	return m_atlasHeight;
}

unsigned char* FontSystem::getAtlasData() {
	return m_atlasData;
}


void FontSystem::generateFontAtlas(){

	// -- freetype initialization.
	FT_Library freetype;
	FT_Face face;

	if (FT_Init_FreeType(&freetype)) {
		std::cout << "ERROR in FontSystem::generateFontAtlas. freetype Initalization.";
		exit(-13);
	}
	if (FT_New_Face(freetype, m_filePath, 0, &face)) {
		std::cout << "ERROR in FontSystem::generateFontAtlas. Face Initalization.";
		exit(-14);
	}
	if (FT_Set_Pixel_Sizes(face, 0, m_fontSize)) {
		std::cout << "ERROR in FontSystem::generateFontAtlas. Font Initalization.";
		exit(-15);
	}

	// -- Generate all renderable ASCII characters. 
	for (unsigned char c = 0; c < 95; c++) {
		// -- Load ASCII Char c as a font glyph.
		if (FT_Load_Char(face, (c + 32), FT_LOAD_RENDER)) {
			std::cout << "ERROR in FontSystem::generateFontAtlas. Could not load char. ";
			exit(-17);
		}

		// -- The left x and bottom y pixel locations of the char in the atlas.
		int x = (c % 16) * m_fontSize;
		int y = (7 - (c / 16)) * m_fontSize;

		// -- Store ASCII Char c in a Glyph and insert it into the map.
		Glyph character = {
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			face->glyph->bitmap_left,
			face->glyph->bitmap_top,
			face->glyph->advance.x,

			x / (float)m_atlasWidth,
			(x + face->glyph->bitmap.width) / (float)m_atlasWidth,
			y / (float)m_atlasHeight,
			(y - face->glyph->bitmap.rows) / (float)m_atlasHeight
		};
		characters.insert({ (c + 32), character });

		// -- Copy the character into the font Atlas at position x and y.
		const FT_Bitmap& bitmap = face->glyph->bitmap;

		for (int r = 0; r < bitmap.rows; r++) {
			for (int c = 0; c < bitmap.width; c++) {
				// -- Getting value from the bitmap
				unsigned char value = bitmap.buffer[ r * bitmap.width + c];
				

				int idx = (y - r) * (m_atlasWidth * 4) + (x + c) * 4;

				m_atlasData[ idx + 0] = value;		// -- R
				m_atlasData[ idx + 1] = value;		// -- G
				m_atlasData[ idx + 2] = value;		// -- B
				m_atlasData[ idx + 3] = (value < 15) ? 0 : 255;
			}
		}
	}

	FT_Done_Face(face);
	FT_Done_FreeType(freetype);


	// -- Creation of the atlas headers
	BitmapFileHeader atlasFileHeader;
	atlasFileHeader.identity = 0x4d42; // -- "BM"
	atlasFileHeader.size = 54 + m_atlasWidth * m_atlasHeight;
	atlasFileHeader.reserved1 = 0;
	atlasFileHeader.reserved2 = 0;
	atlasFileHeader.offset = 54;

	BitmapInfoHeader atlasInfoHeader;
	atlasInfoHeader.headerSize = 40;
	atlasInfoHeader.bitmapWidth = m_atlasWidth;
	atlasInfoHeader.bitmapHeight = m_atlasHeight;
	atlasInfoHeader.colorPlanes = 1;
	atlasInfoHeader.bitsPerPixel = 32;
	atlasInfoHeader.compressionFlag = 0;
	atlasInfoHeader.imageSize = 0;
	atlasInfoHeader.horizontalRes = 0;
	atlasInfoHeader.verticalRes = 0;
	atlasInfoHeader.colors = 0;
	atlasInfoHeader.importantColors = 0;


	// -- Save the atlas as a bmp file.
	FILE* file;
	if (fopen_s(&file, m_fileName.c_str(), "wb")) {
		std::cout << "ERROR in FontSystem::generateFontAtlas.  cannot open bmp file.";
		exit(-420);
	}

	fwrite(&atlasFileHeader, sizeof(atlasFileHeader), 1, file);
	fwrite(&atlasInfoHeader, sizeof(atlasInfoHeader), 1, file);
	fwrite(m_atlasData, sizeof(unsigned char), m_altasDataSize, file);
	fclose(file);
}
