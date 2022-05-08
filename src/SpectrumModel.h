#ifndef SPECTRUMMODEL_H
#define SPECTRUMMODEL_H

#include<iostream>
#include<vector>
#include<fstream>

#include<Windows.h>
#include<mmsystem.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Plot.h"
#include"SpectrumModelSubscriber.h"

using namespace std;


class SpectrumModel {
public:
	vector<Plot*> plots;
	//vector<void (*)()> functionPtrs;
	
	SpectrumModel();
	~SpectrumModel();

	// -- plot object methods
	Plot* detectHit(GLfloat xpos, GLfloat ypos);

	void addPlot(GLfloat xpos, GLfloat ypos, GLfloat width, GLfloat height, int rows, int cols);
	void removePlot(Plot* plot);
	void changePlotRef(Plot* plot, GLfloat x, GLfloat y);
	void movePlot(Plot* plot, GLfloat x, GLfloat y);
	void scalePlot(Plot* plot, GLfloat x, GLfloat y);

	// -- read micData method.
	void readMicData();

	// -- DSP methods
	void VinceProccessDataMethod1();
	// void VinceProccessDataMethod2();


	// -- pub-sub methods
	void addSubscriber(SpectrumModelSubscriber* newSub);


private:
	GLfloat* readData;
	int readDataSize;
	vector<SpectrumModelSubscriber*> subscribers;

	void notifySubscribers();

};





#endif // !SPECTRUMMODEL_H
