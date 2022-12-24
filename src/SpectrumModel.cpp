#include "SpectrumModel.h"

SpectrumModel::SpectrumModel(){
   SAMPLES = 1024 * 4;
   PADDING = 4;

   format = new MicInput(1, 44100, 8);
   pclMyDSP = new SpectrumDSP(44100, SAMPLES * PADDING);

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

   delete format;
   delete pclMyDSP; 
   delete filter; 
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
   GLfloat xpos,		GLfloat ypos,		GLfloat width,		GLfloat height,
   int rows,			int cols,			DSPFUNC methodFlag,
   int windowFlag,     int filterFlag,     bool detrendFlag,	bool normalizeFlag
   ){
   
   // -- Create a new plot.
   Plot* newPlot = new Plot(xpos, ypos, width, height, rows, cols);
   newPlot->setMethodFlag(methodFlag);
   newPlot->setWindowFlag(windowFlag);
   newPlot->setFilterFlag(filterFlag);
   newPlot->setDetrendFlag(detrendFlag);
   newPlot->setNormalizeFlag(normalizeFlag);

   plotVector.push_back(newPlot);
   
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

   // This is temporary code, these spectrum model spectrum functions can be collapsed into one. 

   // Initialize spectrum init packet 
   SpectrumInitPacket* pclSpectrumInitPacket = new SpectrumInitPacket;

   pclSpectrumInitPacket->bDetrend = DETREND;
   pclSpectrumInitPacket->bIsNormalized = NORMALIZE;
   pclSpectrumInitPacket->stMyDSPInitialisation.eSpectrumOutput = MAGNITUDE_SPECTRUM;
   pclSpectrumInitPacket->stMyDSPInitialisation.eWindow = (WindowTypeEnum)WINDOW;
   pclSpectrumInitPacket->stMyDSPInitialisation.eFilter = (FilterTypeEnum)FILTER; 
   pclSpectrumInitPacket->bIsFsandBufferSizeDefault = true;

   // Get an empty output packet 
   SpectrumPacket* pclSpectrumOutputPacket = new SpectrumPacket;
   
   // Send the init packet and input data to the SpectrumDSP class to process output. 
   pclMyDSP->ProcessSpectrumInitPacket(pclSpectrumInitPacket, inputData);
   
   // Copy the output data from SpectrumDSP into the output packet. 
   pclMyDSP->GetSpectrumOutput(*pclSpectrumOutputPacket);

   // Give output data to the plot
   plot->setRawData(pclSpectrumOutputPacket->afMySpectrumArray, inputDataSize * 2); 

   // Delete packets

   delete(pclSpectrumInitPacket);
   delete(pclSpectrumOutputPacket);

}

void SpectrumModel::DBmagnitudeResponse(Plot* plot, int WINDOW, int FILTER, int DETREND, int NORMALIZE) {
   // This is temporary code, these spectrum model spectrum functions can be collapsed into one. 

   // Initialize spectrum init packet 
   SpectrumInitPacket* pclSpectrumInitPacket = new SpectrumInitPacket;

   pclSpectrumInitPacket->bDetrend = DETREND;
   pclSpectrumInitPacket->bIsNormalized = NORMALIZE;
   pclSpectrumInitPacket->stMyDSPInitialisation.eSpectrumOutput = DB_SPECTRUM;
   pclSpectrumInitPacket->stMyDSPInitialisation.eWindow = (WindowTypeEnum)WINDOW;
   pclSpectrumInitPacket->stMyDSPInitialisation.eFilter = (FilterTypeEnum)FILTER;
   pclSpectrumInitPacket->bIsFsandBufferSizeDefault = true;

   // Get an empty output packet 
   SpectrumPacket* pclSpectrumOutputPacket = new SpectrumPacket;

   // Send the init packet and input data to the SpectrumDSP class to process output. 
   pclMyDSP->ProcessSpectrumInitPacket(pclSpectrumInitPacket, inputData);

   // Copy the output data from SpectrumDSP into the output packet. 
   pclMyDSP->GetSpectrumOutput(*pclSpectrumOutputPacket);

   // Give output data to the plot
   plot->setRawData(pclSpectrumOutputPacket->afMySpectrumArray, inputDataSize * 2);

   // Delete packets

   delete(pclSpectrumInitPacket);
   delete(pclSpectrumOutputPacket);
}

void SpectrumModel::powerSpectralDensity(Plot* plot, int WINDOW, int FILTER, int DETREND, int NORMALIZE) {
   // This is temporary code, these spectrum model spectrum functions can be collapsed into one. 

   // Initialize spectrum init packet 
   SpectrumInitPacket* pclSpectrumInitPacket = new SpectrumInitPacket;

   pclSpectrumInitPacket->bDetrend = DETREND;
   pclSpectrumInitPacket->bIsNormalized = NORMALIZE;
   pclSpectrumInitPacket->stMyDSPInitialisation.eSpectrumOutput = PSD_SPECTRUM;
   pclSpectrumInitPacket->stMyDSPInitialisation.eWindow = (WindowTypeEnum)WINDOW;
   pclSpectrumInitPacket->stMyDSPInitialisation.eFilter = (FilterTypeEnum)FILTER;
   pclSpectrumInitPacket->bIsFsandBufferSizeDefault = true;

   // Get an empty output packet 
   SpectrumPacket* pclSpectrumOutputPacket = new SpectrumPacket;

   // Send the init packet and input data to the SpectrumDSP class to process output. 
   pclMyDSP->ProcessSpectrumInitPacket(pclSpectrumInitPacket, inputData);

   // Copy the output data from SpectrumDSP into the output packet. 
   pclMyDSP->GetSpectrumOutput(*pclSpectrumOutputPacket);

   // Give output data to the plot
   plot->setRawData(pclSpectrumOutputPacket->afMySpectrumArray, inputDataSize * 2);

   // Delete packets

   delete(pclSpectrumInitPacket);
   delete(pclSpectrumOutputPacket);
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




/* Pub-Sub methods*/
void SpectrumModel::addSubscriber(SpectrumModelSubscriber* newSub){
   subscribers.push_back(newSub);
}


void SpectrumModel::notifySubscribers(){
   for (SpectrumModelSubscriber* sub : subscribers) {
      sub->DModelChanged();
   }
}
