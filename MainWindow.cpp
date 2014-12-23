/* 
 * File:   MainWindow.cpp
 */

#include "MainWindow.h"

MainWindow::MainWindow():QMainWindow() {
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    
    spect = new SpectrWindow;
    contr = new ControllerWindow;
    
    setWindowTitle(tr("Главное окно"));    
    this->move(0,0);
    //this->setWindowState(Qt::WindowFullScreen);
    
    QDockWidget *contentsWindow = new QDockWidget(tr("Входные данные"),this);
    this->addDockWidget(Qt::LeftDockWidgetArea, contentsWindow);
    contentsWindow->setWidget(contr);
    contentsWindow->setFixedHeight(250);
    contentsWindow->setFixedWidth(200);

    QDockWidget *contentswindows = new QDockWidget(tr("Спектрограмма"),this);
    this->addDockWidget(Qt::RightDockWidgetArea, contentswindows);
    contentswindows->setWidget(spect);
    //contentswindows->setFixedSize(600,550);
    contentswindows->setMinimumWidth(1350);
    contentswindows->setMinimumHeight(600);
    
    QMenu *MenuFile = menuBar()->addMenu(tr("Файл"));
    
    openFileAction = new QAction(tr("Открыть файл"),this);
    MenuFile->addAction(openFileAction); 
    
    exitAction = new QAction(tr("Выход"), this);
    MenuFile->addAction(exitAction);
    
    connect(openFileAction,SIGNAL(triggered()),this,SLOT(openfile()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(this,SIGNAL(fileOpened(QString)),spect,SLOT(setFileName(QString)));
    connect(spect,SIGNAL(sendFileInfo(QString)),contr,SLOT(setInfoText(QString)));
    connect(spect,SIGNAL(blockPlay()),contr,SLOT(blockIt()));
    connect(spect,SIGNAL(unlockPlay()),contr,SLOT(unlockIt()));
    connect(contr->play,SIGNAL(clicked()),spect,SLOT(playMusic()));
}

void MainWindow::openfile() {
    QString filename =  QFileDialog::getOpenFileName(this,
     tr("Открыть WAV-файл"), "", tr("WAV файлы (*.wav)"));
    emit fileOpened(filename);
    /*QMessageBox msg;
    msg.setText(name);
    msg.exec();*/
}



