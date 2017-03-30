/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "MainWindow.hpp"
#include "../core/NewickIO.hpp"
#include "../core/NodePhylo.hpp"
#include "../utils/Tree.hpp"
#include "../core/VisualTree.hpp"
#include "../core/State.hpp"
#include "../core/MetadataIO.hpp"


#include <QMenuBar>
#include <QToolBar>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QDockWidget>
#include <QGridLayout>
#include <QScrollBar>
#include <QPushButton>
#include <QKeySequence>

void MainWindow::createMenus()
{
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuFile = menuBar->addMenu(tr("&File"));
    QMenu *menuEdit = menuBar->addMenu(tr("&Edit"));
    QMenu *menuTree = menuBar->addMenu(tr("&Tree"));
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    QToolBar * treeToolBar = addToolBar(tr("Tree"));

    //menuFile actions
    QAction *openAct = new QAction(menuFile);
    openAct->setText(tr("Open File..."));
    openAct->setShortcuts(QKeySequence::Open);
    menuFile->addAction(openAct);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    QAction *openAnnotationsAct = new QAction(menuFile);
    openAnnotationsAct->setText(tr("Open Annotation File..."));
    menuFile->addAction(openAnnotationsAct);
    connect(openAnnotationsAct, SIGNAL(triggered()), this, SLOT(openAnnotationsFile()));

    //menuEdit actions
    QAction * findAct = new QAction(tr("&Find"), menuEdit);
    findAct->setShortcuts(QKeySequence::Find);
    menuEdit->addAction(findAct);
    connect(findAct, SIGNAL(triggered()), m_simpleSearch, SLOT(show()));

    //menuTree actions
    QAction * sortAscendingAct = new QAction(tr("&Sort Ascending"), menuTree);
    sortAscendingAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
    menuTree->addAction(sortAscendingAct);
    treeToolBar->addAction(sortAscendingAct);
    connect(sortAscendingAct, SIGNAL(triggered()), m_glTreeWidget, SLOT(sortSubtreesAscending()));

    QAction * sortDescendingAct = new QAction(tr("&Sort Descending"), menuTree);
    sortDescendingAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
    menuTree->addAction(sortDescendingAct);
    treeToolBar->addAction(sortDescendingAct);
    connect(sortDescendingAct, SIGNAL(triggered()), m_glTreeWidget, SLOT(sortSubtreesDescending()));

    QAction * midpointRootAct = new QAction(tr("&Midpoint Root"), menuTree);
    midpointRootAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    menuTree->addAction(midpointRootAct);
    treeToolBar->addAction(midpointRootAct);
    connect(midpointRootAct, SIGNAL(triggered()), m_glTreeWidget, SLOT(midpointRoot()));

    const QIcon phylogramBranchesIcon = QIcon("://resources/images/phylogram.xpm");
    QAction * phylogramBranchesAct = new QAction(phylogramBranchesIcon, tr("&Phylogram"), menuTree);
    menuTree->addAction(phylogramBranchesAct);
    treeToolBar->addAction(phylogramBranchesAct);
    connect(phylogramBranchesAct, SIGNAL(triggered()), m_glTreeWidget, SLOT(setPhylogramBranchStyle()));

    const QIcon cladogramBranchesIcon = QIcon("://resources/images/cladogram_rect.xpm");
    QAction * cladogramBranchesAct = new QAction(cladogramBranchesIcon, tr("&Cladogram"), menuTree);
    menuTree->addAction(cladogramBranchesAct);
    treeToolBar->addAction(cladogramBranchesAct);
    connect(cladogramBranchesAct, SIGNAL(triggered()), m_glTreeWidget, SLOT(setCladogramBranchStyle()));

    //menuHelp actions
    QAction *aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    menuBar->addMenu(helpMenu);

    setMenuBar(menuBar);

}


void MainWindow::createDocks()
{

    // Make a dock that will show the information of the currently selected record
    QDockWidget *recordDockWidget = new QDockWidget(tr("Current Record"), this);
    recordDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
                                      Qt::RightDockWidgetArea);
    //recordDockWidget->setWidget(dockWidgetContents);
    addDockWidget(Qt::RightDockWidgetArea, recordDockWidget);

    // Make a dock that will show the information of the currently selected record
    QDockWidget *treeSettingsDockWidget = new QDockWidget(tr("Options"), this);
    treeSettingsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
                                      Qt::RightDockWidgetArea);
    treeSettingsDockWidget->setWidget(m_treeOptions);
    addDockWidget(Qt::LeftDockWidgetArea, treeSettingsDockWidget);
}

