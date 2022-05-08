#include "Plot.h"


/* PUBLIC METHODS */
Plot::Plot(GLfloat centX, GLfloat centY, GLfloat w, GLfloat h, int rows, int cols, SCALE scale){
	centerX = centX;	centerY = centY;
	width = w;		height = h;
	ROWS = rows;	COLS = cols;

	curScale = scale;

	refMinX = -10.0f;	refMaxX = 10.0f;
	refMinY = -10.0f;	refMaxY = 1000.0f;

	// -- initialize the vertex array.
	vertexDataArray = nullptr;
	dataSize = 0;
	initGridVertexArray();
	fillGridVertexArray(curScale);
}

//Plot::~Plot(){
//	delete[] plot;
//	delete[] data;
//}



void Plot::setReferenceFrame(GLfloat minX, GLfloat minY, GLfloat maxX, GLfloat maxY){
	refMinX = minX;	refMaxX = maxX;
	refMinY = minY;	refMaxY = maxY;
	fillDataVertexArray();
}

void Plot::setRowsAndCols(int numRows, int numCols){
	ROWS = numRows;
	COLS = numCols;
	// -- this is most likly not good.
	free(vertexPlotArray);
	
	initGridVertexArray();
	fillGridVertexArray(curScale);
}

void Plot::setRawData(GLfloat* vertexArrayPtr, int size){
	free(rawData);
	// -- vertexArray must contain X and Y values. 
	rawSize = size;
	rawData = (GLfloat*)calloc(rawSize, sizeof(GLfloat));

	// -- copy the data locally
	for (int i = 0; i < size; i++) {
		rawData[i] = vertexArrayPtr[i];
	}

	initDataVertexArray();
	fillDataVertexArray();
}


GLfloat Plot::getHeight(){
	return height;
}

GLfloat Plot::getWidth(){
	return width;
}

GLfloat* Plot::getVertexDataArray(){
	return vertexDataArray;
}

GLfloat* Plot::getVertexPlotArray() {
	return vertexPlotArray;
}

int Plot::getDataSize(){
	return dataSize;
}

int Plot::getPlotSize(){
	return plotSize;
}

void Plot::scalePlot(GLfloat givenW, GLfloat givenH){
	width += givenW;
	height += givenH;
	fillGridVertexArray(curScale);
	fillDataVertexArray();
}

void Plot::movePlot(GLfloat givenX, GLfloat givenY){
	centerX = givenX;
	centerY = givenY;
	fillGridVertexArray(curScale);
	fillDataVertexArray();
}

bool Plot::validClick(GLfloat givenX, GLfloat givenY){
	GLfloat left = centerX - (width / 2);
	GLfloat right = centerX + (width / 2);
	GLfloat top = centerY + (height / 2);
	GLfloat bottom = centerY - (height / 2);
	return (givenX >= left && givenX <= right && givenY >= bottom && givenY <= top);
}





void Plot::initGridVertexArray() {
	free(vertexPlotArray);
	vertexPlotArray = (GLfloat*)calloc(12 * (ROWS + COLS + 2), sizeof(GLfloat));
	plotSize = sizeof(GLfloat) * 12 * (ROWS + COLS + 2);
}

void Plot::initDataVertexArray() {
	free(vertexDataArray);
	dataSize = rawSize * 3;
	vertexDataArray = (GLfloat*)calloc(dataSize, sizeof(GLfloat));
}



void Plot::fillGridVertexArray(SCALE scale){
	// -- optimal for GL_LINE_STRIP
	GLfloat absOffsetX, absOffsetY;
	GLfloat left = centerX - (width / 2);
	GLfloat right = centerX + (width / 2);
	GLfloat top = centerY + (height / 2);
	GLfloat bottom = centerY - (height / 2);

	GLfloat x = right, y = top;

	// -- Scale
	if (scale == LINEAR) {
		absOffsetX = width / static_cast<float>(COLS);
		absOffsetY = height / static_cast<float>(ROWS);
	}
	else{
		absOffsetX = width / static_cast<float>(COLS);
		absOffsetY = height / static_cast<float>(ROWS);
	}
	
	/* fill the plot vertex array.*/
	// TODO: border is the same a each row col divider
	// TODO: I should probably switch seperate internal and border lines.
	// TODO: internal will be based on reference frame and SCALE.
	for (int i = 0; i < 12 * (ROWS + 1); i += 12) {
		vertexPlotArray[i] = left;			vertexPlotArray[i + 1] = y;		vertexPlotArray[i + 2] = 0.0f; // xyz vert1
		vertexPlotArray[i + 6] = right;		vertexPlotArray[i + 7] = y;		vertexPlotArray[i + 8] = 0.0f; // xyz vert2
		vertexPlotArray[i + 3] = 1.0f;		vertexPlotArray[i + 4] = 1.0f;	vertexPlotArray[i + 5] = 1.0f; // rgb vert1
		vertexPlotArray[i + 9] = 0.2f;		vertexPlotArray[i + 10] = 0.2f;	vertexPlotArray[i + 11] = 0.2f; // rgb vert2
		y -= absOffsetY;
	}
	for (int i = 12 * (ROWS + 1); i <= 12 * (ROWS + COLS + 1); i += 12) {
		vertexPlotArray[i] = x;			vertexPlotArray[i + 1] = top;		vertexPlotArray[i + 2] = 0.0f; // xyz vert1
		vertexPlotArray[i + 6] = x;	    vertexPlotArray[i + 7] = bottom;	vertexPlotArray[i + 8] = 0.0f; // xyz vert2
		vertexPlotArray[i + 3] = 1.0f;	vertexPlotArray[i + 4] = 1.0f;		vertexPlotArray[i + 5] = 1.0f; // rgb vert1
		vertexPlotArray[i + 9] = 0.2f;	vertexPlotArray[i + 10] = 0.2f;		vertexPlotArray[i + 11] = 0.2f; // rgb vert2
		x -= absOffsetX;
	}
}


void Plot::fillDataVertexArray() {
	// -- switch to proccess from raw data
	GLfloat left = centerX - (width / 2);
	for (int i = 0; i < dataSize; i += 6) {
		int rawIdx = i / 3;
		vertexDataArray[i + 0] = (rawData[ rawIdx ] * width) + left;
		vertexDataArray[i + 1] = (rawData[rawIdx+1] / (refMaxY - refMinY) * height) + centerY;
		vertexDataArray[i + 2] = 0.0f;// Z
		vertexDataArray[i + 3] = 0.0f;// R
		vertexDataArray[i + 4] = 1.0f;// G
		vertexDataArray[i + 5] = 0.0f;// B
	}
}
