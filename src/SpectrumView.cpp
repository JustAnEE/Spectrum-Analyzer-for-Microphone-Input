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

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	

	// -- freetype
	FT_Library freetype;
	FT_Face face;

	FT_Init_FreeType(&freetype);
	FT_New_Face(freetype, "C:/Users/Shadow/Desktop/openGLtest/openGLtest/fonts/monofonto.otf", 0, &face);
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

	const char* TVS =
		"#version 330 core\n"
		"layout (location = 0) in vec4 vertex;\n"
		"out vec2 TexCoords;\n"
		"uniform mat4 projection;\n"
		"void main() {\n"
		"   gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
		"   TexCoords = vertex.zw;}\0 \n";

	const char* TFS = 
		"#version 330 core\n"
		"in vec2 TexCoords;\n"
		"out vec4 color;\n"
		"uniform sampler2D text;\n"
		"void main() {\n"
		"   vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
		"   color = vec4(0.0, 1.0, 0.0, 1.0) * sampled;\n"
		"}\n\0";

	textureVertexShader = glCreateShader(GL_VERTEX_SHADER);
	textureFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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

	glGenVertexArrays(1, &TVAO);
	glGenBuffers(1, &TVBO);
	glBindVertexArray(TVAO);
	glBindBuffer(GL_ARRAY_BUFFER, TVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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

	glfwSwapBuffers(window);
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
	glDrawArrays(GL_LINES, 0, (plot->getPlotSize() * 2) / (12 * sizeof(GLfloat)));

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


void SpectrumView::drawText(string text, float xNormalized , float yNormalized, float scale) {
	// activate corresponding render state	
	glUseProgram(textureShaderProgram);
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f);
	glUniformMatrix4fv(glGetUniformLocation(textureShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(TVAO);

	// -- Unnormalize.
	float x = (xNormalized + 1.0f) * (800.0f / 2.0f);
	float y = (yNormalized + 1.0f) * (800.0f / 2.0f);

	// iterate through all characters
	for (int i = 0; i < text.size(); i++) {
		Character ch = Characters[text[i]];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;


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
