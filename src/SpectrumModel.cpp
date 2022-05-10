#include "SpectrumModel.h"

SpectrumModel::SpectrumModel(){
}

SpectrumModel::~SpectrumModel(){
	for (Plot* plot : plotVector) {
		delete plot;
	}
	plotVector.clear();
}





vector<Plot*> SpectrumModel::getPlotVector(){
	return plotVector;
}



Plot* SpectrumModel::detectHit(GLfloat xpos, GLfloat ypos){
	for (Plot* plot : plotVector) {
		if (plot->validClick(xpos, ypos)) {
			return plot;
		}
	}
	// -- If no Plot is found, return nullptr.
	return nullptr;
}

void SpectrumModel::addPlot(GLfloat xpos, GLfloat ypos, GLfloat width, GLfloat height, GLfloat refminX, GLfloat refminY, GLfloat refmaxX, GLfloat refmaxY, int rows, int cols, int methodFlag){
	
	Plot* newPlot = new Plot(xpos, ypos, width, height, refminX, refminY, refmaxX, refmaxY, rows, cols, Plot::LINEAR);
	plotVector.push_back(newPlot);

	void (SpectrumModel::*functionPointer)(Plot*);

	switch (methodFlag){
		case 1:
			functionPointer = &SpectrumModel::magnitudeOvertime;
			plotMethodVector.push_back(functionPointer);
			break;
		case 2:
			functionPointer = &SpectrumModel::magnitudeResponse;
			plotMethodVector.push_back(functionPointer);
			break;
		case 3:
			functionPointer = &SpectrumModel::DBmagnitudeResponse;
			plotMethodVector.push_back(functionPointer);
			break;
		case 4:
			functionPointer = &SpectrumModel::powerSpectralDensity;
			plotMethodVector.push_back(functionPointer);
			break;
		default:
			cout << "Invalid function flag in SpectrumModel::addPlot." << endl;
			break;
	}
	notifySubscribers();
}

void SpectrumModel::removePlot(Plot* givenPlot){
	
	for (int i = 0; i < plotVector.size(); i++) {
		if (givenPlot == plotVector[i]) {
			delete plotVector[i];
			plotVector.erase(plotVector.begin() + i);
			//plotProccessMethods.erase(plots.begin() + i);
		}
	}
	notifySubscribers();
}

void SpectrumModel::processData(){
	
	// -- Call each method by its pointer using its corrisponding Plot.
	for (int i = 0; i < plotMethodVector.size(); i++) {
		Plot* plot = plotVector[i];
		(this->*plotMethodVector.at(i))(plot);
	}
	free(inputData);
	notifySubscribers();
}

// -- Need to change implemntation.
void SpectrumModel::changePlotRefenceFrame(Plot* plot, GLfloat x, GLfloat y){	
	plot->changeReferenceFrame(plot->refMinX + 0.03f, plot->refMinY + 2.0f, plot->refMaxX - 0.03f, plot->refMaxY + 2.0f);
	notifySubscribers();
}

void SpectrumModel::movePlot(Plot* plot, GLfloat x, GLfloat y){
	plot->movePlot(x, y);
	notifySubscribers();
}

void SpectrumModel::scalePlot(Plot* plot, GLfloat x, GLfloat y){
	plot->scalePlot(x, y);
	notifySubscribers();
}






void SpectrumModel::readMicData() {
	const int SAMPLES = 1024 * 4;

	inputDataSize = SAMPLES;
	inputData = (GLfloat*)calloc(inputDataSize, sizeof(GLfloat));
	const int rawSize = inputDataSize;
	char* rawBytesPtr = (char*)calloc(rawSize, sizeof(char));


	// -- Device handle pointer.
	HWAVEIN hWaveIn;

	// -- Defining the audio format.
	WAVEFORMATEX formatMono44khz;
	formatMono44khz.wFormatTag = WAVE_FORMAT_PCM;
	formatMono44khz.nChannels = 1;
	formatMono44khz.nSamplesPerSec = 44100L;
	formatMono44khz.nAvgBytesPerSec = 44100L;
	formatMono44khz.nBlockAlign = 1;
	formatMono44khz.wBitsPerSample = 8;
	formatMono44khz.cbSize = 0;


	// -- Creation of the buffer header.
	WAVEHDR bufH;                           /* MUST SET ITEMS BELOW PREPARE! */
	bufH.lpData = (LPSTR)rawBytesPtr;       // -- pointer to the data buffer.     
	bufH.dwBufferLength = rawSize;          // -- buffer size in Bytes.           
	bufH.dwFlags = WHDR_BEGINLOOP;          // -- Flag, indicating buffer status. 
	bufH.dwLoops = 0L;

	auto openResult = waveInOpen(&hWaveIn, WAVE_MAPPER, &formatMono44khz, 0, 0, CALLBACK_NULL);
	auto prepareResult = waveInPrepareHeader(hWaveIn, &bufH, sizeof(bufH));
	auto addBufResult = waveInAddBuffer(hWaveIn, &bufH, sizeof(bufH));
	auto startResult = waveInStart(hWaveIn);

	// -- Do nothing while device driver reads data.
	while (!(bufH.dwFlags & WHDR_DONE)) {}

	auto stopResult = waveInStop(hWaveIn);
	auto unPrepareBuf = waveInUnprepareHeader(hWaveIn, &bufH, sizeof(bufH));
	auto closeResult = waveInClose(hWaveIn);


	int count = 0;

	// -- Loop through each raw sample's byte data and create a 4 byte int.
	for (int i = 0; i < rawSize; i += formatMono44khz.nBlockAlign) {
		int value = 0;
		char intBytes[4] = {};

		for (int j = 0; j < 4; j++) {
			intBytes[j] = (j < formatMono44khz.nBlockAlign) ? rawBytesPtr[i + j] : 0x00 ;
		}
		
		// -- Cast that 4 byte int to GLfloat.
		memcpy(&value, &intBytes, 4);
		inputData[count] = ((GLfloat)value);
		count++;
	}

	free(rawBytesPtr);
}




/* Private - methods */
void SpectrumModel::magnitudeResponse(Plot* plot) {
	GLfloat* result = spectrum_output(inputData, 0);
	plot->setRawData(result, inputDataSize * 2);
	free(result);
}

void SpectrumModel::DBmagnitudeResponse(Plot* plot) {
	GLfloat* result = spectrum_output(inputData, 1);
	plot->setRawData(result, inputDataSize * 2);
	free(result);
}

void SpectrumModel::powerSpectralDensity(Plot* plot) {
	GLfloat* result = spectrum_output(inputData, 3);
	plot->setRawData(result, inputDataSize * 2);
	free(result);
}

void SpectrumModel::magnitudeOvertime(Plot* plot) {
	GLfloat* converted = (GLfloat*)calloc(inputDataSize * 2, sizeof(GLfloat));
	for (int i = 0; i < inputDataSize; i++) {
		converted[2 * i]     = ((GLfloat)i) / ((GLfloat)inputDataSize);	// -- x coord
		converted[2 * i + 1] = inputData[i];							// -- y cord
	}

	plot->setRawData(converted, inputDataSize * 2);
	free(converted);
}




/* Pub-Sub methods*/
void SpectrumModel::addSubscriber(SpectrumModelSubscriber* newSub){
	subscribers.push_back(newSub);
}


void SpectrumModel::notifySubscribers(){
	for (SpectrumModelSubscriber* sub : subscribers) {
		sub->modelChanged();
	}
}
