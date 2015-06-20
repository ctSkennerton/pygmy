#ifndef SIMPLESEARCH_HPP
#define SIMPLESEARCH_HPP

#include <QWidget>

namespace Ui {
class SimpleSearch;
}

class SimpleSearch : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleSearch(QWidget *parent = 0);
    ~SimpleSearch();
//    QSize sizeHint() const
//    {
//        return QSize(600,  75);
//    }
//    QSize minimumSizeHint() const
//    {
//        return QSize(300, 50);
//    }

private slots:
    void on_findButton_clicked();

    void on_advancedButton_clicked();

private:
    Ui::SimpleSearch *ui;
};

#endif // SIMPLESEARCH_HPP
