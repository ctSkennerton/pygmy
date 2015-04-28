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

#include "utils/Point.hpp"
#include "core/State.hpp"

using namespace utils;
using namespace pygmy;

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_xRot(0),
      m_yRot(0),
      m_zRot(0),
      m_program(0)
{
    m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"));
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
    m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
    if (m_transparent)
        setAttribute(Qt::WA_TranslucentBackground);
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(600, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void GLWidget::cleanup()
{
    makeCurrent();
    m_logoVbo.destroy();
    delete m_program;
    m_program = 0;
    doneCurrent();
}

static const char *vertexShaderSourceCore =
    "#version 150\n"
    "in vec4 vertex;\n"
    "in vec3 normal;\n"
    "out vec3 vert;\n"
    "out vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSourceCore =
    "#version 150\n"
    "in highp vec3 vert;\n"
    "in highp vec3 vertNormal;\n"
    "out highp vec4 fragColor;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   fragColor = vec4(col, 1.0);\n"
    "}\n";

static const char *vertexShaderSource =
    "attribute vec4 vertex;\n"
    "attribute vec3 normal;\n"
    "varying vec3 vert;\n"
    "varying vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying highp vec3 vert;\n"
    "varying highp vec3 vertNormal;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   gl_FragColor = vec4(col, 1.0);\n"
    "}\n";

void GLWidget::initializeGL()
{
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();



    // The below sets the openGL context to be current
    // according to Qt documentation this is not required
    // as it happens automatically under Qt API as long as
    // we are in {paint,resize,initialize}GL
    //m_contextGL->SetCurrent(*this);
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



/*


    glClearColor(0, 0, 0, m_transparent ? 0 : 1);

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_core ? vertexShaderSourceCore : vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_core ? fragmentShaderSourceCore : fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");

    // Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
    // implementations this is optional and support may not be present
    // at all. Nonetheless the below code works in all cases and makes
    // sure there is a VAO when one is needed.
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    // Setup our vertex buffer object.
    m_logoVbo.create();
    m_logoVbo.bind();
    m_logoVbo.allocate(m_logo.constData(), m_logo.count() * sizeof(GLfloat));

    // Store the vertex attribute bindings for the program.
    setupVertexAttribs();

    // Our camera never changes in this example.
    m_camera.setToIdentity();
    m_camera.translate(0, 0, -1);

    // Light position is fixed.
    m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));

    m_program->release();
    */
}

void GLWidget::setupVertexAttribs()
{
    m_logoVbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_logoVbo.release();
}

void GLWidget::paintGL()
{
    glUtils::ErrorGL::Check();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*if(m_visualColourMap)
        m_visualColourMap->Render(m_width, size().height());*/

    if(m_visualTree)
    {
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

    /*

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_world.setToIdentity();
    m_world.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
    QMatrix3x3 normalMatrix = m_world.normalMatrix();
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

    glDrawArrays(GL_TRIANGLES, 0, m_logo.vertexCount());

    m_program->release();
    */
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




/*
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
*/
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
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

    update();
}

void GLWidget::SetDefaultZoom()
{
    // reset zooming factor to default value
    float targetZoom;
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
