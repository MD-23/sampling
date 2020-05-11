#define _USE_MATH_DEFINES

#include "Sample.h"
#include <conio.h>

#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


using namespace std;

Sample::Sample(string file_path, int octave_) {

	//CARGA DEL ARCHIVO
	audioFile.load(file_path + ".wav");

	//CONSTANTES
	lenght_sample = audioFile.getNumSamplesPerChannel();
	fs = audioFile.getSampleRate();
	num_octave = octave_;
	note_pressed_time = 0.225;

	float C1 = 32.70320; //Pitch frecuency de C2
	pitch_frecuency = C1 * pow(2, num_octave - 1); //Calculo el pitch dependiendo la muestra C2,C3,C4 ...

	//SAMPLE
	sample = new float[lenght_sample];

	for (int i = 0; i < lenght_sample; i++)
	{
		sample[i] = audioFile.samples[0][i];
	}


	//VENTANA
	int P_0 = (int)(fs / pitch_frecuency);
	T = int(2 * P_0); //Largo de la ventana
	h_s = new float[T]; //Ventana 

	hanning(h_s, T, 0);

	/*
	for (int i = 0; i < T; i++) {
		double multiplier = 0.5 * (1 - cos(2 * M_PI * i / T));
		h_s[i] = multiplier; //Armo ventana de hanning
	}
	*/


	//PEAKS 
	fstream file(file_path + ".txt", ios_base::in);

	if (file.is_open())
	{
		cout << ".txt opened" << endl;
		int cnt = 0;
		int peak;

		file >> lenght_peaks;
		peaks = new int[lenght_peaks];

		while (file >> peak)
			peaks[cnt++] = peak;

		file.close();
	
	
	//DESCOMPOSICION DEL SAMPLE
	float* sample_x = nullptr;

	for (int i = 0; i < lenght_peaks; i++) {
		sample_x = new float[lenght_sample];
		sample_s.push_back(sample_x);
	}

	for (int i = 0; i < lenght_peaks; i++) {
		for (int j = 0; j < lenght_sample; j++) {
			sample_s[i][j] = 0;
		}
	}

	for (int idx_peaks = 0; idx_peaks < lenght_peaks; idx_peaks++) {

		for (int right_i = 0; right_i < (int)(T / 2); right_i++) {
			sample_s[idx_peaks][(int)peaks[idx_peaks] + right_i] = sample[(int)peaks[idx_peaks] + right_i] * h_s[(int)(T / 2) + right_i];
		}
		
		for (int left_i = 0; left_i < (int)(T / 2); left_i++) {
			sample_s[idx_peaks][(int)peaks[idx_peaks] - int(T / 2) + left_i] = sample[(int)peaks[idx_peaks] - int(T / 2) + left_i] * h_s[(int)left_i];
		}

	}


	}

	else cout << "Unable to open file" << endl;

}



void Sample::hanning(float* w, int N, short itype)
{
	int half, i, idx, n;

	if (itype == 1)	//periodic function
		n = N - 1;
	else
		n = N;

	if (n % 2 == 0)
	{
		half = n / 2;
		for (i = 0; i < half; i++) //CALC_HANNING   Calculates Hanning window samples.
			w[i] = 0.5 * (1 - cos(2 * M_PI * (i + 1) / (n + 1)));

		idx = half - 1;
		for (i = half; i < n; i++) {
			w[i] = w[idx];
			idx--;
		}
	}
	else
	{
		half = (n + 1) / 2;
		for (i = 0; i < half; i++) //CALC_HANNING   Calculates Hanning window samples.
			w[i] = 0.5 * (1 - cos(2 * M_PI * (i + 1) / (n + 1)));

		idx = half - 2;
		for (i = half; i < n; i++) {
			w[i] = w[idx];
			idx--;
		}
	}

	if (itype == 1)	//periodic function
	{
		for (i = N - 1; i >= 1; i--)
			w[i] = w[i - 1];
		w[0] = 0.0;
	}

}
