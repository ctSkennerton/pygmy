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

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QSize>

#include "logo.h"

#include "core/DataTypes.hpp"
#include "core/VisualTree.hpp"
#include "glUtils/ErrorGL.hpp"

using namespace pygmy;

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    void setTree(utils::Tree<pygmy::NodePhylo>::Ptr tree);
    void TranslateView(int dx, int dy);
    void ScaleView(int dx, int dy);

    void SetZoom(float zoom);
    void SetTranslation(float translation);

    void SetDefaultZoom();

    void TranslateViewWheel(int dWheel);

    /** Indicate that the font size or style has been modified and that any values
            dependent on the font should be recalculated. */
    void ModifiedFont();

    /** Set min/max values for zooming. */
    void ZoomExtents();

    /** Set min/max values for translation. */
    void TranslationExtents();

    void AdjustViewport();

    void ZoomChanged();

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

    /** Called whenever translation changes. */
    void TranslationChanged() {}

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void cleanup();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

protected:
    /** Tree to be displayed in viewport. */
    VisualTreePtr m_visualTree;

    /** Colour map to be displayed in viewport. */
    VisualColourMapPtr m_visualColourMap;

    /** Overview associated with this viewport. */
    ViewportOverviewPtr m_overview;

private:
    void setupVertexAttribs();

    bool m_core;
    int m_xRot;
    int m_yRot;
    int m_zRot;
    QPoint m_lastPos;
    Logo m_logo;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_logoVbo;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_normalMatrixLoc;
    int m_lightPosLoc;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    bool m_transparent;

    /** Current zoom factor to apply to viewport. */
    float m_zoom;

    /** Current translation to apply to viewport. */
    float m_translate;

    /** Previous size of the widget*/
    QSize m_previousSize;

    /** Limit minimum zoom factor to a reasonable level. */
    float m_zoomMin;

    /** Limit maximum zoom factor to a reasonable level. */
    float m_zoomMax;

    /** Limit minimum translation to a reasonable level. */
    float m_translateMin;

    /** Limit maximum translation to a reasonable level. */
    float m_translateMax;
};

#endif