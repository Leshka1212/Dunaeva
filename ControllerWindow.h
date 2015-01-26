/* 
 * File:   ControllerWindow.h
 * Author: RCparty
 *
 * Created on 21 Декабрь 2014 г., 12:02
 */

#ifndef CONTROLLERWINDOW_H
#define	CONTROLLERWINDOW_H

#include <QtGui>

class ControllerWindow: public QDialog {
    Q_OBJECT
public:
    ControllerWindow();
    QPushButton *play, *pause; 
    QLabel *lb;
private:
public slots:
        void setInfoText(QString);
        void blockIt();
        void unlockIt();
};

#endif	/* CONTROLLERWINDOW_H */

