
#include <iostream>
#include "docx_meta.h"
#include "../../helper/helper_file.h"

docx_meta::docx_meta(int argc, char **argv) {
  this->argc = argc;
  this->argv = argv;
}

void docx_meta::SetPathExtract(const std::string &path) {
  path_extract = path;
}

void docx_meta::SetOutputAsJson(bool output_as_json) {
  outputAsJson = output_as_json;
}

std::string docx_meta::SetCreatorFromCoreXml() {
  return creator = helper::String::GetSubStrBetween(core_xml, kWordMlCreatorLhs, kWordMlCreatorRhs);
}

std::string docx_meta::SetRevisionFromCoreXml() {
  return revision = helper::String::GetSubStrBetween(core_xml, kWordMlRevisionLhs, kWordMlRevisionRhs);
}

std::string docx_meta::SetLanguageFromCoreXml() {
  return language = helper::String::GetSubStrBetween(core_xml, kWordMlLanguageLhs, kWordMlLanguageRhs);
}

std::string docx_meta::SetLastPrintedFromCoreXml() {
  return last_printed = helper::String::GetSubStrBetween(core_xml, kWordMlLastPrintedLhs, kWordMlLastPrintedRhs);
}

std::string docx_meta::SetModifiedFromCoreXml() {
  return date_modification = helper::String::GetSubStrBetween(core_xml, kWordMlModifiedLhs, kWordMlModifiedRhs);
}

std::string docx_meta::SetLastModifiedByFromCoreXml() {
  return last_modified_by = helper::String::GetSubStrBetween(
      core_xml,
      kWordMlLastModifiedByLhs,
      kWordMlLastModifiedByRhs
      );
}

std::string docx_meta::SetTitleFromCoreXml() {
  return title = helper::String::GetSubStrBetween(core_xml, kWordMlTitleLhs, kWordMlTitleRhs);
}

