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
}
