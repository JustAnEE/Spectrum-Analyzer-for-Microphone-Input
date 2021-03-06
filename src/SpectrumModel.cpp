#include "SpectrumModel.h"

SpectrumModel::SpectrumModel(){
	SAMPLES = 1024 * 4;
	PADDING = 4;

	format = new MicInput(1, 44100, 8);
	dsp = new spectrumdsp(44100, SAMPLES * PADDING);

	// -- Setup function pointer list.
	plotMethodVector.push_back(&SpectrumModel::timeSeries);
	plotMethodVector.push_back(&SpectrumModel::magnitudeResponse);
	plotMethodVector.push_back(&SpectrumModel::DBmagnitudeResponse);
	plotMethodVector.push_back(&SpectrumModel::powerSpectralDensity);

}

SpectrumModel::~SpectrumModel(){
	for (Plot* plot : plotVector) {
		delete plot;
	}
	plotVector.clear();

	delete dsp;
	delete format;
}



vector<Plot*> SpectrumModel::getPlotVector(){ return plotVector; }



Plot* SpectrumModel::detectClickPlot(GLfloat xpos, GLfloat ypos){
	for (Plot* plot : plotVector) {
		if (plot->validClick(xpos, ypos)) {
			return plot;
		}
	}
	// -- If no Plot is found, return nullptr.
	return nullptr;
}




void SpectrumModel::addPlot(
	int rows,			int cols,			DSPFUNC methodFlag,
	int windowFlag,     int filterFlag,     bool detrendFlag,	bool normalizeFlag
	){
	
	// -- Create a new plot.
	Plot* newPlot = new Plot(-10.0, -10.0, 1.0, 1.0, rows, cols);;
	newPlot->setMethodFlag(methodFlag);
	newPlot->setWindowFlag(windowFlag);
	newPlot->setFilterFlag(filterFlag);
	newPlot->setDetrendFlag(detrendFlag);
	newPlot->setNormalizeFlag(normalizeFlag);

	plotVector.push_back(newPlot);
	layoutPlots();

	notifySubscribers();
}

void SpectrumModel::removePlot(Plot* givenPlot){
	
	for (int i = 0; i < plotVector.size(); i++) {
		if (givenPlot == plotVector[i]) {
			delete plotVector[i];
			plotVector.erase(plotVector.begin() + i);
			break;
		}
	}

	layoutPlots();
	notifySubscribers();
}





