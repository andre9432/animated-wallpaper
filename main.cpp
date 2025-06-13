#include <glad/glad.h>
#include <windows.h>
#include <wingdi.h>
#include <winuser.h>
#include <iostream>
#include <GL/wglext.h>
#include <chrono>
#include <shaders/shaders.h>

// Extension Pointers needed for loading modern OpenGL pointers
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;



BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    HWND p = FindWindowExW(hwnd, NULL, L"SHELLDLL_DefView", NULL);
    HWND* ret = (HWND*)lParam;

    if (p)
        {
        // Gets the WorkerW Window after the current one.
        *ret = FindWindowExW(NULL, hwnd, L"WorkerW", NULL);
        }
    return true;
}


HWND get_wallpaper_window()
{
    // Fetch the Progman window
    HWND progman = FindWindowW(L"ProgMan", NULL);
    // Send 0x052C to Progman. This message directs Progman to spawn a 
    // WorkerW behind the desktop icons. If it is already there, nothing 
    // happens.
    SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);
    // We enumerate all Windows, until we find one, that has the SHELLDLL_DefView 
    // as a child. 
    // If we found that window, we take its next sibling and assign it to workerw.
    HWND wallpaper_hwnd = nullptr;
    EnumWindows(EnumWindowsProc, (LPARAM)&wallpaper_hwnd);
    // Return the handle you're looking for.
    return wallpaper_hwnd;
}

int createDummyOpenGLContext(HWND windowHandler)
{
    HDC    hdcDUMMY = GetDC(windowHandler);
    HGLRC  hglrcDUMMY; 

    PIXELFORMATDESCRIPTOR pfdDUMMY = { 
    sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd  
    1,                     // version number  
    PFD_DRAW_TO_WINDOW |   // support window  
    PFD_SUPPORT_OPENGL |   // support OpenGL  
    PFD_DOUBLEBUFFER,      // double buffered  
    PFD_TYPE_RGBA,         // RGBA type  
    32,                    // 24-bit color depth  
    0, 0, 0, 0, 0, 0,      // color bits ignored  
    0,                     // no alpha buffer  
    0,                     // shift bit ignored  
    0,                     // no accumulation buffer  
    0, 0, 0, 0,            // accum bits ignored  
    24,                    // 32-bit z-buffer  
    8,                     // no stencil buffer  
    0,                     // no auxiliary buffer  
    PFD_MAIN_PLANE,        // main layer  
    0,                     // reserved  
    0, 0, 0                // layer masks ignored  
    };
    int  iPixelFormat; 
 
    // get the best available match of pixel format for the device context   
    iPixelFormat = ChoosePixelFormat(hdcDUMMY, &pfdDUMMY);
    if (!iPixelFormat)
    {
        std::cerr << "Could not find pixel format that matches PIXELFORMATDESCRIPTOR struct description." << std::endl;
        return FALSE;
    }
 
    // make that the pixel format of the device context  
    if (!SetPixelFormat(hdcDUMMY, iPixelFormat, &pfdDUMMY))
    {
        std::cerr << "Function SetPixelFormat() failed: " << GetLastError() << std::endl;
        return FALSE;
    }

    // create a rendering context  
    hglrcDUMMY = wglCreateContext(hdcDUMMY);
    if (!hglrcDUMMY)
    {
        std::cerr << "Function wglCreateContext() failed: " << GetLastError() << std::endl;
        return FALSE;
    }
    
    // make it the calling thread's current rendering context 
    if (!wglMakeCurrent(hdcDUMMY, hglrcDUMMY))
    {
        std::cerr << "Function wglMakeCurrent() failed: " << GetLastError() << std::endl;
        return FALSE;
    }

    // Load extensions' pointers for the real OpenGL context
    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

    // make the rendering context not current
    wglMakeCurrent(hdcDUMMY, 0);
    
    // delete the rendering context  
    wglDeleteContext(hglrcDUMMY);
    ReleaseDC(windowHandler, hdcDUMMY);

    return TRUE;
}

void handleCommandLine(int argc, const char** argv)
{
    for(int i = 1; i < argc; ++i)
    {
         if(!strcmp(argv[i], "--arg1"))
             std::cout << "You used arg1\n";
         else if(!strcmp(argv[i], "--arg2"))
             std::cout << "You used arg2\n";
         /** handle other possible args **/
    }
}

