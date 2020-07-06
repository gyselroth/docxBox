cmake_minimum_required(VERSION 3.10)
#cmake_minimum_required(VERSION 3.14)

project(docxbox)

include_directories(${CMAKE_SOURCE_DIR})

set (VERSION_MAJOR "1")
set (VERSION_MINOR "0")
set (VERSION_PATCH "0")

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
        docxbox/app/app_log.cc

        docxbox/docx/docx_batch.cc
        docxbox/docx/docx_diff.cc
        docxbox/docx/docx_compare.cc

        docxbox/docx/archive/docx_archive.cc
        docxbox/docx/archive/docx_archive_list.cc
        docxbox/docx/archive/docx_archive_replace.cc

        docxbox/docx/component/contentTypes.cc
        docxbox/docx/component/fontTable.cc
        docxbox/docx/component/meta.cc
        docxbox/docx/component/media.cc
        docxbox/docx/component/numbering.cc
        docxbox/docx/component/partNames.cc
        docxbox/docx/component/rels.cc
        docxbox/docx/component/styles.cc

        docxbox/docx/renderer/docx_renderer.cc
        docxbox/docx/renderer/docx_renderer_delegate.cc
        docxbox/docx/renderer/contentType/docx_renderer_heading.cc
        docxbox/docx/renderer/contentType/docx_renderer_image.cc
        docxbox/docx/renderer/contentType/docx_renderer_link.cc
        docxbox/docx/renderer/contentType/docx_renderer_list.cc
        docxbox/docx/renderer/contentType/docx_renderer_paragraph.cc
        docxbox/docx/renderer/component/docx_renderer_rels.cc
        docxbox/docx/renderer/component/docx_renderer_styles.cc
        docxbox/docx/renderer/contentType/docx_renderer_table.cc
        docxbox/docx/renderer/contentType/docx_renderer_text.cc

        docxbox/docx/xml/docx_xml.cc
        docxbox/docx/xml/component/docx_xml_contentTypes.cc
        docxbox/docx/xml/component/docx_xml_rels.cc
        docxbox/docx/xml/component/docx_xml_styles.cc
        docxbox/docx/xml/contentType/docx_xml_field.cc
        docxbox/docx/xml/visitor/docx_xml_tidy.cc
        docxbox/docx/xml/visitor/docx_xml_lorem.cc
        docxbox/docx/xml/visitor/docx_xml_to_plaintext.cc
        docxbox/docx/xml/visitor/docx_xml_remove.cc
        docxbox/docx/xml/visitor/docx_xml_replace.cc

        docxbox/helper/helper_cli.cc
        docxbox/helper/helper_dateTime.cc
        docxbox/helper/helper_file.cc
        docxbox/helper/helper_json.cc
        docxbox/helper/helper_string.cc
        docxbox/helper/helper_numeric.cc
        docxbox/helper/helper_xml.cc

        vendor/miniz-cpp/zip_file.hpp
        vendor/tinyxml2/tinyxml2.cpp)
