#pragma once

#include "AudioFile.h"
#include "Sample.h"
#include <vector>


using namespace std;


class samplingInstrument{
public:
	samplingInstrument(int num_instrument, const unsigned int buffLength);
	~samplingInstrument() {}

	float repeat_time_begin; //Tiempo a partir del cual se empieza a repetir en caso en que se desee un tiempo de presionado de nota mayor al sample
	float repeat_time_end; //Limite del tiempo de repeticion


	vector <Sample*>* samples; 


	float* temp_buffer; 
	float* temp_buffer_2;


	int lenght_temp_buffer;


	//instrumentCallback synthFunction;
	void pitch_shifting(int num_sample, float B);
	void time_modification(int num_octava, float new_note_pressed_time);
	int get_nearest_peak(Sample* selected_sample, int number);
	void cleanBuffer();
	

	int synthFunction(float* outputBuffer, const unsigned int outputBufferSize, const int keyNumber, const float lengthInMilliseconds, const int velocity, const int sampleRate);

};