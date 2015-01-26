/* 
 * File:   MainWindow.h
 * Author: RCparty
 *
 * Created on 21 ������� 2014 �., 12:00
 */

#ifndef MAINWINDOW_H
#define	MAINWINDOW_H
#include "ControllerWindow.h"
#include "SpectrWindow.h"
#include <QtGui>

class MainWindow: public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
private:
    ControllerWindow *contr;
    SpectrWindow *spect;
    QAction *exitAction;
    QAction *openFileAction;
private slots:
    void openfile();
signals:
    void fileOpened(QString);
};

#endif	/* MAINWINDOW_H */