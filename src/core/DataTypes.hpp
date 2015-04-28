//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================

#ifndef _DATA_TYPES_
#define _DATA_TYPES_

// basic data types
typedef unsigned int uint;
typedef unsigned char byte;
typedef unsigned long ulong;
#include <QSharedPointer>

// smart pointers to classes
namespace pygmy
{
	class Filter;
    typedef QSharedPointer<Filter> FilterPtr;

	class TextSearch;
    typedef QSharedPointer<TextSearch> TextSearchPtr;

	class MetadataInfo;
    typedef QSharedPointer<MetadataInfo> MetadataInfoPtr;

	class ViewportOverview;
	typedef ViewportOverview* ViewportOverviewPtr;

	class ViewportTree;
	typedef ViewportTree* ViewportTreePtr;

	class ViewportMain;
	typedef ViewportMain* ViewportMainPtr;

	class VisualColourMap;
    typedef QSharedPointer<VisualColourMap> VisualColourMapPtr;

	class VisualObject;
    typedef QSharedPointer<VisualObject> VisualObjectPtr;

	class VisualTree;
    typedef QSharedPointer<VisualTree> VisualTreePtr;
}

namespace glUtils
{
	class Viewport;
	typedef Viewport* ViewportPtr;	// smart pointer are not used here since wxWidgets objects
																	// handle deallocation internally	

	class ViewportOrtho;
	typedef ViewportOrtho* ViewportOrthoPtr;	// smart pointer are not used here since wxWidgets objects
																						// handle deallocation internally	
	class Font;
    typedef QSharedPointer<Font> FontPtr;
}

namespace utils
{
	class Colour;
    typedef QSharedPointer<Colour> ColourPtr;
	
	class ColourMap;
    typedef QSharedPointer<ColourMap> ColourMapPtr;

	class ColourMapContinuous;
    typedef QSharedPointer<ColourMapContinuous> ColourMapContinuousPtr;

	class ColourMapDiscrete;
    typedef QSharedPointer<ColourMapDiscrete> ColourMapDiscretePtr;

	class ColourMapManager;
    typedef QSharedPointer<ColourMapManager> ColourMapManagerPtr;

	class ParsimonyCalculator;
    typedef QSharedPointer<ParsimonyCalculator> ParsimonyCalculatorPtr;

}

#endif
