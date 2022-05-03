#include "SpectrumModel.h"
#include "MicInput.cpp"

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


void SpectrumModel::addPlot(GLfloat xpos, GLfloat ypos, GLfloat width, GLfloat height, int rows, int cols){
	Plot* newPlot = new Plot(xpos, ypos, width, height, rows, cols, Plot::LINEAR);
	plots.push_back(newPlot);
	notifySubscribers();
}

void SpectrumModel::removePlot(Plot* givenPlot){
	for (int i = 0; i < plots.size(); i++) {
		// referential equality. (Point to same thing.)
		// -- plots should only contain plot information.
		if (givenPlot == plots[i]) {
			delete plots[i];
			plots.erase(plots.begin() + i);
		}
	}
	notifySubscribers();
}

void SpectrumModel::changePlotAxis(Plot* plot){
	// plot->
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
	const int SAMPLES = 1024;
	readDataSize = SAMPLES;
	readData = byteArrayToIEEE754(readMicInput(SAMPLES));
}

GLfloat* SpectrumModel::byteArrayToIEEE754(char* dataPtr){


}

void SpectrumModel::VinceProccessDataMethod1(){
	// -- manip data
	GLfloat* test;
	test = (GLfloat*)calloc(readDataSize, sizeof(GLfloat));

	for (int i = 0; i < readDataSize; i++) {
		test[i] = readData[i] + 0.5f;
	}

	plots[0]->setRawData(test, readDataSize);
	//free(readData);
	//free(test);
	notifySubscribers();
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
