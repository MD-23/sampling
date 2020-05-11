#include "Sampling.h"
#include "Sample.h"
#include <math.h>
#define _USE_MATH_DEFINES

using namespace std;

template<typename T>
std::vector<T> arange(T start, T stop, T step = 1) {
	std::vector<T> values;
	for (T value = start; value < stop; value += step)
		values.push_back(value);
	return values;
}

samplingInstrument::samplingInstrument(int num_instrument, const unsigned int buffLength) {

	repeat_time_begin = 0.05;
	repeat_time_end = 0.2;

	lenght_temp_buffer = buffLength;

	samples = new vector<Sample*>;
	Sample* sample = nullptr;
	temp_buffer = new float[buffLength];

	switch (num_instrument) {

	case 1: //Numero del instrumento
		cout << "Piano seleccionado" << endl;
		for (int i = 4; i < 5; i++) {
			cout << "Cargando C" + to_string(i) << endl;
			sample = new Sample("./Samples/Piano/C" + to_string(i), i);
			samples->push_back(sample);
		}
		break;
	default: 
			 
		break;
	}


}

void samplingInstrument::pitch_shifting(int num_octava, float B)
{
	Sample* selected_sample = nullptr;

	for (int i = 0; i < samples->size(); i++)
	{
		if (samples->at(i)->num_octave == num_octava) {
			selected_sample = samples->at(i);
			break;
		}
	}


	int P_0 = (int)(selected_sample->fs/ selected_sample->pitch_frecuency);
	int P_1 = (int)((1 / B) * P_0); //Nuevo Pitch period
	int N = selected_sample->lenght_sample; //Largo de la muestra
	int lenght_peaks = selected_sample->lenght_peaks;
	int T = selected_sample->T;

	

	//COMPOSICION DE NUEVA SEÒAL

	cleanBuffer();

	int idx = 0;
	for (int idx_new_peaks = 0; idx_new_peaks < N; idx_new_peaks = idx_new_peaks + int(P_1)) {

		idx = get_nearest_peak(selected_sample, idx_new_peaks);

		for (int right_i = 0; right_i < (int)(T / 2); right_i++) {
			if (idx_new_peaks + right_i > N - 1) {
				break;
			}
			else {
				temp_buffer[idx_new_peaks + right_i] += selected_sample->sample_s[idx][(int)selected_sample->peaks[idx] + right_i];
			}	
		}
		for (int left_i = 0; left_i < (int)(T / 2); left_i++) {
			temp_buffer[idx_new_peaks - (int)(T / 2) + left_i] += selected_sample->sample_s[idx][(int)selected_sample->peaks[idx] - (int)(T / 2) + left_i];
		}
	}
}



