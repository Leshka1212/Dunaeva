/* 
 * File:   ControllerWindow.h
 */

#ifndef CONTROLLERWINDOW_H
#define	CONTROLLERWINDOW_H

#include <QtGui>

class ControllerWindow: public QDialog {
    Q_OBJECT
public:
    ControllerWindow();
    QPushButton *play; 
    QLabel *lb;
private:
public slots:
        void setInfoText(QString);
        void blockIt();
        void unlockIt();
};

#endif	/* CONTROLLERWINDOW_H */

