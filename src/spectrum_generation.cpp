#include "spectrum_generation.h"



void fft(fftwf_complex* input, fftwf_complex* output) {

	// Creates FFT plan for computing forward DFT
	// In the future this plan may change to a DCT plan for faster implementation, this may eliminate the need for shifting. 

	fftwf_plan plan = fftwf_plan_dft_1d(buffer_size, input, output, FFTW_FORWARD, FFTW_ESTIMATE);

	fftwf_execute(plan);
	fftwf_destroy_plan(plan);
	fftwf_cleanup();

}


//Not used for now.
fftwf_complex* set_fft(GLfloat *buffer) {

	fftwf_complex* y = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (buffer_size));
	fftwf_complex* x = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (buffer_size));

	// Cannot directly pass buffer data into FFT, needs to be fftwf_complex* data type.
	for (int k = 0; k < buffer_size; k++) {

		x[k][REAL] = buffer[k];
		x[k][IMAG] = 0;

	}
	
	// fft(x,y) will implicitly overwrite y
	fft(x, y); 


	return y; 

}


fftwf_complex* better_set_fft(fftwf_complex* shifted_input) {

	fftwf_complex* y = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (buffer_size));


	fft(shifted_input, y);


	return y; 

}



// This function is most likely irrelevant for this application, though good to have. 
void ifft(fftwf_complex* in, fftwf_complex* out){

	fftwf_plan plan = fftwf_plan_dft_1d(buffer_size, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftwf_execute(plan);
	fftwf_destroy_plan(plan);
	fftwf_cleanup();

	for (int k = 0; k <= buffer_size; k++) {

		out[k][REAL] /= buffer_size;
		out[k][IMAG] /= buffer_size; 

	}

}

// Not used for now. 
fftwf_complex* fft_shift(fftwf_complex* fft_data) {
	
	// Cursed function :) Probably not the best implementation of a fftshift functionality 

	int half_size = buffer_size/2; 
	fftwf_complex* shifted_fft = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (buffer_size));

	for (int k = 0; k < buffer_size; k++) {

		if (k < half_size) {

			shifted_fft[k + half_size][REAL] = fft_data[k][REAL];
			shifted_fft[k + half_size][IMAG] = fft_data[k][IMAG];

		}


		if (k >= half_size) {

			shifted_fft[k - half_size][REAL] = fft_data[k][REAL];
			shifted_fft[k - half_size][IMAG] = fft_data[k][IMAG];

		}


	}

	return shifted_fft; 

}

fftwf_complex* better_fft_shift(GLfloat* buffer) {

	// Attempt at a better fftshift which preprocesses the input of the fft, instead of postprocessing fft output 

	fftwf_complex* x = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (buffer_size ));

	for (int k = 0; k < buffer_size; k++) {

		// unreadable dsp wizardry :) 
		x[k][REAL] = buffer[k]*pow(-1, k);
		x[k][IMAG] = 0; 

	}

	return x; 

}


GLfloat* frequency_array(){

	// Computes frequency for each FFT bin with the distance between each bin being sample_rate/buffer_size. 
	// May need to increase buffer length to zero pad to an arbitrary power of 2 to interpolate the frequency response data. 

	GLfloat fs = (GLfloat) sample_rate;
	GLfloat N = (GLfloat) buffer_size;

	GLfloat* frequency = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));

	for (int k = 0; k < buffer_size; k++) {

		frequency[k] = - fs/2 + ((GLfloat)k)*fs/N;

	}

	return frequency; 

}


GLfloat* magnitude(fftwf_complex* fft_data) {


	GLfloat* magnitude_data = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));
	
	for (int k = 0; k < buffer_size; k++) {

		// This is most likely not the most hardware efficient way to do this :) 

		GLfloat square = fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG];

		magnitude_data[k] = sqrt(square);

	}

	return magnitude_data;

}


GLfloat* dB_magnitude(GLfloat* magnitude_data) {

	GLfloat* dB_magnitude_data = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));

	// Need to deal with magnitude data close to zero. Noise floor of -120 dB corresponds to a magnitude of 1e-6. 
	GLfloat noise_floor = -80; 


	for (int k = 0; k < buffer_size; k++) {

		if (magnitude_data[k] < 1 * pow(10, -6) || (magnitude_data[k] == 0)) {

			dB_magnitude_data[k] = noise_floor;

		}

		else {

			dB_magnitude_data[k] = (GLfloat)20 * log10(magnitude_data[k]);

		}

	}

	return dB_magnitude_data; 

}


// Same thing as dB_magnitude normalized to 1mW of power. 
GLfloat* dBm_magnitude(GLfloat* magnitude_data) {

	GLfloat* dBm_magnitude_data = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));

	// Need to deal with magnitude data close to zero. 
	GLfloat noise_floor = -200;


	for (int k = 0; k < buffer_size; k++) {

		if (magnitude_data[k] < 1 * pow(10, -6) || (magnitude_data[k] == 0)){

			dBm_magnitude_data[k] = noise_floor;

		}

		else {


			dBm_magnitude_data[k] = (GLfloat)10 * log10(1000*magnitude_data[k]);
		}

	}
	
	return dBm_magnitude_data; 

}


GLfloat* power_spectral_density(GLfloat* magnitude_data) {

	GLfloat* psd = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));

	for (int k = 0; k < buffer_size; k++) {

		psd[k] = magnitude_data[k] / buffer_size;

	}

	return psd; 

}


// Organize data into the form {f_0, m_0, f_1, m_1, ... , f_k, m_k, .... }
GLfloat* prep_data_for_plot(GLfloat* freqs, GLfloat* spectrum) {

	int plot_data_size = 2 * buffer_size; 
	GLfloat* data_for_plot = (GLfloat*)calloc(plot_data_size, sizeof(GLfloat));

	for (int k = 0; k < plot_data_size; k+=2) {

		int index = k / 2; 

		data_for_plot[k] = freqs[index];
		data_for_plot[k + 1] = spectrum[index];
		
	}
	return data_for_plot; 
}


GLfloat* spectrum_output(GLfloat* buffer, int SET_OUTPUT) {

	GLfloat* freqs_array = frequency_array();

	fftwf_complex* fftin_ptr = better_fft_shift(buffer);
	fftwf_complex* fft_ptr = better_set_fft(fftin_ptr);
	
	GLfloat* mag_data = magnitude(fft_ptr); 
	GLfloat* dB_mag_data = dB_magnitude(mag_data);
	GLfloat* dBm_mag_data = dBm_magnitude(mag_data);
	GLfloat* pwr_spec_data = power_spectral_density(mag_data);
	GLfloat* output_ptr; 
	
	switch (SET_OUTPUT) {
	case 0: output_ptr = prep_data_for_plot(freqs_array, mag_data);
		break; 
	case 1: output_ptr = prep_data_for_plot(freqs_array, dB_mag_data);
		break;
	case 2: output_ptr = prep_data_for_plot(freqs_array, dBm_mag_data);
		break; 
	case 3: output_ptr = prep_data_for_plot(freqs_array, pwr_spec_data);
		break; 
	default:
		output_ptr = prep_data_for_plot(freqs_array, mag_data); 
	}

	fftwf_free(fftin_ptr);
	fftwf_free(fft_ptr);
	free(freqs_array);
	free(mag_data);
	free(dB_mag_data);
	free(dBm_mag_data);
	free(pwr_spec_data);

	return output_ptr; 

}