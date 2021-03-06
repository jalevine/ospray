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

ospray_build_component(snappy EXCLUDE_FROM_ALL)
ospray_build_component(mpiCommon)

# ------------------------------------------------------------------
# The MAML library itself
# ------------------------------------------------------------------
ospray_create_library(ospray_mpi_maml
    maml/maml.cpp
    maml/Context.cpp
  COMPONENT mpi
)

target_link_libraries(ospray_mpi_maml PRIVATE snappy PUBLIC ospray_mpi_common)

target_include_directories(ospray_mpi_maml
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ospray/maml>
)

install(FILES maml/maml.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ospray/maml
  COMPONENT devel
)

# ------------------------------------------------------------------
# tests and examples
# ------------------------------------------------------------------

ospray_create_test(mamlTest
  apps/mamlTest.cpp
LINK
  ospray_mpi_maml
)

ospray_create_test(mamlTestMessageSwarm
  apps/testMessageSwarm.cpp
LINK
  ospray_mpi_maml
)
