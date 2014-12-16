#define _USE_MATH_DEFINES
#include<stdio.h>
#include<windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <time.h>

#include "riff.h"
#include "fft.h"

#define LEN 1024
#define MAXUint32 4294967295
#define NORM_SPEC 0.0039
//#define MAXUint32 100000
#define STEP 2
#define SDVIG 49
#define COLOR 16777215
//#define _USE_MATH_DEFINES

using namespace std;

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

void main()
{
	//______________________________читаем даннные из wave_________________________
	const char* SongWAV="G:\\dip_test\\11_3.wav";
	//printf("Vvedite put' k failu dlia obrabotki (ex. C:\\muz\\pesnia.mp3):");
	//getline(cin,SongWAV);
	system("mode con cols=250 lines=80");

	riffwave_reader in(SongWAV);
	
	// печатаем общую информацию о файле
	printf("channels=%d\n", in.channels());
	printf("bitspersample=%d\n", in.bitspersample());

	int len = in.data_size();
	int sps = in.samplespersec();
	printf("len=%d\n", len);
	printf("sps=%d\n", sps);

	// читаем данные в массив
	int* data = static_cast<int*>(malloc(len * sizeof(int)));
	double* dataAfterFiltr = static_cast<double*>(malloc(len * sizeof(double)));
	double* signal = static_cast<double*>(malloc((len/9) * sizeof(double)));
	if( data == 0 ){
		printf("can't allocate buffer\n");
		exit(1);
	}    
	for( int j = 0; j < len; j++ )
		data[j] = in(j, riffwave_reader::LEFT);
 
	// находим диапазон изменения данных
	int min = data[0];
	int max = data[0];
	for( int j = 0; j < len; j++ ){
	  if( data[j] < min )
	    min = data[j];
	  if( data[j] > max )
	    max = data[j];
	}
	printf("min=%d max=%d\n", min, max);

	//______________________________читаем даннные из wave_________________________
	// фильтруем сигнал
	Filter (data, dataAfterFiltr, len);

	// прореживаем сигнал в 9 раз
	for (int i=0,j=0;i<len;i+=9, j++)
		signal[j]=dataAfterFiltr[i];
	len/=9;
	int time=0;
	//______________________________делаем fft_____________________________________
	for (int i=0,Step=0;i<len-LEN;i+=SDVIG,Step+=STEP)
	{
		int ColorRed;
		int Color;
		double Ck;
		double xr[LEN], xi[LEN];
		for(int j = 0; j < LEN; j++ )
		{
			xr[j] = signal[i+j];
			xi[j] = 0.0;
		}

		// выполняем обратное БПФ
		cifft(xr, xi, LEN);
		time++;
		//___________________рисуем спектр______________________
		for( int j = 0; j < LEN/2; j++ )
		{
			HDC hDC;
			HWND hWnd;
			RECT Rect;
			DWORD mode;
			HANDLE hInput;
			HPEN hPen, hOldPen;
			HBRUSH hBrush, hOldBrush;
			hWnd=GetConsoleWindow();
			COLORREF Color16;
			char buf[10];
			unsigned long MaxUint32, MaxColor;
			MaxUint32=MAXUint32;
			MaxColor=COLOR;
			if(!hWnd)
			{
				printf("Can't get hWnd of console!\n");
				return;
			}
			hDC=GetDC(hWnd);
			if(!hDC)
			{
				printf("Can't get device context!\n");
				return;
			}

			Ck=fabs(xr[j]+xi[j])*(double)(2.0/LEN);
		
			Color=Ck;

			Color16=(COLORREF)Color;

			hPen=CreatePen(PS_SOLID,1,Color16);
			hOldPen=(HPEN)SelectObject(hDC,hPen);

			hInput=GetStdHandle(STD_INPUT_HANDLE);
			GetClientRect(hWnd,&Rect);
 
			MoveToEx(hDC,Rect.left+Step,Rect.top+j,0);
			LineTo(hDC,Rect.left+Step+STEP,Rect.top+j);
 
			GetConsoleMode(hInput,&mode);
			SetConsoleMode(hInput,mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
 
			SelectObject(hDC,hOldPen);
 
			DeleteObject(hPen);
			ReleaseDC(hWnd, hDC);

			//printf("%f \n",Ck);			

		}
		//___________________рисуем спектр______________________
		
	}
	printf("%d",&time);
	//______________________________делаем fft_____________________________________	
}