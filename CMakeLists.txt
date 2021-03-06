cmake_minimum_required(VERSION 2.8)
project(LAS_VEGAS)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
set( CMAKE_MODULE_PATH ${LAS_VEGAS_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})

link_directories("${CMAKE_SOURCE_DIR}/lib")
link_directories("$ENV{HOME}/local/lib")

include_directories(/usr/include/libusb-1.0/)
include_directories(${LAS_VEGAS_SOURCE_DIR}/ext/Eigen)
include_directories(${LAS_VEGAS_SOURCE_DIR}/ext/stann)
include_directories(${LAS_VEGAS_SOURCE_DIR}/ext/psimpl)
include_directories(${LAS_VEGAS_SOURCE_DIR})
include_directories(${LAS_VEGAS_SOURCE_DIR}/include/liblvr)


##################################################################################
# Some compiler specific settings
##################################################################################

if(MSVC)
add_definitions(-DNOMINMAX)
endif()

IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0 -fPIC -Wno-deprecated")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0 -fPIC -Wno-deprecated")
  add_definitions("-std=c++0x")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

###############################################################
# EXTERNAL LIBRARIES
###############################################################

####
## Searching for OpenCV
##############################
FIND_PACKAGE( OpenCV REQUIRED )

####
## Searching for Eigen3
##############################
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

####
## Searching for MPI
##############################
FIND_PACKAGE( MPI )
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

####
## Searching for VTK
##############################
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


####
## Searching for NABO 
##############################

SET(NABO_INCLUDE_DIRS "")
SET(NABO_LIBRARIES "")
find_package(Nabo)
if(NABO_FOUND)
   include_directories(${NABO_INCLUDE_DIRS})
   link_directories(${NABO_LIBRARIES})
   add_definitions(-D_USE_NABO)
endif(NABO_FOUND)


####
## Searching for PCL
##############################

# We intentionally do this twice as there seems to be a bug in PCL 1.4 which
# leads to a non set PCL_VERSION variable if you call it only once.

find_package(PCL 1.0)
if ( "${PCL_VERSION}" STREQUAL "" )
    find_package(PCL 1.0)
endif ( "${PCL_VERSION}" STREQUAL "" )
if(PCL_FOUND)
    message(STATUS "Found PCL ${PCL_VERSION}")
	if ( "${PCL_VERSION}" VERSION_GREATER "1.0" )
        include_directories(${PCL_INCLUDE_DIRS})
        link_directories(${PCL_LIBRARY_DIRS})
        add_definitions(${PCL_DEFINITIONS})
        add_definitions(-D_USE_PCL_)
		if ( "${PCL_VERSION}" VERSION_GREATER "1.2" )
            add_definitions(-D_PCL_VERSION_GT_12_)
		else ( "${PCL_VERSION}" VERSION_GREATER "1.2" )
            add_definitions(-D_PCL_VERSION_12_)
		endif ( "${PCL_VERSION}" VERSION_GREATER "1.2" )
    else ( "${PCL_VERSION}" VERSION_GREATER "1.0" )
        message( "-- Your PCL version is to old.." )
        message( "-- PCL related stuff will be disabled." )
	endif ( "${PCL_VERSION}" VERSION_GREATER "1.0" )
else(PCL_FOUND)
        message( "-- No PCL found." )
        message( "-- PCL related stuff will be disabled." )
endif(PCL_FOUND)


####
## Searching CGAL
##############################

find_package(CGAL)
if(CGAL_FOUND)
include(${CGAL_USE_FILE})
add_definitions(${CGAL_CXX_FLAGS_INIT})
include_directories(${CGAL_INCLUDE_DIRS})
link_directories(${CGAL_LIBRARIES_DIR})
message(STATUS "FOUND CGAL")
endif(CGAL_FOUND)


####
## Searching for Boost
##############################
#


#set(Boost_USE_STATIC_LIBS       ON)
set(Boost_USE_MULTITHREADED     ON)

find_package( Boost 
    COMPONENTS 
    program_options 
    filesystem system 
    thread
    REQUIRED )

if(Boost_FOUND)
    link_directories(${Boost_LIBRARY_DIR})
    include_directories(${Boost_INCLUDE_DIRS})
    add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
endif()


####
## Searching for OpenMP
##############################

find_package(OpenMP)
if(OpenMP_FOUND)
	link_directories(${OpenMP_LIBRARY_DIRS})
	include_directories(${OpenMP_INCLUDE_DIRS})
endif(OpenMP_FOUND)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" )


####
## Searching for OpenGL
##############################

find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
    link_directories(${OpenGL_LIBRARY_DIRS})
    include_directories(${OpenGL_INCLUDE_DIRS})
    message( "OPEN GL LIBS 1 : " ${OPENGL_LIBRARIES}) 
    message( "OPEN GL LIBS 2 : " ${OPENGL_LIBRARY}) 
endif(OpenGL_FOUND)

IF(APPLE)
    link_directories(/opt/local/lib)
    include_directories(/System/Library/Frameworks/GLUT.framework/Headers)
    include_directories(/System/Library/Frameworks/OpenGL.framework/Headers)
ENDIF(APPLE)

if(UNIX)
    FIND_PATH(OPENGL_INC gl.h /usr/include/GL)
    include_directories(${OPENGL_INC})
endif(UNIX)


##############################################################
## Searching for glut and it's additional runtime libraries.
##############################################################

