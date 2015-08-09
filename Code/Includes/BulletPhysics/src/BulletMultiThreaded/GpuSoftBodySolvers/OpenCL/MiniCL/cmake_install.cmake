# Install script for directory: /Users/PhilCK/Desktop/bullet-2.82-r2704/src/BulletMultiThreaded/GpuSoftBodySolvers/OpenCL/MiniCL

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Library/Frameworks")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE DIRECTORY FILES "/Users/PhilCK/Desktop/bullet-2.82-r2704/src/BulletMultiThreaded/GpuSoftBodySolvers/OpenCL/MiniCL/BulletSoftBodySolvers_OpenCL_Mini.framework" USE_SOURCE_PERMISSIONS)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./BulletSoftBodySolvers_OpenCL_Mini.framework/Versions/2.82/BulletSoftBodySolvers_OpenCL_Mini" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./BulletSoftBodySolvers_OpenCL_Mini.framework/Versions/2.82/BulletSoftBodySolvers_OpenCL_Mini")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "/Library/Frameworks/BulletSoftBodySolvers_OpenCL_Mini.framework/Versions/2.82/BulletSoftBodySolvers_OpenCL_Mini"
      -change "/Users/PhilCK/Desktop/bullet-2.82-r2704/src/BulletCollision/BulletCollision.framework/Versions/2.82/BulletCollision" "/Library/Frameworks/BulletCollision.framework/Versions/2.82/BulletCollision"
      -change "/Users/PhilCK/Desktop/bullet-2.82-r2704/src/BulletDynamics/BulletDynamics.framework/Versions/2.82/BulletDynamics" "/Library/Frameworks/BulletDynamics.framework/Versions/2.82/BulletDynamics"
      -change "/Users/PhilCK/Desktop/bullet-2.82-r2704/src/BulletMultiThreaded/BulletMultiThreaded.framework/Versions/2.82/BulletMultiThreaded" "/Library/Frameworks/BulletMultiThreaded.framework/Versions/2.82/BulletMultiThreaded"
      -change "/Users/PhilCK/Desktop/bullet-2.82-r2704/src/BulletSoftBody/BulletSoftBody.framework/Versions/2.82/BulletSoftBody" "/Library/Frameworks/BulletSoftBody.framework/Versions/2.82/BulletSoftBody"
      -change "/Users/PhilCK/Desktop/bullet-2.82-r2704/src/LinearMath/LinearMath.framework/Versions/2.82/LinearMath" "/Library/Frameworks/LinearMath.framework/Versions/2.82/LinearMath"
      -change "/Users/PhilCK/Desktop/bullet-2.82-r2704/src/MiniCL/MiniCL.framework/Versions/2.82/MiniCL" "/Library/Frameworks/MiniCL.framework/Versions/2.82/MiniCL"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./BulletSoftBodySolvers_OpenCL_Mini.framework/Versions/2.82/BulletSoftBodySolvers_OpenCL_Mini")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./BulletSoftBodySolvers_OpenCL_Mini.framework/Versions/2.82/BulletSoftBodySolvers_OpenCL_Mini")
    endif()
  endif()
endif()
