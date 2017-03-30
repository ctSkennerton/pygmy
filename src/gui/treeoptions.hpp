#ifndef TREEOPTIONS_HPP
#define TREEOPTIONS_HPP

#include <QStringList>
#include <QWidget>
#include "../core/State.hpp"


namespace Ui {
class TreeOptions;
}

class TreeOptions : public QWidget
{
    Q_OBJECT

signals:
    void TreeOptionsChanged(void);
    void leafLabelsChanged(void);
    void leafFontSizeChanged(int);
    void leafFontChanged(void);

public:
    explicit TreeOptions(QWidget *parent = 0);
    ~TreeOptions();
    void setupFromState();
    void loadMetadataKeys(QStringList& labels);

private slots:
    void on_ShowInternalLabel_stateChanged(int);
    void on_ShowMetadataLeafLabel_stateChanged(int);
    void on_ShowLeafName_stateChanged(int);

    void on_InternalLabelFontSize_valueChanged(int value);
    void on_LeafLabelFontSize_valueChanged(int value);


private:
    Ui::TreeOptions *ui;
};

#endif // TREEOPTIONS_HPP
