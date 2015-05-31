#ifndef GLWIDGETBASE_HPP
#define GLWIDGETBASE_HPP

#include <QObject>

class GlWidgetBase : public QOpenGlWindow
{
public:
    GlWidgetBase();
    ~GlWidgetBase();
};

#endif // GLWIDGETBASE_HPP
