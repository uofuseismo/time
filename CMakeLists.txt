cmake_minimum_required(VERSION 3.16)
project(Time VERSION 1.0.0 LANGUAGES CXX)
enable_testing()

# Ensure we have necessary packages 
include(CheckCXXCompilerFlag)
find_package(GTest REQUIRED)
find_package(date COMPONENTS date::date REQUIRED)

# Include information
configure_file(${CMAKE_SOURCE_DIR}/include/private/version.hpp.in
               ${CMAKE_SOURCE_DIR}/include/time/version.hpp)
set(PUBLIC_HEADER_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PRIVATE_HEADER_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/private)

# The library
set(SRC
    src/utc.cpp
    src/version.cpp)
add_library(time SHARED ${SRC})
target_include_directories(time
                           PRIVATE date::date
                           PUBLIC $<BUILD_INTERFACE:${PUBLIC_HEADER_DIRECTORIES}>
                           PUBLIC $<INSTALL_INTERFACE:${PUBLIC_HEADER_DIRECTORIES}>)
set_target_properties(time PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO)

# Python bindings
option(WRAP_PYTHON "WRAP_PYTHON" OFF)
if (WRAP_PYTHON)
   find_package(pybind11 REQUIRED)
   add_library(pytime MODULE
               python/pytime.cpp
               python/putc.cpp)
   target_link_libraries(pytime PRIVATE pybind11::module time)
   target_include_directories(pytime
                              PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/python>
                              PRIVATE $<BUILD_INTERFACE:${PUBLIC_HEADER_DIRECTORIES})
   set_target_properties(pytime PROPERTIES
                         PREFIX ""
                         CXX_STANDARD 17
                         CXX_STANDARD_REQUIRED YES
                         CXX_EXTENSIONS NO) 
endif()

# Unit testing
set(TEST_SRC
    testing/main.cpp
    testing/utc.cpp)
add_executable(unitTests ${TEST_SRC})
set_target_properties(unitTests PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO)
target_link_libraries(unitTests PRIVATE time ${GTEST_BOTH_LIBRARIES})
target_include_directories(unitTests
                           PRIVATE ${GTEST_INCLUDE_DIRS}
                           PUBLIC $<BUILD_INTERFACE:${PUBLIC_HEADER_DIRECTORIES}>)
add_test(NAME unitsTests
         COMMAND unitTests)

if (WRAP_PYTHON)
   file(COPY ${CMAKE_SOURCE_DIR}/python/unit_test.py DESTINATION .)
   add_test(NAME python_tests
            COMMAND ${PYTHON_EXECUTABLE} -m pytest)
endif()

#========================================================================================#
#                                      Installation                                      #
#========================================================================================#
include(GNUInstallDirs)
if (WRAP_PYTHON)
   install(TARGETS time pytime
           RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
           LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
           ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
   #        INCLUDES
   #        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
else()
   install(TARGETS time
           RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
           LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
           ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
install(DIRECTORY ${PUBLIC_HEADER_DIRECTORIES}/time
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
##########################################################################################
#                                     CPACK Packaging                                    #
##########################################################################################
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_VENDOR "UUSS")
set(CPACK_PACKAGE_CONTACT "ben.baker@utah.edu")
set(CPACK_PACKAGE_LICENSE "MIT")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A library for common time manipulations in seismology.")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_COMPONENTS_ALL libraries headers)
include(CPack)
