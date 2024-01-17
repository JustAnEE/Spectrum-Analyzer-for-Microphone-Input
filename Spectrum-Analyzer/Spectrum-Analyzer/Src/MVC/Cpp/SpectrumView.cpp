#include "../Hpp/SpectrumView.hpp"



SpectrumView::SpectrumView(GLuint _windowWidth, GLuint _windowHeight){
	fSys = new FontSystem("./fonts/monofonto.otf", 32, "FONTATLAS.bmp");
	fSys->generateFontAtlas();

	windowWidth = _windowWidth;
	windowHeight = _windowHeight;

	// -- Read glsl files.
	std::string VSSinput = readShaderCode("./Src/Shaders/VSS.glsl");
	std::string FSSinput = readShaderCode("./Src/Shaders/FSS.glsl");
	std::string TVSSinput = readShaderCode("./Src/Shaders/TVSS.glsl");
	std::string TFSSinput = readShaderCode("./Src/Shaders/TFSS.glsl");
	const char* VSS = VSSinput.c_str();
	const char* FSS = FSSinput.c_str();
	const char* TVS = TVSSinput.c_str();
	const char* TFS = TFSSinput.c_str();

	glfwInit();

	// --  Define OpenGL version MACROS.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// --  Initialize window.
	window = glfwCreateWindow(windowWidth, windowHeight, "Spectrum Analyizer", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGL();

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Compile each shader's source code into a program. */
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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

	GLuint textureVertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint textureFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	textureShaderProgram = glCreateProgram();
	glShaderSource(textureVertexShader, 1, &TVS, NULL);
	glShaderSource(textureFragmentShader, 1, &TFS, NULL);
	glCompileShader(textureVertexShader);
	glCompileShader(textureFragmentShader);
	glAttachShader(textureShaderProgram, textureVertexShader);
	glAttachShader(textureShaderProgram, textureFragmentShader);
	glLinkProgram(textureShaderProgram);
	glDeleteShader(textureVertexShader);	// -- delete shader after compilation.
	glDeleteShader(textureFragmentShader);	// -- delete shader after compilation.
	

	glGenTextures(1, &atlasID);
	glBindTexture(GL_TEXTURE_2D, atlasID);
	// -- Set textures options.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		fSys->getAtlasWidth(),
		fSys->getAtlasHeight(),
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		fSys->m_atlasData
	);

	//glBindTexture(GL_TEXTURE_2D, 0);

	// -- Generate text buffer of size 50.
	glGenVertexArrays(1, &TVAO);
	glGenBuffers(1, &TVBO);
	glBindVertexArray(TVAO);
	glBindBuffer(GL_ARRAY_BUFFER, TVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 5 * 50, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glViewport(0, 0, windowWidth, windowHeight);


	// -- adding widgets
	Button* b1 = new Button(glm::vec3(-0.8f, 0.9f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.2f, 0.1f, "Methods", "Method");
	Button* b2 = new Button(glm::vec3(-0.5f, 0.9f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.2f, 0.1f, "Windows", "Window");
	Button* b3 = new Button(glm::vec3(-0.2f, 0.9f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.2f, 0.1f, "Filters", "Filter");
	Button* b4 = new Button(glm::vec3( 0.1f, 0.9f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.2f, 0.1f, "Detrend", "Detrend");


	methods = { "Time series", "Frequency Response", "dB Magnitude Response", "Power spectrum"};
	windows = { "Hamming", "Blackman", "Barlett", "Rectangular"};
	filters = { "A", "B",  "C",  "D",  "E",  "F...",  "G...",  "H...",  "I...",  "J...", };

	ListMenu* menu = new ListMenu(glm::vec3(0.75f, -0.25f, 0.0f), 0.4f, 1.25f, "Methods", methods, "Method");

	widgets.push_back(b1);
	widgets.push_back(b2);
	widgets.push_back(b3);
	widgets.push_back(b4);
	widgets.push_back(menu);

}

SpectrumView::~SpectrumView(){
	/* Memory Management */
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &TVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &TVBO);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(textureShaderProgram);
	//glDeleteBuffers(1, &EBO);
	glfwDestroyWindow(window);

	delete fSys;

	glfwTerminate();
}




void SpectrumView::swapListMenu(std::string menuID){
	if (menuID == "Method") { ((ListMenu*)widgets[4])->setText(menuID, methods); }
	else if (menuID == "Window") { ((ListMenu*)widgets[4])->setText(menuID, windows); }
	else if (menuID == "Filter") { ((ListMenu*)widgets[4])->setText(menuID, filters); }
}


void SpectrumView::draw(){
	// -- Draw background.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// -- Draw Plots.
	for (Plot* plot : model->getPlotVector()) {
		drawPlot(plot);

		// -- Draw Plot text.
		for (TextLabel* text : plot->getText()) {
			drawText(text);
		}

		// -- Draw selection Box if applicable.

	}

	// -- Draw Widgets.
	for (Widget* w : widgets) {
		drawWidget(w);
	}
	//drawText(new TextLabel(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), 34, "test"));
	glfwSwapBuffers(window);
}


void SpectrumView::drawWidget(Widget* widget) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, widget->getVertexBufferSizeBytes(), widget->getVertexBuffer(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, widget->getVertexBufferSizeBytes() / (6 * sizeof(float)));

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	for (TextLabel* t : widget->getTextList()) {
		drawText(t);
	}
}




void SpectrumView::drawPlot(Plot* plot) {
	/* Draw Plot structure. */

	// -- Gen Buffers.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, plot->getPlotSize(), plot->getVertexPlotArray(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, plot->getPlotSize() / (6 * sizeof(GLfloat)));

	// -- Delete buffers.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	/* Draw Plot data. */
	if (plot->getVertexDataArray() != nullptr) {
		// -- Gen Buffers.
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, plot->getDataSize() * 4, plot->getVertexDataArray(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, plot->getDataSize() / 6);

		// -- Delete buffers.
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

}


void SpectrumView::drawText(TextLabel* textLabel) {
	// -- Bind textures. 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlasID);

	// -- Bind buffers
	glBindVertexArray(TVAO);
	glBindBuffer(GL_ARRAY_BUFFER, TVBO);

	// -- Set uniforms
	glUniform3f(glGetUniformLocation(textureShaderProgram, "textColor"), textLabel->color.r, textLabel->color.g, textLabel->color.b);
	
	std::string text = textLabel->text;

	float span = 0.0f;
	for (int i = 0; i < text.size(); i++) {
		span += (fSys->getCharacterInformation(text[i]).advance >> 6);
	}

	// -- Calculate how much to scale the text to fit the label.
	//float scale = (textLabel->width / 2.0f * 800) / span;
	float scale = 0.5f;

	// -- Pixel world locations of TextLabel position.
	float x = (textLabel->origin.x + 1.0f) * (windowWidth / 2.0f)-(span / 2.0f * scale);
	float y = (textLabel->origin.y + 1.0f) * (windowHeight / 2.0f);


	// iterate through all characters
	int offset = 0;
	for (int i = 0; i < text.size(); i++) {
		FontSystem::Glyph ch = fSys->getCharacterInformation(text[i]);

		float xpos = x + ch.bearingX;
		float ypos = y - (ch.height - ch.bearingY)*scale;
		float z = textLabel->origin.z;

		// -- Normalized position to -1.0 to 1.0
		xpos = ((2.0f * xpos) / windowWidth) - 1.0f;
		ypos = ((2.0f * ypos) / windowHeight) - 1.0f;
		float w = (ch.width * scale) / (windowWidth / 2.0f);
		float h = (ch.height * scale) / (windowHeight / 2.0f);

		// -- Load each char quad into the buffer.
		GLfloat vertices[6][5] =
		{
			{ xpos,     ypos + h,	z,      ch.atlasLeft ,      ch.atlasTop    },
			{ xpos,     ypos,       z,      ch.atlasLeft ,      ch.atlasBottom },
			{ xpos + w, ypos,       z,      ch.atlasRight,      ch.atlasBottom },

			{ xpos,     ypos + h,   z,      ch.atlasLeft ,      ch.atlasTop    },
			{ xpos + w, ypos,       z,      ch.atlasRight,      ch.atlasBottom },
			{ xpos + w, ypos + h,   z,      ch.atlasRight,      ch.atlasTop    }
		};
		
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(vertices), vertices);

		x += (ch.advance >> 6) * scale;
		offset += sizeof(vertices);
	}
	glUseProgram(textureShaderProgram);
	glDrawArrays(GL_TRIANGLES, 0, text.length() * 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}



GLFWwindow* SpectrumView::getWindow() { 
	return window; 
}


void SpectrumView::setModels(SpectrumModel* _model, InteractionModel* _IModel) {
	model = _model;
	IModel = _IModel;
}


void SpectrumView::detectClick(double xpos, double ypos) {
	// -- Yucky click detection method in the view
	for (Widget* w : widgets) {
		int result = w->detectClick(xpos, ypos);
		if (result != -1) {
			// -- call widgets callback function pointer.
			w->clickAction(result);
		}
	}
}


void SpectrumView::detectScroll(double xpos, double ypos, int direction) {
	((ListMenu*)widgets[4])->scrollOptions(xpos, ypos, direction);
}


void SpectrumView::setController(SpectrumController* _controller){
	// -- GLFW Event Handling
	glfwSetWindowUserPointer(window, _controller);

	auto mouseClick = [](GLFWwindow* window, int button, int action, int mods) {
		static_cast<SpectrumController*>(glfwGetWindowUserPointer(window))->handleMouseClick(window, button, action, mods);
	};

	auto keyPress = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		static_cast<SpectrumController*>(glfwGetWindowUserPointer(window))->handleKeyPressed(window, key, scancode, action, mods);
	};

	auto mouseScroll = [](GLFWwindow* window, double xoffset, double yoffset) {
		static_cast<SpectrumController*>(glfwGetWindowUserPointer(window))->handleMouseScroll(window, xoffset, yoffset);
	};


	// -- Setting controllers
	for (Widget* w : widgets) {
		w->setController(_controller);
	}

	// -- Setting widget callback functions
	widgets[0]->setClickCallback(&(SpectrumController::handleListButton));
	widgets[1]->setClickCallback(&(SpectrumController::handleListButton));
	widgets[2]->setClickCallback(&(SpectrumController::handleListButton));
	widgets[3]->setClickCallback(&(SpectrumController::handleBooleanButton));
	widgets[4]->setClickCallback(&(SpectrumController::handleListMenu));


	glfwSetMouseButtonCallback(window, mouseClick);
	glfwSetKeyCallback(window, keyPress);
	glfwSetScrollCallback(window, mouseScroll);
}


std::string SpectrumView::readShaderCode(const char* filename) {
	std::string shader, line;
	std::ifstream inputFile(filename);
	if (!inputFile.good()) {
		std::cout << "FAILED TO LOAD FILE: " << filename << " IN readShaderCode()" << std::endl;
		exit(-42);
	}
	while (getline(inputFile, line)) {
		shader.append(line);
		shader.append("\n");
	}
	return shader;
}



void SpectrumView::DModelChanged() { 
	draw(); 
}

void SpectrumView::IModelChanged() { 
	if (IModel->getCurrentListMenuID() != ((ListMenu*)widgets[4])->getID()) {
		swapListMenu(IModel->getCurrentListMenuID());
	}
	((ListMenu*)widgets[4])->setSelectedOption(IModel->getCurrentListMenuOption());
	draw(); 
}
