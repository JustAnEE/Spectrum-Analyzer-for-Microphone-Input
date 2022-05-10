#include "SpectrumView.h"



SpectrumView::SpectrumView(){

	// -- TODO: seperate shader files.
	const char* VSS = "#version 330 core\n"
		"layout (location = 0) in vec3 pos;\n"
		"layout (location = 1) in vec3 color;\n"
		"out vec3 outColor;\n"
		"void main() {\n"
		"gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
		"outColor = color; }\0";


	const char* FSS = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 outColor;\n"
		"void main() { FragColor = vec4(outColor, 1.0f); }\n\0";

	glfwInit();

	// --  Define OpenGL version MACROS.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// --  Initialize window.
	window = glfwCreateWindow(800, 800, "Spectrum Anal", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGL();

	/* Compile each shader's source code into a program. */
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();
	glShaderSource(vertexShader, 1, &VSS, NULL);
	glShaderSource(fragmentShader, 1, &FSS, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);	// -- delete shader after compilation.
	glDeleteShader(fragmentShader);	// -- delete shader after compilation.

	glViewport(0, 0, 800, 800);
}

SpectrumView::~SpectrumView(){
	/* Memory Management */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	//glDeleteBuffers(1, &EBO);
	glfwDestroyWindow(window);

	glfwTerminate();
}



void SpectrumView::draw(){
	// -- Draw background.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (Plot* plot : model->plotVector) {
		drawPlot(plot);
	}

	glfwSwapBuffers(window);
}

void SpectrumView::drawPlot(Plot* plot) {
	// -- Draw Plot grid.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, plot->getPlotSize(), plot->getVertexPlotArray(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, plot->getPlotSize(), plot->getVertexPlotArray(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, (plot->getPlotSize() * 2) / (12 * sizeof(GLfloat)));

	// -- Draw Plot data.
	if (plot->getVertexDataArray() != nullptr) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, plot->getDataSize() * 4, plot->getVertexDataArray(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, plot->getDataSize() * 4, plot->getVertexDataArray(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, plot->getDataSize() / 6);
	}
}


GLFWwindow* SpectrumView::getWindow() {
	return window;
}

void SpectrumView::setModel(SpectrumModel* givenModel){
	model = givenModel;
}


void SpectrumView::setController(SpectrumController* givenController){
	// -- Event Handling
	glfwSetWindowUserPointer(window, givenController);

	auto mouseClick = [](GLFWwindow* window, int button, int action, int mods) {
		static_cast<SpectrumController*>(glfwGetWindowUserPointer(window))->handleMouseClick(window, button, action, mods);
	};

	auto keyPress = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		static_cast<SpectrumController*>(glfwGetWindowUserPointer(window))->handleKeyPressed(window, key, scancode, action, mods);
	};
	glfwSetMouseButtonCallback(window, mouseClick);
	glfwSetKeyCallback(window, keyPress);


	//glfwSetCursorPosCallback(window, handleMouseMovement);
}


void SpectrumView::modelChanged(){
	draw();
}
