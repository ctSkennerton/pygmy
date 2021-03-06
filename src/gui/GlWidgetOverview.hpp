//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================

#ifndef _OVERVIEW_TREE_H_
#define _OVERVIEW_TREE_H_

#include "../core/NodePhylo.hpp"


#include "GlWidgetBase.hpp"

namespace pygmy
{

/**
 * @brief Overview tree.
 */
class GLWidgetOverview : public GLWidgetBase
{
    Q_OBJECT

public slots:
    /**
     * @brief Set fraction of tree height below the translation.
     * @param frac Fraction translated.
     */
    virtual void TranslationFraction(float frac) { m_translationFrac = frac; }

    /**
     * @brief Set fraction of tree height visible in the viewport.
     * @param frac Fraction visible.
     */
    virtual void ViewportHeightFraction(float frac) { m_viewportHeightFrac = frac; }

    void Redraw();
   /**
    * @brief Reconstruct tree display lists.
    */
   void RedrawTree();

   /**
    * @brief Reconstruct text search display lists.
    */
   void RedrawTextSearch();

public:
	/** Constructor. */
    GLWidgetOverview(QWidget * parent);

	/** Destructor. */
    ~GLWidgetOverview();

    QSize sizeHint() const
    {
        return QSize(200, 600);
    }

    QSize minimumSizeHint() const
    {
        return QSize(200, 400);
    }
	/**
	 * @brief Set phylogenetic tree to be displayed.
	 *
	 * @param visualTree Tree to display.
	 */
	virtual void SetTree(VisualTreePtr visualTree);

	/**
	 * @brief Get phylogenetic tree to be displayed.
	 *
	 * @param tree Tree to display.
	 */
	virtual VisualTreePtr GetTree() { return m_visualTree; }

    virtual void SetSearchFilter(pygmy::FilterPtr filter);


    
protected:
	/** 
	 * @brief Render the GL scene. Called by Render() and OnPaint() which 
	 *				set up the appropriate device context. 
	 */
     void paintGL();

     void resizeGL(int w, int h);

     void initializeGL();
	
	/** Set min/max values for zooming. */
	virtual void ZoomExtents() { m_zoomMin = 1.0f; m_zoomMax = 1.0f; }

	/** Set min/max values for translation. */
	virtual void TranslationExtents() { m_translateMin = 0.0f; m_translateMax = 0.0f; }

	// ** Scale viewport. */
	virtual void ScaleView(int dx, int dy) {}

	// ** Set default zoom level. */
	virtual void SetDefaultZoom() {}

	/** Called whenever zoom level has changed. */
	virtual void ZoomChanged() {}

	/** Called whenever translation changes. */
	virtual void TranslationChanged() {}

	// ** Window size has changed, so adjust the viewport size. */
    virtual void AdjustViewport() { update(); }

	// ** Translate viewport. */
//	virtual void TranslateView(int dx, int dy);

	// ** Translate viewport due to scroll wheel. */
    //virtual void TranslateViewWheel(int dWheel);

     void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;


	/** 
	 * @brief A left mouse button click has occured.
	 * @param mousePt Mouse position in window when button was clicked.
	 */
    void LeftClick(const utils::Point& mousePt);

protected:
	/** Fraction of tree's height below the translation. */
	float m_translationFrac;

	/** Fraction of a tree's height visible in the viewport. */
	float m_viewportHeightFrac;

	/** Border around overview map. */
	uint m_borderX, m_borderY;

	/** Display list used to render tree. */
	uint m_treeList;

	/** Display list used to render results of text search. */
	uint m_textSearchList;




signals:
    void newTranslationFraction(float);
};

}

#endif