std::string docx_meta::SetCreatedFromCoreXml() {
  return date_creation = helper::String::GetSubStrBetween(core_xml, kWordMlCreatedLhs, kWordMlCreatedRhs);
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

bool docx_meta::InitModificationArguments() {
  if (argc <= 3) {
    std::cout << "Missing argument: Meta attribute to be set\n";

    return false;
  }

  attribute_ = ResolveAttributeByName(argv[3]);

  if (attribute_ == Attribute::Attribute_Unknown) {
    std::cout << "Invalid argument: Unknown or unsupported attribute: " << argv[3] << "\n";

    return false;
  }

  if (argc <= 4) {
    std::cout << "Missing argument: Value to set attribute to\n";

    return false;
  }

  value_ = argv[4];

  return true;
}

bool docx_meta::UpsertAttribute() {
  path_core_xml = path_extract + "/docProps/core.xml";
  LoadCoreXml(path_core_xml);

  return AttributeExistsInCoreXml(attribute_)
    ? UpdateAttribute(attribute_, value_)
    : InsertAttribute(attribute_, value_);
}

bool docx_meta::UpdateAttribute(Attribute attribute, const std::string& value) {
  EnsureIsLoadedCoreXml();

  switch (attribute) {
    case Attribute_Title:
      core_xml = helper::String::Replace(
          core_xml,
          std::string(kWordMlTitleLhs).append(SetTitleFromCoreXml()).c_str(),
          std::string(kWordMlTitleLhs).append(value).c_str()
      );

      return true;
    case Attribute_Language:
      core_xml = helper::String::Replace(
          core_xml,
          std::string(kWordMlLanguageLhs).append(SetLanguageFromCoreXml()).c_str(),
          std::string(kWordMlLanguageLhs).append(value).c_str()
      );

      return true;
    case Attribute_Revision:
      core_xml = helper::String::Replace(
          core_xml,
          std::string(kWordMlRevisionLhs).append(SetRevisionFromCoreXml()).c_str(),
          std::string(kWordMlRevisionLhs).append(value).c_str()
      );

      return true;
    case Attribute_Creator:
      core_xml = helper::String::Replace(
          core_xml,
          std::string(kWordMlCreatorLhs).append(SetCreatorFromCoreXml()).c_str(),
          std::string(kWordMlCreatorLhs).append(value).c_str()
      );

      return true;
    case Attribute_LastModifiedBy:
      core_xml = helper::String::Replace(
          core_xml,
          std::string(kWordMlLastModifiedByLhs).append(SetLastModifiedByFromCoreXml()).c_str(),
          std::string(kWordMlLastModifiedByLhs).append(value).c_str()
      );

      return true;
    case Attribute_Created:
      core_xml = helper::String::Replace(
          core_xml,
          std::string(kWordMlCreatedLhs).append(SetCreatedFromCoreXml()).c_str(),
          std::string(kWordMlCreatedLhs).append(value).c_str()
      );

      return true;
    case Attribute_Modified:
      core_xml = helper::String::Replace(
          core_xml,
          std::string(kWordMlModifiedLhs).append(SetModifiedFromCoreXml()).c_str(),
          std::string(kWordMlModifiedLhs).append(value).c_str()
      );

      return true;
    case Attribute_LastPrinted:
      core_xml = helper::String::Replace(
          core_xml,
          std::string(kWordMlLastPrintedLhs).append(SetTitleFromCoreXml()).c_str(),
          std::string(kWordMlLastPrintedLhs).append(value).c_str()
      );

      return true;
    default:return false;
  }
}

bool docx_meta::InsertAttribute(Attribute attribute, std::string value) {
  // @todo implement

  return true;
}

bool docx_meta::AttributeExistsInCoreXml(Attribute attribute) {
  EnsureIsLoadedCoreXml();

  switch (attribute) {
    case Attribute_Title:return helper::String::Contains(core_xml, kWordMlTitleLhs);
    case Attribute_Language:return helper::String::Contains(core_xml, kWordMlLanguageLhs);
    case Attribute_Revision:return helper::String::Contains(core_xml, kWordMlRevisionLhs);
    case Attribute_Creator:return helper::String::Contains(core_xml, kWordMlCreatorLhs);
    case Attribute_LastModifiedBy:return helper::String::Contains(core_xml, kWordMlLastModifiedByLhs);
    case Attribute_Created:return helper::String::Contains(core_xml, kWordMlCreatedLhs);
    case Attribute_Modified:return helper::String::Contains(core_xml, kWordMlModifiedLhs);
    case Attribute_LastPrinted:return helper::String::Contains(core_xml, kWordMlLastPrintedLhs);
    default:return false;  
  }
}

void docx_meta::EnsureIsLoadedCoreXml() {
  if (core_xml.empty()) {
    path_core_xml = path_extract + "/docProps/core.xml";
    core_xml = helper::File::GetFileContents(path_core_xml);
  }
}

void docx_meta::LoadCoreXml(std::string path) {
  core_xml = helper::File::GetFileContents(path);
}

bool docx_meta::SaveCoreXml() {
  if (helper::File::FileExists(core_xml)) helper::File::Remove(path_core_xml.c_str());

  return helper::File::WriteToNewFile(path_core_xml, core_xml);
}

void docx_meta::CollectFromAppXml(std::string path_app_xml_current, std::string app_xml) {
  // Attempt output after collecting meta data from app.xml and core.xml,
  // but when parsing the 2nd app.xml w/o having parsed a rel. core.xml: output prematurely
  if (has_collected_from_app_xml) Output();

  path_app_xml = std::move(path_app_xml_current);

  unsigned long offset = 0;

  xml_schema = helper::String::GetSubStrBetween(app_xml, kWordMlXmlSchemeLhs, kWordMlXmlSchemeRhs, offset);

  // Remove last segment from schema, leaving:  http://schemas.openxmlformats.org/officeDocument/<year>
  std::vector<std::string> segments = helper::String::Explode(xml_schema, '/');
  helper::String::Replace(xml_schema, (std::string("/")+segments[segments.size()-1]).c_str(), "");

  has_collected_from_app_xml = true;

  if (has_collected_from_app_xml && has_collected_from_core_xml) Output();
}

void docx_meta::CollectFromCoreXml(std::string path_core_xml_current) {
  // Attempt output after collecting meta data from app.xml and core.xml,
  // but when parsing the 2nd core.xml w/o having parsed a rel. app.xml: output prematurely
  if (has_collected_from_core_xml) Output();

  path_core_xml = std::move(path_core_xml_current);

  SetCreatedFromCoreXml();
  SetTitleFromCoreXml();
  SetLastModifiedByFromCoreXml();
  SetModifiedFromCoreXml();
  SetLastPrintedFromCoreXml();
  SetLanguageFromCoreXml();
  SetRevisionFromCoreXml();
  SetCreatorFromCoreXml();

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
