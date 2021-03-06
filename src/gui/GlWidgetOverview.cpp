//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================

#include "GlWidgetOverview.hpp"
#include "../core/State.hpp"
#include <QDebug>
#include <QMouseEvent>


using namespace pygmy;
using namespace utils;

//*** Member Functions***

GLWidgetOverview::GLWidgetOverview(QWidget * parent)
    : GLWidgetBase(parent), m_borderX(5), m_borderY(5)
{
    m_zoomMin = 1.0f;
    m_zoomMax = 1.0f;
    m_translateMin = 0.0f;
    m_translateMax = 0.0f;
}

GLWidgetOverview::~GLWidgetOverview()
{
}

void GLWidgetOverview::initializeGL()
{
    //initializeOpenGLFunctions();
    qDebug() <<__FILE__<<" "<<__LINE__<<" "<<__PRETTY_FUNCTION__;

    glUtils::ErrorGL::Check();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);		// White Background

    // setup antialiasing and blending to optimize appearance
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // setup how depth testing will be performed
    glDisable(GL_DEPTH_TEST);	// disable depth testing since we are using a single 2D plane
    glDepthMask(GL_FALSE);		// disable depth mask to improve visual quality of lines

    // setup desired default point size and line width
    glPointSize(1.0);
    glLineWidth(1.0f);
    glEnable(GL_LINE_STIPPLE);

    // setup texture filtering
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // adjust orthographic projection settings
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, (GLint) size().width(), 0, (GLint) size().height());
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, (GLint) size().width(), (GLint) size().height());
    glLoadIdentity();

    glUtils::ErrorGL::Check();
    m_treeList = glGenLists(1);
    m_textSearchList = glGenLists(1);

}

void GLWidgetOverview::SetTree(VisualTreePtr visualTree)
{
	m_visualTree = visualTree;
    // redraw the tree in the overview
    RedrawTree();
}

/*void GLWidgetOverview::TranslateView(int dx, int dy)
{
	// move viewport window in accordance with the amount of mouse movement
	float curTransFrac = m_viewport->TranslationFraction();
	float transFrac = float(dy)/(m_height - 2*m_borderY);

	float newFrac = curTransFrac - transFrac;
	if(newFrac < 0.0) newFrac = 0.0;
	if(newFrac > 1.0f) newFrac = 1.0f;

	m_viewport->TranslationFraction(newFrac);
}*/

void GLWidgetOverview::mousePressEvent(QMouseEvent *event)
{
    qDebug() << __FILE__ << __LINE__ << event;
    //m_lastMousePos = event->pos();
    // do nothing if the tree has yet to be set
    if(m_visualTree == NULL) {
        return;
    }
    if(event->button() == Qt::LeftButton) {
        // We need to minus the height of the widget from the position
        // of the mouse click as the coordinate system for Qt and the
        // openGL viewport are inverted on the y-axis
        // Note: Mouse (window) have 0,0 at the top, left
        // whereas the viewport has 0,0 at the bottom, left
        LeftClick(utils::Point(event->x(), event->y()));

    } else if (event->button() == Qt::RightButton) {
        // right button pressed
    }
}

void GLWidgetOverview::LeftClick(const utils::Point& mousePt)
{
    // determine where mouse is on tree
    float frac = (mousePt.y - m_borderY) / (size().height() - 2*m_borderY);

    // NOTE: The following line was commented to fix a bug where clicking
    // towards the bottom of the overview never resulted in the viewport
    // actually going there. This is a partial fix for Issue #4 in github.
    // center the viewport at this point
    //frac -= 0.5*m_viewportHeightFrac;
	
	// bound position
	if(frac < 0.0f) frac = 0.0f;
	if(frac > 1.0f) frac = 1.0f;

    // send a signal to the other widgets that the main viewport
    // should change its position to this fraction of the length
    // of the tree
    emit newTranslationFraction(frac);
}

void GLWidgetOverview::Redraw()
{
    if(!m_visualTree)
		return;

    RedrawTree();
    RedrawTextSearch();

    //update();
}

