## ======================================================================== ##
## Copyright 2009-2019 Intel Corporation                                    ##
##                                                                          ##
## Licensed under the Apache License, Version 2.0 (the "License");          ##
## you may not use this file except in compliance with the License.         ##
## You may obtain a copy of the License at                                  ##
##                                                                          ##
##     http://www.apache.org/licenses/LICENSE-2.0                           ##
##                                                                          ##
## Unless required by applicable law or agreed to in writing, software      ##
## distributed under the License is distributed on an "AS IS" BASIS,        ##
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. ##
## See the License for the specific language governing permissions and      ##
## limitations under the License.                                           ##
## ======================================================================== ##

# avoid CMake policy CMP0072 warning:
# prefer libGL over libOpenGl for better compatibility with SWR
set(OpenGL_GL_PREFERENCE "LEGACY")
find_package(OpenGL 2 REQUIRED)

ospray_disable_compiler_warnings()

##############################################################
# Build support libs
##############################################################

include(cmake/configure_glfw.cmake)

add_subdirectory(common)
add_subdirectory(widgets)

##############################################################
# Build app
##############################################################

ospray_create_application(ospExampleViewer
  ospExampleViewer.cpp
LINK
  ospray_app
  ospray_imgui3d_sg
)
