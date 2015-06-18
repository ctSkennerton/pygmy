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

#include "GlWidget.hpp"
#include "GlWidgetOverview.hpp"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <QtDebug>
#include <QMenu>

#include "../utils/Point.hpp"
#include "../core/State.hpp"

using namespace utils;
using namespace pygmy;

GLWidget::GLWidget(QWidget *parent) : GLWidgetBase(parent)
{
    m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"));
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
    m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
    if (m_transparent)
        QOpenGLWidget::setAttribute(Qt::WA_TranslucentBackground);

    m_zoomMin = 1.0f;
    m_zoomMax = 6.0f; //State::Inst().GetZoomMax();
    m_translateMin = 0.0f;
    m_translateMax = 0.0f;
    SetTranslation(0.0f);
}

GLWidget::~GLWidget()
{}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(500, 400);
}

QSize GLWidget::sizeHint() const
{
    return QSize(800, 600);
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
    gluOrtho2D(0, (GLint) QOpenGLWidget::size().width(), 0, (GLint) QOpenGLWidget::size().height());
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, (GLint) QOpenGLWidget::size().width(), (GLint) QOpenGLWidget::size().height());
    glLoadIdentity();

    glUtils::ErrorGL::Check();

}



void GLWidget::paintGL()
{
    qDebug() <<__FILE__<<" "<<__LINE__<<" "<<__PRETTY_FUNCTION__;

    glUtils::ErrorGL::Check();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*if(m_visualColourMap)
        m_visualColourMap->Render(m_width, size().height());*/

    if(m_visualTree)
    {
        m_visualTree->Render(QOpenGLWidget::size().width(), QOpenGLWidget::size().height(), GetTranslation(), GetZoom());


        emit TranslationFractionChanged(TranslationFraction());
        emit ViewportHeightFraction(m_visualTree->GetViewportHeightFraction());
        emit ShouldUpdateOverview();

    }

    glUtils::ErrorGL::Check();

}

void GLWidget::resizeGL(int w, int h)
{
    glUtils::ErrorGL::Check();

    if(QOpenGLWidget::isVisible())
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
        qDebug() <<__FILE__<<" "<<__LINE__<<" "<<__PRETTY_FUNCTION__<< " "<<GetTranslation()<< " "<< GetZoom();

        // save dimensions
        m_previousSize.setWidth(w);
        m_previousSize.setHeight(h);

        // repaint viewport
        //QOpenGLWidget::update();
    }

    glUtils::ErrorGL::Check();

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug() << __FILE__ << __LINE__ << event;
    m_lastMousePos = event->pos();
    // do nothing if the tree has yet to be set
    if(m_visualTree == NULL) {
        return;
    }
    if(event->button() == Qt::LeftButton) {
        // We need to minus the height of the widget from the position
        // of the mouse click as the coordinate system for Qt and the
        // openGL viewport are inverted on the y-axis
        m_visualTree->MouseLeftDown(utils::Point(event->x(), size().height() - event->y()));
        update();

    } else if (event->button() == Qt::RightButton) {
        // right button pressed
        m_visualTree->MouseLeftDown(utils::Point(event->x(), size().height() - event->y()));
        update();

        ShowContextMenu(event->pos());
    }
}

void GLWidget::ShowContextMenu(const QPoint& pos) // this is a slot
{
    // for most widgets
    QPoint globalPos = this->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    //QPoint globalPos = viewport()->mapToGlobal(pos);

    QMenu myMenu;
    QAction * rerootAct = myMenu.addAction(tr("&Reroot"));

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem == rerootAct)
    {
        m_visualTree->Reroot();
        update();
    }
    else
    {
        // nothing was chosen
    }
}

void GLWidget::setTree(utils::Tree<pygmy::NodePhylo>::Ptr tree)
{
    m_visualTree.reset(new pygmy::VisualTree(tree));
    m_visualTree->Layout();

    // calculate bounding boxes for all leaf node labels
    m_visualTree->LabelBoundingBoxes();
    m_visualTree->CalculateTreeDimensions(QOpenGLWidget::size().width(), QOpenGLWidget::size().height(), GetZoom());
    // set min/max values for zoom
    SetDefaultZoom();

    ZoomExtents();

    // set initial zoom:
    Point border = State::Inst().GetBorderSize();
    if(QOpenGLWidget::size().height()-2*border.y > m_visualTree->GetTreeHeight())
    {
        // The entire tree can fit within the viewport, so we set the zoom
        // so it fills the entire viewport.
        SetZoom((QOpenGLWidget::size().height()-2*border.y) / m_visualTree->GetTreeHeight());
        SetTranslation(0);
    }
    else
    {
        // Since the entire tree can't be displayed in the viewport, set the
        // zoom factor to the default value and place the node of the graph
        // at the center of the viewport.
        SetZoom(m_zoomMin*State::Inst().GetZoomDefault());
        SetTranslation((m_visualTree->GetTreeHeight() + border.x) * GetZoom()/2 - QOpenGLWidget::size().height()/2);
    }

    emit TranslationChanged(static_cast<int>(GetTranslation()));
    // Rebuild any display lists and render the scene
    QOpenGLWidget::update();
}

void GLWidget::SetTranslation(float translation)
{

    if(translation > m_translateMax)
        translation = m_translateMax;
    else if(translation < m_translateMin)
        translation = m_translateMin;

    m_translate = translation;

    // NOTE: You do not want to emit this signal here as it
    // creates a circular signal with the scrollbar and
    // causes a seqfault
    //emit TranslationChanged(static_cast<int>(translation));
}

void GLWidget::translate(int position)
{
    SetTranslation(m_translateMax - position);
    update();
}

