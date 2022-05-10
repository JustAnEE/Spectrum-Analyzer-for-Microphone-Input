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
#include"spectrum_generation.h"
#include"SpectrumModelSubscriber.h"

using namespace std;


class SpectrumModel {

public:
	vector<Plot*> plotVector;
	vector<void(SpectrumModel::*)(Plot*)> plotMethodVector;
	
	SpectrumModel();
	~SpectrumModel();

	GLfloat phase = 0.0f;
	// -- plot object methods
	Plot* detectHit(GLfloat xpos, GLfloat ypos);

	void addPlot(GLfloat xpos, GLfloat ypos, GLfloat width, GLfloat height, GLfloat refminX, GLfloat refminY, GLfloat refmaxX, GLfloat refmaxY, int rows, int cols, int funFlag);
	void removePlot(Plot* plot);

	// -- plot modifiers
	void processData();
	void changePlotRefenceFrame(Plot* plot, GLfloat x, GLfloat y);
	void movePlot(Plot* plot, GLfloat x, GLfloat y);
	void scalePlot(Plot* plot, GLfloat x, GLfloat y);

	// -- read micData method.
	void readMicData();

	// -- pub-sub methods
	void addSubscriber(SpectrumModelSubscriber* newSub);


private:
	GLfloat* inputData;
	int inputDataSize;
	vector<SpectrumModelSubscriber*> subscribers;

	// -- DSP methods
	void magnitudeOvertime(Plot* plot);
	void magnitudeResponse(Plot* plot);
	void DBmagnitudeResponse(Plot* plot);
	void generateSine(Plot* plot);

	void notifySubscribers();

};





#endif // !SPECTRUMMODEL_H
