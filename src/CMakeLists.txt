
add_executable(ComplexPlotting
	"main.cpp"
  "PlotWindow.hpp" "PlotWindow.cpp" "PlotManager.hpp" "PlotManager.cpp" "objects/Shader.hpp" "objects/Shader.cpp"  )

file(GLOB vendor_SRC
	${CMAKE_SOURCE_DIR}/vendor/src/*.c
	${CMAKE_SOURCE_DIR}/vendor/src/*.cpp
)

target_sources(ComplexPlotting PRIVATE
	${vendor_SRC}
	objects
)

target_include_directories(ComplexPlotting PRIVATE
	glfw
	${CMAKE_SOURCE_DIR}/vendor/include
	${CMAKE_SOURCE_DIR}/vendor/include/imgui
	objects
)

target_link_libraries(ComplexPlotting
	glfw
)

add_custom_command(TARGET ComplexPlotting POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:ComplexPlotting>/shaders
)