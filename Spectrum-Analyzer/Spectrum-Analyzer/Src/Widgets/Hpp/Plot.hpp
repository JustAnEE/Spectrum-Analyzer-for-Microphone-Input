#ifndef PLOT_H
#define PLOT_H

#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../Hpp/TextLabel.hpp"

class Plot
{
public:
    GLfloat refMinX, refMinY, refMaxX, refMaxY;

    Plot(GLfloat centX, GLfloat centY, GLfloat w, GLfloat h, int rows, int cols );
    ~Plot();

    std::vector<TextLabel*> getText();

    void setRowsAndCols(int numRows, int numCols);
    void setRawData(GLfloat* vertexArrayPtr, int size);
    void setTitle(std::string newTitle);
    void setAxisLables(std::string xAxisLabel, std::string yAxisLabel);
    GLfloat getHeight();
    GLfloat getWidth();
    GLfloat* getVertexDataArray();
    GLfloat* getVertexPlotArray();
    int getDataSize();
    int getPlotSize();
    void scalePlot(GLfloat givenW, GLfloat givenH);
    void movePlot(GLfloat givenX, GLfloat givenY);
    void changeReferenceFrame(GLfloat minX, GLfloat minY, GLfloat maxX, GLfloat maxY);
    bool validClick(GLfloat xpos, GLfloat ypos);

    // Flag methods
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
    // World locations. Values must be between -1.0 and 1.0.
    GLfloat centerX, centerY, width, height;
    GLfloat left, right, top, bottom;

    // vertex arrays.
    GLfloat* vertexPlotArray;
    GLfloat* vertexDataArray;
    GLfloat* rawDataArray;

    // Plot parameters
    int WINDOWFLAG, METHODFLAG, FILTERFLAG;
    bool DETREND, NORMALIZE;

    int plotSize, dataSize, rawSize, labelSize;
    int ROWS, COLS;
    bool isXAxisLinear, isYAxisLinear;

    // text objects.
    std::string title, xAxisLabel, yAxisLabel;
    std::vector<TextLabel*> text;

    //GLfloat refMinX, refMinY, refMaxX, refMaxY;

    void updateBounds();

    void fillGridVertexArray();

    void fillDataVertexArray();

    void fillText();

    void initGridVertexArray();

    void initDataVertexArray();
};
#endif // !PLOT_H
