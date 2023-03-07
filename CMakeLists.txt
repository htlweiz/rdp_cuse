CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
PROJECT(cmake_test_template)
INCLUDE(CTest)

# -------------------------------------------------------------------------------- 
# build all project programs located in ./src/programs/<program_name>/*.cpp
# program wide headers are located in ./src/programs/<program_name>/include/
# -------------------------------------------------------------------------------- 

# -------------------------------------------------------------------------------- 
# build all project libraries located in ./src/libraries/<lib_name>/*.cpp
# lib wide headers are located in ./src/libs/<lib_name>/include/
# -------------------------------------------------------------------------------- 

FILE(GLOB target_types "src/*/")
FOREACH(target_type_dir ${target_types})
  GET_FILENAME_COMPONENT(target_type ${target_type_dir} NAME)
  FILE(GLOB target_directories "${target_type_dir}/*/")
  FOREACH(target_directory ${target_directories})
    GET_FILENAME_COMPONENT(target_name ${target_directory} NAME)
    MESSAGE("Creating build instructions for ${target_name}")
    FILE(GLOB target_sources "${target_directory}/*.cpp")
    FILE(GLOB target_headers "${target_directory}/include/*.*")
    IF(${target_type} MATCHES libraries)
      ADD_LIBRARY(${target_name} ${target_sources} ${target_headers})
    ELSEIF(${target_type} MATCHES programs)
      ADD_EXECUTABLE(${target_name} ${target_sources} ${target_headers})
    ELSE()
      MESSAGE(FATAL_ERROR "Unknown target type ${target_type}")
    ENDIF()
    SET_PROPERTY(TARGET ${target_name} PROPERTY CXX_STANDARD 17)
    TARGET_INCLUDE_DIRECTORIES(${target_name} PRIVATE ${target_directory}/include)
    FILE(GLOB target_dependencies "${target_directory}/deps/*/")
    FOREACH(target_dependency_dir ${target_dependencies})
      GET_FILENAME_COMPONENT(target_dependency ${target_dependency_dir} NAME)
      MESSAGE("Add dependency for ${target_name} ${target_dependency}")
      ADD_DEPENDENCIES(${target_name} ${target_dependency})
      TARGET_LINK_LIBRARIES(${target_name} ${target_dependency})
      TARGET_INCLUDE_DIRECTORIES(${target_name} PUBLIC ${target_dependency_dir}/include)
    ENDFOREACH()
  ENDFOREACH()
ENDFOREACH()

# --------------------------------------------------------------------------------
# handle all tests.
# --------------------------------------------------------------------------------

FILE(GLOB test_types "tests/*/")
ADD_TEST("Dummy_test" true)
FOREACH(test_type_dir ${test_types})
  GET_FILENAME_COMPONENT(test_type ${test_type_dir} NAME)
  FILE(GLOB test_targets "${test_type_dir}/*/")
  FOREACH(test_target_dir ${test_targets})
    GET_FILENAME_COMPONENT(test_target ${test_target_dir} NAME)
    MESSAGE("----> ${test_target}")
    IF(${test_type} MATCHES io_tests)
      IF(EXISTS "${CMAKE_BINARY_DIR}/${test_target}")
        FILE(GLOB in_files "${test_target_dir}/*.in")
        FOREACH(in_file ${in_files})
          GET_FILENAME_COMPONENT(test_number ${in_file} NAME_WLE)
          MESSAGE("TEST NUMBER ${test_number} of ${in_file}")
          ADD_TEST(NAME "${test_target}${test_number}" COMMAND ${CMAKE_SOURCE_DIR}/tests/test.sh ${CMAKE_BINARY_DIR}/${test_target} ${in_file} ${test_target_dir}/${test_number}.out)
        ENDFOREACH()
      ELSE()
        ADD_TEST("Implement_${test_target}" false)
      ENDIF()
    ELSE()
      MESSAGE(FATAL_ERROR "Error: unknown test type ${test_type}")
    ENDIF()
  ENDFOREACH()
ENDFOREACH()

# --------------------------------------------------------------------------------
# todos
# --------------------------------------------------------------------------------

# Implement additional testing (unit tests)