int main(int argc, const char** argv)
{
    handleCommandLine(argc, argv);

    HWND wallpaperWHandler = get_wallpaper_window();

    // create a fake/dummy context for getting necessary extensions
    if (!createDummyOpenGLContext(wallpaperWHandler))
    {
        std::cerr << "Failed to create dummy context for loading needed functions for modern OpenGL" << std::endl;
        return -1;
    }
    

    // After initializing the extension pointers for loading modern OpenGL functions, we can create a real GLcontent
    // -------------------------------------------------------------------------------------------------------------
    HDC    hdc = GetDC(wallpaperWHandler);
    HGLRC  hglrc;
 
    // get the best available match of pixel format for the device context   
    int pixel_format_attribs[] = {
        WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
        WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,         32,
        WGL_DEPTH_BITS_ARB,         24,
        WGL_STENCIL_BITS_ARB,       8,
        0
    };

    int pixel_format;
    UINT num_formats;
    wglChoosePixelFormatARB(hdc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
    if (!num_formats)
    {
        std::cerr << "Could not find pixel format that matches PIXELFORMATDESCRIPTOR struct description." << std::endl;
        return -1;
    }
 
    // make that the pixel format of the device context
    PIXELFORMATDESCRIPTOR pfd;
    if (!DescribePixelFormat(hdc, pixel_format, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
    {
        std::cerr << "Function DescribePixelFormat() in main failed: " << GetLastError() << std::endl;
    }
    if (!SetPixelFormat(hdc, pixel_format, &pfd))
    {
        std::cerr << "Function SetPixelFormat() in main failed: " << GetLastError() << std::endl;
        //return -1;
    }

    const int attribContextList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };

    // create a rendering context  
    hglrc = wglCreateContextAttribsARB(hdc, 0, attribContextList);
    if (!hglrc)
    {
        std::cerr << "Function wglCreateContextAttribsARB() failed: " << GetLastError() << std::endl;
        return -1;
    }
    
    // make it the calling thread's current rendering context 
    if (!wglMakeCurrent(hdc, hglrc))
    {
        std::cerr << "Function wglMakeCurrent() failed: " << GetLastError() << std::endl;
        return -1;
    }

    int version = gladLoadGL();
    if (!version)
    {
        std::cerr << "Failed to initialize OpenGL modern context" << std::endl;
        return -1;
    }
    else std::cout << "Loaded OpenGL version " << glGetString(GL_VERSION) << std::endl;

    // TIME FOR OPENGL STUFF ...
    // -------------------------
    wglSwapIntervalEXT(6);

    float vertices[] = {
        -1.f, -1.f,
         1.f, -1.f,
        -1.f,  1.f,

        -1.f,  1.f,
         1.f, -1.f,
         1.f,  1.f
    };
    std::cout << "Hello! Inside OpenGL!\n";
    GLuint VAO, VBO;
    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);

    glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexArrayAttrib(VAO, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 2 * sizeof(float));

    Shader shader{ "./shaders" };

    // Start timer since start of main loop
    auto start_time = std::chrono::high_resolution_clock::now();

    double lastTime = 0.0;
    double deltaTime = 0.0;


    MSG inputmessage = { };
    while (true)
    {
        std::chrono::_V2::system_clock::time_point current_time = std::chrono::high_resolution_clock::now();
        double elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(current_time - start_time).count() * 0.000001;
        deltaTime = elapsedTime - lastTime;
        lastTime = elapsedTime;
        //std::cout << elapsedTime << '\n';

        // Input
        glm::vec2 normalizedMousePos;
            // get primary screen size in pixels
            int scrSizeX = GetSystemMetrics(SM_CXSCREEN);
            int scrSizeY = GetSystemMetrics(SM_CYSCREEN);

        // Clear screen
        glClearColor(1.f, 0.f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw
        glBindVertexArray(VAO);
        shader.use();
        shader.setfloat("iTime", (float)elapsedTime);
        shader.set2f("iResolution", glm::vec2(scrSizeX, scrSizeY));
        glDrawArrays(GL_TRIANGLES, 0, 6);



        SwapBuffers(hdc);
    }
    
    // when the rendering context is no longer needed ...   
    
    // make the rendering context not current  
    wglMakeCurrent(NULL, NULL);
    
    // delete the rendering context  
    wglDeleteContext(hglrc);

    return 0;
}