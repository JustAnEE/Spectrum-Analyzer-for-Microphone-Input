#include "SpectrumModel.h"

SpectrumModel::SpectrumModel(){
	format = new MicInput(1, 44100, 8);
}

SpectrumModel::~SpectrumModel(){
	for (Plot* plot : plotVector) {
		delete plot;
	}
	plotVector.clear();

	delete format;
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

void SpectrumModel::addPlot(
	GLfloat xpos,		GLfloat ypos,		GLfloat width,		GLfloat height,
	GLfloat refminX,	GLfloat refminY,	GLfloat refmaxX,	GLfloat refmaxY,
	int rows,			int cols,			DSP_METHOD methodFlag, bool xLinear, bool yLinear){
	
	// -- Create a new plot.
	Plot* newPlot = new Plot(xpos, ypos, width, height, refminX, refminY, refmaxX, refmaxY, rows, cols, xLinear, yLinear);
	plotVector.push_back(newPlot);

	// -- Create a function pointer.
	void (SpectrumModel::*functionPointer)(Plot*);

	switch (methodFlag){
		case NORMAL:
			newPlot->setTitle("Time Series");
			newPlot->setAxisLables("t", "A");
			functionPointer = &SpectrumModel::timeSeries;
			break;
		case MAG:
			newPlot->setTitle("Magnitude Response");
			newPlot->setAxisLables("f(Hz)", "|X(f)|");
			functionPointer = &SpectrumModel::magnitudeResponse;
			break;
		case DB_MAG:
			newPlot->setTitle("Decible Magnitude Response");
			newPlot->setAxisLables("f(Hz)", "dB");
			functionPointer = &SpectrumModel::DBmagnitudeResponse;
			break;
		case PWR_SPECTRUM:
			newPlot->setTitle("Power Spectral Density");
			newPlot->setAxisLables("f(Hz)", "A");
			functionPointer = &SpectrumModel::powerSpectralDensity;
			break;
		default:
			cout << "Invalid function flag in SpectrumModel::addPlot." << endl;
			exit(-69);
			break;
	}

	plotMethodVector.push_back(functionPointer);
	notifySubscribers();
}

void SpectrumModel::removePlot(Plot* givenPlot){
	
	for (int i = 0; i < plotVector.size(); i++) {
		if (givenPlot == plotVector[i]) {
			delete plotVector[i];
			plotVector.erase(plotVector.begin() + i);
			plotMethodVector.erase(plotMethodVector.begin() + i);
			break;
		}
	}
	notifySubscribers();
}

void SpectrumModel::processData(){
	
	// -- Call each Plot's corrisponding method
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
	
	SAMPLES = 1024*4;
	const int PADDING = 4;

	inputDataSize = SAMPLES * PADDING;
	inputData = (GLfloat*)calloc(inputDataSize, sizeof(GLfloat));
	
	const int rawSize = SAMPLES;
	char* rawBytesPtr = (char*)calloc(rawSize, sizeof(char));

	format->readMicInput(rawBytesPtr, rawSize);

	int count = 0;
	// -- Loop through each raw sample's byte data and create a 4 byte int.
	for (int i = 0; i < rawSize; i += format->getBlockAlign()) {
		int value = 0;
		char intBytes[4] = {};

		for (int j = 0; j < 4; j++) {
			intBytes[j] = (j < format->getBlockAlign()) ? rawBytesPtr[i + j] : 0x00;
		}

		// -- Cast that 4 byte int to GLfloat.
		memcpy(&value, &intBytes, 4);
		inputData[count] = ((GLfloat)value) - 128.0f;
		count++;

	}
	ofstream audioFile;
	audioFile.open("AFTER.WAV", ios::binary | ios::out);
	audioFile.write(rawBytesPtr, rawSize);
	audioFile.close();
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

void SpectrumModel::timeSeries(Plot* plot) {
	GLfloat* converted = (GLfloat*)calloc(SAMPLES*2, sizeof(GLfloat));
	for (int i = 0; i < SAMPLES; i++) {
		converted[2 * i]     = ((GLfloat)i) / ((GLfloat)SAMPLES);	// -- x coord
		converted[2 * i + 1] = inputData[i];							// -- y cord
	}

	plot->setRawData(converted, SAMPLES * 2);
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
