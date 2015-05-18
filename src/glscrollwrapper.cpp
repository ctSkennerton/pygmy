#include "glscrollwrapper.h"
#include "glwidget.h"
#include <QResizeEvent>
#include <QtDebug>
#include <QScrollBar>

GLScrollWrapper::GLScrollWrapper(QWidget * parent) : QAbstractScrollArea(parent)
{

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);



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
    qDebug() << "resizing"<<event->size().width()<<" "<<event->size().height()<<" "<<event->oldSize().width()<<" "<<event->oldSize().height();
    //QAbstractScrollArea::resizeEvent(event);
    GLWidget * glw = dynamic_cast<GLWidget *>(viewport());
    glw->resizeEvent(event);
    //glWidget->resizeGL(event->size().width(), event->size().height());
}

void GLScrollWrapper::paintEvent(QPaintEvent *event)
{


    //QAbstractScrollArea::paintEvent(event);
    //glWidget->updateGL();
    GLWidget * glw = dynamic_cast<GLWidget *>(viewport());

    glw->paintEvent(event);
}

void GLScrollWrapper::canvasHeight(float height)
{
    //qDebug() << __FILE__ <<" "<< __LINE__ << " "<<__PRETTY_FUNCTION__<< " "<<height;
    verticalScrollBar()->setRange(0, height);
    //qDebug() << __FILE__ <<" "<< __LINE__ << " "<<__PRETTY_FUNCTION__<< " "<<verticalScrollBar()->maximum();

}
