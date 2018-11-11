/*
 * Copyright 2018 Vladimir Balun
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Window.h"

Platforms::WindowSystem::Window::Window(HINSTANCE& instance, INT cmdShow) 
    : _appInstance(instance), _cmdShow(cmdShow)
{
    memset(&_windowEvent, 0, sizeof(_windowEvent));
    _windowClass.lpszClassName = _windowClassName;
    _windowClass.cbSize = sizeof(WNDCLASSEX);
    _windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    _windowClass.lpfnWndProc = on_window_event;
    _windowClass.hInstance = _appInstance;
    _windowClass.hIcon = NULL;
    _windowClass.hIconSm = NULL;
    _windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    _windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    _windowClass.lpszMenuName = NULL;
    _windowClass.cbClsExtra = 0;
    _windowClass.cbWndExtra = 0;

    if (!RegisterClassEx(&_windowClass)) 
        throw std::runtime_error("Window class was not registered.");
}

VOID Platforms::WindowSystem::Window::showWindow(LPCSTR windowTitle, INT windowWidth, INT windowHeight)
{
    _windowHandle = CreateWindowEx(NULL, _windowClassName, windowTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0, windowWidth, windowHeight, NULL, NULL, _appInstance, NULL);

    if (!_windowHandle)
        throw std::runtime_error("Window was not created.");
 
    ShowWindow(_windowHandle, _cmdShow);
    UpdateWindow(_windowHandle);

    initWindowContext();
    Graphics::Scene scene(_windowContext, windowWidth, windowHeight);
    scene.initScene();

    // Game loop
    while (_windowEvent.message != WM_QUIT)
    {
        if (PeekMessage(&_windowEvent, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&_windowEvent);
            DispatchMessage(&_windowEvent);
        }

        scene.updateScene();
        scene.renderScene();
    }
}

VOID Platforms::WindowSystem::Window::initWindowContext() 
{
    PIXELFORMATDESCRIPTOR pixelFormat;
    memset(&pixelFormat, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pixelFormat.nVersion = 1;
    pixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixelFormat.iPixelType = PFD_TYPE_RGBA;
    pixelFormat.cColorBits = 16;
    pixelFormat.cDepthBits = 16;

    _windowContext = GetDC(_windowHandle);
    GLuint chosenPixelFormat = ChoosePixelFormat(_windowContext, &pixelFormat);
    if (chosenPixelFormat != 0)
    {
        PIXELFORMATDESCRIPTOR bestMatchPixelFormat;
        DescribePixelFormat(_windowContext, chosenPixelFormat, sizeof(pixelFormat), &bestMatchPixelFormat);
        if (bestMatchPixelFormat.cDepthBits < pixelFormat.cDepthBits)
            throw std::runtime_error("Pixel format is incorrect.");
        if (SetPixelFormat(_windowContext, chosenPixelFormat, &pixelFormat) == FALSE)
            throw std::runtime_error("Pixel format was not set.");
    }
    else
    {
        throw std::runtime_error("Pixel format was not chosen.");
    }

    _openGLContext = wglCreateContext(_windowContext);
    wglMakeCurrent(_windowContext, _openGLContext);
}

Platforms::WindowSystem::Window::~Window()
{
    if (!_openGLContext)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(_openGLContext);
        _openGLContext = NULL;
    }

    if (!_windowContext)
    {
        ReleaseDC(_windowHandle, _windowContext);
        _windowContext = NULL;
    }
}
