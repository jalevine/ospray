// ======================================================================== //
// Copyright 2018-2019 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include "ArcballCamera.h"
// ospcommon
#include "ospcommon/box.h"
#include "ospcommon/vec.h"
#include "ospcommon/containers/TransactionalBuffer.h"
// ospray
#include "ospray/ospray.h"

struct WindowState {
  bool quit;
  bool cameraChanged;
  bool fbSizeChanged;
  int spp;
  ospcommon::vec2i windowSize;
  ospcommon::vec3f eyePos;
  ospcommon::vec3f lookDir;
  ospcommon::vec3f upDir;

  WindowState();
};

class GLFWDistribOSPRayWindow
{
 public:
  GLFWDistribOSPRayWindow(const ospcommon::vec2i &windowSize,
                   const ospcommon::box3f &worldBounds,
                   OSPModel model,
                   OSPRenderer renderer);

  ~GLFWDistribOSPRayWindow();

  static GLFWDistribOSPRayWindow *getActiveWindow();

  OSPModel getModel();
  void setModel(OSPModel newModel);

  void clearFrameBuffer();

  void registerDisplayCallback(
      std::function<void(GLFWDistribOSPRayWindow *)> callback);

  void registerImGuiCallback(std::function<void()> callback);

  void mainLoop();

  void addObjectToCommit(OSPObject obj);

 protected:
  void reshape(const ospcommon::vec2i &newWindowSize);
  void motion(const ospcommon::vec2f &position);
  void display();
  void startNewOSPRayFrame();
  void waitOnOSPRayFrame();
  void updateTitleBar();

  static GLFWDistribOSPRayWindow *activeWindow;

  ospcommon::vec2i windowSize;
  ospcommon::box3f worldBounds;
  OSPModel model       = nullptr;
  OSPRenderer renderer = nullptr;

  int mpiRank = -1;
  int mpiWorldSize = -1;

  // GLFW window instance
  GLFWwindow *glfwWindow = nullptr;

  // Arcball camera instance
  std::unique_ptr<ArcballCamera> arcballCamera;

  // OSPRay objects managed by this class
  OSPCamera camera           = nullptr;
  OSPFrameBuffer framebuffer = nullptr;
  OSPFuture currentFrame     = nullptr;

  // List of OSPRay handles to commit before the next frame
  ospcommon::TransactionalBuffer<OSPObject> objectsToCommit;

  // OpenGL framebuffer texture
  GLuint framebufferTexture = 0;

  // optional registered display callback, called before every display()
  std::function<void(GLFWDistribOSPRayWindow *)> displayCallback;

  // toggles display of ImGui UI, if an ImGui callback is provided
  bool showUi = true;

  // optional registered ImGui callback, called during every frame to build UI
  std::function<void()> uiCallback;

  // FPS measurement of last frame
  float latestFPS{0.f};

  // The window state to be sent out over MPI to the other rendering processes
  WindowState windowState;
};
