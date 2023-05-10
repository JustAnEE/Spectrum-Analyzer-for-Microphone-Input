#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cmath>

#include "../../Utils/enums.h"
#include "../../Utils/constants.h"
#include "../../../Libraries/include/glad/glad.h"

class Window
{
public:
   Window() {}

public:
   virtual GLfloat* GetWindow();

private:
   virtual void GenerateWindow() = 0;

protected:
   GLfloat aufMyWindow[SAMPLE_BUFFER_SIZE];

};

#endif 
