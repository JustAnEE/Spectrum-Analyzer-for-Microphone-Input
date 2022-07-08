#include "Plot.h"


/* PUBLIC METHODS */
Plot::Plot( GLfloat centX, GLfloat centY, GLfloat w, GLfloat h, int rows, int cols){

	centerX = centX;	centerY = centY;
	width = w;		height = h;
	ROWS = rows;	COLS = cols;

	refMinX = 0.0;	refMaxX = 0.0;
	refMinY = 0.0;	refMaxY = 0.0;

	isXAxisLinear = true;
	isYAxisLinear = true;

	updateBounds();

	// -- initialize the vertex array.
	vertexDataArray = nullptr;
	dataSize = 0;

	initGridVertexArray();
	fillGridVertexArray();
	fillText();
}

Plot::~Plot(){
	free(vertexPlotArray);
	free(vertexDataArray);
	free(rawDataArray);
}


void Plot::setMethodFlag(int flag) { 
	// -- Also sets the reference frame and labels. (move this to a seperate function.)
	switch (flag) {
		case 0:
			this->setTitle("Time Series");
			this->setAxisLables("t", "A");
			refMinX = 0.0f;	refMaxX = 1.0f;
			refMinY = -128.0f;	refMaxY = 128.0f;
			isXAxisLinear = true;
			isYAxisLinear = true;
			break;
		case 1:
			this->setTitle("Magnitude Response");
			this->setAxisLables("f(Hz)", "|X(f)|");
			refMinX = -1000.0f;	refMaxX = 1000.0f;
			refMinY =  0.0f;	refMaxY = 10000.0f;
			isXAxisLinear = true;
			isYAxisLinear = true;
			break;
		case 2:
			this->setTitle("Decibel Magnitude Response");
			this->setAxisLables("f(Hz)", "dB");
			refMinX = 0.0f;	refMaxX = 10000.0f;
			refMinY = -90.0f;	refMaxY = 180.0f;
			isXAxisLinear = false;
			isYAxisLinear = true;
			break;
		case 3:
			this->setTitle("Power Spectral Density");
			this->setAxisLables("f(Hz)", "A");
			refMinX = -1000.0f;	refMaxX = 1000.0f;
			refMinY = -0.0f;	refMaxY = 500.0f;
			isXAxisLinear = true;
			isYAxisLinear = true;
			break;
		default:
			cout << "Invalid function flag in SpectrumModel::addPlot." << endl;
			exit(-69);
			break;
	}

	fillDataVertexArray();
	fillText();

	METHODFLAG = flag; 
}
void Plot::setWindowFlag(int flag) { WINDOWFLAG = flag; }
void Plot::setFilterFlag(int flag) { FILTERFLAG = flag; }
void Plot::setDetrendFlag(bool flag)   { DETREND = flag; }
void Plot::setNormalizeFlag(bool flag) { NORMALIZE = flag; };
int  Plot::getWindowFlag()  { return WINDOWFLAG; }
int  Plot::getMethodFlag()  { return METHODFLAG; }
int  Plot::getFilterFlag()  { return FILTERFLAG; }
bool Plot::getDetrendFlag() { return DETREND; }
bool Plot::getNormalizeFlag() { return NORMALIZE; }

void Plot::setRowsAndCols(int numRows, int numCols){
	ROWS = numRows;
	COLS = numCols;

	initGridVertexArray();
	fillGridVertexArray();
	fillText();
}

void Plot::setRawData(GLfloat* vertexArrayPtr, int size){
	free(rawDataArray);
	 
	rawSize = size;
	rawDataArray = (GLfloat*)calloc(rawSize, sizeof(GLfloat));

	for (int i = 0; i < size; i++) {
		rawDataArray[i] = vertexArrayPtr[i];
	}

	initDataVertexArray();
	fillDataVertexArray();
}


void Plot::setTitle(string newTitle){
	title = newTitle;
	fillText();
}
void Plot::setAxisLables(string xLabel, string yLabel){
	xAxisLabel = xLabel;
	yAxisLabel = yLabel;
	fillText();
}

vector<TextLabel*> Plot::getText(){ return text; }
GLfloat Plot::getHeight(){ return height; }
GLfloat Plot::getWidth() { return width; }
GLfloat* Plot::getVertexDataArray() { return vertexDataArray; }
GLfloat* Plot::getVertexPlotArray() { return vertexPlotArray; }
int Plot::getDataSize(){ return dataSize; }
int Plot::getPlotSize(){ return plotSize; }



