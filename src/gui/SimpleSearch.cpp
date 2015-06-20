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
    // check for the modifiers for insensitive and regular expression
    if (ui->regularExpressionCheckBox->isChecked())
    {

    }
    if (ui->caseInsensitiveCheckBox->isChecked())
    {

    }
}

void SimpleSearch::on_advancedButton_clicked()
{
    // open the Advanced search dialog
}