void GLWidgetOverview::RedrawTree()
{
	glUtils::ErrorGL::Check();

	if(!m_visualTree)
		return;

	// *** Draw the tree. ***
	glNewList(m_treeList, GL_COMPILE);
	{
		glDisable(GL_LINE_SMOOTH);

		glLineWidth(State::Inst().GetOverviewLineWidth());
        int thicknessMajor = static_cast<int>(1 + State::Inst().GetOverviewLineWidth()*0.5);
        int thicknessMinor = static_cast<int>((State::Inst().GetOverviewLineWidth()-0.5)*0.5);

		// scale factor for tree (Note: glScalef() is not used as we want lines to be pixel aligned)
        float sx = size().width() - 2*m_borderX;
        float sy = size().height() - 2*m_borderY;

		// translation factor for tree (Note: glTranslatef() is not used as we want lines to be pixel aligned)
		float dx = m_borderX + 0.5;
		float dy = m_borderY + 0.5;
		
		// *** Draw overview tree
		std::vector<NodePhylo*> nodes = m_visualTree->GetTree()->GetNodes();
        for(NodePhylo* node : nodes)
		{			
			// adjust position of nodes based on desired orientation
			Point parentPos = node->GetPosition();

			// Draw branches
			float yMin = 1000.0f, yMax = 0.0f;
			std::vector<NodePhylo*> children = node->GetChildren();
            for(NodePhylo* child : children)
			{				
				Point childPos = child->GetPosition();
				
				// draw horizontal line
				glBegin(GL_QUADS);
					if(State::Inst().GetColourOverviewTree())
						node->GetColour().SetColourGL();
					else
						glColor3f(0.5f, 0.5f, 0.5f);
					glVertex2i(int(parentPos.x*sx + dx), int(childPos.y*sy + dy + thicknessMajor));
					glVertex2i(int(parentPos.x*sx + dx), int(childPos.y*sy + dy - thicknessMinor));

					if(State::Inst().GetColourOverviewTree())
							child->GetColour().SetColourGL();
						else
							glColor3f(0.5f, 0.5f, 0.5f);
					glVertex2i(int(childPos.x*sx + dx), int(childPos.y*sy + dy - thicknessMinor));
					glVertex2i(int(childPos.x*sx + dx), int(childPos.y*sy + dy + thicknessMajor));						
				glEnd();

				if(yMin > childPos.y) 
					yMin = childPos.y;

				if(yMax < childPos.y) 
					yMax = childPos.y;
			}

			// It is better to draw a single vertical line as this reduces the number of line segments
			// that must be rendered and ensures that only a single crisp line is drawn (as opposed to multiple
			// overlapping lines which can cause strange visual artifacts).
			if(!node->IsLeaf())
			{
				if(State::Inst().GetColourOverviewTree())
					node->GetColour().SetColourGL();
				else
					glColor3f(0.5f, 0.5f, 0.5f);

				// Draw vertical line adjusting for the width of the line being drawn. For some reason, the offset
				// needed to account for the line width is different for the start and end of the line (???).
				glBegin(GL_QUADS); 
					glVertex2i(int(parentPos.x*sx + dx + thicknessMajor), int(parentPos.y*sy + dy - thicknessMinor));
					glVertex2i(int(parentPos.x*sx + dx - thicknessMinor), int(parentPos.y*sy + dy - thicknessMinor));
					glVertex2i(int(parentPos.x*sx + dx - thicknessMinor), int(yMax*sy + dy + thicknessMajor));
					glVertex2i(int(parentPos.x*sx + dx + thicknessMajor), int(yMax*sy + dy + thicknessMajor));
				glEnd();

				glBegin(GL_QUADS); 
					glVertex2i(int(parentPos.x*sx + dx + thicknessMajor), int(yMin*sy + dy - thicknessMinor));
					glVertex2i(int(parentPos.x*sx + dx - thicknessMinor), int(yMin*sy + dy - thicknessMinor));
					glVertex2i(int(parentPos.x*sx + dx - thicknessMinor), int(parentPos.y*sy + dy + thicknessMajor));
					glVertex2i(int(parentPos.x*sx + dx + thicknessMajor), int(parentPos.y*sy + dy + thicknessMajor));
				glEnd();
			}
		}		

		glEnable(GL_LINE_SMOOTH);
	}
  glEndList();

	glUtils::ErrorGL::Check();
}

