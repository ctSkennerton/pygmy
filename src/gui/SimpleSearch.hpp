#ifndef SIMPLESEARCH_HPP
#define SIMPLESEARCH_HPP

#include "../core/DataTypes.hpp"
#include "../core/TextSearch.hpp"

#include <QWidget>

namespace Ui {
class SimpleSearch;
}

class SimpleSearch : public QWidget
{
    Q_OBJECT

signals:
    void SearchResultsChanged(void);

public:
    explicit SimpleSearch(QWidget *parent = 0);
    ~SimpleSearch();

    void SetTextSearch(pygmy::TextSearchPtr search)
    {
        m_textSearch = search;
    }

private slots:
    void on_findButton_clicked();

    void on_advancedButton_clicked();

private:
    Ui::SimpleSearch *ui;
    pygmy::TextSearchPtr m_textSearch;
};

#endif // SIMPLESEARCH_HPP
