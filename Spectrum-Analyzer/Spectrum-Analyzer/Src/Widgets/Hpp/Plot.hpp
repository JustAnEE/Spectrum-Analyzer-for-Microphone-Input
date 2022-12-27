#ifndef PLOT_H
#define PLOT_H

#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../Hpp/TextLabel.hpp"


class Plot {
public:
	GLfloat refMinX, refMinY, refMaxX, refMaxY;

	/**
	* Plot Object Constructor
	*
	* @param way too many,	 :(.
	* @return,				 Plot object.
	*/
	Plot(GLfloat centX, GLfloat centY, GLfloat w, GLfloat h, int rows, int cols );
	
	~Plot();
	
	/**
	* Gets a vector of Textlabels.
	*
	* @return, vector<Textlabel*> pointer
	*/
	std::vector<TextLabel*> getText();
	
	
	
	/**
	* Sets the number of rows and columns of the THIS Plot object.
	*
	* @param numRows,	An Integer representing the total number of rows in the Plot.
	* @param numCols,	An Integer representing the total number of rows in the Plot.
	* @return,			Void.
	*/
	void setRowsAndCols(int numRows, int numCols);



	/**
	* Copys the raw data sample from vertexArrayPtr into rawDataPtr. Each raw data
	* sample MUST have contain X, Y, values in that order. 
	* 
	* @param vertexArrayPtr,	An GLfloat Array representing the Plot's function data.
	* @param size,				An Integer representing the Raw data's size in SAMPLES.
	* @return,					Void.
	*/
	void setRawData(GLfloat* vertexArrayPtr, int size);
	

	/**
	* Sets the title text of the Plot.
	*
	* @param newTitle,	An GLfloat Array representing the Plot's function data.
	* @return,					Void.
	*/
	void setTitle(std::string newTitle);


	/**
	* Sets the title text of the Plot.
	*
	* @param newTitle,	An GLfloat Array representing the Plot's function data.
	* @return,					Void.
	*/
	void setAxisLables(std::string xAxisLabel, std::string yAxisLabel);


	/**
	* Gets the Plot's World height with respect to the OpenGL window.
	* NOTE: values are between -1.0f and +1.0f.
	* 
	* @return, GLfloat
	*/
	GLfloat getHeight();



	/**
	* Gets the Plot's World width with respect to the OpenGL window.
	* NOTE: values are between -1.0f and +1.0f.
	*
	* @return, GLfloat
	*/
	GLfloat getWidth();


	/**
	* Gets the Plot's function data.
	* NOTE: The values are organized for OpenGL 
	*       E.G: one data point has six GLfloats (X,Y,Z,R,G,B).
	*
	* @return, GLfloat*
	*/
	GLfloat* getVertexDataArray();


	/**
	* Gets the Plot's boundary data.
	* NOTE: The values are organized for OpenGL
	*       E.G: one data point has six GLfloats (X,Y,Z,R,G,B).
	*
	* @return, GLfloat*
	*/
	GLfloat* getVertexPlotArray();


	/**
	* Gets the size of the function data array.
	* NOTE: dataSize is three times larger than the number of data points.
	* 
	* @return, int
	*/
	int getDataSize();


	/**
	* Gets the size of the boundry array.
	*
	* @return, int
	*/
	int getPlotSize();


	/**
	* Scales the boundary of the Plot.
	*
	* @param givenW,	An GLfloat representing the change in width.   E.G: 0.02f
	* @param givenH,	An GLfloat representing the change in height.  E.G: 0.04f
	* @return,					Void.
	*/
	void scalePlot(GLfloat givenW, GLfloat givenH);


	/**
	* Moves the plot to the given position in the world.
	* NOTE: All values must be between -1.0f and 1.0f.
	* 
	* @param givenX,	An GLfloat representing the new X location.   E.G:  0.5f
	* @param givenY,	An GLfloat representing the new Y location.   E.G: -0.2f
	* @return,					Void.
	*/
	void movePlot(GLfloat givenX, GLfloat givenY);


	/**
	* Changes the Plot's frame of reference.
	*
	* @param minX,	An GLfloat representing the new minimum X coordinate.
	* @param minY,	An GLfloat representing the new minimum Y coordinate.
	* @param maxX,	An GLfloat representing the new maximum X coordinate.
	* @param maxY,	An GLfloat representing the new maximum Y coordinate.
	* @return,					Void.
	*/
	void changeReferenceFrame(GLfloat minX, GLfloat minY, GLfloat maxX, GLfloat maxY);


	/**
	* Checks whether the given coordinates are within the bounds of the Plot.
	* NOTE: All values must be between -1.0f and 1.0f.
	*
	* @param xpos,	An GLfloat representing the X coordinate.
	* @param ypos,	An GLfloat representing the Y coordinate.
	* @return,					Void.
	*/
	bool validClick(GLfloat xpos, GLfloat ypos);


	// -- Flag methods
	int getWindowFlag();
	int getMethodFlag();
	int getFilterFlag();
	bool getDetrendFlag();
	bool getNormalizeFlag();
	void setWindowFlag(int flag);
	void setMethodFlag(int flag);
	void setFilterFlag(int flag);
	void setDetrendFlag(bool flag);
	void setNormalizeFlag(bool flag);


private:
	// -- World locations. Values must be between -1.0 and 1.0.
	GLfloat centerX, centerY, width, height;
	GLfloat left, right, top, bottom;

	// -- vertex arrays.
	GLfloat* vertexPlotArray;
	GLfloat* vertexDataArray;
	GLfloat* rawDataArray;

	// -- Plot parameters
	int WINDOWFLAG, METHODFLAG, FILTERFLAG;
	bool DETREND, NORMALIZE;

	int plotSize, dataSize, rawSize, labelSize;
	int ROWS, COLS;
	bool isXAxisLinear, isYAxisLinear;

	// -- text objects.
	std::string title, xAxisLabel, yAxisLabel;
	std::vector<TextLabel*> text;

	//GLfloat refMinX, refMinY, refMaxX, refMaxY;
	
	/**
	* Calculates top, left, bottom, right values.
	*
	* @return,					Void.
	*/
	void updateBounds();

	/**
	* Fills vertexPlotArray with GLfloats used by OpenGL.
	*
	* @return,					Void.
	*/
	void fillGridVertexArray();
	

	/**
	* Fills vertexDataArray with GLfloats used by OpenGL.
	*
	* @return,					Void.
	*/
	void fillDataVertexArray();


	/**
	* Fills 'text' vector with TextLabels.
	*
	* @return,					Void.
	*/
	void fillText();


	/**
	* Initializes vertexPlotArray dynamically to plotSize * sizeof(GLfloat).
	*
	* @return,					Void.
	*/
	void initGridVertexArray();


	/**
	* Initializes vertexDataArray dynamically to dataSize * sizeof(GLfloat).
	*
	* @return,					Void.
	*/
	void initDataVertexArray();

};


#endif // !PLOT_H

