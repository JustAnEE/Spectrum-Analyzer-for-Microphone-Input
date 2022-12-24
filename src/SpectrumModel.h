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
#include"spectrumdsp.h"
#include"Filter.h"
#include"SpectrumModelSubscriber.h"
#include"SpectrumTypes.hpp"
#include"SpectrumInitPacket.hpp"
#include"SpectrumPacket.hpp"

// !Todo: This bad 
using namespace std;


class SpectrumModel {

public:
	enum DSPFUNC {TIMESERIES, DB_MAG, MAG, PWR_SPECTRUM};

	SpectrumModel();
	~SpectrumModel();

	// -- Get and Set methods.
	vector<Plot*> getPlotVector();

	// -- Plot object methods.
	Plot* detectClickPlot(GLfloat xpos, GLfloat ypos);
	void removePlot(Plot* plot);
	void addPlot(
		GLfloat xpos,		GLfloat ypos,		GLfloat width,		GLfloat height,
		int rows,			int cols,			DSPFUNC methodFlag,
		int windowFlag,		int filterFlag,		bool detrendFlag,	bool normalizeFlag
	);


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
	int inputDataSize, SAMPLES, PADDING;
	GLfloat* inputData;

	SpectrumDSP* pclMyDSP;
	Filter* filter; 

	vector<Plot*> plotVector;

	vector<SpectrumModelSubscriber*> subscribers;
	vector<void(SpectrumModel::*)(Plot*, int, int, int, int)> plotMethodVector;
	MicInput* format;
	

	// -- DSP methods.
	void timeSeries(Plot* plot, int WINDOW, int FILTER, int DETREND, int NORMALIZE);
	void magnitudeResponse(Plot* plot, int WINDOW, int FILTER, int DETREND, int NORMALIZE);
	void DBmagnitudeResponse(Plot* plot, int WINDOW, int FILTER, int DETREND, int NORMALIZE);
	void powerSpectralDensity(Plot* plot, int WINDOW, int FILTER, int DETREND, int NORMALIZE);

	// -- Pub-Sub methods.
	void notifySubscribers();

};





#endif // !SPECTRUMMODEL_H
