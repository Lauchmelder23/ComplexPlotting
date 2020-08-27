# CMakeList.txt : CMake project for ComplexPlotting, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if(WIN32)
	file(GLOB SDL2_LIBRARIES
		${SDL2_INCLUDES}/*.lib
	)
else()

endif()

# Add source to this project's executable.
add_executable (ComplexPlotting  
	"main.cpp"
 "PlotWindow.hpp" "PlotWindow.cpp" "PlotWindowManager.hpp")

target_include_directories(ComplexPlotting PRIVATE
	${SDL2_INCLUDES}
	${CMAKE_SOURCE_DIR}/SDLFramework/src/sdlf
)

target_link_libraries(ComplexPlotting PRIVATE
	${SDL2_LIBRARIES}
	sdlf
)