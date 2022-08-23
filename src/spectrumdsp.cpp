#include "spectrumdsp.h"

spectrumdsp::spectrumdsp(int _sample_rate, int _buffer_size) {
    sample_rate = _sample_rate;
    buffer_size = _buffer_size;
    hammingWindow = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));
    blackmanWindow = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));
    barlettWindow = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));
    frequencyArray = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));
    genWindows();
    setFreqs(); 
}

spectrumdsp::~spectrumdsp() {
    free(hammingWindow);
    free(blackmanWindow);
    free(barlettWindow); 
    free(frequencyArray);
}

void spectrumdsp::genWindows() {

    for (int k = 0; k < buffer_size; k++) {
        hammingWindow[k] = 0.54 - 0.46 * cosf(2.0 * 3.14159 * (GLfloat)k / ((GLfloat)(buffer_size - 1)));
        blackmanWindow[k] = 0.42 - 0.5 * cosf(2.0 * 3.14159 * (GLfloat)k / ((GLfloat)(buffer_size - 1))) + 0.08 * cos(4.0 * 3.14159 * (GLfloat)k / ((GLfloat)(buffer_size - 1)));
        barlettWindow[k] = (2.0 / ((GLfloat)(buffer_size - 1))) * (((GLfloat)(buffer_size - 1)) / 2.0 - (GLfloat)abs((GLfloat)k - (((GLfloat)buffer_size - 1)) / 2.0));
    }
}

void spectrumdsp::setFreqs() {

    GLfloat fs = (GLfloat)sample_rate;
    GLfloat N = (GLfloat)buffer_size;

    for (int k = 0; k < buffer_size; k++) {

        frequencyArray[k] = -fs / 2 + ((GLfloat)k) * fs / N;

    }

}

GLfloat* spectrumdsp::applyWindow(GLfloat* input, int WINDOW_FLAG){
    
    GLfloat* output = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));
    switch (WINDOW_FLAG) {

    case 0: for (int k = 0; k < buffer_size; ++k) { output[k] = input[k] * hammingWindow[k]; }
          break;
    case 1: for (int k = 0; k < buffer_size; ++k) { output[k] = input[k] * blackmanWindow[k]; }
          break;
    case 2: for (int k = 0; k < buffer_size; ++k) { output[k] = input[k] * barlettWindow[k]; }
          break;
    default: for (int k = 0; k < buffer_size; ++k) { output[k] = input[k] * hammingWindow[k]; }

    }

    return output; 

}

void spectrumdsp::fft(fftwf_complex* input, fftwf_complex* output)
{

    fftwf_plan plan = fftwf_plan_dft_1d(buffer_size, input, output, FFTW_FORWARD, FFTW_ESTIMATE);

    fftwf_execute(plan);
    fftwf_destroy_plan(plan);
    fftwf_cleanup();

}

void spectrumdsp::ifft(fftwf_complex* in, fftwf_complex* out)
{
    fftwf_plan plan = fftwf_plan_dft_1d(buffer_size, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftwf_execute(plan);
    fftwf_destroy_plan(plan);
    fftwf_cleanup();
    
    for (int k = 0; k <= buffer_size; k++) {

        out[k][REAL] /= buffer_size;
        out[k][IMAG] /= buffer_size;

    }
}

fftwf_complex* spectrumdsp::set_fft(fftwf_complex* shifted_input)
{
    fftwf_complex* y = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (buffer_size));

    fft(shifted_input, y);

    return y;
}

fftwf_complex* spectrumdsp::fft_shift(GLfloat* buffer)
{
    fftwf_complex* x = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (buffer_size));

        for (int k = 0; k < buffer_size; k++) {
            x[k][REAL] = buffer[k] * pow(-1, k);
            x[k][IMAG] = 0;
        }

    return x;
}

GLfloat* spectrumdsp::magnitude(fftwf_complex* fft_data)
{

    GLfloat* magnitude_data = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));

    for (int k = 0; k < buffer_size; k++) {

        // This is most likely not the most hardware efficient way to do this :) 

        GLfloat square = fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG];

        magnitude_data[k] = sqrt(square);

    }

    return magnitude_data;
}

GLfloat* spectrumdsp::dB_magnitude(GLfloat* magnitude_data)
{
    GLfloat* dB_magnitude_data = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));

    // Need to deal with magnitude data close to zero. Noise floor of -120 dB corresponds to a magnitude of 1e-6. 
    GLfloat noise_floor = -120;


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

