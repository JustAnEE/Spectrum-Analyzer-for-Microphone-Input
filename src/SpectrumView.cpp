#include "SpectrumView.h"



SpectrumView::SpectrumView(){

	// -- Read glsl files.
	std::string VSSinput = readShaderCode("VSS.glsl");
	std::string FSSinput = readShaderCode("FSS.glsl");
	std::string TVSSinput = readShaderCode("TVSS.glsl");
	std::string TFSSinput = readShaderCode("TFSS.glsl");
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
	window = glfwCreateWindow(800, 800, "Spectrum Anal", NULL, NULL);
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


	glViewport(0, 0, 800, 800);
	

	// -- freetype
	FT_Library freetype;
	FT_Face face;

	FT_Init_FreeType(&freetype);
	FT_New_Face(freetype, "./fonts/monofonto.otf", 0, &face);
	FT_Set_Pixel_Sizes(face, 0, 14);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 32; c < 128; c++){
		// -- Load "c" as a font glyph.
		FT_Load_Char(face, c, FT_LOAD_RENDER);

		// -- Create a texture around "c".
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// -- Set textures options.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// -- Store "c".
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(freetype);
	
	// -- generate text buffer. (switch text to batch rendering not char by char.)
	glGenVertexArrays(1, &TVAO);
	glGenBuffers(1, &TVBO);
	glBindVertexArray(TVAO);
	glBindBuffer(GL_ARRAY_BUFFER, TVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	// -- adding widgets
	Button* b1 = new Button(glm::vec3(-0.8f, 0.9f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.2f, 0.1f, "Methods", "Method");
	Button* b2 = new Button(glm::vec3(-0.5f, 0.9f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.2f, 0.1f, "Windows", "Window");
	Button* b3 = new Button(glm::vec3(-0.2f, 0.9f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.2f, 0.1f, "Filters", "Filter");
	Button* b4 = new Button(glm::vec3( 0.1f, 0.9f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.2f, 0.1f, "Detrend", "Detrend");


	methods = { "Time series", "Frequency Response", "dB Magnitude Response", "Power spectrum", "2D Spectrogram"};
	windows = { "Hamming", "Blackman", "Barlett", "Rectangular"};
	filters = { "A", "B",  "C",  "D",  "E",  "F...",  "G...",  "H...",  "I...",  "J...", };

	ListMenu* menu = new ListMenu(glm::vec3(0.65f, -0.25f, 0.0f), 0.5f, 1.25f, "Methods", methods, "list");

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

	glfwTerminate();
}




void SpectrumView::detectClick(double xpos, double ypos) {
	// -- Yucky click detection method in the view
	for (Widget* w : widgets) {
		int result = w->detectClick(xpos, ypos);
		if (result != -1) {
			// -- call widgets callback function pointer.
			w->action(result);
		}
	}
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
		drawText(plot->getTitle(),  plot->getTitleXPos(), plot->getTitleYPos(), 1.0f);
		drawText(plot->getXLabel(), plot->getXLabelXPos(), plot->getXLabelYPos(), 1.0f);
		drawText(plot->getYLabel(), plot->getYLabelXPos(), plot->getYLabelYPos(), 1.0f);
		// -- row lables all have same x. like wise for cols with y
		for (int i = 0; i < plot->getRowLabelsYPos().size(); i++){
			drawText(plot->getRowLabels()[i], plot->getRowLabelXpos(), plot->getRowLabelsYPos()[i], 1.0f);
		}
		for (int i = 0; i < plot->getColLabelsXPos().size(); i++) {
			drawText(plot->getColLabels()[i], plot->getColLabelsXPos()[i], plot->getColLabelYpos(), 1.0f);
		}
	}

	// -- Draw Widgets.
	for (Widget* w : widgets) {
		drawWidget(w);
	}


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

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, plot->getDataSize() / 6);

		// -- Delete buffers.
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

}


void SpectrumView::drawText(TextLabel* textLabel) {
	// activate corresponding render state	
	glUseProgram(textureShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(TVAO);

	// -- set uniforms
	glUniform3f(glGetUniformLocation(textureShaderProgram, "textColor"), textLabel->color.r, textLabel->color.g, textLabel->color.b);
	
	string text = textLabel->text;


	float span = 0.0f;
	for (int i = 0; i < text.size(); i++) {
		span += (Characters[text[i]].Advance >> 6);
	}

	// -- Pixel world locations of TextLabel position.
	float x = ((textLabel->origin.x - textLabel->width / 2.0f) + 1.0f) * (600.0f / 2.0f);
	float y = (textLabel->origin.y + 1.0f) * (800.0f / 2.0f);

	// -- Calculate how much to scale the text to fit the label.
	float scale = (textLabel->width / 2.0f * 600) / span;
	float scale = 1.0f;

	// iterate through all characters
	for (int i = 0; i < text.size(); i++) {
		Character ch = Characters[text[i]];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		// -- Normalized position to -1.0 to 1.0
		xpos = ((2.0f * xpos) / 600.0f) - 1.0f;
		ypos = ((2.0f * ypos) / 800.0f) - 1.0f;
		float w = (ch.Size.x * scale) / (600.0f / 2.0f);
		float h = (ch.Size.y * scale) / (800.0f / 2.0f);


		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, TVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}



GLFWwindow* SpectrumView::getWindow() { return window; }
void SpectrumView::setModels(SpectrumModel* _model, InteractionModel* _IModel) {
	model = _model;
	IModel = _IModel;
}


void SpectrumView::setController(SpectrumController* _controller){
	// -- Event Handling
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

	widgets[0]->setWidgetCallback(&(SpectrumController::handleListButton), _controller);
	widgets[1]->setWidgetCallback(&(SpectrumController::handleListButton), _controller);
	widgets[2]->setWidgetCallback(&(SpectrumController::handleListButton), _controller);
	widgets[3]->setWidgetCallback(&(SpectrumController::handleBooleanButton), _controller);
	widgets[4]->setWidgetCallback(&(SpectrumController::handleListMenu), _controller);

	glfwSetMouseButtonCallback(window, mouseClick);
	glfwSetKeyCallback(window, keyPress);
	glfwSetScrollCallback(window, mouseScroll);
	//glfwSetCursorPosCallback(window, handleMouseMovement);
}


string SpectrumView::readShaderCode(const char* filename) {
	string shader, line;
	ifstream inputFile(filename);
	if (!inputFile.good()) {
		cout << "FAILED TO LOAD FILE: " << filename << " IN readShaderCode()" << endl;
		exit(-42);
	}
	while (getline(inputFile, line)) {
		shader.append(line);
		shader.append("\n");
	}
	return shader;
}



void SpectrumView::DModelChanged() { draw(); }
void SpectrumView::IModelChanged() { 
	swapListMenu(IModel->getCurrentListMenuID());
	draw(); 
}
