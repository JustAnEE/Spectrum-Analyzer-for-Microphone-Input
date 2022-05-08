#ifndef PLOT_H
#define PLOT_H
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

class Plot {
public:
	enum SCALE { LINEAR = 0, LOGARITHMIC = 1 };
	GLfloat refMinX, refMinY, refMaxX, refMaxY;

	// -- Plot constructor
	Plot(GLfloat centX, GLfloat centY, GLfloat w, GLfloat h, int rows, int cols, SCALE scale);
	//~Plot();

	void setRowsAndCols(int numRows, int numCols);
	void setRawData(GLfloat* vertexArrayPtr, int size);
	GLfloat getHeight();
	GLfloat getWidth();
	GLfloat* getVertexDataArray();
	GLfloat* getVertexPlotArray();
	int getDataSize();
	int getPlotSize();

	// -- graph methods
	void scalePlot(GLfloat givenW, GLfloat givenH);
	void movePlot(GLfloat givenX, GLfloat givenY);
	void changeReferenceFrame(GLfloat minX, GLfloat minY, GLfloat maxX, GLfloat maxY);

	// -- misc
	bool validClick(GLfloat xpos, GLfloat ypos);



private:
	// -- actual locations inside OpenGL window
	// -- given values should be normalized
	GLfloat centerX, centerY, width, height;
	GLfloat* vertexPlotArray;
	GLfloat* vertexDataArray;
	GLfloat* rawData;
	int plotSize, dataSize, rawSize;
	int ROWS, COLS;
	SCALE curScale;

	// -- reference frame
	//GLfloat refMinX, refMinY, refMaxX, refMaxY;

	void fillGridVertexArray (SCALE scale);
	void fillDataVertexArray();
	void initGridVertexArray();
	//void initRawDataArray();
	void initDataVertexArray();
};



#endif // !PLOT_H

