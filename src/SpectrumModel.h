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
#include"MicInput.h"
#include"spectrum_generation.h"
#include"SpectrumModelSubscriber.h"

using namespace std;


class SpectrumModel {

public:
	enum DSPFUNC {TIMESERIES, DB_MAG, MAG, PWR_SPECTRUM};
	enum WINDOWFUNC {};
	enum FILTERFUNC {};

	SpectrumModel();
	~SpectrumModel();

	// -- Get and Set methods.
	vector<Plot*> getPlotVector();

	// -- Plot object methods.
	Plot* detectHitPlot(GLfloat xpos, GLfloat ypos);
	void removePlot(Plot* plot);
	void addPlot(
		GLfloat xpos,		GLfloat ypos,		GLfloat width,		GLfloat height,
		GLfloat refminX,	GLfloat refminY,	GLfloat refmaxX,	GLfloat refmaxY,
		int rows,			int cols,           DSPFUNC funFlag, bool xLinear, bool yLinear
	);

	// -- Flagset methods
	void setWindowFlag(int flag);
	void setMethodFlag(int flag);
	void setFilterFlag(int flag);
	void setDetrendFlag();
	//void setNormalizeFlag();


	// -- Plot object modifier methods.
	void processData();
	void changePlotRefenceFrame(Plot* plot, GLfloat x, GLfloat y);
	void movePlot(Plot* plot, GLfloat x, GLfloat y);
	void scalePlot(Plot* plot, GLfloat x, GLfloat y);

	// -- Read micData.
	void readMicData();

	// -- Pub-Sub methods.
	void addSubscriber(SpectrumModelSubscriber* newSub);


private:
	int WINDOWFLAG, METHODFLAG, FILTERFLAG;
	bool DETREND, NORMALIZE;

	int inputDataSize, SAMPLES;
	GLfloat* inputData;

	vector<Plot*> plotVector;

	vector<SpectrumModelSubscriber*> subscribers;
	vector<void(SpectrumModel::*)(Plot*)> plotMethodVector;
	MicInput* format;
	

	// -- DSP methods.
	void timeSeries(Plot* plot);
	void magnitudeResponse(Plot* plot);
	void DBmagnitudeResponse(Plot* plot);
	void powerSpectralDensity(Plot* plot);

	// -- Pub-Sub methods.
	void notifySubscribers();

};





#endif // !SPECTRUMMODEL_H
