find_program(GLSL_GEN glslangValidator)
if(NOT GLSL_GEN)
  message(FATAL_ERROR "Failed to find glslangValidator")
endif()

macro(gen_spirv_headers Var)
  cmake_parse_arguments(GEN_SPIRV_HEADERS "" "DESTINATION" "FILES" ${ARGN})
  set(_filelist)

  foreach(f ${GEN_SPIRV_HEADERS_FILES})
    get_filename_component(_basename ${f} NAME)
    string(REPLACE "." "_" _basename ${_basename})
    set(_outname "${GEN_SPIRV_HEADERS_DESTINATION}/${_basename}_generated.h")

    add_custom_command(OUTPUT ${_outname}
      COMMAND ${GLSL_GEN}
      ARGS -G --vn "${_basename}" -o "${_outname}" ${f}
      DEPENDS ${f}
      COMMENT "Generating code for ${f}"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    list(APPEND _filelist ${_outname})
  endforeach()

  set_source_files_properties(${_filelist} PROPERTIES GENERATED TRUE)
  set(${Var} ${_filelist})
  unset(_filelist)
endmacro()
