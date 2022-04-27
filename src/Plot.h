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
	Plot(GLfloat minX, GLfloat maxX, GLfloat minY, GLfloat maxY, SCALE scale);
	~Plot();

	void setReferenceFrame(GLfloat minX, GLfloat minY, GLfloat maxX, GLfloat maxY);
	void setRowsAndCols(int numRows, int numCols);
	GLfloat* getReferenceFrameArray();
	GLfloat* getPlotPositionArray();
	GLfloat* getPlotVertexArray();

	// -- graph methods
	void scalePlot(GLfloat givenW, GLfloat givenH);
	void movePlot(GLfloat givenX, GLfloat givenY);
	
	// -- misc
	void validClick(float x, float y);



private:
	// -- actual locations inside OpenGL window
	// -- given values should be normalized
	GLfloat left, right, top, bottom;
	GLfloat* plot;
	SCALE curScale;

	// -- reference frame
	GLfloat refMinX, refMinY, refMaxX, refMaxY;

	void init (int ROWS, int COLS, SCALE scale);
};



#endif // !PLOT_H

