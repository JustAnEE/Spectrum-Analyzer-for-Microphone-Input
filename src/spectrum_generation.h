#ifndef spectrum_generation_H
#define spectrum_generation_H

#include <fftw3.h>
#include <math.h> 
#include <iostream>
#include <GLFW/glfw3.h>


#define REAL 1
#define IMAG 0
#define sample_rate 44100
#define buffer_size 1024*4


/* USING THESE FUNCTIONS:
The ideal method of implementation as of now is as follows: The buffer is passed to the better_fft_shift function to preprocess the buffer such that the correct
shifting occurs at FFT output. Then the shifted data is passed to the better_set_fft function to implement the FFT on the buffer data (fft is called in this
function), and this data is in turn passed to the magnitude function to compute the complex modulus of each FFT element. Finally, frequency_array() is called
to obtain the frequency data for each FFT magnitude bin. So each frequency[k] corresponds to magnitude[k].

x axis for plotting: frequency array.
y axis for plotting: magnitude array.

The data is ready for plot via passing the frequency and magnitude (or other output) array to prep_data_for_plot.
prep_data_for_plot returns a pointer to a GLfloat array with entries [freq_0, mag_0, freq_1, mag_1, ... ].

Other:
dB_magnitude returns the magnitude array according to the dB scale, i.e 20log_10(magnitude). This function has a default noise floor of -120dB.
dBm_magnitude returns the same thing as dB_magnitude normalized to 1mW of power. Also has a default noise floor of -120dB
power_spectral_density returns the PSD of the magnitude data (normalized magnitude data squared).

Prebuilt Output Functions:
There is a prebuilt output function called spectrum_output(). This function takes in the buffer and a SET_OUTPUT integer where SET_OUTPUT = 0,1,2,3 corresponds to 
the magnitude spectrum, the dB spectrum, the dBm spectrum and the PSD spectrum respectively. Should SET_OUTPUT be set incorrectly, the function returns the magnitude
spectrum by default. 


NOTE: The standard .wav or PCM data sampling rate for audio applications is 44100Hz, this means the FFT analysis given by these functions captures the entire range
of human hearing (-22050Hz, 22050Hz). In most audio applications, the audio present is contained within a much smaller frequency support, it may be necessary to only plot on
fractions of the returned frequency range for a more coherent output.

When freeing variables of the fftwf_complex data-type, make sure to use fftw_free instead of free or delete. 

*/


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
GLfloat* spectrum_output(GLfloat* buffer, int SET_OUTPUT);

#endif // !spectrum_generation.h