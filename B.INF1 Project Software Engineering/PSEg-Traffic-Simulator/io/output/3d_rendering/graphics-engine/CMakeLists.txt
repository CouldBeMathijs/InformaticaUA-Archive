cmake_minimum_required(VERSION 3.28)

project(engine)

############################################################
# Set compiler flags
############################################################
# If supported by your compiler, you can add the -Wall, -Wextra, –fstack-protector-all and -g3 flags here.
set(OWN_GXX_FLAGS "-std=c++17 -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OWN_GXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OWN_GXX_FLAGS}")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${OWN_GXX_FLAGS}")

############################################################
# List all sources
############################################################
set(engine_sources
		external/easy_image.cc
	engine.cc
		external/ini_configuration.cc
		external/l_parser.cc
		external/l_parser.h
		external/vector3d.cc
		external/vector3d.h
	ImageGenerators/ImageGenerator.cpp
	ImageGenerators/ImageGenerator.h
	ImageGenerators/LSystem2DGenerator.cpp
	ImageGenerators/LSystem2DGenerator.h
	ImageGenerators/Figure3DGenerator.cpp
	ImageGenerators/Figure3DGenerator.h
		ImageGenerators/3DUtils/Transformations.cpp
		ImageGenerators/3DUtils/Transformations.h
		ImageGenerators/3DUtils/PrimitiveFigures.cpp
		ImageGenerators/3DUtils/PrimitiveFigures.h
		ImageGenerators/3DUtils/LSystem3DGenerator.cpp
		ImageGenerators/3DUtils/LSystem3DGenerator.h
		ImageGenerators/3DUtils/LineDrawingGenerator.cpp
		ImageGenerators/3DUtils/LineDrawingGenerator.h
		ImageGenerators/3DUtils/Utils.cpp
		ImageGenerators/3DUtils/Utils.h
		ImageGenerators/3DUtils/ZBuffer.cpp
		ImageGenerators/3DUtils/ZBuffer.h
)

if(WIN32)
	add_compile_definitions(_USE_MATH_DEFINES)
endif()

############################################################
# Create an executable
############################################################
set(exe_name "engine")
add_executable( ${exe_name} ${engine_sources} )
install( TARGETS ${exe_name} DESTINATION ${PROJECT_SOURCE_DIR}/ )
