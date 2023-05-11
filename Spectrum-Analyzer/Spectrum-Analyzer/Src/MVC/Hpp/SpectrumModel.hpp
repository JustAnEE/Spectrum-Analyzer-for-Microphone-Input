#ifndef SPECTRUMMODEL_H
#define SPECTRUMMODEL_H

#include<iostream>
#include<vector>
#include<fstream>
#include<Windows.h>
#include<mmsystem.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "../../Dsp/Hpp/Filter.hpp"
#include "../../Dsp/Hpp/spectrumdsp.hpp"
#include "../../Dsp/Hpp/MagnitudeSpectrum.hpp"
#include "../../Dsp/Hpp/PSDSpectrum.hpp"
#include "../../Dsp/Hpp/DBSpectrum.hpp"
#include "../../Dsp/Hpp/SpectrumDSPConfig.hpp"
#include "../../Dsp/Hpp/SpectrumInitPacket.hpp"
#include "../../Dsp/Hpp/SpectrumPacket.hpp"
#include "../../Utils/enums.h"
#include "../../Utils/constants.h"
#include "../../Misc/Hpp/MicInput.hpp"
#include "../../MVC/Hpp/SpectrumModelSubscriber.hpp"
#include "../../Widgets/Hpp/Plot.hpp"

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
    int inputDataSize, SAMPLES, PADDING;
    GLfloat* inputData;

    SpectrumDSP* pclMyDSP;
    SpectrumBase* apclSpectrumDSPs[SpectrumTypeEnum::COUNT];
    Filter* filter; 

    std::vector<Plot*> plotVector;

    std::vector<SpectrumModelSubscriber*> subscribers;
    std::vector<void(SpectrumModel::*)(Plot*, int, int, int, int)> plotMethodVector;
    MicInput* format;

    void layoutPlots();

    // -- DSP methods.
    void timeSeries(Plot* plot, int WINDOW, int FILTER, int DETREND, int NORMALIZE);
    void magnitudeResponse(Plot* plot, int WINDOW, int FILTER, int DETREND, int NORMALIZE);
    void DBmagnitudeResponse(Plot* plot, int WINDOW, int FILTER, int DETREND, int NORMALIZE);
    void powerSpectralDensity(Plot* plot, int WINDOW, int FILTER, int DETREND, int NORMALIZE);

    // -- Pub-Sub methods.
    void notifySubscribers();

};

#endif // !SPECTRUMMODEL_H
