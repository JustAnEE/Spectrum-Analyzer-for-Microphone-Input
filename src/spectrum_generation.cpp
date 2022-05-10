#include "spectrum_generation.h"



void fft(fftw_complex* input, fftw_complex* output) {

	// Creates FFT plan for computing forward DFT
	// In the future this plan may change to a DCT plan for faster implementation, this may eliminate the need for shifting. 

	fftw_plan plan = fftw_plan_dft_1d(buffer_size, input, output, FFTW_FORWARD, FFTW_ESTIMATE); 

	fftw_execute(plan);
	fftw_destroy_plan(plan);
	fftw_cleanup();

}


//Not used for now.
fftw_complex* set_fft(double *buffer) {

	fftw_complex* y = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (buffer_size));
	fftw_complex* x = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (buffer_size));

	// Cannot directly pass buffer data into FFT, needs to be fftw_complex* data type.
	for (int k = 0; k < buffer_size; k++) {

		x[k][REAL] = buffer[k];
		x[k][IMAG] = 0;

	}
	
	// fft(x,y) will implicitly overwrite y
	fft(x, y); 


	return y; 

}


fftw_complex* better_set_fft(fftw_complex* shifted_input) {

	fftw_complex* y = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (buffer_size));


	fft(shifted_input, y);


	return y; 

}



// This function is most likely irrelevant for this application, though good to have. 
void ifft(fftw_complex* in, fftw_complex* out){

	fftw_plan plan = fftw_plan_dft_1d(buffer_size, in, out, FFTW_BACKWARD, FFTW_ESTIMATE); 
	fftw_execute(plan);
	fftw_destroy_plan(plan);
	fftw_cleanup(); 

	for (int k = 0; k <= buffer_size; k++) {

		out[k][REAL] /= buffer_size;
		out[k][IMAG] /= buffer_size; 

	}

}

// Not used for now. 
fftw_complex* fft_shift(fftw_complex* fft_data) {
	
	// Cursed function :) Probably not the best implementation of a fftshift functionality 

	int half_size = buffer_size/2; 
	fftw_complex* shifted_fft = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (buffer_size));

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

fftw_complex* better_fft_shift(double* buffer) {

	// Attempt at a better fftshift which preprocesses the input of the fft, instead of postprocessing fft output 

	fftw_complex* x = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (buffer_size ));

	for (int k = 0; k < buffer_size; k++) {

		// unreadable dsp wizardry :) 
		x[k][REAL] = buffer[k]*pow(-1, k);
		x[k][IMAG] = 0; 

	}

	return x; 

}


double* frequency_array(){

	// Computes frequency for each FFT bin with the distance between each bin being sample_rate/buffer_size. 
	// May need to increase buffer length to zero pad to an arbitrary power of 2 to interpolate the frequency response data. 

	double fs = (double) sample_rate;
	double N = (double) buffer_size;

	double* frequency = (double*)calloc(buffer_size, sizeof(double));

	for (int k = 0; k < buffer_size; k++) {

		frequency[k] = - fs/2 + ((double)k)*fs/N;

	}

	return frequency; 

}


double* magnitude(fftw_complex* fft_data) {


	double* magnitude_data = (double*)calloc(buffer_size, sizeof(double));
	
	for (int k = 0; k < buffer_size; k++) {

		// This is most likely not the most hardware efficient way to do this :) 

		double square = fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG];

		magnitude_data[k] = sqrt(square);

	}

	return magnitude_data;

}


double* dB_magnitude(double* magnitude_data) {

	double* dB_magnitude_data = (double*)calloc(buffer_size, sizeof(double));

	// Need to deal with magnitude data close to zero. Noise floor of -120 dB corresponds to a magnitude of 1e-6. 
	double noise_floor = -120; 


	for (int k = 0; k < buffer_size; k++) {

		if (magnitude_data[k] < 1 * pow(10, -6) || (magnitude_data[k] == 0)) {

			dB_magnitude_data[k] = noise_floor;

		}

		else {

			dB_magnitude_data[k] = (double)20 * log10(magnitude_data[k]);

		}

	}

	return dB_magnitude_data; 

}


// Same thing as dB_magnitude normalized to 1mW of power. 
double* dBm_magnitude(double* magnitude_data) {

	double* dBm_magnitude_data = (double*)calloc(buffer_size, sizeof(double));

	// Need to deal with magnitude data close to zero. 
	double noise_floor = -120;


	for (int k = 0; k < buffer_size; k++) {

		if (magnitude_data[k] < 1 * pow(10, -6) || (magnitude_data[k] == 0)){

			dBm_magnitude_data[k] = noise_floor;

		}

		else {


			dBm_magnitude_data[k] = (double)10 * log10(1000*magnitude_data[k]);
		}

	}
	
	return dBm_magnitude_data; 

}


double* power_spectral_density(double* magnitude_data) {

	double* psd = (double*)calloc(buffer_size, sizeof(double));

	for (int k = 0; k < buffer_size; k++) {

		psd[k] = magnitude_data[k] / buffer_size;

	}

	return psd; 

}


// Organize data into the form {f_0, m_0, f_1, m_1, ... , f_k, m_k, .... }
double* prep_data_for_plot(double* freqs, double* spectrum) {

	int plot_data_size = 2 * buffer_size; 
	double* data_for_plot = (double*)calloc(plot_data_size, sizeof(double));

	for (int k = 0; k < plot_data_size; k+=2) {

		int index = k / 2; 

		data_for_plot[k] = freqs[index];
		data_for_plot[k + 1] = spectrum[index];
		
	}

	return data_for_plot; 
}


double* spectrum_output(double* buffer, int SET_OUTPUT) {

	double*	freqs_array = frequency_array();

	fftw_complex* fftin_ptr = better_fft_shift(buffer);
	fftw_complex* fft_ptr = better_set_fft(fftin_ptr);
	
	double* mag_data = magnitude(fft_ptr); 
	double* output_ptr; 
	
	switch (SET_OUTPUT) {
	case 0: output_ptr = prep_data_for_plot(freqs_array, mag_data);
		break; 
	case 1: output_ptr = prep_data_for_plot(freqs_array, dB_magnitude(mag_data));
		break;
	case 2: output_ptr = prep_data_for_plot(freqs_array, dBm_magnitude(mag_data));
		break; 
	case 3: output_ptr = prep_data_for_plot(freqs_array, power_spectral_density(mag_data));
		break; 
	default:
		output_ptr = prep_data_for_plot(freqs_array, mag_data); 
	}

	fftw_free(fftin_ptr);
	fftw_free(fft_ptr);
	free(mag_data);

	return output_ptr; 

}