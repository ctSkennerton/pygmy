#include "glscrollwrapper.h"
#include "glwidget.h"
#include <QResizeEvent>
#include <QtDebug>
#include <QScrollBar>

GLScrollWrapper::GLScrollWrapper(QWidget * parent) : QAbstractScrollArea(parent)
{
}

GLScrollWrapper::~GLScrollWrapper()
{

}

QSize GLScrollWrapper::sizeHint() const
{
    return QSize(800, 600);
}

void GLScrollWrapper::resizeEvent(QResizeEvent * event)
{
    GLWidget * glw = dynamic_cast<GLWidget *>(viewport());

    // When we update the size of the window we also change the translation
    // and how much is visible. Therefore we must also pass these new values
    // along to the scrollbar to keep it in sync
    verticalScrollBar()->setPageStep(glw->VisibleHeight());
    verticalScrollBar()->setMaximum(glw->GetTranslationMax());

    glw->resizeEvent(event);
}

void GLScrollWrapper::paintEvent(QPaintEvent *event)
{
    GLWidget * glw = dynamic_cast<GLWidget *>(viewport());
    glw->paintEvent(event);
}

void GLScrollWrapper::canvasHeight(float height)
{
    GLWidget * glw = dynamic_cast<GLWidget *>(viewport());
    verticalScrollBar()->setPageStep(glw->VisibleHeight());
    verticalScrollBar()->setRange(0, height);
}

void GLScrollWrapper::mousePressEvent(QMouseEvent * event)
{
    GLWidget * glw = dynamic_cast<GLWidget *>(viewport());
    glw->mousePressEvent(event);
}
