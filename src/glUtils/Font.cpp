//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================


#include "../glUtils/Font.hpp"
#include "../glUtils/ErrorGL.hpp"

#include <QFile>
#include <QByteArray>
#include <QtDebug>


using namespace glUtils;
using namespace utils;

Font::Font(const QString& fontFile): m_fontFile(fontFile)
		
{
    qDebug() << "Fontfile: "<<fontFile;

    //glDisable(GL_LINE_SMOOTH);
    QFile font_file(fontFile);
    if (!font_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open resources font file";
        return;
    }

    QByteArray contents = font_file.readAll();
    m_fontBuffer = new unsigned char[contents.size()];
    for(int i = 0; i < contents.size(); ++i)
    {
        m_fontBuffer[i] = static_cast<unsigned char>(contents.at(i));
    }

    m_font = new FTTextureFont(m_fontBuffer, contents.size());

	// check if font opened correctly
	utils::Error::Assert(!m_font->Error());

	m_font->CharMap(ft_encoding_unicode);
	m_font->UseDisplayList(true);
}

Font::~Font()
{
	if(m_font != NULL)
	{
		//delete m_font;
		m_font = NULL;
	}
    if(m_fontBuffer != NULL)
    {
        delete [] m_fontBuffer;
    }
}

void Font::SetTypeface(const QString& fontFile)
{
    m_font = new FTTextureFont(fontFile.toLatin1().data());

	// check if font opened correctly
	utils::Error::Assert(!m_font->Error());

	m_font->CharMap(ft_encoding_unicode);
	m_font->UseDisplayList(true);
}

void Font::Render(const QString& text, uint x, uint y)
{
	glUtils::ErrorGL::Check();
    m_font->Render(text.toLatin1().data(), -1, FTPoint(x, y), FTPoint(), FTGL::RENDER_FRONT);
	glUtils::ErrorGL::Check();
}

void Font::SetSize(uint size)
{
	bool rtn = m_font->FaceSize(size);
	utils::Error::Assert(rtn);
}

BBox Font::GetBoundingBox(const QString& text)
{ 
	glUtils::ErrorGL::Check();
    FTBBox bbox = m_font->BBox(text.toLatin1().data());
	glUtils::ErrorGL::Check();

	return BBox(bbox.Lower().Xf(), bbox.Lower().Yf(), 
								bbox.Upper().Xf(), bbox.Upper().Yf());
}