void GLWidget::TranslateView(int dx, int dy)
{
    // translate view
    SetTranslation(GetTranslation() + State::Inst().GetTranslationSensitivity()*dy);

    QOpenGLWidget::update();
}

void GLWidget::ScaleView(int dx, int dy)
{
    float zoomSensitivity = State::Inst().GetZoomSensitivity();
    SetZoom(GetZoom() + zoomSensitivity*(dy)*GetZoom());

    QOpenGLWidget::update();
}

void GLWidget::SetZoom(float zoom)
{
    //qDebug() << __FILE__ << ":"<<__LINE__<<" "<<m_zoom<<" "<<m_translate << " "<<zoom << " "<<m_zoomMin << " "<<m_zoomMax;

    float previousZoom = GetZoom();
    float previousTranslation = GetTranslation();

    if(zoom > m_zoomMax)
        zoom = m_zoomMax;
    else if(zoom < m_zoomMin)
        zoom = m_zoomMin;
    m_zoom = zoom;
    ZoomChanged();

    // modify translation so the middle line does not move during zooming
    SetTranslation(previousTranslation + (previousTranslation+QOpenGLWidget::size().height()*0.5)*(GetZoom()-previousZoom)/previousZoom);

    //qDebug() << __FILE__ << " "<<__LINE__<<" "<<m_zoom<<" "<<m_translate << " "<<zoom << " "<<m_zoomMin << " "<<m_zoomMax;
    //QOpenGLWidget::update();
}

void GLWidget::SetDefaultZoom()
{
    // reset zooming factor to default value
    float targetZoom;
    //qDebug() <<__FILE__<<" "<<__LINE__<<" "<< size().height() << " "<< State::Inst().GetBorderSize().y <<" "<< m_visualTree->GetTreeHeight();
    if((QOpenGLWidget::size().height()-2*State::Inst().GetBorderSize().y) > m_visualTree->GetTreeHeight())
        targetZoom = (QOpenGLWidget::size().height()-2*State::Inst().GetBorderSize().y)/m_visualTree->GetTreeHeight();
    else
        targetZoom = m_zoomMin*State::Inst().GetZoomDefault();

    SetZoom(targetZoom);
}

void GLWidget::TranslateViewWheel(int dWheel)
{
    SetTranslation(GetTranslation() + State::Inst().GetScrollSensitivity()*dWheel);

    QOpenGLWidget::update();
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
        //qDebug() <<__FILE__<<":"<<__LINE__<<" "<<m_zoom <<" "<<m_zoomMin << " "<< m_zoomMax << " "<<size().height() << " "<< 2*State::Inst().GetBorderSize().y << " "<< m_visualTree->GetTreeHeight();
        if(m_zoomMax < (QOpenGLWidget::size().height()-2*State::Inst().GetBorderSize().y)/m_visualTree->GetTreeHeight())
        {
            // make sure the maximum zoom is large enough to allow
            // small graphs to fill the entire viewport
            m_zoomMax = (QOpenGLWidget::size().height()-2*State::Inst().GetBorderSize().y)/m_visualTree->GetTreeHeight();
        }

        // make sure zoom factor is within allowable range
        //SetZoom(m_zoom);

        //QOpenGLWidget::update();
    }
}

void GLWidget::TranslationExtents()
{
    if(!m_visualTree)
        return;

    // calculate extents of translation
    m_translateMin = 0;	// negative translations are not allowed

    m_translateMax = m_visualTree->GetTreeHeight()*GetZoom() + 2*State::Inst().GetBorderSize().y - QOpenGLWidget::size().height();
    if(m_translateMax < 0)
        m_translateMax = 0;

    emit treeSizeChanged(static_cast<int>(m_translateMax));
    // make sure translation factor is within allowable range
    SetTranslation(GetTranslation());

    //QOpenGLWidget::update();
}

void GLWidget::AdjustViewport()
{
    if(!m_visualTree)
        return;

    // adjust translation so mid-line of the viewport is unchanged
    SetTranslation(GetTranslation() + 0.5f*(m_previousSize.height() - QOpenGLWidget::size().height()));

    QOpenGLWidget::update();
}

void GLWidget::ZoomChanged()
{
    if(!m_visualTree)
        return;

    m_visualTree->CalculateTreeDimensions(QOpenGLWidget::size().width(), QOpenGLWidget::size().height(), GetZoom());
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

    float posY = node->GetPosition().y * m_visualTree->GetTreeHeight() * GetZoom() + State::Inst().GetBorderSize().y;
    float translatedY = posY-GetTranslation();

    if(translatedY < 0 || translatedY > QOpenGLWidget::size().height())
    {
        // Node is currently outside the viewport.
        emit TranslationChanged(posY-0.5*QOpenGLWidget::size().height());
        //SetTranslation(posY-0.5*QOpenGLWidget::size().height());
    }

    //update();
}

void GLWidget::TranslationFraction(float frac)
{
    SetTranslation(frac*(m_visualTree->GetTreeHeight()*GetZoom()
                                        + 2*State::Inst().GetBorderSize().y) + State::Inst().GetBorderSize().y);

    update();
}

float GLWidget::TranslationFraction()
{
    if(!m_visualTree)
        return 0.0f;

    //qDebug() << __FILE__<<__LINE__<<__PRETTY_FUNCTION__<<GetTranslation() << m_visualTree->GetTreeHeight() << GetZoom();
    float frac = (GetTranslation() - State::Inst().GetBorderSize().y)
                                    / (m_visualTree->GetTreeHeight()*GetZoom() + State::Inst().GetBorderSize().y);
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