void Plot::changeReferenceFrame(GLfloat minX, GLfloat minY, GLfloat maxX, GLfloat maxY) {
	refMinX = minX;	refMaxX = maxX;
	refMinY = minY;	refMaxY = maxY;
	fillDataVertexArray();
	fillText();
}

void Plot::scalePlot(GLfloat givenW, GLfloat givenH){
	width += givenW;
	height += givenH;
	updateBounds();
	fillGridVertexArray();
	fillDataVertexArray();
	fillText();
}

void Plot::movePlot(GLfloat givenX, GLfloat givenY){
	centerX = givenX;
	centerY = givenY;
	updateBounds();
	fillGridVertexArray();
	fillDataVertexArray();
	fillText();
}

bool Plot::validClick(GLfloat givenX, GLfloat givenY){
	return (givenX >= left && givenX <= right && givenY >= bottom && givenY <= top);
}





void Plot::initGridVertexArray() {
	free(vertexPlotArray);
	vertexPlotArray = (GLfloat*)calloc(12 * (ROWS + COLS), sizeof(GLfloat));
	plotSize = sizeof(GLfloat) * 12 * (ROWS + COLS);
}

void Plot::initDataVertexArray() {
	free(vertexDataArray);
	dataSize = rawSize * 3;
	vertexDataArray = (GLfloat*)calloc(dataSize, sizeof(GLfloat));
}


void Plot::fillGridVertexArray(){
	GLfloat absOffsetX, absOffsetY;
	GLfloat x = left, y = top;

	absOffsetX = width / static_cast<float>(COLS);
	absOffsetY = height / static_cast<float>(ROWS);
	
	// -- Holy Smokes this is some duplicate code!
	// -- Fill border data.
	for (int i = 0; i < 24; i += 12) { 
		if (i == 0) {
			vertexPlotArray[  i  ] = left;	 vertexPlotArray[i + 1] = top;				vertexPlotArray[i + 2] = 0.0f; // xyz vert1
			vertexPlotArray[i + 6] = left;	 vertexPlotArray[i + 7] = bottom - 0.05f;	vertexPlotArray[i + 8] = 0.0f; // xyz vert2
		}
		else {
			vertexPlotArray[  i  ] =  left - 0.05f;	 vertexPlotArray[i + 1] = bottom;	vertexPlotArray[i + 2] = 0.0f; // xyz vert1
			vertexPlotArray[i + 6] = right;	 vertexPlotArray[i + 7] = bottom;	vertexPlotArray[i + 8] = 0.0f; // xyz vert2
		}
		vertexPlotArray[i + 3] = 1.0f;		 vertexPlotArray[i + 4] = 1.0f;		vertexPlotArray[i + 5] = 1.0f; // rgb vert1
		vertexPlotArray[i + 9] = 1.0f;		 vertexPlotArray[i + 10] = 1.0f;	vertexPlotArray[i + 11] = 1.0f; // rgb vert2
	}

	const int rowVerts = (12 * (ROWS - 1)) + 24;
	const int totalVerts = 12 * (ROWS + COLS) - 12;
	// -- Fill internal grid line data.
	for (int i = 24; i < rowVerts; i += 12) { // -- rows
		y -= absOffsetY;
		vertexPlotArray[i] = left;			vertexPlotArray[i + 1] = y;		vertexPlotArray[i + 2] = 0.0f; // xyz vert1
		vertexPlotArray[i + 6] = right;		vertexPlotArray[i + 7] = y;		vertexPlotArray[i + 8] = 0.0f; // xyz vert2
		vertexPlotArray[i + 3] = 0.3f;		vertexPlotArray[i + 4] = 0.3f;	vertexPlotArray[i + 5] = 0.3f; // rgb vert1
		vertexPlotArray[i + 9] = 0.2f;		vertexPlotArray[i + 10] = 0.2f;	vertexPlotArray[i + 11] = 0.2f; // rgb vert2
	}
	for (int i = rowVerts; i <= totalVerts; i += 12) {  // -- cols
		x += absOffsetX;
		vertexPlotArray[i] = x;			vertexPlotArray[i + 1] = top;		vertexPlotArray[i + 2] = 0.0f; // xyz vert1
		vertexPlotArray[i + 6] = x;	    vertexPlotArray[i + 7] = bottom;	vertexPlotArray[i + 8] = 0.0f; // xyz vert2
		vertexPlotArray[i + 3] = 0.3f;	vertexPlotArray[i + 4] = 0.3f;		vertexPlotArray[i + 5] = 0.3f; // rgb vert1
		vertexPlotArray[i + 9] = 0.2f;	vertexPlotArray[i + 10] = 0.2f;		vertexPlotArray[i + 11] = 0.2f; // rgb vert2
	}
}


