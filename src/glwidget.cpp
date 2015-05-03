/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <QtDebug>

#include "utils/Point.hpp"
#include "core/State.hpp"

using namespace utils;
using namespace pygmy;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"));
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
    m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
    if (m_transparent)
        setAttribute(Qt::WA_TranslucentBackground);

    m_zoomMin = 1.0f;
    m_zoomMax = 6.0f; //State::Inst().GetZoomMax();
    m_translateMin = 0.0f;
    m_translateMax = 0.0f;
    m_translate = 0.0f;
}

GLWidget::~GLWidget()
{}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(600, 400);
}


void GLWidget::initializeGL()
{
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.

    //initializeOpenGLFunctions();

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

}



void GLWidget::paintGL()
{
    glUtils::ErrorGL::Check();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*if(m_visualColourMap)
        m_visualColourMap->Render(m_width, size().height());*/

    if(m_visualTree)
    {
        qDebug() << size().width() <<" "<< size().height()<< " "<<m_translate << " "<<m_zoom;
        m_visualTree->Render(size().width(), size().height(), m_translate, m_zoom);

        /*if(m_overview)
        {
            // render the overview scene to reflect changes in the viewport
            m_overview->TranslationFraction(TranslationFraction());
            m_overview->ViewportHeightFraction(m_visualTree->GetViewportHeightFraction());
            m_overview->Render();
        }*/
    }

    glUtils::ErrorGL::Check();

}

void GLWidget::resizeGL(int w, int h)
{
    glUtils::ErrorGL::Check();

    if(isVisible())
    {
        // adjust orthographic projection settings
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, (GLint) w, 0, (GLint) h);
        glMatrixMode(GL_MODELVIEW);
        glViewport(0, 0, (GLint) w, (GLint) h);
        glLoadIdentity();

        // indicate that the viewport dimensions have changed
        AdjustViewport();
        ZoomExtents();
        TranslationExtents();

        // save dimensions
        m_previousSize.setWidth(w);
        m_previousSize.setHeight(h);

        // repaint viewport
        update();
    }

    glUtils::ErrorGL::Check();

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}


void GLWidget::setTree(utils::Tree<pygmy::NodePhylo>::Ptr tree)
{
    m_visualTree.reset(new pygmy::VisualTree(tree));
    m_visualTree->Layout();

    // calculate bounding boxes for all leaf node labels
    m_visualTree->LabelBoundingBoxes();
    m_visualTree->CalculateTreeDimensions(size().width(), size().height(), m_zoom);
    // set min/max values for zoom
    SetDefaultZoom();

    ZoomExtents();

    // set initial zoom:
    Point border = State::Inst().GetBorderSize();
    if(size().height()-2*border.y > m_visualTree->GetTreeHeight())
    {
        // The entire tree can fit within the viewport, so we set the zoom
        // so it fills the entire viewport.
        SetZoom((size().height()-2*border.y) / m_visualTree->GetTreeHeight());
        SetTranslation(0);
    }
    else
    {
        // Since the entire tree can't be displayed in the viewport, set the
        // zoom factor to the default value and place the node of the graph
        // at the center of the viewport.
        SetZoom(m_zoomMin*State::Inst().GetZoomDefault());
        SetTranslation((m_visualTree->GetTreeHeight() + border.x) * m_zoom/2 - size().height()/2);
    }

    // Rebuild any display lists and render the scene
    update();
}

void GLWidget::SetTranslation(float translation)
{
    m_translate = translation;

    if(m_translate > m_translateMax)
        m_translate = m_translateMax;
    else if(m_translate < m_translateMin)
        m_translate = m_translateMin;

    TranslationChanged();
}

void GLWidget::TranslateView(int dx, int dy)
{
    // translate view
    SetTranslation(m_translate + State::Inst().GetTranslationSensitivity()*dy);

    update();
}

void GLWidget::ScaleView(int dx, int dy)
{
    float zoomSensitivity = State::Inst().GetZoomSensitivity();
    SetZoom(m_zoom + zoomSensitivity*(dy)*m_zoom);

    update();
}

void GLWidget::SetZoom(float zoom)
{
    qDebug() << __FILE__ << ":"<<__LINE__<<" "<<m_zoom<<" "<<m_translate << " "<<zoom << " "<<m_zoomMin << " "<<m_zoomMax;

    float previousZoom = m_zoom;
    float previousTranslation = m_translate;

    // bounds check and update any quantities dependent on the zoom
    m_zoom = zoom;

    if(m_zoom > m_zoomMax)
        m_zoom = m_zoomMax;
    else if(m_zoom < m_zoomMin)
        m_zoom = m_zoomMin;

    ZoomChanged();

    // modify translation so the middle line does not move during zooming
    SetTranslation(previousTranslation + (previousTranslation+size().height()*0.5)*(m_zoom-previousZoom)/previousZoom);

    qDebug() << __FILE__ << " "<<__LINE__<<" "<<m_zoom<<" "<<m_translate << " "<<zoom << " "<<m_zoomMin << " "<<m_zoomMax;
    update();
}

