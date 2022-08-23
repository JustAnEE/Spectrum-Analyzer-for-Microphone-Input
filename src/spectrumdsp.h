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
	GLfloat* hammingWindow;
	GLfloat* blackmanWindow;
	GLfloat* barlettWindow;
	GLfloat* frequencyArray; 
	void genWindows();
	void setFreqs(); 

public:
	spectrumdsp(int sample_rate, int buffer_size);
	~spectrumdsp();

	GLfloat* applyWindow(GLfloat* input, int WINDOW_FLAG);
	
	void fft(fftwf_complex* input, fftwf_complex* output);
	void ifft(fftwf_complex* in, fftwf_complex* out);
	fftwf_complex* set_fft(fftwf_complex* shifted_input);
	fftwf_complex* fft_shift(GLfloat* buffer);
	GLfloat* magnitude(fftwf_complex* fft_data);
	GLfloat* dB_magnitude(GLfloat* magnitude_data);
	GLfloat* dBm_magnitude(GLfloat* magnitude_data);
	GLfloat* power_spectral_density(GLfloat* magnitude_data);
	GLfloat* prep_data_for_plot(GLfloat* spectrum);
	GLfloat* phase_spectrum(fftwf_complex* fft_data);
	GLfloat* detrend_buffer(GLfloat* buffer);
	GLfloat* spectrum_output(GLfloat* buffer, int SET_OUTPUT);

};