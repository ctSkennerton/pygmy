//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================

#include "../core/State.hpp"
#include "../glUtils/Font.hpp"

#include <QCoreApplication>
#include <QSettings>

using namespace pygmy;
using namespace utils;
/*
<Pygmy>
    <ColourMapVis width="20" show="1" showTickMarks="0" ignoreLeafNodes="1" />
    <ColourMapLabel fontSize="14" fontPrecision="3" scientific="0" valuesToShow="9" red="0" green="0" blue="0" />
</Pygmy>
*/
State::State()
{
	
}

State& State::Inst()
{
	static State state;	
	return state;
}

void State::Load()
{
    QCoreApplication::setOrganizationName("Ecogenomics");
    QCoreApplication::setOrganizationDomain("pygmy.ecogenomic.org");
    QCoreApplication::setApplicationName("pygmy");
    QSettings settings;

    m_fontFile = settings.value("Font/file", ":/resources/fonts/arial.ttf").toString();
    m_treeFontSize = settings.value("Font/size", 10).toInt();

    float red = settings.value("Font/red", 0.0f).toFloat();
    float green = settings.value("Font/green", 0.0f).toFloat();
    float blue = settings.value("Font/blue", 0.0f).toFloat();

    m_treeFontColour = Colour(red, green, blue);
    m_bShowLeafLabels = settings.value("Labels/showLeafLabels", true).toBool();
    m_bShowMetadataLabels = settings.value("Labels/showMetadataLabels", false).toBool();

    m_zoomDefault = settings.value("Zoom/default", 1.5f).toFloat();
    m_zoomMax = settings.value("Zoom/max", 6.0f).toFloat();

    m_border.x = settings.value("Layout/borderX", 10.0f).toFloat();
    m_border.y = settings.value("Layout/borderY", 20.0f).toFloat();
    m_labelOffset = settings.value("Layout/labelOffset", 3).toUInt();
    m_distanceOffsetX = settings.value("Layout/distanceOffsetX", 3).toUInt();
    m_distanceOffsetY = settings.value("Layout/distanceOffsetY", 3).toUInt();

    m_lineWidth = settings.value("Tree/lineWidth", 1.0f).toFloat();
    m_bOptimizeLeafNodes = settings.value("Tree/optimizeLeafOrdering", false).toBool();

    m_bShowInternalLabels = settings.value("InternalLabels/showLabels", false).toBool();
    m_internalLabelField = settings.value("InternalLabels/field", "Parsimony Data").toString();
    m_internalLabelPos = settings.value("InternalLabels/position", "Above Left").toString();
    m_internalLabelSize = settings.value("InternalLabels/fontSize", 10).toUInt();
    m_bInternalLabelScientific = settings.value("InternalLabels/scientific", false).toBool();
    m_internalLabelPrecision = settings.value("InternalLabels/fontPrecision", 2).toUInt();

    red = settings.value("InternalLabels/red", 0.0f).toFloat();
    green = settings.value("InternalLabels/green", 0.0f).toFloat();
    blue = settings.value("InternalLabels/blue", 0.0f).toFloat();
    m_internalLabelColour = Colour(red, green, blue);

    m_overviewLineWidth = settings.value("Overview/lineWidth", 1.0f).toFloat();
    m_bColourOverviewTree = settings.value("Overview/colourTree", true).toBool();
    m_bOverviewPosOverlay = settings.value("Overview/showPositionOverlay", true).toBool();

    red = settings.value("MissingDataColour/red", 0.0f).toFloat();
    green = settings.value("MissingDataColour/green", 0.0f).toFloat();
    blue = settings.value("MissingDataColour/blue", 0.0f).toFloat();
    m_missingDataColour = Colour(red, green, blue);

    red = settings.value("DefaultTreeColour/red", 0.0f).toFloat();
    green = settings.value("DefaultTreeColour/green", 0.0f).toFloat();
    blue = settings.value("DefaultTreeColour/blue", 0.0f).toFloat();
    m_defaultTreeColour = Colour(red, green, blue);

    m_scrollSensitivity = settings.value("Mouse/scrollSensitivity", 157.0f).toFloat();
    m_translationSensitivity = settings.value("Mouse/translationSensitivity", 15.8f).toFloat();
    m_zoomSensitivity = settings.value("Mouse/zoomSensitivity", 0.015f).toFloat();

        /*elem = pRoot->FirstChildElement("ColourMapVis");
		elem->GetAttribute("width", &m_colourMapWidth);
		elem->GetAttribute("show", &m_bShowColourMapVis);
		elem->GetAttribute("showTickMarks", &m_bShowColourMapTickMarks);
		elem->GetAttribute("ignoreLeafNodes", &m_bIgnoreLeafNodes);

		elem = pRoot->FirstChildElement("ColourMapLabel");
		elem->GetAttribute("fontSize", &m_colourMapFontSize);
		elem->GetAttribute("fontPrecision", &m_colourMapFontPrecision);
		elem->GetAttribute("scientific", &m_bColourMapScientific);
		elem->GetAttribute("valuesToShow", &m_colourMapLegendValuesToShow);
		elem->GetAttribute("red", &red);
		elem->GetAttribute("green", &green);
		elem->GetAttribute("blue", &blue);

        m_colourMapFontColour = Colour(red, green, blue);*/


    m_font.reset(new glUtils::Font(GetFontFile()));
}

void State::Save()
{ 
}
