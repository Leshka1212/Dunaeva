/* 
 * File:   SpectrWindow.cpp
 */

#include "SpectrWindow.h"
#include <Windows.h>
//#include <stdlib.h>

#include "riff.h"
#include "fft.h"

#define LEN 1024
#define MAXUint32 4294967295
#define NORM_SPEC 0.0039
//#define MAXUint32 100000
#define STEP 2
#define SDVIG 49 // 49
double xr[LEN], xi[LEN];
int Step=0;


SpectrWindow::SpectrWindow(QWidget *parent):QWidget(parent) {
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(code);
    
    filename="";
    drawSignal=false;
    pixmap = QPixmap(1350,600);
    
}

//---------------------------------------------------------------------------
//***************************************************************************
//** ФНЧ
//** in [] -> массив входных отчетов размером sizeIn **
//** out [] -> массив выходных отчетов, размер д.б. sizeIn - N **
//***************************************************************************
void Filter (int in[], double out[], int sizeIn)
{
	const int N = 51; //Разрядность фильтра
	double Fd = 44100; //Частота дискретизации входных данных
	double Fs = 2450; //Частота полосы пропускания
	double Fx = 2450; //Частота полосы затухания

	double H [N] = {0}; //Импульсная характеристика фильтра
	double H_id [N] = {0}; //Идеальная импульсная характеристика
	double W [N] = {0}; //Взвешивамое окно

	//Расчет импульсной хар-ки фильтра
	double Fc = (Fs + Fx) / (2 * Fd);
	for (int i=0; i < N; i++)
	{
		if (i==N/2) 
			H_id[i] = 2*M_PI*Fc;
		else 
			H_id[i] = ( sinl( 2*M_PI*Fc*(i-N/2) ) ) / (i-N/2);
		// весовая функция Блекмена
		W [i] = 0.42 - 0.5 * cosl((2*M_PI*i) / N) + 0.08 * cosl((4*M_PI*i) / N);
		H [i] = H_id[i] * W[i];
	}

	//Нормировка импульсной характеристики
	double SUM=0;
	for (int i=0; i<N; i++) 
		SUM = SUM + H[i];
	for (int i=0; i<N; i++) 
		H[i]= H[i] / SUM;

	//Фильтрация входных данных
	for (int i=0; i<sizeIn-N; i++)
	{
		out[i]=0.;
		for (int j=0; j<N; j++) 
			out[i]=out[i] + H[j]*in[(int)N + i-j];
	}
}

void SpectrWindow::playMusic() {
    if(filename!="")
        doMusic(filename);
}

void SpectrWindow::doMusic(QString filename) {
    riffwave_reader in(filename.toStdString().c_str());
        QString fileInfo="";
        fileInfo+=tr("Channels: ")+QString::number(in.channels())+tr("\n");
        fileInfo+=tr("Bitspersample: ")+QString::number(in.bitspersample())+tr("\n");
        
        int len = in.data_size();
	int sps = in.samplespersec();
        fileInfo+=tr("len: ")+QString::number(len)+tr("\n");
        fileInfo+=tr("sps: ")+QString::number(sps)+tr("\n");
        
        /*QMessageBox *mes = new QMessageBox();
        mes->setText(tr("len: ")+QString::number(len)+tr(" sps: ")+QString::number(sps));
        mes->exec();*/
        // читаем данные в массив
        int* data = static_cast<int*>(malloc(len * sizeof(int)));
	double* dataAfterFiltr = static_cast<double*>(malloc(len * sizeof(double)));
	double* signal = static_cast<double*>(malloc((len/5) * sizeof(double)));
	if( data == 0 ){
                QMessageBox *mes = new QMessageBox();
                mes->setText(tr("Can't allocate buffer!"));
                mes->exec();
		return;
	}
        for( int j = 0; j < len; j++ )
		data[j] = in(j, riffwave_reader::LEFT);
        int min = data[0];
	int max = data[0];
	for( int j = 0; j < len; j++ ){
	  if( data[j] < min )
	    min = data[j];
	  if( data[j] > max )
	    max = data[j];
	}
        fileInfo+=tr("Min: ")+QString::number(min)+tr("\nMax: ")+QString::number(max);
        emit sendFileInfo(fileInfo);
        
        //______________________________читаем даннные из wave_________________________
	// фильтруем сигнал
	Filter (data, dataAfterFiltr, len);

	// прореживаем сигнал в 9 раз
	for (int i=0,j=0;i<len;i+=5, j++)
		signal[j]=dataAfterFiltr[i];
	len/=5;
	int time=0;
        emit blockPlay();
        playMusic(filename);
        //______________________________делаем fft_____________________________________
	for (int i=0,Step=0;i<len-LEN;i+=SDVIG)
	{
		//double Ck;		
		for(int j = 0; j < LEN; j++ )
		{
			xr[j] = signal[i+j];
			xi[j] = 0.0;
		}
		// выполняем обратное БПФ
		cifft(xr, xi, LEN);		
                drawSignal=true;
                qApp->processEvents();  
                Sleep(50);
                //this->update(QRect(Step,0,1,this->height()));
                this->update();
                time++;
		//___________________рисуем спектр______________________
               
        }
        emit unlockPlay();
}

void SpectrWindow::paintEvent(QPaintEvent *) {
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);   
    //painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    //painter.setPen(QPen(Qt::gray,0,Qt::SolidLine));
    //painter.setBrush(QBrush(Qt::white));
    //painter.drawRect(0,0,this->width(),this->height());
    //painter.translate(0,this->height()); 
    //painter.scale(1,-1);
    
    if(drawSignal)
    {
        double Ck;
        int width = this->width();
        if(Step - ((int)(Step/width))*width==0)
        {
            painter.setBrush(QBrush(Qt::black));
            painter.drawRect(0,0,this->width(),this->height());
        }        
        for( int j = 0; j < LEN/2; j++ )
	{
            Ck=fabs(xr[j]+xi[j])*(double)(2.0/LEN);
                painter.setPen(QPen(QColor(255,0,0,(Ck > 255 ? 255 : Ck)),0,Qt::SolidLine));
            painter.drawPoint(Step - ((int)(Step/width))*width,j);
        }
        
        Step+=STEP;
    }
    QPainter p(this);
    p.drawPixmap(0, 0, pixmap);
}

void SpectrWindow::setFileName(QString _filename) {
    filename=_filename;
    if(filename!="")
    {
        doMusic(filename);
        /*QMessageBox *msg = new QMessageBox();
        msg->setText(QString::number(time));
        msg->exec();*/
    }
}

void SpectrWindow::playMusic(QString _path)
{
    QSound::play(_path);
}



