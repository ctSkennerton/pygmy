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

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "GlWidgetBase.hpp"
#include "GlWidgetOverview.hpp"

using namespace pygmy;

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLScrollWrapper;

class GLWidget : public GLWidgetBase
{
    Q_OBJECT

signals:
    void treeSizeChanged(float treeHeight);

    // emits the current position of the view
    void TranslationChanged(int position);

    void TranslationFractionChanged(float frac);
    void ViewportHeightFraction(float frac);
    void ShouldUpdateOverview();


public slots:
    void setTree(utils::Tree<pygmy::NodePhylo>::Ptr tree);

    void translate(int position);

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    void TranslateView(int dx, int dy);
    void ScaleView(int dx, int dy);

    void SetZoom(float zoom);
    void SetTranslation(float translation);

    void SetDefaultZoom();

    void TranslateViewWheel(int dWheel);

    VisualTreePtr GetVisualTree() {return m_visualTree;}

    /** Indicate that the font size or style has been modified and that any values
            dependent on the font should be recalculated. */
    void ModifiedFont();

    void AdjustViewport();

    /**
     * @brief Ensure specified node is within the viewport. If it is
     *				currently outside the viewport, adjust the viewport so
     *				it is centered of the Y position of this node.
     * @param id Id of node of interest.
     */
    void CenterNode(uint id);

    void TranslationFraction(float frac);

    float TranslationFraction();

    void SetBranchStyle(VisualTree::BRANCH_STYLE branchStyle);
    void SetColourMap(VisualColourMapPtr visualColourMap);

    void setOverview(GLWidgetOverview * overview)
    {
        m_overview.reset(overview);
    }

protected:
    /** Sets up the OpenGL resources and state.
      * Gets called once before the first time resizeGL() or paintGL() is called.
      */
    void initializeGL() Q_DECL_OVERRIDE;

    /** Renders the OpenGL scene. Gets called whenever the widget needs to be updated.
      */
    void paintGL() Q_DECL_OVERRIDE;

    /** Sets up the OpenGL viewport, projection, etc.
      * Gets called whenever the widget has been resized
      * (and also when it is shown for the first time because
      * all newly created widgets get a resize event automatically).
      */
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void ShowContextMenu(const QPoint& pos);

    void ZoomChanged();
    void ZoomExtents();
    void TranslationExtents();

protected:
    /** Tree to be displayed in viewport. */
    VisualTreePtr m_visualTree;

    /** Colour map to be displayed in viewport. */
    VisualColourMapPtr m_visualColourMap;

    /** Overview associated with this viewport. */
    ViewportOverviewPtr m_overview;

    /** previous position of the mouse. */
    QPoint m_lastMousePos;

private:

    bool m_core;

    QPoint m_lastPos;
    bool m_transparent;


    friend class GLScrollWrapper;
};

#endif
