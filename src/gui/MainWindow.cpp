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

#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QPushButton>

void MainWindow::createMenus()
{
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuFile = menuBar->addMenu(tr("&File"));
    QAction *openAct = new QAction(menuFile);
    openAct->setText(tr("Open File..."));
    menuFile->addAction(openAct);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    setMenuBar(menuBar);

    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    QAction *aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    menuBar->addMenu(helpMenu);
}


void MainWindow::createDocks()
{
    // Make a dock for the search dialog
    QDockWidget *searchDockWidget = new QDockWidget(tr("Search"), this);
    searchDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
                                      Qt::RightDockWidgetArea);
    //searchDockWidget->setWidget(dockWidgetContents);
    addDockWidget(Qt::RightDockWidgetArea, searchDockWidget);


    // Make a dock that will show the information of the currently selected record
    QDockWidget *recordDockWidget = new QDockWidget(tr("Current Record"), this);
    recordDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
                                      Qt::RightDockWidgetArea);
    //recordDockWidget->setWidget(dockWidgetContents);
    addDockWidget(Qt::RightDockWidgetArea, recordDockWidget);
}

MainWindow::MainWindow()
{
    createMenus();
    m_glTreeWidgetCanvas = new GLScrollWrapper(this);
    m_glTreeWidget = new GLWidget(this);
    m_glTreeWidgetCanvas->setViewport(m_glTreeWidget);
    m_glTreeWidgetOverview = new GLWidgetOverview(this);
    //m_glTreeWidget->setOverview(m_glTreeWidgetOverview);

    QWidget * window = new QWidget;
    QHBoxLayout *main_window_layout = new QHBoxLayout;
    main_window_layout->setContentsMargins(0,0,0,0);
    //QWidget * dummy_overview = new QPushButton("dummy");
    //main_window_layout->addWidget(dummy_overview);
    main_window_layout->addWidget(m_glTreeWidgetOverview);
    main_window_layout->addWidget(m_glTreeWidgetCanvas);

    window->setLayout(main_window_layout);
    setCentralWidget(window);

    //NOTE: For some reason this connection needs to be written this way, while all the other
    //connections below can be written the other way. I'm sure there is something about Qt that
    //I'm just not understanding
    connect(m_glTreeWidget, SIGNAL(ShouldUpdateOverview()), m_glTreeWidgetOverview, SLOT(update()));

    connect(m_glTreeWidget, &GLWidget::treeSizeChanged, m_glTreeWidgetCanvas, &GLScrollWrapper::canvasHeight);
    connect(m_glTreeWidgetCanvas->verticalScrollBar(), &QScrollBar::valueChanged, m_glTreeWidget, &GLWidget::translate);
    connect(m_glTreeWidget, &GLWidget::TranslationChanged, m_glTreeWidgetCanvas, &GLScrollWrapper::SetVerticalPosition);
    connect(m_glTreeWidget, &GLWidget::TranslationFractionChanged, m_glTreeWidgetOverview, &GLWidgetOverview::TranslationFraction);
    connect(m_glTreeWidget, &GLWidget::ViewportHeightFraction, m_glTreeWidgetOverview, &GLWidgetOverview::ViewportHeightFraction);
    connect(m_glTreeWidgetOverview, &GLWidgetOverview::newTranslationFraction, m_glTreeWidgetCanvas, &GLScrollWrapper::GoToViewportFraction);

    createDocks();

    readSettings();

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
    VisualTreePtr ptr = m_glTreeWidget->GetVisualTree();
    m_glTreeWidgetOverview->SetTree(ptr);
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
            tr("Copyright (c) 2009, Donovan Parks (donovan.parks@gmail.com) \n"
               "              2015, Donovan Parks, Connor Skennerton (c.skennerton@gmail.com) "
               ));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
}
