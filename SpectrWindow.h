/* 
 * File:   SpectrWindow.h
 * Author: RCparty
 *
 * Created on 21 ������� 2014 �., 12:05
 */

#ifndef SPECTRWINDOW_H
#define	SPECTRWINDOW_H
#include <QtGui>

class SpectrWindow:public QWidget {
    Q_OBJECT
public:
    SpectrWindow(QWidget *parent = 0);
private:
private slots:
    void MyTimerProc();
};

#endif	/* SPECTRWINDOW_H */

