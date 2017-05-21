/*******************************************************************************************
*
*   Challenge 02:   DUNGEON GAME
*   Lesson 05:      image and textures
*   Description:    Image data loading, texture creation and drawing
*
*   NOTE: This example requires OpenGL 3.3 or ES2 for shaders support,
*         OpenGL 1.1 does not support shaders but it can also be used.
*
*   Compile rlgl module using:
*       gcc -c rlgl.c -Wall -std=c99 -DRLGL_STANDALONE -DRAYMATH_IMPLEMENTATION -DGRAPHICS_API_OPENGL_33
*
*   NOTE: rlgl module requires the following header-only files:
*       glad.h    - OpenGL extensions loader (stripped to only required extensions)
*       raymath.h - Vector and matrix math functions
*
*   Compile example using:
*       gcc -o $(NAME_PART).exe $(FILE_NAME) rlgl.o -lglfw3 -lopengl32 -lgdi32 -Wall -std=c99
*
*   This example has been created using raylib 1.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <GLFW/glfw3.h>         // Windows/Context and inputs management

#define RLGL_STANDALONE
#include "rlgl.h"               // rlgl library: OpenGL 1.1 immediate-mode style coding


GLFWwindow *window;

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
static void ErrorCallback(int error, const char* description);
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Drawing functions (uses rlgl functionality)
static void DrawRectangleV(Vector2 position, Vector2 size, Color color);

//----------------------------------------------------------------------------------
// Main Entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight);
    
    InitGraphicsDevice(screenWidth, screenHeight);
    
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------    

    // Main game loop    
    while (!glfwWindowShouldClose(window))
    {
        // Update
        //----------------------------------------------------------------------------------
        // ...
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        rlClearScreenBuffers();             // Clear current framebuffer
        
            DrawRectangleV((Vector2){ 10.0f, 10.0f }, (Vector2){ 780.0f, 20.0f }, DARKGRAY);

            // NOTE: Internal buffers drawing (2D data)
            rlglDraw();
            
        glfwSwapBuffers(window);
        glfwPollEvents();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    rlglClose();                    // Unload rlgl internal buffers and default shader/texture
    
    CloseWindow();
    //--------------------------------------------------------------------------------------
    
    return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definitions
//----------------------------------------------------------------------------------

// GLFW3: Error callback
static void ErrorCallback(int error, const char* description)
{
    TraceLog(ERROR, description);
}

// GLFW3: Keyboard callback
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void InitWindow(int screenWidth, int screenHeight)
{
    // GLFW3 Initialization + OpenGL 3.3 Context + Extensions
    glfwSetErrorCallback(ErrorCallback);
    
    if (!glfwInit())
    {
        TraceLog(WARNING, "GLFW3: Can not initialize GLFW");
        return 1;
    }
    else TraceLog(INFO, "GLFW3: GLFW initialized successfully");
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
   
    window = glfwCreateWindow(screenWidth, screenHeight, "rlgl standalone", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        return 2;
    }
    else TraceLog(INFO, "GLFW3: Window created successfully");
    
    glfwSetWindowPos(window, 200, 200);
    
    glfwSetKeyCallback(window, KeyCallback);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

void CloseWindow(void)
{
    glfwDestroyWindow(window);      // Close window
    glfwTerminate();                // Free GLFW3 resources
}

void InitGraphicsDevice(int screenWidth, int screenHeight)
{
    // Load OpenGL 3.3 supported extensions
    rlglLoadExtensions(glfwGetProcAddress);

    // Initialize OpenGL context (states and resources)
    rlglInit(screenWidth, screenHeight);

    // Initialize viewport and internal projection/modelview matrices
    rlViewport(0, 0, screenWidth, screenHeight);
    rlMatrixMode(RL_PROJECTION);                        // Switch to PROJECTION matrix
    rlLoadIdentity();                                   // Reset current matrix (PROJECTION)
    rlOrtho(0, screenWidth, screenHeight, 0, 0.0f, 1.0f); // Orthographic projection with top-left corner at (0,0)
    rlMatrixMode(RL_MODELVIEW);                         // Switch back to MODELVIEW matrix
    rlLoadIdentity();                                   // Reset current matrix (MODELVIEW)

    rlClearColor(245, 245, 245, 255);                   // Define clear color
    rlEnableDepthTest();                                // Enable DEPTH_TEST for 3D
}

// Draw rectangle using rlgl OpenGL 1.1 style coding (translated to OpenGL 3.3 internally)
static void DrawRectangleV(Vector2 position, Vector2 size, Color color)
{
    rlBegin(RL_TRIANGLES);
        rlColor4ub(color.r, color.g, color.b, color.a);

        rlVertex2i(position.x, position.y);
        rlVertex2i(position.x, position.y + size.y);
        rlVertex2i(position.x + size.x, position.y + size.y);

        rlVertex2i(position.x, position.y);
        rlVertex2i(position.x + size.x, position.y + size.y);
        rlVertex2i(position.x + size.x, position.y);
    rlEnd();
}









typedef struct Image {
    unsigned int width;
    unsigned int height;
    unsigned char *data;
} Image;

static Image LoadImage(const char *fileName)
{
    Image image = { 0 };
	// Load BMP File... or create image Data!
	int imgWidth;
	int imgHeight;
	short imgBpp;
	int imgDataOffset;
	//char *imgData;
	
	FILE *bmpFile = fopen(fileName, "rb");

	fseek(bmpFile, 10, SEEK_SET);
	fread(&imgDataOffset, 4, 1, bmpFile);
	fseek(bmpFile, 18, SEEK_SET);
	fread(&imgWidth, 4, 1, bmpFile); //reading the bmp Width
	fread(&imgHeight, 4, 1, bmpFile); //reading the bmp Height
	fseek(bmpFile, 28, SEEK_SET);
	fread(&imgBpp, 2, 1, bmpFile);
	
	pixel *imgData = (pixel *)malloc(imgWidth * imgHeight * sizeof(pixel));
	
	fseek(bmpfile, imgDataOffset, SEEK_SET);
	
	// Calculate Image Padding per line
	int padding = (imgWidth * imgBpp) % 32;
	int unusedData = 0;
	int extraBytes = 0;
	
	if ((padding/8) > 0) extraBytes = 4 - (padding/8);
	
	// Read image data
	for(int i=0; i<imgHeight; i++)
	{
		for(int j=0; j<imgWidth; j++)
		{
			fread(&imgData[i*imgWidth + j].b, 1, 1, bmpfile);
			fread(&imgData[i*imgWidth + j].g, 1, 1, bmpfile);
			fread(&imgData[i*imgWidth + j].r, 1, 1, bmpfile);
		}
		fread(&unusedData, extraBytes, 1, bmpfile);
	}
	
	fclose(bmpfile);
	
	pixel *imgDataFlip; 
	imgDataFlip = new pixel [imgWidth * imgHeight]; 
	
	// Flip image vertically
    for (int k=0; k < imgHeight; k++)
	{
		for(int j=0; j<imgWidth; j++)
		{
			imgDataFlip[k*imgWidth + j] = imgData[((imgHeight-1) - k)*imgWidth + j];
		}
	}
	
	free(imgData);
}

// Texture2D type
// NOTE: Data stored in GPU memory
typedef struct Texture2D {
    unsigned int id;            // OpenGL texture id
    unsigned int width;         // Texture base width
    unsigned int height;        // Texture base height
} Texture2D;

static Texture2D LoadTextureFromImage(Image image)
{
    Texture2D texture = { 0 };

    texture.id = rlglLoadTexture(image.data, image.width, image.height, image.format, image.mipmaps);

    texture.width = image.width;
    texture.height = image.height;
    texture.mipmaps = image.mipmaps;
    texture.format = image.format;

    return texture;
}