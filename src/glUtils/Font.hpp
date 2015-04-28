//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================

#ifndef _OPENGL_FONT_
#define _OPENGL_FONT_

#include "../utils/Common.hpp"
#include <QString>
#include <FTGL/ftgl.h>

namespace glUtils
{

/**
 * @brief Render TrueType fonts on OpenGL canvas.
 */
class Font
{
public:
	/**
	 * @brief Build a new font.
	 * @param fontFile TrueType font file (e.g., times.ttf, arial.ttf).
	 */
    Font(const QString& fontFile);

	/** Destructor. */
	~Font();

	/**
	 * @brief Change the font typeface.
	 * @param fontFile TrueType font file (e.g., times.ttf, arial.ttf).
	 */
    void SetTypeface(const QString& fontFile);

	/**
	 * @brief Render text to current OpenGL canvas.
	 * @param text Text to render.
	 * @param x,y Location to render bottom, left corner of text.
	 */
    void Render(const QString& text, uint x, uint y);

	/**
	 * @brief Set size of font.
	 * @param size Desired size of font.
	 * @throw Exception If font can not be set to specified size.
	 */
	void SetSize(uint size);

	/**
	 * @brief Get size of font.
	 * @return Size of font.
	 */
	uint GetSize() {	return m_font->FaceSize(); }

	/**
	 * @brief Get descender amount for font.
	 * @return Descender of font.
	 */
	float GetDescender() { return m_font->Descender(); }

	/**
	 * @brief Get ascender amount for font.
	 * @return Ascender of font.
	 */
	float GetAscender() { return m_font->Ascender(); }

	/**
	 * @brief Get bounding box of a string.
	 * @param text Text to determine bounding box of.
	 * @return Bounding box.
	 */
    utils::BBox GetBoundingBox(const QString& text);

	/**
	 * @brief Type of font (e.g., Times.ttf, Arial.ttf).
	 * @return TrueType filename.
	 */
    const QString& GetFontFile() { return m_fontFile; }

private:
	/** TTF font file. */
    QString m_fontFile;

    /** The contents of the TTF font file used for FTGL*/
    unsigned char * m_fontBuffer;

	/** Smart pointer to a FTFont. */
	FTTextureFont* m_font;
};

}

#endif
