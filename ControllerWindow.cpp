/* 
 * File:   ControllerWindow.cpp
 * Author: RCparty
 * 
 * Created on 21 Декабрь 2014 г., 12:02
 */

#include "ControllerWindow.h"

ControllerWindow::ControllerWindow(): QDialog()  {
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(code); 
    
    play = new QPushButton("Play");
    play->setDisabled(true);
    pause = new QPushButton("Stop");
    pause->setDisabled(true);
    lb = new QLabel();
    lb->setText(tr("Выберите файл для \nотображения информации."));
    QGridLayout *main = new QGridLayout(this);
    main->addWidget(lb,0,0);
    main->addWidget(play,1,0); 
    main->addWidget(pause,2,0);
    
}

void ControllerWindow::blockIt() {
    play->setDisabled(true);
    pause->setDisabled(false);
}

void ControllerWindow::unlockIt() {
    play->setDisabled(false);
    pause->setDisabled(true);
}

void ControllerWindow::setInfoText(QString _str)
{
    lb->setText(_str);
    play->setEnabled(true);
}