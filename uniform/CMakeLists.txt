set(destdir "${CMAKE_CURRENT_BINARY_DIR}")

gen_spirv_headers(uniform_headers DESTINATION ${destdir} FILES shader.vert shader.frag)

add_executable(uniform main.cpp ${uniform_headers})
target_include_directories(uniform PRIVATE ${destdir})
target_link_libraries(uniform GLEW::GLEW OpenGL::GL glfw)
