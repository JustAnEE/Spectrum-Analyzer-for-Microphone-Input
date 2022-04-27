#ifndef PLOT_H
#define PLOT_H
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

class Plot {
public:
	enum SCALE { LINEAR = 0, LOGARITHMIC = 1 };

	// -- Plot constructor
	Plot(GLfloat centX, GLfloat centY, GLfloat w, GLfloat h, int rows, int cols, SCALE scale);
	~Plot();

	void setReferenceFrame(GLfloat minX, GLfloat minY, GLfloat maxX, GLfloat maxY);
	void setRowsAndCols(int numRows, int numCols);
	GLfloat getHeight();
	GLfloat getWidth();
	GLfloat* getReferenceFrameArray();
	GLfloat* getPlotPositionArray();
	GLfloat* getPlotVertexArray();
	int getPlotSize();

	// -- graph methods
	void scalePlot(GLfloat givenW, GLfloat givenH);
	void movePlot(GLfloat givenX, GLfloat givenY);
	
	// -- misc
	bool validClick(GLfloat xpos, GLfloat ypos);



private:
	// -- actual locations inside OpenGL window
	// -- given values should be normalized
	GLfloat centerX, centerY, width, height;
	GLfloat* plot;
	int plotSize;
	int ROWS, COLS;
	SCALE curScale;

	// -- reference frame
	GLfloat refMinX, refMinY, refMaxX, refMaxY;

	void fillVertexArray (SCALE scale);
	void initVertexArray();
};



#endif // !PLOT_H

