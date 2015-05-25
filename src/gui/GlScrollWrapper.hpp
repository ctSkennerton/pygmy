#ifndef GLSCROLLWRAPPER_H
#define GLSCROLLWRAPPER_H

#include <QWidget>
#include <QAbstractScrollArea>


/** Implements a canvas that enables a GLWidget to be scrolled*/
class GLScrollWrapper : public QAbstractScrollArea
{
public:
    GLScrollWrapper(QWidget *parent);
    ~GLScrollWrapper();
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void update();

public slots:
    void canvasHeight(float height);

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private:
};

#endif // GLSCROLLWRAPPER_H
