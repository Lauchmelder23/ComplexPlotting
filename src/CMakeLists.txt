
add_executable(ComplexPlotting
	"main.cpp"
  "PlotWindow.hpp" "PlotWindow.cpp" "PlotManager.hpp" "PlotManager.cpp")

file(GLOB vendor_SRC
	${CMAKE_SOURCE_DIR}/vendor/src/*.c
)

target_sources(ComplexPlotting PRIVATE
	${vendor_SRC}
)

target_include_directories(ComplexPlotting PRIVATE
	glfw
	${CMAKE_SOURCE_DIR}/vendor/include
)

target_link_libraries(ComplexPlotting
	glfw
)

#add_custom_command(TARGET ComplexPlotting POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/shaders $<TARGET_FILE_DIR:ComplexPlotting>/shaders
#)