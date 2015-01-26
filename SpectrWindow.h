/* 
 * File:   SpectrWindow.h
 * Author: RCparty
 *
 * Created on 21 Декабрь 2014 г., 12:05
 */

#ifndef SPECTRWINDOW_H
#define	SPECTRWINDOW_H
#include <QtGui>

class SpectrWindow:public QWidget {
    Q_OBJECT
public:
    SpectrWindow(QWidget *parent = 0);
    void paintEvent(QPaintEvent* );
    void playMusic(QString);
    void doMusic(QString);
    QString filename;
    QPixmap pixmap; 
    bool drawSignal;
    QSound *player;
    QTimer *ptimer;
private:
private slots:
    void setFileName(QString);
    void playMusic();
    void stopMusic();
    void MyTimerProc();
signals:
    void sendFileInfo(QString);
    void draw(double[],double[]);
    void blockPlay();
    void unlockPlay();
    void painted();
};

#endif	/* SPECTRWINDOW_H */

