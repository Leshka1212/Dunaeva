/* 
 * File:   SpectrWindow.cpp
 * Author: RCparty
 * 
 * Created on 21 Декабрь 2014 г., 12:05
 */

#include "SpectrWindow.h"
#include <Windows.h>

#include "riff.h"
#include "fft.h"
#include <ctime>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>


#define LEN 1024
#define MAXUint32 4294967295
#define NORM_SPEC 0.0039
#define STEP 2
#define SHIFT 512
#define THINNING 1
double xr[LEN], xi[LEN];
int Step=0, len, sps;
int I=0;

//int dt;



SpectrWindow::SpectrWindow(QWidget *parent):QWidget(parent) {
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(code);
    
    filename="";
    drawSignal=false;
    pixmap = QPixmap(1350,600);  
} 