GLfloat* spectrumdsp::dBm_magnitude(GLfloat* magnitude_data)
{

    GLfloat* dBm_magnitude_data = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));

    // Need to deal with magnitude data close to zero. 
    GLfloat noise_floor = -200;


    for (int k = 0; k < buffer_size; k++) {

        if (magnitude_data[k] < 1 * pow(10, -6) || (magnitude_data[k] == 0)) {

            dBm_magnitude_data[k] = noise_floor;

        }

        else {


            dBm_magnitude_data[k] = (GLfloat)10 * log10(1000 * magnitude_data[k]);
        }

    }

    return dBm_magnitude_data;
}

GLfloat* spectrumdsp::power_spectral_density(GLfloat* magnitude_data)
{
   
        GLfloat* psd = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));

        for (int k = 0; k < buffer_size; k++) {

            psd[k] = magnitude_data[k]*magnitude_data[k] / buffer_size;

        }

        return psd;

   
}

GLfloat* spectrumdsp::prep_data_for_plot(GLfloat* spectrum)
{
    int plot_data_size = 2 * buffer_size;
    GLfloat* data_for_plot = (GLfloat*)calloc(plot_data_size, sizeof(GLfloat));

    for (int k = 0; k < plot_data_size; k += 2) {

        int index = k / 2;

        data_for_plot[k] = frequencyArray[index];
        data_for_plot[k + 1] = spectrum[index];

    }
    return data_for_plot;
}

GLfloat* spectrumdsp::phase_spectrum(fftwf_complex* fft_data)
{
    GLfloat* phase_array = (GLfloat*)calloc(buffer_size, sizeof(GLfloat));

    GLfloat phase_threshold = 0;

    for (int k = 0; k < buffer_size; k++) {

        if (phase_threshold < sqrt(fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG])) {

            phase_threshold = sqrt(fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG]);

        }

    }

    for (int k = 0; k < buffer_size; k++) {

        if (sqrt(fft_data[k][REAL] * fft_data[k][REAL] + fft_data[k][IMAG] * fft_data[k][IMAG]) < phase_threshold) {

            phase_array[k] = 0;

        }

        else {

            phase_array[k] = (180.0f / 3.14159) * atan2f(fft_data[k][IMAG], fft_data[k][REAL]);

        }

    }

    return phase_array;

}

GLfloat* spectrumdsp::detrend_buffer(GLfloat* buffer)
{
    GLfloat* detrended_data = (GLfloat*)calloc(buffer_size, sizeof(GLfloat*));

    GLfloat buffer_avg = 0;

    for (int k = 0; k < buffer_size; k++) {

        buffer_avg += buffer[k];

    }

    for (int k = 0; k < buffer_size; k++) {

        detrended_data[k] = buffer[k] - buffer_avg;

    }

    return detrended_data;
}

GLfloat* spectrumdsp::spectrum_output(GLfloat* buffer, int SET_OUTPUT)
{
 
    fftwf_complex* fftin_ptr = fft_shift(buffer);
    fftwf_complex* fft_ptr = set_fft(fftin_ptr);

    GLfloat* mag_data = magnitude(fft_ptr);
    GLfloat* dB_mag_data = dB_magnitude(mag_data);
    GLfloat* dBm_mag_data = dBm_magnitude(mag_data);
    GLfloat* pwr_spec_data = power_spectral_density(mag_data);
    GLfloat* phase_data = phase_spectrum(fft_ptr);
    GLfloat* output_ptr;

    switch (SET_OUTPUT) {
    case 0: output_ptr = prep_data_for_plot(mag_data);
        break;
    case 1: output_ptr = prep_data_for_plot(dB_mag_data);
        break;
    case 2: output_ptr = prep_data_for_plot(dBm_mag_data);
        break;
    case 3: output_ptr = prep_data_for_plot(pwr_spec_data);
        break;
    case 4: output_ptr = prep_data_for_plot(phase_data);
        break;
    default:
        output_ptr = prep_data_for_plot( mag_data);
    }

    fftwf_free(fftin_ptr);
    fftwf_free(fft_ptr);
    free(mag_data);
    free(dB_mag_data);
    free(dBm_mag_data);
    free(phase_data);
    free(pwr_spec_data);

    return output_ptr;
}