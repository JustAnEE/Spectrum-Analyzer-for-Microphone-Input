#include "Plot.h"



Plot::Plot(GLfloat centX, GLfloat centY, GLfloat w, GLfloat h, int rows, int cols, SCALE scale){
	centerX = centX;	centerY = centY;
	width = w;		height = h;
	ROWS = rows;	COLS = cols;
	curScale = scale;

	// -- initialize the vertex array.
	initVertexArray();
	fillVertexArray(curScale);
}

Plot::~Plot(){
	delete[] plot;
}




void Plot::setReferenceFrame(GLfloat minX, GLfloat minY, GLfloat maxX, GLfloat maxY){
}

void Plot::setRowsAndCols(int numRows, int numCols){
	ROWS = numRows;
	COLS = numCols;
	// -- this is most likly not good.
	delete[] plot;
	
	initVertexArray();
	fillVertexArray(curScale);
}

GLfloat Plot::getHeight(){
	return height;
}

GLfloat Plot::getWidth(){
	return width;
}

GLfloat* Plot::getReferenceFrameArray(){
	GLfloat ref[4] = { refMinX,refMaxX, refMinY, refMaxY };
	return ref;
}

GLfloat* Plot::getPlotPositionArray(){
	GLfloat position[4] = { };
	return position;
}

GLfloat* Plot::getPlotVertexArray(){
	return plot;
}

int Plot::getPlotSize(){
	return plotSize;
}

void Plot::scalePlot(GLfloat givenW, GLfloat givenH){
}

void Plot::movePlot(GLfloat givenX, GLfloat givenY){
	centerX = givenX;
	centerY = givenY;
	fillVertexArray(curScale);
}

bool Plot::validClick(GLfloat givenX, GLfloat givenY){
	GLfloat left = centerX - (width / 2);
	GLfloat right = centerX + (width / 2);
	GLfloat top = centerY + (height / 2);
	GLfloat bottom = centerY - (height / 2);
	return (givenX >= left && givenX <= right && givenY >= bottom && givenY <= top);
}



void Plot::initVertexArray() {
	plot = (GLfloat*)calloc(12 * (ROWS + COLS + 2), sizeof(GLfloat));
	plotSize = sizeof(GLfloat) * 12 * (ROWS + COLS + 2);
}

void Plot::fillVertexArray(SCALE scale){
	// -- optimal for GL_LINE_STRIP
	GLfloat absOffsetX, absOffsetY;
	GLfloat left = centerX - (width / 2);
	GLfloat right = centerX + (width / 2);
	GLfloat top = centerY + (height / 2);
	GLfloat bottom = centerY - (height / 2);

	GLfloat x = right, y = top;

	// -- Scale
	if (scale == LINEAR) {
		// -- type weirdness?? probs
		absOffsetX = width / static_cast<float>(COLS);
		absOffsetY = height / static_cast<float>(ROWS);
	}
	else{
		absOffsetX = width / static_cast<float>(COLS);
		absOffsetY = height / static_cast<float>(ROWS);
	}
	
	/* fill the plot vertex array.*/
	// -- border is the same a each divider
	// -- I should probably switch internal lines to be based on the reference frame and display rules.
	for (int i = 0; i < 12 * (ROWS + 1); i += 12) {
		plot[i] = left;			plot[i + 1] = y;		plot[i + 2] = 0.0f; // xyz vert1
		plot[i + 6] = right;	plot[i + 7] = y;		plot[i + 8] = 0.0f; // xyz vert2
		plot[i + 3] = 1.0f;		plot[i + 4] = 1.0f;		plot[i + 5] = 1.0f; // rgb vert1
		plot[i + 9] = 0.2f;		plot[i + 10] = 0.2f;	plot[i + 11] = 0.2f; // rgb vert2
		y -= absOffsetY;
	}
	for (int i = 12 * (ROWS + 1); i <= 12 * (ROWS + COLS + 1); i += 12) {
		plot[i] = x;			plot[i + 1] = top;		plot[i + 2] = 0.0f; // xyz vert1
		plot[i + 6] = x;	    plot[i + 7] = bottom;	plot[i + 8] = 0.0f; // xyz vert2
		plot[i + 3] = 1.0f;		plot[i + 4] = 1.0f;		plot[i + 5] = 1.0f; // rgb vert1
		plot[i + 9] = 0.2f;		plot[i + 10] = 0.2f;	plot[i + 11] = 0.2f; // rgb vert2
		x -= absOffsetX;
	}
}
