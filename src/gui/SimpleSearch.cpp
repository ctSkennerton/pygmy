#include "SimpleSearch.hpp"
#include "ui_SimpleSearch.h"

SimpleSearch::SimpleSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpleSearch)
{
    ui->setupUi(this);
}

SimpleSearch::~SimpleSearch()
{
    delete ui;
}

void SimpleSearch::on_findButton_clicked()
{
    QString searchString = ui->lineEdit->text();
    if (!searchString.isEmpty()) {
        m_textSearch->FilterData(searchString, ui->regularExpressionCheckBox->isChecked(), ui->caseInsensitiveCheckBox->isChecked() );
        // check for the modifiers for insensitive and regular expression
        emit SearchResultsChanged();
    }
}

void SimpleSearch::on_advancedButton_clicked()
{
    // open the Advanced search dialog
}
