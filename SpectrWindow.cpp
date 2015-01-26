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

// оконная функция BLACKMAN_HARRIS
void windowfunc (int in[], double out[], int sizeIn)
{
        
        for (int i = 0; i < sizeIn; i++)
        {
            double a = 2.0 * M_PI * i / (sizeIn - 1);
            in [i] *= 0.35875 - 0.48829 * cosl(a) + 0.14128 * cosl(2.0 * a) - 0.01168 * cosl(3.0 * a);
            out[i]= in[i];
        } 
}

SpectrWindow::SpectrWindow(QWidget *parent):QWidget(parent) {
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(code);
    
    filename="";
    drawSignal=false;
    pixmap = QPixmap(1350,600);  
} 

void SpectrWindow::MyTimerProc()
{
    riffwave_reader in(filename.toStdString().c_str());
            
    int* data = static_cast<int*>(malloc(LEN*THINNING * sizeof(int)));
	int* dataR = static_cast<int*>(malloc(LEN * sizeof(int)));
    double* signal = static_cast<double*>(malloc((LEN) * sizeof(double)));
    
    for( int k = 0; k < LEN*THINNING; k++ )    
        data[k] = in(I+k, riffwave_reader::LEFT);

	// прореживаем сигнал 
    for(int l=0,k=0;l<LEN*THINNING;l+=THINNING, k++)
        dataR[k]=data[l];

	// вызов оконной функции
    windowfunc(dataR, signal, LEN);

    for(int j = 0; j < LEN; j++ )
    {
        xr[j] = signal[j];
        xi[j] = 0.0;
    }

    cifft(xr, xi, LEN);		
    drawSignal=true;  
    this->update();        
    
    I+=SHIFT*THINNING;
    
    if(I>=len-LEN*THINNING)
        stopMusic();
}

void SpectrWindow::playMusic() {
    if(filename!="")
        doMusic(filename);
}

void SpectrWindow::stopMusic() {
    drawSignal = false;
    player->stop();
    ptimer->stop();
    emit unlockPlay();
}

void SpectrWindow::doMusic(QString filename) {
    riffwave_reader in(filename.toStdString().c_str());
    
    len = in.data_size();
    sps = in.samplespersec();
    playMusic(filename);
    ptimer = new QTimer(this);
    connect(ptimer, SIGNAL(timeout()), SLOT(MyTimerProc()));
    ptimer->start((1000/(double)sps) * ((double)SHIFT) *((double)THINNING));
}

void SpectrWindow::paintEvent(QPaintEvent *) {
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);   

    if(drawSignal)
    {
        double Ck;
        COLORREF Color16;
        int width = this->width();
        if(Step - ((int)(Step/width))*width==0)
        {
            painter.setBrush(QBrush(Qt::black));
            painter.drawRect(0,0,this->width(),this->height());
        }        
        for( int j = 0; j < LEN/2; j++ )
	{
           
            Ck=fabs(xr[j]+xi[j])*(double)(4.0/LEN);
            Color16 = (COLORREF)Ck;
            painter.setPen(QPen(QColor::fromRgb(GetRValue(Color16), GetGValue(Color16), GetBValue(Color16),Ck > 255 ? 255 : Ck),0,Qt::SolidLine));
            painter.drawLine(Step - ((int)(Step/width))*width,j,Step - ((int)(Step/width))*width,j+1);
        }
        emit painted();
        Step+=STEP;
    }
    QPainter p(this);   
    p.drawPixmap(0, 0, pixmap);
    p.setBrush(QBrush(Qt::white));
    p.setPen(QPen(Qt::white,1,Qt::SolidLine));
    p.drawText(675,570,tr("Время"));
    p.drawText(10,300,tr("Ч"));
    p.drawText(10,310,tr("а"));
    p.drawText(10,320,tr("с"));
    p.drawText(10,330,tr("т"));
    p.drawText(10,340,tr("о"));
    p.drawText(10,350,tr("т"));
    p.drawText(10,360,tr("а"));
}