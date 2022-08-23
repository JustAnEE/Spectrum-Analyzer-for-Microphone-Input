#pragma once
#include <glad/glad.h>
#include <iostream>

class Filter {

private:
	int sampleRate, bufferSize, filterLength;
	float cutOff, TBW;
	GLfloat* filter;

public:
	Filter(int _sampleRate, int _bufferSize, float _cutOffHz, float _TBW);
	~Filter();
	void genLPF();
	void lowToHigh();
	GLfloat* convolve(GLfloat* in);
};
