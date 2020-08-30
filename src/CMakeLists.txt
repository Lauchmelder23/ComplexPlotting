
add_executable(ComplexPlotting
	"main.cpp"
)

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