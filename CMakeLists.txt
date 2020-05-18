cmake_minimum_required(VERSION 3.10)
#cmake_minimum_required(VERSION 3.14)

project(docxbox)

include_directories(${CMAKE_SOURCE_DIR})

set (VERSION_MAJOR "0")
set (VERSION_MINOR "0")
set (VERSION_PATCH "4")

# configure config.h to be generated from config.h.in,
# with injected version number
configure_file ("docxbox/config.h.in" "docxbox/config.h")

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/linux)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y -pedantic")

add_executable(docxbox
        docxbox/main.cc

        docxbox/app/app.cc
        docxbox/app/app_argument.cc
        docxbox/app/app_command.cc
        docxbox/app/app_help.cc
        docxbox/app/app_help.h

        docxbox/docx/docx_archive.cc
        docxbox/docx/docx_archive_list.cc
        docxbox/docx/docx_archive_replace.cc

        docxbox/docx/docx_fileListCompare.cc
        docxbox/docx/docx_fontTable.cc
        docxbox/docx/docx_meta.cc
        docxbox/docx/docx_media.cc
        docxbox/docx/docx_numbering.cc

        docxbox/docx/renderer/docx_renderer.cc
        docxbox/docx/renderer/docx_renderer_delegate.cc
        docxbox/docx/renderer/docx_renderer_heading.cc
        docxbox/docx/renderer/docx_renderer_image.cc
        docxbox/docx/renderer/docx_renderer_list.cc
        docxbox/docx/renderer/docx_renderer_rels.cc
        docxbox/docx/renderer/docx_renderer_table.cc

        docxbox/docx/xml/docx_xml.cc
        docxbox/docx/xml/docx_xml_field.cc
        docxbox/docx/xml/docx_xml_indent.cc
        docxbox/docx/xml/docx_xml_lorem.cc
        docxbox/docx/xml/docx_xml_to_plaintext.cc
        docxbox/docx/xml/docx_xml_rels.cc
        docxbox/docx/xml/docx_xml_remove.cc
        docxbox/docx/xml/docx_xml_replace.cc

        docxbox/helper/helper_cli.cc
        docxbox/helper/helper_dateTime.cc
        docxbox/helper/helper_file.cc
        docxbox/helper/helper_string.cc
        docxbox/helper/helper_numeric.cc

        vendor/miniz-cpp/zip_file.hpp
        vendor/tinyxml2/tinyxml2.cpp)
