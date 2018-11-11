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

#include "Scene.h"

GLvoid Graphics::Scene::initScene()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glViewport(0, 0, _sceneWidth, _sceneHeight);
    gluOrtho2D(0.0, _sceneWidth, 0.0, _sceneHeight);
}

GLvoid Graphics::Scene::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _gameWorld.draw();
    SwapBuffers(_windowContext);
}

GLvoid Graphics::Scene::updateScene()
{

}