void SpectrumModel::processData(){
	
	// -- Call each Plot's corrisponding method
	for (Plot* plot : plotVector) {
		(this->*plotMethodVector[plot->getMethodFlag()])(plot, plot->getWindowFlag(), plot->getFilterFlag(), plot->getDetrendFlag(), 0);
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
	// -- To do swap this to multiple formats, and super sampling
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
	/*ofstream audioFile;
	audioFile.open("AFTER.WAV", ios::binary | ios::out);
	audioFile.write(rawBytesPtr, rawSize);
	audioFile.close();*/
	free(rawBytesPtr);
}




/* Private - methods */
void SpectrumModel::magnitudeResponse(Plot* plot, int WINDOW, int FILTER, int DETREND, int NORMALIZE) {
	GLfloat* result = dsp->spectrum_output(inputData, 0);
	plot->setRawData(result, inputDataSize * 2);
	free(result);
}

void SpectrumModel::DBmagnitudeResponse(Plot* plot, int WINDOW, int FILTER, int DETREND, int NORMALIZE) {
	GLfloat* result = dsp->spectrum_output(inputData, 1);
	plot->setRawData(result, inputDataSize * 2);
	free(result);
}

void SpectrumModel::powerSpectralDensity(Plot* plot, int WINDOW, int FILTER, int DETREND, int NORMALIZE) {
	GLfloat* result = dsp->spectrum_output(inputData, 3);
	plot->setRawData(result, inputDataSize * 2);
	free(result);
}

void SpectrumModel::timeSeries(Plot* plot, int WINDOW, int FILTER, int NORMALIZE, int DETREND) {
	GLfloat* converted = (GLfloat*)calloc(SAMPLES*2, sizeof(GLfloat));
	for (int i = 0; i < SAMPLES; i++) {
		converted[2 * i]     = ((GLfloat)i) / ((GLfloat)SAMPLES);	// -- x coord
		converted[2 * i + 1] = inputData[i];						// -- y cord
	}

	plot->setRawData(converted, SAMPLES * 2);
	free(converted);
}


void SpectrumModel::layoutPlots() {
	int plotBoxWidth = 800.0f;
	int plotBoxHeight = 800.0f;

	GLfloat PBHalfW = (plotBoxWidth / 1000.0f);
	GLfloat PBHalfH = (plotBoxHeight / 900.0f);
	GLfloat PBQuarterW = (plotBoxWidth / 1000.0f) / 2.0f;
	GLfloat PBQuarterH = (plotBoxHeight / 900.0f) / 2.0f;

	GLfloat PlotBoxCenterx = -1.0f + PBHalfW;
	GLfloat PlotBoxCenterY = -1.0f + PBHalfH;

	GLfloat largeWidth = PBHalfW * 2.0f * 0.80f;
	GLfloat largeHeight = PBHalfH * 2.0f * 0.80f;
	GLfloat smallWidth = PBHalfW * 2.0f * 0.30f;
	GLfloat smallHeight = PBHalfH * 2.0f * 0.30f;

	switch (plotVector.size()) {
	case 1:  // -- one large center screen plot
		plotVector[0]->movePlot(PlotBoxCenterx, PlotBoxCenterY);
		plotVector[0]->scalePlot(largeWidth, largeHeight);
		break;
	case 2:  // -- two, vertical long plots.
		plotVector[0]->movePlot(PlotBoxCenterx, PlotBoxCenterY + PBQuarterH);
		plotVector[1]->movePlot(PlotBoxCenterx, PlotBoxCenterY - PBQuarterH);
		
		plotVector[0]->scalePlot(largeWidth, smallHeight);
		plotVector[1]->scalePlot(largeWidth, smallHeight);
		break;
	case 3:  // -- one vertical long plot on top, two small plots bottom.
		plotVector[0]->movePlot(PlotBoxCenterx, PlotBoxCenterY + PBQuarterH);
		plotVector[1]->movePlot(PlotBoxCenterx + PBQuarterW, PlotBoxCenterY - PBQuarterH);
		plotVector[2]->movePlot(PlotBoxCenterx - PBQuarterW, PlotBoxCenterY - PBQuarterH);

		plotVector[0]->scalePlot(largeWidth, smallHeight);
		plotVector[1]->scalePlot(smallWidth, smallHeight);
		plotVector[2]->scalePlot(smallWidth, smallHeight);
		break;
	case 4:  // -- four small plots.
		plotVector[0]->movePlot(PlotBoxCenterx - PBQuarterW, PlotBoxCenterY + PBQuarterH);
		plotVector[1]->movePlot(PlotBoxCenterx + PBQuarterW, PlotBoxCenterY + PBQuarterH);
		plotVector[2]->movePlot(PlotBoxCenterx + PBQuarterW, PlotBoxCenterY - PBQuarterH);
		plotVector[3]->movePlot(PlotBoxCenterx - PBQuarterW, PlotBoxCenterY - PBQuarterH);

		plotVector[0]->scalePlot(smallWidth, smallHeight);
		plotVector[1]->scalePlot(smallWidth, smallHeight);
		plotVector[2]->scalePlot(smallWidth, smallHeight);
		plotVector[3]->scalePlot(smallWidth, smallHeight);
		break;
	default:
		std::cout << "ERROR in SpectrumModel::addPlot().   Invalid plotVectorSize() \n";
		exit(-21);
		break;
	}
}



/* Pub-Sub methods*/
void SpectrumModel::addSubscriber(SpectrumModelSubscriber* newSub){
	subscribers.push_back(newSub);
}


void SpectrumModel::notifySubscribers(){
	for (SpectrumModelSubscriber* sub : subscribers) {
		sub->DModelChanged();
	}
}