void samplingInstrument::time_modification(int num_octava, float new_note_pressed_time)
{
	Sample* selected_sample = nullptr;

	for (int i = 0; i < samples->size(); i++)
	{
		if (samples->at(i)->num_octave == num_octava) {
			selected_sample = samples->at(i);
			break;
		}
	}

	int N_0 = selected_sample->lenght_sample;
	int lenght_peaks = selected_sample->lenght_peaks;
	int T = selected_sample->T;
	float note_pressed_time = selected_sample->note_pressed_time;
	int fs = selected_sample->fs;


	

	if (new_note_pressed_time > note_pressed_time) {

		int N_0 = selected_sample->lenght_sample;
		int extra_N = (new_note_pressed_time - note_pressed_time) * selected_sample->fs;
		int N_1 = int(N_0 + extra_N); // Largo de la nueva seÒal
		float d_1 = N_1 / selected_sample->fs;
		cleanBuffer();
		
		int P_0 = (int)(selected_sample->fs / selected_sample->pitch_frecuency);


		int peaks_idx_end_pressed = 0; //Indice de peak donde termina note pressed
		int peaks_idx_begin_repeat = 0;  // Indice de peak donde empiza la region de repeticion
		int peaks_idx_end_repeat = 0;  //Indice de peak donde termina la region de repeticion



		for (int i = 0; i < lenght_peaks; i++)
		{
			if (selected_sample->peaks[i] > (int)(note_pressed_time * fs)) {
				peaks_idx_end_pressed = i - 1;
				break;
			}
		}

		for (int i = 0; i < lenght_peaks; i++)
		{
			if (selected_sample->peaks[i] > (int)(repeat_time_begin * fs)) {
				peaks_idx_begin_repeat = i - 1;
				break;
			}
		}

		for (int i = 0; i < lenght_peaks; i++)
		{
			if (selected_sample->peaks[i] > (int)(repeat_time_end * fs)) {
				peaks_idx_end_repeat = i - 1;
				break;
			}
		}


		int count = 0;
		int idx = 0;
		//Relleno desde 0 hasta el tiempo de presionada la nota original

		for (int new_peaks = selected_sample->peaks[0]; new_peaks < (int)note_pressed_time * fs; new_peaks = new_peaks * (int)P_0) {

			idx = count;

			for (int right_i = 0; right_i < int(T/2) ; right_i++)
			{
				if (new_peaks + right_i > N_0 - 1) {
					break;
				}
				else {
				
					temp_buffer[new_peaks + right_i] += selected_sample->sample_s[idx][selected_sample->peaks[idx] + right_i];
				}		
			}

			for (int left_i = 0; left_i < int(T / 2);left_i++)
			{
				temp_buffer[new_peaks - int(T / 2) + left_i] += selected_sample->sample_s[idx][selected_sample->peaks[idx] - int(T / 2) + left_i];

			}

			count = count + 1;
		}

	
		/*
		int lenght_new_peaks = (int)(((new_note_pressed_time * fs) - (note_pressed_time * fs)) / P_0);
		
		for (int i = 0; i < lenght_new_peaks; i++)
		{
			for (int j = int(note_pressed_time * fs) ; i < int(new_note_pressed_time * fs); i = i * int(P_0))

			
		}
		*/

		count = 0;
		idx = 0;
		//Relleno desde el tiempo de presionada la nota original hasta el tiempo de presionada la nota nueva

		int lenght_new_peaks = 0;

		//for (int idx_new_peaks = 1; idx_new_peaks < lenght_new_peaks; idx_new_peaks++)
		//{
		//	idx = peaks_idx_begin_repeat + count;




		//}

		count = 0;
		idx = 0;

		for (int new_peaks = selected_sample->peaks[0]; new_peaks < (int)note_pressed_time * fs; new_peaks = new_peaks * (int)P_0) {

			idx = count;

			for (int right_i = 0; right_i < int(T / 2); right_i++)
			{
				if (new_peaks + right_i > N_0 - 1) {
					break;
				}
				else {

					temp_buffer[new_peaks + right_i] += selected_sample->sample_s[idx][selected_sample->peaks[idx] + right_i];
				}
			}

			for (int left_i = 0; left_i < int(T / 2); left_i++)
			{
				temp_buffer[new_peaks - int(T / 2) + left_i] += selected_sample->sample_s[idx][selected_sample->peaks[idx] - int(T / 2) + left_i];

			}

			count = count + 1;
		}
















		//Relleno desde el tiempo de presionada la nota nueva hasta el final
		count = 0;
		idx = 0;

		for (int new_peaks = int(new_note_pressed_time * fs); new_peaks < N_1; new_peaks = new_peaks * (int)P_0) {

			idx = peaks_idx_end_pressed + count;

			for (int right_i = 0; right_i < int(T / 2); right_i++)
			{
				if (new_peaks + right_i > N_0 - 1) {
					break;
				}
				else {

					temp_buffer[new_peaks + right_i] += selected_sample->sample_s[idx][selected_sample->peaks[idx] + right_i];
				}
			}

			for (int left_i = 0; left_i < int(T / 2); left_i++)
			{
				if (new_peaks + left_i > N_0 - 1) {
					break;
				}
				else {
					temp_buffer[new_peaks - int(T / 2) + left_i] += selected_sample->sample_s[idx][selected_sample->peaks[idx] - int(T / 2) + left_i];
				}
			}

			if (idx < lenght_peaks)
				count += 1;

		}



















	





	}


}



void samplingInstrument::cleanBuffer() {
	for (int i = 0; i < lenght_temp_buffer; i++) {
		temp_buffer[i] = 0;
		temp_buffer_2[i] = 0;
	}
}

int samplingInstrument::get_nearest_peak(Sample* selected_sample, int number)
{
	int peaks_lenght = selected_sample->lenght_peaks;
	int temp = selected_sample->peaks[peaks_lenght - 1] - selected_sample->peaks[0];
	int i = 0;
	for (i = 0; i < peaks_lenght; i++) {
		if (abs(selected_sample->peaks[i] - number) < temp) {
			temp = abs(selected_sample->peaks[i] - number);
		}
		else
		{
			break;
		}
	}
	return(i - 1);
}




/*
int samplingInstrument::synthFunction(float* outputBuffer, const unsigned int outputBufferSize, const int keyNumber, const float lengthInMilliseconds, const int velocity, const int sampleRate) 
{
	int first_keyNumber = 4;
	int keys_per_octave = 11;
	int num_octave = 0;

	for (int i = 0; i < 9; i++)
	{
		if ((keyNumber >= first_keyNumber + (i * keys_per_octave) && keyNumber <= (first_keyNumber + keys_per_octave) + (i * keys_per_octave))) {
			num_octave = i+1;
		}
	}

	if (num_octave == 0) {
		cout << "No hay sample para sintetizar la nota pedida" << endl;
		return 1;
	}
	
	cout << "Numero de octava que se va a utilizar = " << num_octave << endl;
	
	int key_num_octave = 4 + (12 * (num_octave - 1));
	int key_dif = key_num_octave - keyNumber;

	if (key_dif != 0) {

		//PITCH SHIFTING
		float B = pow(2, key_dif / 12);
		pitch_shifting(num_octave, B);

	}

	for (int i = 0; i < outputBufferSize; i++)
	{
		outputBuffer[i] = temp_buffer[i];
	}


	return 0;
}

*/