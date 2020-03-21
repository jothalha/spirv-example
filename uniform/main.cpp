/**
  * This example uses demo code from https://open.gl/ [2020-03-21].
  */

#include <GL/glew.h>  // include header file
#include <GLFW/glfw3.h>

#include <cstdio>
#include <chrono>
#include <cmath>
#include <vector>

#include "shader_vert_generated.h"
#include "shader_frag_generated.h"

namespace {
const char *gldebug_srcstr(unsigned int src)
{
	switch(src) {
	case GL_DEBUG_SOURCE_API:
		return "api";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return "wsys";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return "sdrc";
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return "3rdparty";
	case GL_DEBUG_SOURCE_APPLICATION:
		return "app";
	case GL_DEBUG_SOURCE_OTHER:
		return "other";
	default:
		break;
	}
	return "unknown";
}

const char *gldebug_typestr(unsigned int type)
{
	switch(type) {
	case GL_DEBUG_TYPE_ERROR:
		return "error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return "deprecated";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return "undefined behavior";
	case GL_DEBUG_TYPE_PORTABILITY:
		return "portability";
	case GL_DEBUG_TYPE_PERFORMANCE:
		return "performance";
	case GL_DEBUG_TYPE_OTHER:
		return "other";
	default:
		break;
	}
	return "unknown";
}

void GLAPIENTRY gldebug(GLenum src, GLenum type, GLuint id, GLenum severity,
		GLsizei len, const char *msg, const void *cls)
{
	printf("[GLDEBUG] (%s) %s: %s\n", gldebug_srcstr(src), gldebug_typestr(type), msg);
}
} // namespace

int main(int argc, char* argv[])
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  glewInit();
  glDebugMessageCallback(gldebug, 0);

  float vertices[] = {
    0.0f, 0.5f,
    0.5f, -0.5f,
    -0.5f, -0.5f,
  };

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderBinary(1, &vertexShader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB,
                 shader_vert, sizeof(shader_vert));
  glSpecializeShaderARB(vertexShader, "main", 0 , 0, 0);

  GLint isCompiled = 0;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(maxLength);
    glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
    std::printf("vertexShader Log:\n%s\n", infoLog.data());

    glDeleteShader(vertexShader);
    return 1;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderBinary(1, &fragmentShader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB,
                 shader_frag, sizeof(shader_frag));
  glSpecializeShaderARB(fragmentShader, "main", 0 , 0, 0);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(maxLength);
    glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
    std::printf("fragmentShader Log:\n%s\n", infoLog.data());

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 1;
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  GLint isLinked = 0;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);
  if (isLinked == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
    std::printf("Program Log:\n%s\n", infoLog.data());

    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 1;
  }

  // AMD on Windows doesn't like detaching the shader
  //glDetachShader(shaderProgram, vertexShader);
  //glDetachShader(shaderProgram, fragmentShader);

  glUseProgram(shaderProgram);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  auto t_start = std::chrono::high_resolution_clock::now();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  while(!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GL_TRUE);
    }

    glClear(GL_COLOR_BUFFER_BIT);

    float time = std::chrono::duration_cast<std::chrono::duration<float>>(
        std::chrono::high_resolution_clock::now() - t_start).count();

    glUniform3f(1, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
  }

  //glDeleteBuffers(vbo);
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  glDeleteBuffers(1, &vbo);

  glfwTerminate();
  return 0;
}