void Plot::fillDataVertexArray() {
	// -- switch to proccess from raw data
	for (int i = 0; i < dataSize; i += 6) {
		int rawIdx = i / 3;
		GLfloat x = 0;
		GLfloat y = 0;

		// -- Linear / log PLOTTING
		if (isXAxisLinear) {
			x = ((rawDataArray[rawIdx] - refMinX) / (refMaxX - refMinX) * width) + left;
		}
		else {
			x = (log10(rawDataArray[rawIdx] - refMinX) / log10(refMaxX - refMinX) * width) + left;
		}

		if (isYAxisLinear) {
			y = ((rawDataArray[rawIdx + 1] - refMinY) / (refMaxY - refMinY) * height) + bottom;
		}
		else{
			y = ( log10(rawDataArray[rawIdx + 1] - refMinY) / log10(refMaxY - refMinY) * height) + bottom;
		}
		

		// -- SMUSHING NOT GOOD need to remove the points from rendering buffer.
		if (y < bottom) { y = bottom; }
		else if (y > top) { y = top; }
		if (x < left) { x = left; }
		else if (x > right) { x = right; }

		vertexDataArray[i + 0] = x;
		vertexDataArray[i + 1] = y;
		vertexDataArray[i + 2] = 0.0f;// Z
		vertexDataArray[i + 3] = 0.0f;// R
		vertexDataArray[i + 4] = 1.0f;// G
		vertexDataArray[i + 5] = 0.0f;// B
	}
}

void Plot::fillText() {
	text.clear();

	GLfloat y = bottom;
	GLfloat x = left;
	GLfloat absOffsetX = width / static_cast<float>(COLS);
	GLfloat absOffsetY = height / static_cast<float>(ROWS);
	GLfloat deltaX = (isXAxisLinear) ? (refMaxX - refMinX) / static_cast<float>(COLS) : 1.0f;
	GLfloat deltaY = (isYAxisLinear) ? (refMaxY - refMinY) / static_cast<float>(ROWS) : 1.0f;

	// -- Title and unit labels.
	text.push_back(new TextLabel(glm::vec3(centerX, top + 0.03, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), width * 0.75, title));
	text.push_back(new TextLabel(glm::vec3(right + 0.1, bottom, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1, xAxisLabel));
	text.push_back(new TextLabel(glm::vec3(left, top + 0.03, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1, yAxisLabel));

	// -- Push max and min values
	text.push_back(new TextLabel(glm::vec3(left - 0.1, bottom, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1, to_string((int)refMinY)));
	text.push_back(new TextLabel(glm::vec3(left - 0.1,    top, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1, to_string((int)refMaxY)));
	text.push_back(new TextLabel(glm::vec3(right, bottom - 0.1, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1, to_string((int)refMaxX)));
	text.push_back(new TextLabel(glm::vec3( left, bottom - 0.1, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1, to_string((int)refMinX)));

	// -- fill main positions and calculate label strings
	for (int i = 1; i < ROWS; i++) {
		y += absOffsetY;

		int value = (int)(refMinY + (i * deltaY));
		if (!isYAxisLinear) { 
			deltaY *= 10.0f;
			value = deltaY;
		}

		text.push_back(new TextLabel(glm::vec3(left-0.1, y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1, to_string(value)));
	}

	for (int i = 1; i < COLS; i++) {
		x += absOffsetX;

		int value = (int)(refMinX + (i * deltaX));
		if (!isXAxisLinear) {
			deltaX *= 10.0f;
			value = deltaX;
		}

		text.push_back(new TextLabel(glm::vec3(x, bottom - 0.1, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1, to_string(value)));
	}

}


void Plot::updateBounds() {
	left = centerX - (width / 2);
	right = centerX + (width / 2);
	top = centerY + (height / 2);
	bottom = centerY - (height / 2);
}
