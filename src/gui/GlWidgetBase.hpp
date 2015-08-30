#ifndef GLWIDGETBASE_HPP
#define GLWIDGETBASE_HPP

#include <QObject>
#include <QOpenGLWidget>
#include <QSize>

#include "../core/DataTypes.hpp"
#include "../core/VisualTree.hpp"
#include "../glUtils/ErrorGL.hpp"

class GLWidgetBase : public QOpenGLWidget
{
    Q_OBJECT
public:
    GLWidgetBase(QWidget * parent);
    ~GLWidgetBase();

    virtual float GetZoom() { return m_zoom; }

    virtual float GetTranslation() const { return m_translate; }

    void SetTranslation(float translation);

    /**
     * @brief Filter node labels based on a text search.
     * @param filter Filter object containing all items filtered by the search.
     */
    virtual void SetSearchFilter(pygmy::FilterPtr filter)
    {
        m_searchFilter = filter;
        m_visualTree->SetSearchFilter(filter);
    }



protected:
    virtual void ZoomChanged() {}
    /** Set min/max values for zooming. */
    virtual void ZoomExtents() = 0;

    /** Set min/max values for translation. */
    virtual void TranslationExtents() = 0;

    float GetTranslationMax() {return m_translateMax;}

    // the height (in pixels) that is currently visible in the viewport
    // m_translate * m_zoom
    float VisibleHeight() {return m_zoom * size().height();}

    /** Limit minimum zoom factor to a reasonable level. */
    float m_zoomMin;

    /** Limit maximum zoom factor to a reasonable level. */
    float m_zoomMax;

    /** Limit minimum translation to a reasonable level. */
    float m_translateMin;

    /** Limit maximum translation to a reasonable level. */
    float m_translateMax;

    float m_zoom;

    float m_translate;

    /** Previous size of the widget*/
    QSize m_previousSize;

    pygmy::FilterPtr m_searchFilter;

    /** Tree to be displayed in viewport. */
    pygmy::VisualTreePtr m_visualTree;
};

#endif // GLWIDGETBASE_HPP
