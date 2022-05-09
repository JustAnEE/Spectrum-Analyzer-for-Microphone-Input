#include "SpectrumModel.h"

/* constructor and destructor */
SpectrumModel::SpectrumModel(){
}


SpectrumModel::~SpectrumModel(){
	for (Plot* plot : plots) {
		delete plot;
	}
	plots.clear();
}





Plot* SpectrumModel::detectHit(GLfloat xpos, GLfloat ypos){
	for (Plot* plot : plots) {
		if (plot->validClick(xpos, ypos)) {
			cout << " WE VALID." << endl;
			return plot;
		}
	}
	// -- on no Plot found return nullptr.
	return nullptr;
}


void SpectrumModel::addPlot(GLfloat xpos, GLfloat ypos, GLfloat width, GLfloat height, int rows, int cols, int funFlag){
	Plot* newPlot = new Plot(xpos, ypos, width, height, rows, cols, Plot::LINEAR);
	plots.push_back(newPlot);

	void (SpectrumModel::*funPtr)(Plot*);

	switch (funFlag){
		case 0:
			funPtr = &SpectrumModel::VinceProccessDataMethod1;
			plotProccessMethods.push_back(funPtr);
			break;
		case 1:
			funPtr = &SpectrumModel::generateSine;
			plotProccessMethods.push_back(funPtr);
			break;
	default:
		break;
	}
	notifySubscribers();
}

void SpectrumModel::removePlot(Plot* givenPlot){
	for (int i = 0; i < plots.size(); i++) {
		// referential equality. (Point to same thing.)
		// -- plots should only contain plot information.
		if (givenPlot == plots[i]) {
			delete plots[i];
			plots.erase(plots.begin() + i);
			//plotProccessMethods.erase(plots.begin() + i);
		}
	}
	notifySubscribers();
}

void SpectrumModel::processData(){
	// -- reproccess all data regardless.
	for (int idx = 0; idx < plots.size(); idx++) {
		Plot* test = plots[idx];
		(this->*plotProccessMethods.at(idx))(test);
	}
	free(readData);
	notifySubscribers();
}

void SpectrumModel::changePlotRef(Plot* plot, GLfloat x, GLfloat y){
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
	readDataSize = SAMPLES;
	readData = (GLfloat*)calloc(readDataSize, sizeof(GLfloat));
	const int rawSize = readDataSize;
	char* rawBytesPtr = (char*)calloc(rawSize, sizeof(char));


	// -- device handle pointer.
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


	// -- creation of the buffer header
	WAVEHDR bufH;                           /* MUST SET ITEMS BELOW PREPARE! */
	bufH.lpData = (LPSTR)rawBytesPtr;       // -- pointer to the data buffer.     
	bufH.dwBufferLength = rawSize;          // -- buffer size in Bytes.           
	bufH.dwFlags = WHDR_BEGINLOOP;          // -- Flag, indicating buffer status. 
	bufH.dwLoops = 0L;

	auto openResult = waveInOpen(&hWaveIn, WAVE_MAPPER, &formatMono44khz, 0, 0, CALLBACK_NULL);
	auto prepareResult = waveInPrepareHeader(hWaveIn, &bufH, sizeof(bufH));
	auto addBufResult = waveInAddBuffer(hWaveIn, &bufH, sizeof(bufH));
	auto startResult = waveInStart(hWaveIn);

	// -- Busy wait while device driver reads data.
	while (!(bufH.dwFlags & WHDR_DONE)) {}

	auto stopResult = waveInStop(hWaveIn);
	auto unPrepareBuf = waveInUnprepareHeader(hWaveIn, &bufH, sizeof(bufH));
	auto closeResult = waveInClose(hWaveIn);


	int count = 0;

	// -- loop through each ith sample -- mono only
	for (int i = 0; i < rawSize; i += formatMono44khz.nBlockAlign) {
		int value = 0;
		char intBytes[4] = {};

		for (int j = 0; j < 4; j++) {
			intBytes[j] = (j < formatMono44khz.nBlockAlign) ? rawBytesPtr[i + j] : 0x00 ;
		}

		// -- Imma just let the compiler do the conversion ;)
		memcpy(&value, &intBytes, 4);
		readData[count] = ((GLfloat)value);
		count++;
	}

	free(rawBytesPtr);

	// -- RAW DATA OUT SANITY CHECK
	//ofstream audioFile;
	//audioFile.open("AFTER.WAV", ios::binary | ios::out);
	//audioFile.write(rawBytesPtr, rawSize);

}



void SpectrumModel::VinceProccessDataMethod1(Plot* plot){
	// -- convert to X, Y data
	GLfloat* test = (GLfloat*)calloc(readDataSize * 2, sizeof(GLfloat));
	for (int i = 0; i < readDataSize; i++ ) {
		test[2*i] = ((GLfloat)i) / ((GLfloat)readDataSize);	// -- x coord
		test[2*i+1] = readData[i];					// -- y cord
	}

	plot->setRawData(test, readDataSize * 2);
	free(test);
}

void SpectrumModel::generateSine(Plot* plot){
	const int SAMPLES = 45;
	phase += 0.1f;

	GLfloat* test = (GLfloat*)calloc(SAMPLES * 2, sizeof(GLfloat));
	for (int i = 0; i < SAMPLES; i++) {
		test[2 * i] = ((GLfloat)i) / ((GLfloat)SAMPLES);		// -- x coord
		test[2 * i + 1] = (100.0f * sin(i/10.0f + phase)) + 125.0f;		// -- y cord
	}
	
	plot->setRawData(test, SAMPLES * 2);
	free(test);
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
