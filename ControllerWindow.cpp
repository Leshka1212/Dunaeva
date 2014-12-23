/* 
 * File:   ControllerWindow.cpp
 */

#include "ControllerWindow.h"

ControllerWindow::ControllerWindow(): QDialog()  {
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(code); 
    
    play = new QPushButton("Play");
    play->setDisabled(true);
    lb = new QLabel();
    lb->setText(tr("Выберите файл для \nотображения информации."));
    QGridLayout *main = new QGridLayout(this);
    main->addWidget(lb,0,0);
    main->addWidget(play,1,0);    
    
}

void ControllerWindow::blockIt() {
    play->setDisabled(true);
}

void ControllerWindow::unlockIt() {
    play->setDisabled(false);
}

void ControllerWindow::setInfoText(QString _str)
{
    lb->setText(_str);
    play->setEnabled(true);
}