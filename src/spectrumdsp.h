#pragma once

#include <fftw3.h>
#include <math.h> 
#include <iostream>
#include <glad/glad.h>

#define REAL 1
#define IMAG 0


class spectrumdsp
{

private: 
	int sample_rate, buffer_size;

public:

	void setSampleRate(int samples);

	spectrumdsp(int sample_rate, int buffer_size);

	void fft(fftwf_complex* input, fftwf_complex* output);
	void ifft(fftwf_complex* in, fftwf_complex* out);
	fftwf_complex* set_fft(GLfloat* buffer);
	fftwf_complex* better_set_fft(fftwf_complex* shifted_input);
	fftwf_complex* fft_shift(fftwf_complex* fft_data);
	fftwf_complex* better_fft_shift(GLfloat* buffer);
	GLfloat* magnitude(fftwf_complex* fft_data);
	GLfloat* dB_magnitude(GLfloat* magnitude_data);
	GLfloat* dBm_magnitude(GLfloat* magnitude_data);
	GLfloat* power_spectral_density(GLfloat* magnitude_data);
	GLfloat* frequency_array();
	GLfloat* prep_data_for_plot(GLfloat* freqs, GLfloat* spectrum);
	GLfloat* phase_spectrum(fftwf_complex* fft_data);
	GLfloat* detrend_buffer(GLfloat* buffer);
	GLfloat* window_gen(int WINDOW_FLAG);
	GLfloat* spectrum_output(GLfloat* buffer, int SET_OUTPUT);

	

};



