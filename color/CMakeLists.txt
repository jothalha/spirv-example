set(destdir "${CMAKE_CURRENT_BINARY_DIR}")

gen_spirv_headers(color_headers DESTINATION ${destdir} FILES shader.vert shader.frag)

add_executable(color main.cpp ${color_headers})
target_include_directories(color PRIVATE ${destdir})
target_link_libraries(color GLEW::GLEW OpenGL::GL glfw)
