#include <iostream>
#include <algorithm>    // std::min_element, std::max_element
#include "AudioFile.h"
#include <iterator>     // std::distance
#include <vector>
#include "Sampling.h"
#include <sstream>
#include <string>


AudioFile<double> audioFile;

using namespace std;

void find_peaks(float* x, float* peaks, int N)
{ 
	
	int step = 0;
	int bigger_idx = 0;
	int previous = 0;
	int result = 0;
	for (int i = 0; i < 10; i++)
	{
		
		for (int count = 0; count < 168; count++) {

			if (x[count + step ] > x[count + step + 1] && x[count + step] > x[previous]) {
				previous = count ;

			}
			if (x[count + step] < x[count + step + 1] && x[count + step] > x[previous]) {
				previous = count + 1 ;
			}		
		}

		result += previous;
		step = 168 * i;
		peaks[i] = result;
		//cout << peaks[i] << endl;
		previous = 0;
	}

	std::cout << peaks[0] << '\n';
	std::cout <<  168  + peaks[1] << '\n';
	std::cout << (168*2)  + peaks[2] << '\n';


	//std::cout << x[bigger_idx] << endl;
	std::cout << "The largest element is " << *std::max_element(x + 0, x + 167) << '\n';

}



int main(void) {

	int buffLength = 70000;

	float* out_buffer = new float[buffLength];
	
	samplingInstrument Piano(1, buffLength);


	//Piano.synthFunction(out_buffer, buffLength, 42, 0.02, 65, 326);

	/*
	AudioFile<double>::AudioBuffer buffer;
	buffer.resize(1);
	buffer[0].resize(buffLength);
	for (int i = 0; i < buffLength; i++)
	{
		buffer[0][i] = out_buffer[i];
	}

	bool ok = audioFile.setAudioBuffer(buffer);
	audioFile.save("./out.wav");
	*/

}
















