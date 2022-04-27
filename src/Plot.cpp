#include "Plot.h"



Plot::Plot(GLfloat minX, GLfloat maxX, GLfloat minY, GLfloat maxY, SCALE scale){
	left = minX;	right  = maxX;
	top  = minY;	bottom = maxY;
	curScale = scale;
	// -- set up defaults
	init(10.0f, 10.0f, curScale);
}

Plot::~Plot(){
	delete[] plot;
}




void Plot::setReferenceFrame(GLfloat minX, GLfloat minY, GLfloat maxX, GLfloat maxY){
}

void Plot::setRowsAndCols(int numRows, int numCols){
	init(numRows, numCols, curScale);
}

GLfloat* Plot::getReferenceFrameArray(){
	GLfloat ref[4] = { refMinX,refMaxX, refMinY, refMaxY };
	return ref;
}

GLfloat* Plot::getPlotPositionArray(){
	GLfloat position[4] = { left, right, top, bottom };
	return position;
}

GLfloat* Plot::getPlotVertexArray(){
	return plot;
}

void Plot::scalePlot(GLfloat givenW, GLfloat givenH){
}

void Plot::movePlot(GLfloat givenX, GLfloat givenY){
}

void Plot::validClick(float x, float y){
}





void Plot::init(int ROWS, int COLS, SCALE scale){
	// -- optimal for GL_LINE_STRIP
	plot = (GLfloat*) calloc( 12 * (ROWS + COLS), sizeof(GLfloat));
	
	GLfloat absOffsetX, absOffsetY;
	GLfloat x = right - left, y = top - bottom;

	// -- Scale
	if (scale == LINEAR) {
		// -- type weirdness?? probs
		absOffsetX = x / static_cast<float>(COLS);
		absOffsetY = y / static_cast<float>(ROWS);
	}
	else{
		absOffsetX = x / COLS;
		absOffsetY = y / ROWS;
	}

	// -- fill the plot vertex array.
	for (int i = 0; i < 12 * ROWS; i += 12) {
		plot[i] = left;			plot[i + 1] = y;		plot[i + 2] = 0.0f; // xyz vert1
		plot[i + 6] = right;	plot[i + 7] = y;		plot[i + 8] = 0.0f; // xyz vert2
		plot[i + 3] = 1.0f;		plot[i + 4] = 1.0f;		plot[i + 5] = 1.0f; // rgb vert1
		plot[i + 9] = 0.2f;		plot[i + 10] = 0.2f;	plot[i + 11] = 0.2f; // rgb vert2
		y -= absOffsetY;
	}

	for (int i = 12 * ROWS; i < 12 * (ROWS + COLS); i += 12) {
		plot[i] = x;			plot[i + 1] = top;		plot[i + 2] = 0.0f; // xyz vert1
		plot[i + 6] = x;	    plot[i + 7] = bottom;	plot[i + 8] = 0.0f; // xyz vert2
		plot[i + 3] = 1.0f;		plot[i + 4] = 1.0f;		plot[i + 5] = 1.0f; // rgb vert1
		plot[i + 9] = 0.2f;		plot[i + 10] = 0.2f;	plot[i + 11] = 0.2f; // rgb vert2
		x -= absOffsetX;
	}
}
