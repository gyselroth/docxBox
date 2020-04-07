
#include <iostream>
#include "docx_meta.h"

docx_meta::docx_meta(int argc, char **argv) {
  this->argc = argc;
  this->argv = argv;
}

void docx_meta::SetOutputAsJson(bool output_as_json) {
  outputAsJson = output_as_json;
}

docx_meta::Attribute docx_meta::ResolveAttributeByName(const std::string &attribute) {
  if (attribute=="title") return Attribute_Title;
  if (attribute=="language") return Attribute_Language;
  if (attribute=="revision") return Attribute_Revision;
  if (attribute=="creator") return Attribute_Creator;
  if (attribute=="lastModifiedBy") return Attribute_LastModifiedBy;
  if (attribute=="created") return Attribute_Created;
  if (attribute=="modified") return Attribute_Modified;
  if (attribute=="lastPrinted") return Attribute_LastPrinted;

  return Attribute_Unknown;
}

bool docx_meta::AreModificationArgumentsValid() {
  if (argc <= 3) {
    std::cout << "Missing argument: Meta attribute to be set\n";

    return false;
  }

  if (!IsSupportedAttribute(argv[3])) {
    std::cout << "Invalid argument: Unknown or unsupported attribute: " << argv[3] << "\n";

    return false;
  }

  if (argc <= 4) {
    std::cout << "Missing argument: Value to set attribute to\n";

    return false;
  }

  return true;
}

bool docx_meta::IsSupportedAttribute(const std::string& attribute) {
  return Attribute_Unknown != ResolveAttributeByName(attribute);
}

void docx_meta::CollectFromAppXml(std::string path_app_xml_current, std::string app_xml) {
  // Attempt output after collecting meta data from app.xml and core.xml,
  // but when parsing the 2nd app.xml w/o having parsed a rel. core.xml: output prematurely
  if (has_collected_from_app_xml) Output();

  path_app_xml = std::move(path_app_xml_current);

  unsigned long offset = 0;

  xml_schema = helper::String::GetSubStrBetween(app_xml, "<Properties xmlns=\"", "\" ", offset);

  // Remove last segment from schema, leaving:  http://schemas.openxmlformats.org/officeDocument/<year>
  std::vector<std::string> segments = helper::String::Explode(xml_schema, '/');
  helper::String::Replace(xml_schema, (std::string("/")+segments[segments.size()-1]).c_str(), "");

  has_collected_from_app_xml = true;

  if (has_collected_from_app_xml && has_collected_from_core_xml) Output();
}

void docx_meta::CollectFromCoreXml(std::string path_core_xml_current, std::string core_xml) {
  // Attempt output after collecting meta data from app.xml and core.xml,
  // but when parsing the 2nd core.xml w/o having parsed a rel. app.xml: output prematurely
  if (has_collected_from_core_xml) Output();

  path_core_xml = std::move(path_core_xml_current);

  date_creation = helper::String::GetSubStrBetween(
      core_xml,
      "<dcterms:created xsi:type=\"dcterms:W3CDTF\">",
      "</dcterms:created>"
  );

  title = helper::String::GetSubStrBetween(core_xml, "<dc:title>", "</dc:title>");
  last_modified_by = helper::String::GetSubStrBetween(core_xml, "<cp:lastModifiedBy>", "</cp:lastModifiedBy>");

  date_modification = helper::String::GetSubStrBetween(
      core_xml,
      "<dcterms:modified xsi:type=\"dcterms:W3CDTF\">",
      "</dcterms:modified>"
  );

  last_printed = helper::String::GetSubStrBetween(core_xml, "<cp:lastPrinted>", "</cp:lastPrinted>");
  language = helper::String::GetSubStrBetween(core_xml, "<dc:language>", "</dc:language>");
  revision = helper::String::GetSubStrBetween(core_xml, "<cp:revision>", "</cp:revision>");
  creator = helper::String::GetSubStrBetween(core_xml, "<dc:creator>", "</dc:creator>");

  has_collected_from_core_xml = true;

  if (has_collected_from_app_xml && has_collected_from_core_xml) Output();
}

void docx_meta::Output() {
  if (has_collected_from_app_xml || has_collected_from_core_xml) {
    if (outputAsJson) OutputJson();
    else OutputPlain();
  }

  Clear();
}

void docx_meta::OutputPlain() {
  std::cout
      << (has_collected_from_app_xml
          ? path_app_xml + ":\n"
            + helper::String::Repeat("-", path_app_xml.length() + 1) + "\n"
            + "XML Schema: " + xml_schema + "\n"
          : ""
      )
      << (has_collected_from_core_xml
          ? (has_collected_from_app_xml ? "\n" : "")
            + path_core_xml + ":\n"
            + helper::String::Repeat("-", path_core_xml.length() + 1) + "\n"
            + "Created:        " + date_creation + "\n"
            + "Language:       " + language + "\n"
            + "Revision:       " + revision + "\n"
            + "Creator:        " + creator + "\n"
            + "LastModified:   " + date_modification + "\n"
            + "LastModifiedBy: " + last_modified_by + "\n"
            + "LastPrinted:    " + last_printed + "\n"
          : ""
      ) << "\n";
}

void docx_meta::OutputJson() {
  std::cout
      << "["
      << (has_collected_from_app_xml
          ? R"({"app.xml": ")" + path_app_xml + "\","
            R"("schema": ")" + xml_schema + "\"}"
          : ""
      )
      << (has_collected_from_core_xml
          ? std::string(has_collected_from_app_xml ? "," : "")
            + R"({"core.xml": ")" + path_core_xml + "\","
            + R"("created":")" + date_creation + "\","
            + R"("language":")" + language + "\","
            + R"("revision":")" + revision + "\","
            + R"("creator":")" + creator + "\","
            + R"("modified":")" + date_modification + "\","
            + R"("lastModifiedBy":")" + last_modified_by + "\","
            + R"("lastPrinted":")" + last_printed + "\"}"
          : ""
      ) << "]";
}

void docx_meta::Clear() {
  has_collected_from_app_xml = false;
  has_collected_from_core_xml = false;

  path_app_xml = "";
  path_core_xml = "";
}
