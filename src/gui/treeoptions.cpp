#include "treeoptions.hpp"
#include "ui_treeoptions.h"
#include "GlScrollWrapper.hpp"

#include <QStringList>

TreeOptions::TreeOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeOptions)
{
    ui->setupUi(this);
    QStringList internal_labels;
    internal_labels.append("Name");
    internal_labels.append("Bootstrap");
    internal_labels.append("Number of Leaves");


    ui->InternalMetadataField->addItems(internal_labels);

    connect(ui->InternalMetadataField,
            static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            [=](const QString &text){ pygmy::State::Inst().SetInternalNodeField(text); emit TreeOptionsChanged(); });

    connect(ui->MetadataField,
            static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            [=](const QString &text){ pygmy::State::Inst().SetMetadataField(text); emit leafLabelsChanged(); emit TreeOptionsChanged(); });
}

void TreeOptions::setupFromState()
{
    ui->InternalMetadataField->setCurrentText(pygmy::State::Inst().GetInternalNodeField());
    ui->InternalLabelFontSize->setValue(pygmy::State::Inst().GetInternalNodeFontSize());
    ui->MetadataField->setCurrentText(pygmy::State::Inst().GetInternalNodeField());
    ui->LeafLabelFontSize->setValue(pygmy::State::Inst().GetTreeFontSize());
    ui->ShowInternalLabel->setChecked(pygmy::State::Inst().GetShowInternalLabels());
    ui->ShowLeafName->setChecked(pygmy::State::Inst().GetShowLeafLabels());
    ui->ShowMetadataLeafLabel->setChecked(pygmy::State::Inst().GetShowMetadataLabels());
}

TreeOptions::~TreeOptions()
{
    delete ui;
}

void TreeOptions::loadMetadataKeys(QStringList &labels)
{
    ui->MetadataField->clear();
    ui->MetadataField->addItems(labels);
}

void TreeOptions::on_ShowInternalLabel_stateChanged(int state)
{
    pygmy::State::Inst().SetShowInternalLabels(state);
    emit TreeOptionsChanged();
}

void TreeOptions::on_ShowMetadataLeafLabel_stateChanged(int state)
{
    pygmy::State::Inst().SetShowMetadataLabels(state);
    emit leafLabelsChanged();
    emit TreeOptionsChanged();
}

void TreeOptions::on_ShowLeafName_stateChanged(int state)
{
    pygmy::State::Inst().SetShowLeafLabels(state);
    emit leafLabelsChanged();
    emit TreeOptionsChanged();
}

void TreeOptions::on_InternalLabelFontSize_valueChanged(int value)
{
    pygmy::State::Inst().SetInternalNodeFontSize(value);
    emit TreeOptionsChanged();
}

void TreeOptions::on_LeafLabelFontSize_valueChanged(int value)
{
    pygmy::State::Inst().SetTreeFontSize(value);
    emit leafFontChanged();
    //emit TreeOptionsChanged();
}