MainWindow::MainWindow() : m_textSearch(new TextSearch)
{
    QWidget * window = new QWidget(this);
    QGridLayout *main_window_layout = new QGridLayout(window);
    m_glTreeWidgetCanvas = new GLScrollWrapper(window);
    m_glTreeWidget = new GLWidget(window);
    m_glTreeWidgetCanvas->setViewport(m_glTreeWidget);
    m_glTreeWidgetOverview = new GLWidgetOverview(window);
    m_simpleSearch = new SimpleSearch(window);
    m_treeOptions = new TreeOptions(window);
    // hide the find widget untile the user wants to show it by
    // clicking find from the menu
    m_simpleSearch->hide();

    m_textSearch->DataFilter()->SetColour(utils::Colour(0.8f, 0.9f, 0.9f, 1.0f));

    createMenus();


    main_window_layout->setContentsMargins(0,0,0,0);
    main_window_layout->addWidget(m_glTreeWidgetOverview, 0, 0, 2, 1);
    main_window_layout->addWidget(m_glTreeWidgetCanvas, 0, 1);
    main_window_layout->addWidget(m_simpleSearch, 1, 1);

    window->setLayout(main_window_layout);
    setCentralWidget(window);

    //NOTE: For some reason this connection needs to be written this way, while all the other
    //connections below can be written the other way. I'm sure there is something about Qt that
    //I'm just not understanding
    connect(m_glTreeWidget, SIGNAL(ShouldUpdateOverview()), m_glTreeWidgetOverview, SLOT(update()));

    connect(m_simpleSearch, SIGNAL(SearchResultsChanged()), m_glTreeWidget, SLOT(update()));
    connect(m_simpleSearch, SIGNAL(SearchResultsChanged()), m_glTreeWidgetOverview, SLOT(RedrawTextSearch()));

    connect(m_glTreeWidget, &GLWidget::treeSizeChanged, m_glTreeWidgetCanvas, &GLScrollWrapper::canvasHeight);
    connect(m_glTreeWidgetCanvas->verticalScrollBar(), &QScrollBar::valueChanged, m_glTreeWidget, &GLWidget::translate);
    connect(m_glTreeWidget, &GLWidget::TranslationChanged, m_glTreeWidgetCanvas, &GLScrollWrapper::SetVerticalPosition);
    connect(m_glTreeWidget, &GLWidget::LargestLabelHeight, m_glTreeWidgetCanvas, &GLScrollWrapper::VerticalSingleStepSize);

    connect(m_glTreeWidget, &GLWidget::TranslationFractionChanged, m_glTreeWidgetOverview, &GLWidgetOverview::TranslationFraction);
    connect(m_glTreeWidget, &GLWidget::ViewportHeightFraction, m_glTreeWidgetOverview, &GLWidgetOverview::ViewportHeightFraction);
    connect(m_glTreeWidget, &GLWidget::ShouldRedrawOverviewTree, m_glTreeWidgetOverview, &GLWidgetOverview::Redraw);
    connect(m_glTreeWidgetOverview, &GLWidgetOverview::newTranslationFraction, m_glTreeWidgetCanvas, &GLScrollWrapper::GoToViewportFraction);

    connect(m_treeOptions, SIGNAL(TreeOptionsChanged()), m_glTreeWidget, SLOT(update()));
    connect(m_treeOptions, &TreeOptions::leafFontChanged, m_glTreeWidget, &GLWidget::ModifiedFont);
    connect(m_treeOptions, SIGNAL(leafLabelsChanged()), m_glTreeWidget, SLOT(updateLeafWidths()));
    connect(m_treeOptions, &TreeOptions::leafLabelsChanged, this, &MainWindow::updateSearchFields);


    createDocks();

    readSettings();

}

void MainWindow::updateSearchFields()
{
    utils::Tree<NodePhylo>::Ptr tree = m_glTreeWidget->GetVisualTree()->GetTree();
    // set up the text search object
    std::vector<NodePhylo *> leaf_nodes = tree->GetLeaves();
    m_textSearch->Clear();
    for(NodePhylo * leaf : leaf_nodes)
    {
        m_textSearch->Add(leaf->GetLabel(), leaf->GetId());
    }
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Tree file"),
                                            (State::Inst().GetPreviousDirectory().isEmpty()) ? QDir::homePath() : State::Inst().GetPreviousDirectory(),
                                            tr("Tree Files (*.tree *.tre);;All Files (*)")
                                            );
    // The user did not choose a file - clicked cancel
    if(fileName.isNull())
    {
        return;
    }
    QFileInfo file_info(fileName);
    State::Inst().SetPreviousDirectory(file_info.absoluteDir().absolutePath());
    pygmy::NewickIO newickIO;
    utils::Tree<pygmy::NodePhylo>::Ptr tree(new utils::Tree<pygmy::NodePhylo>());
    bool readOk = newickIO.Read(tree, fileName);
    if(! readOk || tree->GetNumberOfLeaves() == 0)
    {
        QMessageBox::critical(this, tr("Pygmy: Error"), tr("Failed to read Newick file, or the file was empty"));
        return;
    }
    setWindowTitle(tree->GetName());

    m_glTreeWidget->setTree(tree);
    m_textSearch->Clear();
    m_glTreeWidget->SetSearchFilter(m_textSearch->DataFilter());
    VisualTreePtr ptr = m_glTreeWidget->GetVisualTree();
    m_glTreeWidgetOverview->SetTree(ptr);
    m_glTreeWidgetOverview->SetSearchFilter(m_textSearch->DataFilter());

    m_simpleSearch->SetTextSearch(m_textSearch);
    updateSearchFields();
    m_treeOptions->setupFromState();
}

void MainWindow::openAnnotationsFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Annotations file"),
                                            (State::Inst().GetPreviousDirectory().isEmpty()) ? QDir::homePath() : State::Inst().GetPreviousDirectory(),
                                            tr("Tree Files (*.tsv *.txt);;All Files (*)")
                                            );
    // The user did not choose a file - clicked cancel
    if(fileName.isNull())
    {
        return;
    }
    m_metadataInfo.reset( new MetadataInfo());
    VisualTreePtr treePtr = m_glTreeWidget->GetVisualTree();
    if(pygmy::MetadataIO::Read(fileName, treePtr, m_metadataInfo)) {
        treePtr->SetMetadataInfo(m_metadataInfo);
    }

    QStringList metadata_fields = m_metadataInfo->GetFields();
    m_treeOptions->loadMetadataKeys(metadata_fields);
    m_glTreeWidget->update();
}

void MainWindow::writeSettings()
{
    State::Inst().Save();
}

void MainWindow::readSettings()
{
   State::Inst().Load();
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("Pygmy"),
            tr("Copyright (c) 2009, Donovan Parks     (donovan.parks@gmail.com) \n"
               "              2015 - 2017, Connor Skennerton (c.skennerton@gmail.com) "
               ));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
}