find_package(GLUT REQUIRED)
if ( GLUT_FOUND )
    message(STATUS "-- Found OpenGL Utility Toolkit" )
    include_directories(${GLUT_INCLUDE_DIR})
else ( GLUT_FOUND )
    message( SEND_ERROR "-- could not find OpenGL Utility Toolkit" )
endif ( GLUT_FOUND )

if(LINUX)
if ( GLUT_Xmu_LIBRARY )
    message( "-- Found X.Org X11 libXmu/libXmuu runtime libraries" )
else ( GLUT_Xmu_LIBRARY )
    message( SEND_ERROR "-- Could not find X.Org X11 libXmu/libXmuu runtime libraries" )
endif ( GLUT_Xmu_LIBRARY )

if ( GLUT_Xi_LIBRARY )
    message( "-- Found X.Org X11 libXi runtime library" )
else ( GLUT_Xi_LIBRARY )
    message( SEND_ERROR "-- Could not find X.Org X11 libXi runtime library" )
endif ( GLUT_Xi_LIBRARY )

if ( OPENGL_glu_LIBRARY ) 
    message( "-- Found GLU library" )
else ( OPENGL_glu_LIBRARY ) 
    message( SEND_ERROR "-- Could not find GLU library" )
endif ( OPENGL_glu_LIBRARY ) 

endif(LINUX)

##############################################################
# USED THIRD PARTY STUFF
##############################################################

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/ext )

# RPLY
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/ext/rply/ )
add_subdirectory(ext/rply)

# LASLIB
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/ext/laslib/inc )
add_subdirectory(ext/laslib)

# OBJ
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/ext/obj/ )
add_subdirectory(ext/obj)

# SLAM6D stuff
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/ext/slam6d )
add_subdirectory(ext/slam6d)

# Freenect
#if(APPLE)
#include_directories( /opt/local/include/libusb-1.0/)
#endif(APPLE)
#include_directories(ext/libfreenect/include/)
#add_subdirectory(ext/libfreenect)

if(MPI_FOUND)
add_subdirectory(src/tools/mpi)
endif(MPI_FOUND)

if(CGAL_FOUND)
add_subdirectory(src/tools/cgal_reconstruction)
#add_subdirectory(src/tools/fusion)
add_subdirectory(src/tools/slicer)
endif(CGAL_FOUND)

if(PCL_FOUND)
    add_subdirectory(src/tools/scanfilter)
#    add_subdirectory(src/tools/kinectgrabber)
endif(PCL_FOUND)

##############################################################
# VIEWERS
##############################################################

set( QT_USE_QTOPENGL TRUE )
set( QT_USE_XML TRUE)
find_package( Qt4 )
if( QT4_FOUND )
    include_directories(${QT_INCLUDE_DIR})
    include_directories(${QT_QTGUI_INCLUDE_DIR})
    include_directories(${QT_QTOPENGL_INCLUDE_DIR})
    include_directories(${QT_QTCORE_INCLUDE_DIR})
    include_directories(${QT_QTXML_INCLUDE_DIR})
    
    add_definitions(-D__WITH_QT4__)

    # Try to find libQGLViewer
    find_library(QGLVIEWER_LIBRARY_RELEASE 
        NAMES qglviewer-qt4 qglviewer QGLViewer QGLViewer2 
        PATHS /usr/lib /usr/local/lib ENV QGLVIEWERROOT 
        ENV LD_LIBRARY_PATH ENV LIBRARY_PATH
        PATH_SUFFIXES QGLViewer QGLViewer/release)
    if(QGLVIEWER_LIBRARY_RELEASE)
        message(STATUS "Found libqglviewer")
        message(STATUS "Building qglviewer application")
        add_subdirectory(src/qglviewer)
    endif(QGLVIEWER_LIBRARY_RELEASE)

   	find_package(QVTK)
	if(QVTK_FOUND)
		include_directories(${QVTK_INCLUDE_DIR})
   		add_subdirectory(src/tools/viewer)
	endif(QVTK_FOUND)

endif(QT4_FOUND)


##############################################################
# LIBRARIES
##############################################################

add_subdirectory(src/liblvr)

##############################################################
# BINARIES
##############################################################

add_subdirectory(src/tools/reconstruct)
add_subdirectory(src/tools/classifier)
#add_subdirectory(src/tools/colorize)
add_subdirectory(src/tools/scanreduce)
add_subdirectory(src/tools/convert)
add_subdirectory(src/tools/asciiconverter)
add_subdirectory(src/tools/texman)
add_subdirectory(src/tools/tiogen)
add_subdirectory(src/tools/meshoptimizer)
add_subdirectory(src/tools/transform)
add_subdirectory(src/tools/registration)
add_subdirectory(src/tools/normals)
add_subdirectory(src/tools/leica_converter)

################################################################
# DOCUMENTATION
################################################################

# Try to find doxygen
find_package( Doxygen )

# Doxygen found
if( NOT DOXYGEN_FOUND )
    message( WARNING "Could not find Doxygen. "
        "You will not be able to build the documentation." )
endif( NOT DOXYGEN_FOUND )

# GraphViz found
if( NOT DOXYGEN_DOT_FOUND )
    message( WARNING "Could not find GraphViz. "
        "You will not be able to build the documentation." )
endif( NOT DOXYGEN_DOT_FOUND )

if( DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND )
    configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in 
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY )
    add_custom_target( doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM )
endif( DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND )

