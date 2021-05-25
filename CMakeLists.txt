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
                           PUBLIC $<INSTALL_INTERFACE:${PUBLIC_HEADER_DIRECTORIES})
set_target_properties(time PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO)

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

#========================================================================================#
#                                      Installation                                      #
#========================================================================================#
include(GNUInstallDirs)
if (WRAP_PYTHON)
   install(TARGETS time pytime
           RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
           LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
           ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
           INCLUDES
           PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
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
set(CPACK_PACKAGE_NAME "time")
set(CPACK_PACKAGE_VENDOR "UUSS")
set(CPACK_PACKAGE_CONTACT "bbaker@seis.utah.edu")
set(CPACK_PACKAGE_LICENSE "MIT")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A library for common time manipulations in seismology.")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
