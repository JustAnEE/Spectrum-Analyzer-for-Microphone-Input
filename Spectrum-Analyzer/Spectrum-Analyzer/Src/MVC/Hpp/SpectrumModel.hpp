#ifndef SPECTRUMMODEL_H
#define SPECTRUMMODEL_H

#include<iostream>
#include<vector>
#include<fstream>
#include<Windows.h>
#include<mmsystem.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"../../Dsp/Hpp/SpectrumTypes.hpp"

// Forward Declarations 
class SpectrumInitPacket;
class SpectrumDSP; 
class SpectrumPacket;
class MicInput;
class Filter;
class Plot;
class SpectrumModelSubscriber;

class SpectrumModel {

public:

    enum DSPFUNC {TIMESERIES, DB_MAG, MAG, PWR_SPECTRUM};

    SpectrumModel();
    ~SpectrumModel();

    // -- Get and Set methods.
    std::vector<Plot*> getPlotVector();

    // -- Plot object methods.
    Plot* detectClickPlot(GLfloat xpos, GLfloat ypos);
    void removePlot(Plot* plot);
    void addPlot(
        int rows, int cols, DSPFUNC methodFlag,
        int windowFlag, int filterFlag, bool detrendFlag, bool normalizeFlag
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

   void layoutPlots();

   // -- DSP methods.
   void timeSeries(Plot* plot);
   void CalculateSpectrum(Plot* pclPlot);
   // -- Pub-Sub methods.
   void notifySubscribers();

private:
    int inputDataSize;
    int SAMPLES;
    int PADDING;
    float* pafTimeData;
    SpectrumDSP* pclMyDSP;
    SpectrumInitPacket* pclMySpectrumInit; 
    SpectrumPacket* pclMySpectrum; 
    std::vector<Plot*> plotVector;
    std::vector<SpectrumModelSubscriber*> subscribers;
    MicInput* format;

};

#endif // !SPECTRUMMODEL_H
