#include "treeoptions.hpp"
#include "ui_treeoptions.h"

TreeOptions::TreeOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeOptions)
{
    ui->setupUi(this);
}

TreeOptions::~TreeOptions()
{
    delete ui;
}
