#include "../Hpp/SpectrumModel.hpp"
#include"../../Dsp/Hpp/Filter.hpp"
#include"../../Dsp/Hpp/spectrumdsp.hpp"
#include"../../Dsp/Hpp/SpectrumInitPacket.hpp"
#include"../../Dsp/Hpp/SpectrumPacket.hpp"
#include"../../Misc/Hpp/MicInput.hpp"
#include"../../Dsp/Hpp/SpectrumTypes.hpp"
#include"../../Widgets/Hpp/Plot.hpp"
#include"../../MVC/Hpp/SpectrumModelSubscriber.hpp"
#include <map>


// The following two std::map objects exist because the enums used across the system do not match
// They can be removed once one set of enums is used everywhere. 
static std::map<int, SpectrumTypeEnum> mMethodFlagToSpectrumType =
{
   {0, TIME_SERIES},
   {1, MAGNITUDE_SPECTRUM},
   {2, DB_SPECTRUM},
   {3, PSD_SPECTRUM}
};

static std::map<int, WindowTypeEnum> mWindowFlagToWindowType =
{
   {0, HAMMING_WINDOW},
   {1, BLACKMAN_WINDOW},
   {2, BARLETTE_WINDOW},
   {3, RECTANGULAR_WINDOW}
};


SpectrumModel::SpectrumModel()
{
   SAMPLES = 1024 * 4;
   PADDING = 4;
   inputDataSize = SAMPLES * PADDING;

   format = new MicInput();
   pclMyDSP = new SpectrumDSP(44100, SAMPLES * PADDING);
   
   pclMySpectrumInit = new SpectrumInitPacket(); 
   pclMySpectrum = new SpectrumPacket();
   pafTimeData = new float[SAMPLES * 2];

   return; 
}


SpectrumModel::~SpectrumModel()
{
   for (Plot* plot : plotVector)
   {
      delete plot;
   }
   plotVector.clear();

   delete format;
   delete pclMyDSP; 
   delete pclMySpectrum;
   delete pclMySpectrumInit;
   delete[] pafTimeData;
   return; 
}


std::vector<Plot*> SpectrumModel::getPlotVector()
{ 
   return plotVector;
}


Plot* SpectrumModel::detectClickPlot(GLfloat xpos, GLfloat ypos)
{
   for (Plot* plot : plotVector) {
      if (plot->validClick(xpos, ypos)) {
         return plot;
      }
   }
   // -- If no Plot is found, return nullptr.
   return nullptr;
}


void SpectrumModel::addPlot(
    int rows, int cols, DSPFUNC methodFlag,
    int windowFlag, int filterFlag, bool detrendFlag, bool normalizeFlag)
{

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
    return; 
}


void SpectrumModel::removePlot(Plot* givenPlot)
{
    for (int i = 0; i < plotVector.size(); i++)
    {
        if (givenPlot == plotVector[i]) 
        {
            delete plotVector[i];
            plotVector.erase(plotVector.begin() + i);
            break;
        }
    }

    layoutPlots();
    notifySubscribers();
    return; 
}


void SpectrumModel::processData()
{
   
   // -- Call each Plot's corrisponding method
   for (Plot* plot : plotVector) 
   {

      if(plot->getMethodFlag() != TIMESERIES)
      {
         this->CalculateSpectrum(plot); 
      }
      else if (plot->getMethodFlag() == TIMESERIES)
      {
         this->timeSeries(plot);
      }
      else
      {
         continue;
      }
   }

   notifySubscribers();
   return; 
}


void SpectrumModel::changePlotRefenceFrame(Plot* plot, GLfloat x, GLfloat y)
{
   plot->changeReferenceFrame(plot->refMinX + 0.03f, plot->refMinY + 2.0f, plot->refMaxX - 0.03f, plot->refMaxY + 2.0f);
   notifySubscribers();
   return; 
}


void SpectrumModel::movePlot(Plot* plot, GLfloat x, GLfloat y)
{
   plot->movePlot(x, y);
   notifySubscribers();
   return; 
}


void SpectrumModel::scalePlot(Plot* plot, GLfloat x, GLfloat y)
{
   plot->scalePlot(x, y);
   notifySubscribers();
   return; 
}


void SpectrumModel::readMicData() 
{
   format->readMicInput();

   /*ofstream audioFile;
   audioFile.open("AFTER.WAV", ios::binary | ios::out);
   audioFile.write(rawBytesPtr, rawSize);
   audioFile.close();*/
   return; 
}


/* Private - methods */
void SpectrumModel::CalculateSpectrum(Plot* pclPlot)
{
   pclMySpectrumInit->bIsNormalized = pclPlot->getNormalizeFlag();
   pclMySpectrumInit->bDetrend = pclPlot->getDetrendFlag();
   pclMySpectrumInit->stMyDSPInitialisation.eSpectrumOutput = mMethodFlagToSpectrumType[pclPlot->getMethodFlag()];
   pclMySpectrumInit->stMyDSPInitialisation.eWindow = mWindowFlagToWindowType[pclPlot->getWindowFlag()];
   pclMySpectrumInit->stMyDSPInitialisation.eFilter = NO_FILTER; // Always set for now, as the filters don't even work 
   pclMyDSP->ProcessSpectrumInitPacket(pclMySpectrumInit, format->GetMicData());
   pclMyDSP->GetSpectrumOutput(*pclMySpectrum);
   pclPlot->setRawData(pclMySpectrum->afMySpectrumArray, inputDataSize * 2);
   return; 
}


void SpectrumModel::timeSeries(Plot* plot) {
   for (int i = 0; i < SAMPLES; i++) 
   {
      pafTimeData[2 * i]     = ((GLfloat)i) / ((GLfloat)SAMPLES);  // -- x coord
      pafTimeData[2 * i + 1] = format->GetMicData()[i];  // -- y cord
   }

   plot->setRawData(pafTimeData, SAMPLES * 2);
   return; 
}


void SpectrumModel::layoutPlots(){
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

    switch (plotVector.size()) 
    {

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
    return;
}


/* Pub-Sub methods*/
void SpectrumModel::addSubscriber(SpectrumModelSubscriber* newSub)
{
   subscribers.push_back(newSub);
   return; 
}


void SpectrumModel::notifySubscribers(){
   for (SpectrumModelSubscriber* sub : subscribers) 
   {
      sub->DModelChanged();
   }
   return; 
}