void GLWidgetOverview::RedrawTextSearch()
{
	glUtils::ErrorGL::Check();

	if(!m_visualTree)
		return;

	glNewList(m_textSearchList, GL_COMPILE);
	{
        float adjHeight = size().height() - 2*m_borderY;
		
		std::vector<NodePhylo*> nodes = m_visualTree->GetTree()->GetNodes();
        for(NodePhylo* node: nodes)
		{
			if(m_searchFilter->Filtered(node->GetId()))
			{
				Point parentPos = node->GetPosition();

				glColor3f(0.9f, 0.1f, 0.14f);

				float yPos = parentPos.y*adjHeight + m_borderY;					
				glBegin(GL_QUADS);
                    glVertex2f(size().width() - m_borderX, yPos - 2);
                    glVertex2f(size().width(), yPos - 2);
                    glVertex2f(size().width(), yPos + 2);
                    glVertex2f(size().width() - m_borderX, yPos + 2);
				glEnd();		
			
			}
		}
	}
    glEndList();

	glUtils::ErrorGL::Check();
}

void GLWidgetOverview::paintGL()
{
	const float MIN_POS_INDICATOR_HEIGHT = 5.0f;

	glUtils::ErrorGL::Check();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ------------------ render the tree ---------------
	if(m_visualTree)
	{		
        //qDebug() <<__FILE__<<__LINE__<<__PRETTY_FUNCTION__;

        //RedrawTree();
        //RedrawTextSearch();

        // *** Render overview tree. ***
		glPushMatrix();	
		{			
			glCallList(m_treeList);
		}
		glPopMatrix();

		// *** Highlight text search results
		glPushMatrix();		
		{		
			glCallList(m_textSearchList);			
		}
		glPopMatrix();

		// *** Render position indicator. ***
		if(State::Inst().GetOverviewPositionOverlay())
		{
			glPushMatrix();	
			{
				glTranslatef(0.0f, m_borderY, 0.0f);
                glScalef(size().width() - m_borderX, size().height() - 2*m_borderY, 1.0f);

				glColor4f(0.6f, 0.8f, 0.8f, 0.5f);

				// ensure position indicator is always visible
                if(m_viewportHeightFrac*(size().height() - 2*m_borderY) >= MIN_POS_INDICATOR_HEIGHT)
				{
                    glBegin(GL_QUADS);
						glVertex2f(0.0f, m_translationFrac);
						glVertex2f(1.0f, m_translationFrac);
						glVertex2f(1.0f, m_translationFrac + m_viewportHeightFrac);
						glVertex2f(0.0f, m_translationFrac + m_viewportHeightFrac);
					glEnd();
				}
				else
				{
					// determine height that is equivalent to the desired minimum height
                    float height = MIN_POS_INDICATOR_HEIGHT / (size().height() - 2*m_borderY);
					float midPoint = m_translationFrac + 0.5*m_viewportHeightFrac;
					glBegin(GL_QUADS);
						glVertex2f(0.0f, midPoint - 0.5*height);
						glVertex2f(1.0f, midPoint - 0.5*height);
						glVertex2f(1.0f, midPoint + 0.5*height);
						glVertex2f(0.0f, midPoint + 0.5*height);
					glEnd();
				}
			}
			glPopMatrix();
		}
	}

	glUtils::ErrorGL::Check();

}

void GLWidgetOverview::resizeGL(int w, int h)
{
    glUtils::ErrorGL::Check();

    if(isVisible())
    {
        qDebug() <<__FILE__<<__LINE__<<__PRETTY_FUNCTION__;

        // adjust orthographic projection settings
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, (GLint) w, 0, (GLint) h);
        glMatrixMode(GL_MODELVIEW);
        glViewport(0, 0, (GLint) w, (GLint) h);
        glLoadIdentity();


        // save dimensions
        m_previousSize.setWidth(w);
        m_previousSize.setHeight(h);

    }

    glUtils::ErrorGL::Check();

}


void GLWidgetOverview::SetSearchFilter(pygmy::FilterPtr filter)
{
    m_searchFilter = filter;
    m_visualTree->SetSearchFilter(filter);
    RedrawTextSearch();
}

