#ifndef TREEOPTIONS_HPP
#define TREEOPTIONS_HPP

#include <QWidget>

namespace Ui {
class TreeOptions;
}

class TreeOptions : public QWidget
{
    Q_OBJECT

public:
    explicit TreeOptions(QWidget *parent = 0);
    ~TreeOptions();

private:
    Ui::TreeOptions *ui;
};

#endif // TREEOPTIONS_HPP