void GLWidget::SetDefaultZoom()
{
    // reset zooming factor to default value
    float targetZoom;
    qDebug() <<__FILE__<<" "<<__LINE__<<" "<< size().height() << " "<< State::Inst().GetBorderSize().y <<" "<< m_visualTree->GetTreeHeight();
    if((size().height()-2*State::Inst().GetBorderSize().y) > m_visualTree->GetTreeHeight())
        targetZoom = (size().height()-2*State::Inst().GetBorderSize().y)/m_visualTree->GetTreeHeight();
    else
        targetZoom = m_zoomMin*State::Inst().GetZoomDefault();

    SetZoom(targetZoom);
}

void GLWidget::TranslateViewWheel(int dWheel)
{
    SetTranslation(m_translate + State::Inst().GetScrollSensitivity()*dWheel);

    update();
}

void GLWidget::ModifiedFont()
{
    if(!m_visualTree)
        return;

    m_visualTree->LabelBoundingBoxes();
    AdjustViewport();
    ZoomExtents();
}

void GLWidget::ZoomExtents()
{
    if(m_visualTree)
    {
        // calculate extents of zoom
        m_zoomMin = 1.0f;
        m_zoomMax = m_zoomMin*State::Inst().GetZoomMax();
        qDebug() <<__FILE__<<":"<<__LINE__<<" "<<m_zoom <<" "<<m_zoomMin << " "<< m_zoomMax << " "<<size().height() << " "<< 2*State::Inst().GetBorderSize().y << " "<< m_visualTree->GetTreeHeight();
        if(m_zoomMax < (size().height()-2*State::Inst().GetBorderSize().y)/m_visualTree->GetTreeHeight())
        {
            // make sure the maximum zoom is large enough to allow
            // small graphs to fill the entire viewport
            m_zoomMax = (size().height()-2*State::Inst().GetBorderSize().y)/m_visualTree->GetTreeHeight();
        }

        // make sure zoom factor is within allowable range
        SetZoom(m_zoom);

        update();
    }
}

void GLWidget::TranslationExtents()
{
    if(!m_visualTree)
        return;

    // calculate extents of translation
    m_translateMin = 0;	// negative translations are not allowed

    m_translateMax = m_visualTree->GetTreeHeight()*m_zoom + 2*State::Inst().GetBorderSize().y - size().height();
    if(m_translateMax < 0)
        m_translateMax = 0;

    // make sure translation factor is within allowable range
    SetTranslation(m_translate);

    update();
}

void GLWidget::AdjustViewport()
{
    if(!m_visualTree)
        return;

    // adjust translation so mid-line of the viewport is unchanged
    SetTranslation(m_translate + 0.5f*(m_previousSize.height() - size().height()));

    update();
}

void GLWidget::ZoomChanged()
{
    if(!m_visualTree)
        return;

    m_visualTree->CalculateTreeDimensions(size().height(), size().height(), m_zoom);
    TranslationExtents();
}

void GLWidget::CenterNode(uint id)
{
    // find node with specified id
    std::vector< NodePhylo* > leaves = m_visualTree->GetTree()->GetLeaves();
    NodePhylo* node = NULL;
    for(NodePhylo* leaf : leaves)
    {
        if(leaf->GetId() == id)
        {
            node = leaf;
            break;
        }
    }

    float posY = node->GetPosition().y * m_visualTree->GetTreeHeight() * m_zoom + State::Inst().GetBorderSize().y;
    float translatedY = posY-m_translate;

    if(translatedY < 0 || translatedY > size().height())
    {
        // Node is currently outside the viewport.
        SetTranslation(posY-0.5*size().height());
    }

    update();
}

void GLWidget::TranslationFraction(float frac)
{
    SetTranslation(frac*(m_visualTree->GetTreeHeight()*m_zoom
                                        + 2*State::Inst().GetBorderSize().y) + State::Inst().GetBorderSize().y);

    update();
}

float GLWidget::TranslationFraction()
{
    if(!m_visualTree)
        return 0.0f;

    float frac = (m_translate - State::Inst().GetBorderSize().y)
                                    / (m_visualTree->GetTreeHeight()*m_zoom + State::Inst().GetBorderSize().y);
    if(frac < 0.0f)
        frac = 0.0f;

    return frac;
}

void GLWidget::SetBranchStyle(VisualTree::BRANCH_STYLE branchStyle)
{
    if(!m_visualTree)
        return;

    if(m_visualTree->GetBranchStyle() != branchStyle)
    {
        m_visualTree->SetBranchStyle(branchStyle);
        update();
    }
}


void GLWidget::SetColourMap(VisualColourMapPtr visualColourMap)
{
    m_visualColourMap = visualColourMap;

    // Rebuild any display lists and render the scene
    update();
}
