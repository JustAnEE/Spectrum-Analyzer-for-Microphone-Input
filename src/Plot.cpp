#include "Plot.h"


/* PUBLIC METHODS */
Plot::Plot( GLfloat centX,	  GLfloat centY,	GLfloat w,		  GLfloat h,
	        GLfloat refminX,  GLfloat refminY,  GLfloat refmaxX,  GLfloat refmaxY,
	        int rows,		  int cols,		    bool XLinear,     bool YLinear){

	centerX = centX;	centerY = centY;
	width = w;		height = h;
	ROWS = rows;	COLS = cols;

	isXAxisLinear = XLinear;
	isYAxisLinear = YLinear;

	refMinX = refminX;	refMaxX = refmaxX;
	refMinY = refminY;	refMaxY = refmaxY;

	// -- initialize the vertex array.
	vertexDataArray = nullptr;
	dataSize = 0;

	initGridVertexArray();
	initRowColLabelVectors();
	fillRowColLabelVectors();
	fillGridVertexArray();
}

//Plot::~Plot(){
//	delete[] plot;
//	delete[] data;
//}


void Plot::setRowsAndCols(int numRows, int numCols){
	ROWS = numRows;
	COLS = numCols;

	initRowColLabelVectors();
	initGridVertexArray();
	initRowColLabelVectors();
	fillRowColLabelVectors();
	fillGridVertexArray();
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
}

void Plot::setAxisLables(string xLabel, string yLabel){
	xAxisLabel = xLabel;
	yAxisLabel = yLabel;
}

string Plot::getTitle() { return title; }
string Plot::getXLabel() { return xAxisLabel; }
string Plot::getYLabel() { return yAxisLabel; }
GLfloat Plot::getTitleXPos() { return centerX - (0.01f * title.size() / 2.0f); }
GLfloat Plot::getTitleYPos() { return centerY + (height / 2.0f) + 0.025f; }
GLfloat Plot::getXLabelXPos() { return centerX + (width / 2.0f) + 0.025f; }
vector<GLfloat> Plot::getRowLabelsYPos(){ return rowLabelsYPos; }
vector<GLfloat> Plot::getColLabelsXPos(){ return colLabelsXPos; }
vector<string> Plot::getRowLabels(){ return rowLabels; }
vector<string> Plot::getColLabels(){ return colLabels; }
GLfloat Plot::getRowLabelXpos() { return centerX - (width / 2.0f) - 0.15f; }
GLfloat Plot::getColLabelYpos() { return centerY - (height / 2.0f) - 0.075f; }
GLfloat Plot::getXLabelYPos() { return centerY - (height / 2.0f); }
GLfloat Plot::getYLabelXPos() { return centerX - (width / 2.0f); }
GLfloat Plot::getYLabelYPos() { return centerY + (height / 2.0f) + 0.025f; }
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
	initRowColLabelVectors();
	fillRowColLabelVectors();
}

void Plot::scalePlot(GLfloat givenW, GLfloat givenH){
	width += givenW;
	height += givenH;
	fillGridVertexArray();
	fillDataVertexArray();
	initRowColLabelVectors();
	fillRowColLabelVectors();
}

void Plot::movePlot(GLfloat givenX, GLfloat givenY){
	centerX = givenX;
	centerY = givenY;
	fillGridVertexArray();
	fillDataVertexArray();
	initRowColLabelVectors();
	fillRowColLabelVectors();
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
	vertexPlotArray = (GLfloat*)calloc(12 * (ROWS + COLS), sizeof(GLfloat));
	plotSize = sizeof(GLfloat) * 12 * (ROWS + COLS);
}

void Plot::initDataVertexArray() {
	free(vertexDataArray);
	dataSize = rawSize * 3;
	vertexDataArray = (GLfloat*)calloc(dataSize, sizeof(GLfloat));
}

void Plot::initRowColLabelVectors() {
	// -- discregard contents, aquire indices.
	rowLabels.clear();
	colLabels.clear();
	rowLabelsYPos.clear();
	colLabelsXPos.clear();
}


void Plot::fillGridVertexArray(){

	GLfloat absOffsetX, absOffsetY;
	GLfloat left = centerX - (width / 2);
	GLfloat right = centerX + (width / 2);
	GLfloat top = centerY + (height / 2);
	GLfloat bottom = centerY - (height / 2);

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
	GLfloat left = centerX - (width / 2);
	GLfloat right = centerX + (width / 2);
	GLfloat bottom = centerY - (height / 2);
	GLfloat top = centerY + (height / 2);

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

void Plot::fillRowColLabelVectors() {

	GLfloat left = centerX - (width / 2);
	GLfloat right = centerX + (width / 2);
	GLfloat bottom = centerY - (height / 2);
	GLfloat top = centerY + (height / 2);

	GLfloat absOffsetX = width / static_cast<float>(COLS);
	GLfloat absOffsetY = height / static_cast<float>(ROWS);
	GLfloat deltaX = (isXAxisLinear) ? (refMaxX - refMinX) / static_cast<float>(COLS) : 1.0f;
	GLfloat deltaY = (isYAxisLinear) ? (refMaxY - refMinY) / static_cast<float>(ROWS) : 1.0f;
	

	rowLabels.push_back(to_string((int)refMinY));
	rowLabelsYPos.push_back(bottom);
	rowLabels.push_back(to_string((int)refMaxY));
	rowLabelsYPos.push_back(top);

	colLabels.push_back(to_string((int)refMinX));
	colLabelsXPos.push_back(left);
	colLabels.push_back(to_string((int)refMaxX));
	colLabelsXPos.push_back(right);


	// -- fill main positions and calculate label strings
	for (int i = 1; i < ROWS; i++) {
		bottom += absOffsetY;
		rowLabelsYPos.push_back(bottom);

		int value = (int)(refMinY + (i * deltaY));
		if (!isYAxisLinear) { 
			deltaY *= 10.0f;
			value = deltaY;
		}

		rowLabels.push_back(to_string(value));
	}

	for (int i = 1; i < COLS; i++) {
		left += absOffsetX;
		colLabelsXPos.push_back(left);

		int value = (int)(refMinX + (i * deltaX));
		if (!isXAxisLinear) {
			deltaX *= 10.0f;
			value = deltaX;
		}

		colLabels.push_back(to_string(value));
	}

}
