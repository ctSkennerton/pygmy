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
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

    void update();

public slots:
    void canvasHeight(float height);
    /** Sets the position of the vertical scrollbar*/
    void SetVerticalPosition(int position);
    void GoToViewportFraction(float fraction);
    void VerticalSingleStepSize(int step);

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private:
};

#endif // GLSCROLLWRAPPER_H
