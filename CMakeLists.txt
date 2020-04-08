cmake_minimum_required(VERSION 3.10)
#cmake_minimum_required(VERSION 3.14)

project(docxbox)

set (DOCXBOX_VERSION_MAJOR 0)
set (DOCXBOX_VERSION_MINOR 0)
set (DOCXBOX_VERSION_PATCH 1)

# configure config.h to be generated from config.h.in, with injected version number
configure_file ("src/docxbox/config.h.in" "src/docxbox/config.h")

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/linux)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y -pedantic")

add_executable(docxbox
        src/docxbox/main.cpp

        src/docxbox/class/app/app.cc
        src/docxbox/class/app/app_argument.cc
        src/docxbox/class/app/app_command.cc
        src/docxbox/class/app/app_help.cc
        src/docxbox/class/app/app_help.h
        src/docxbox/class/docx/docx_archive.cc
        src/docxbox/class/docx/docx_fontTable.cc
        src/docxbox/class/docx/docx_meta.cc
        src/docxbox/class/docx/docx_xml.cc
        src/docxbox/helper/helper_file.cc
        src/docxbox/helper/helper_string.cc
        src/docxbox/helper/helper_numeric.cc

        src/vendor/tinyxml2/tinyxml2.cpp)
