/* 
 * File:   SpectrWindow.h
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
private:
private slots:
    void setFileName(QString);
    void playMusic();
signals:
    void sendFileInfo(QString);
    void draw(double[],double[]);
    void blockPlay();
    void unlockPlay();
};

#endif	/* SPECTRWINDOW_H */

