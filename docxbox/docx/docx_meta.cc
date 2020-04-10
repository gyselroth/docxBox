// Copyright (c) 2020 gyselroth GmbH

#include <iostream>

#include "docx_meta.h"

docx_meta::docx_meta(int argc, char **argv) {
  argc_ = argc;
  argv_ = argv;
}

void docx_meta::SetPathExtract(const std::string &path) {
  path_extract_ = path;
}

void docx_meta::SetOutputAsJson(bool output_as_json) {
  output_as_json_ = output_as_json;
}

std::string docx_meta::SetCreatorFromCoreXml() {
  return creator_ = helper::String::GetSubStrBetween(
      core_xml_,
      kWordMlCreatorLhs,
      kWordMlCreatorRhs);
}

std::string docx_meta::SetRevisionFromCoreXml() {
  return revision_ = helper::String::GetSubStrBetween(
      core_xml_,
      kWordMlRevisionLhs,
      kWordMlRevisionRhs);
}

std::string docx_meta::SetLanguageFromCoreXml() {
  return language_ = helper::String::GetSubStrBetween(
      core_xml_,
      kWordMlLanguageLhs,
      kWordMlLanguageRhs);
}

std::string docx_meta::SetLastPrintedFromCoreXml() {
  return last_printed_ = helper::String::GetSubStrBetween(
      core_xml_,
      kWordMlLastPrintedLhs,
      kWordMlLastPrintedRhs);
}

std::string docx_meta::SetModifiedFromCoreXml() {
  return date_modification_ = helper::String::GetSubStrBetween(
      core_xml_,
      kWordMlModifiedLhs,
      kWordMlModifiedRhs);
}

std::string docx_meta::SetLastModifiedByFromCoreXml() {
  return last_modified_by_ = helper::String::GetSubStrBetween(
      core_xml_,
      kWordMlLastModifiedByLhs,
      kWordMlLastModifiedByRhs);
}

std::string docx_meta::SetTitleFromCoreXml() {
  return title_ = helper::String::GetSubStrBetween(
      core_xml_,
      kWordMlTitleLhs,
      kWordMlTitleRhs);
}

std::string docx_meta::SetCreatedFromCoreXml() {
  return date_creation_ = helper::String::GetSubStrBetween(
      core_xml_,
      kWordMlCreatedLhs,
      kWordMlCreatedRhs);
}

docx_meta::Attribute docx_meta::ResolveAttributeByName(
    const std::string &attribute) {
  if (attribute == "title") return Attribute_Title;
  if (attribute == "language") return Attribute_Language;
  if (attribute == "revision") return Attribute_Revision;
  if (attribute == "creator") return Attribute_Creator;
  if (attribute == "lastModifiedBy") return Attribute_LastModifiedBy;
  if (attribute == "created") return Attribute_Created;
  if (attribute == "modified") return Attribute_Modified;
  if (attribute == "lastPrinted") return Attribute_LastPrinted;

  return Attribute_Unknown;
}

bool docx_meta::InitModificationArguments() {
  if (!docxbox::AppArguments::IsArgumentGiven(
      argc_,
      3,
      "Meta attribute to be set"))
    return false;

  attribute_ = ResolveAttributeByName(argv_[3]);

  if (attribute_ == Attribute::Attribute_Unknown) {
    std::cout
        << "Invalid argument: Unknown or unsupported attribute: "
        << argv_[3] << "\n";

    return false;
  }

  if (!docxbox::AppArguments::IsArgumentGiven(
      argc_,
      4,
      "Value to set attribute to"))
    return false;

  value_ = argv_[4];

  return true;
}

bool docx_meta::UpsertAttribute() {
  path_core_xml_ = path_extract_ + "/docProps/core.xml";

  LoadCoreXml(path_core_xml_);

  return AttributeExistsInCoreXml(attribute_)
    ? UpdateAttribute(attribute_, value_)
    : InsertAttribute(attribute_, value_);
}

bool docx_meta::UpdateAttribute(Attribute attribute, const std::string& value) {
  EnsureIsLoadedCoreXml();

  switch (attribute) {
    case Attribute_Title:
      core_xml_ = helper::String::Replace(
          core_xml_,
          std::string(kWordMlTitleLhs).append(SetTitleFromCoreXml()).c_str(),
          std::string(kWordMlTitleLhs).append(value).c_str());

      return true;
    case Attribute_Language:
      core_xml_ = helper::String::Replace(
          core_xml_,
          std::string(kWordMlLanguageLhs)
              .append(SetLanguageFromCoreXml()).c_str(),
          std::string(kWordMlLanguageLhs).append(value).c_str());

      return true;
    case Attribute_Revision:
      core_xml_ = helper::String::Replace(
          core_xml_,
          std::string(kWordMlRevisionLhs)
              .append(SetRevisionFromCoreXml()).c_str(),
          std::string(kWordMlRevisionLhs).append(value).c_str());

      return true;
    case Attribute_Creator:
      core_xml_ = helper::String::Replace(
          core_xml_,
          std::string(kWordMlCreatorLhs)
              .append(SetCreatorFromCoreXml()).c_str(),
          std::string(kWordMlCreatorLhs).append(value).c_str());

      return true;
    case Attribute_LastModifiedBy:
      core_xml_ = helper::String::Replace(
          core_xml_,
          std::string(kWordMlLastModifiedByLhs).append(
              SetLastModifiedByFromCoreXml()).c_str(),
          std::string(kWordMlLastModifiedByLhs).append(value).c_str());

      return true;
    case Attribute_Created:
      core_xml_ = helper::String::Replace(
          core_xml_,
          std::string(kWordMlCreatedLhs)
              .append(SetCreatedFromCoreXml()).c_str(),
          std::string(kWordMlCreatedLhs).append(value).c_str());

      return true;
    case Attribute_Modified:
      core_xml_ = helper::String::Replace(
          core_xml_,
          std::string(kWordMlModifiedLhs)
              .append(SetModifiedFromCoreXml()).c_str(),
          std::string(kWordMlModifiedLhs).append(value).c_str());

      return true;
    case Attribute_LastPrinted:
      core_xml_ = helper::String::Replace(
          core_xml_,
          std::string(kWordMlLastPrintedLhs)
          .append(SetLastPrintedFromCoreXml()).c_str(),
          std::string(kWordMlLastPrintedLhs).append(value).c_str());

      return true;
    default:return false;
  }
}

bool docx_meta::InsertAttribute(Attribute attribute, const std::string& value) {
  EnsureIsLoadedCoreXml();

  std::string replacement;

  switch (attribute) {
    case Attribute_Title:
      replacement =
          kWordMlTitleLhs + value + kWordMlTitleRhs + kWordMlCorePropertiesRhs;

      core_xml_ = helper::String::Replace(
          core_xml_,
          kWordMlCorePropertiesRhs,
          replacement.c_str());

      return true;
    case Attribute_Language:
      replacement = kWordMlLanguageLhs + value + kWordMlLanguageRhs
          + kWordMlCorePropertiesRhs;

      core_xml_ = helper::String::Replace(
          core_xml_,
          kWordMlCorePropertiesRhs,
          replacement.c_str());

      return true;
    case Attribute_Revision:
      replacement = kWordMlRevisionLhs + value + kWordMlRevisionRhs
          + kWordMlCorePropertiesRhs;

      core_xml_ = helper::String::Replace(
          core_xml_,
          kWordMlCorePropertiesRhs,
          replacement.c_str());

      return true;
    case Attribute_Creator:
      replacement = kWordMlCreatorLhs + value + kWordMlCreatorRhs
          + kWordMlCorePropertiesRhs;

      core_xml_ = helper::String::Replace(
          core_xml_,
          kWordMlCorePropertiesRhs,
          replacement.c_str());

      return true;
    case Attribute_LastModifiedBy:
      replacement = kWordMlLastModifiedByLhs + value + kWordMlLastModifiedByRhs
          + kWordMlCorePropertiesRhs;

      core_xml_ = helper::String::Replace(
          core_xml_,
          kWordMlCorePropertiesRhs,
          replacement.c_str());

      return true;
    case Attribute_Created:
      replacement = kWordMlCreatedLhs + value + kWordMlCreatedRhs
          + kWordMlCorePropertiesRhs;

      core_xml_ = helper::String::Replace(
          core_xml_,
          kWordMlCorePropertiesRhs,
          replacement.c_str());

      return true;
    case Attribute_Modified:
      replacement = kWordMlModifiedLhs + value + kWordMlModifiedRhs
          + kWordMlCorePropertiesRhs;

      core_xml_ = helper::String::Replace(
          core_xml_,
          kWordMlCorePropertiesRhs,
          replacement.c_str());

      return true;
    case Attribute_LastPrinted:
      replacement = kWordMlLastPrintedLhs + value + kWordMlLastPrintedRhs
          + kWordMlCorePropertiesRhs;

      core_xml_ = helper::String::Replace(
          core_xml_,
          kWordMlCorePropertiesRhs,
          replacement.c_str());

      return true;
    default:return false;
  }
}

// Check whether core.xml of current DOCX contains given attribute
bool docx_meta::AttributeExistsInCoreXml(Attribute attribute) {
  EnsureIsLoadedCoreXml();

  switch (attribute) {
    case Attribute_Title:
      return helper::String::Contains(core_xml_, kWordMlTitleLhs);
    case Attribute_Language:
      return helper::String::Contains(core_xml_, kWordMlLanguageLhs);
    case Attribute_Revision:
      return helper::String::Contains(core_xml_, kWordMlRevisionLhs);
    case Attribute_Creator:
      return helper::String::Contains(core_xml_, kWordMlCreatorLhs);
    case Attribute_LastModifiedBy:
      return helper::String::Contains(core_xml_, kWordMlLastModifiedByLhs);
    case Attribute_Created:
      return helper::String::Contains(core_xml_, kWordMlCreatedLhs);
    case Attribute_Modified:
      return helper::String::Contains(core_xml_, kWordMlModifiedLhs);
    case Attribute_LastPrinted:
      return helper::String::Contains(core_xml_, kWordMlLastPrintedLhs);
    default:return false;
  }
}

void docx_meta::EnsureIsLoadedCoreXml() {
  if (core_xml_.empty()) {
    path_core_xml_ = path_extract_ + "/docProps/core.xml";
    core_xml_ = helper::File::GetFileContents(path_core_xml_);
  }
}

void docx_meta::LoadCoreXml(std::string path) {
  core_xml_ = helper::File::GetFileContents(path);
}

bool docx_meta::SaveCoreXml() {
  if (helper::File::FileExists(core_xml_))
    helper::File::Remove(path_core_xml_.c_str());

  return helper::File::WriteToNewFile(path_core_xml_, core_xml_);
}

void docx_meta::CollectFromAppXml(std::string path_app_xml_current,
                                  std::string app_xml) {
  // Attempt output after collecting meta data from app.xml and core.xml,
  // but when parsing the 2nd app.xml w/o having parsed a rel. core.xml:
  // output prematurely
  if (has_collected_from_app_xml_) Output();

  path_app_xml_ = std::move(path_app_xml_current);

  u_int32_t offset = 0;

  xml_schema_ = helper::String::GetSubStrBetween(
      app_xml,
      kWordMlXmlSchemeLhs,
      kWordMlXmlSchemeRhs,
      offset);

  // Remove last segment from schema,
  // leaving:  http://schemas.openxmlformats.org/officeDocument/<year>
  std::vector<std::string> segments = helper::String::Explode(xml_schema_, '/');

  helper::String::Replace(
      xml_schema_,
      (std::string("/")+segments[segments.size()-1]).c_str(),
      "");

  has_collected_from_app_xml_ = true;

  if (has_collected_from_app_xml_ && has_collected_from_core_xml_) Output();
}

void docx_meta::CollectFromCoreXml(std::string path_core_xml_current) {
  // Attempt output after collecting meta data from app.xml and core.xml,
  // but when parsing the 2nd core.xml w/o having parsed a rel. app.xml:
  // output prematurely
  if (has_collected_from_core_xml_) Output();

  path_core_xml_ = std::move(path_core_xml_current);

  SetCreatedFromCoreXml();
  SetTitleFromCoreXml();
  SetLastModifiedByFromCoreXml();
  SetModifiedFromCoreXml();
  SetLastPrintedFromCoreXml();
  SetLanguageFromCoreXml();
  SetRevisionFromCoreXml();
  SetCreatorFromCoreXml();

  has_collected_from_core_xml_ = true;

  if (has_collected_from_app_xml_ && has_collected_from_core_xml_) Output();
}

void docx_meta::Output() {
  if (has_collected_from_app_xml_ || has_collected_from_core_xml_) {
    if (output_as_json_)
      OutputJson();
    else
      OutputPlain();
  }

  Clear();
}

void docx_meta::OutputPlain() {
  std::cout
      << (has_collected_from_app_xml_
          ? path_app_xml_ + ":\n"
            + helper::String::Repeat("-", path_app_xml_.length() + 1) + "\n"
            + "XML Schema: " + xml_schema_ + "\n"
          : "")
      << (has_collected_from_core_xml_
          ? (has_collected_from_app_xml_ ? "\n" : "")
            + path_core_xml_ + ":\n"
            + helper::String::Repeat("-", path_core_xml_.length() + 1) + "\n"
            + "Created:        " + date_creation_ + "\n"
            + "Language:       " + language_ + "\n"
            + "Revision:       " + revision_ + "\n"
            + "Creator:        " + creator_ + "\n"
            + "LastModified:   " + date_modification_ + "\n"
            + "LastModifiedBy: " + last_modified_by_ + "\n"
            + "LastPrinted:    " + last_printed_ + "\n"
          : "") << "\n";
}

void docx_meta::OutputJson() {
  std::cout
      << "["
      << (has_collected_from_app_xml_
          ? R"({"app.xml": ")" + path_app_xml_ + "\","
            R"("schema": ")" + xml_schema_ + "\"}"
          : "")
      << (has_collected_from_core_xml_
          ? std::string(has_collected_from_app_xml_ ? "," : "")
            + R"({"core.xml": ")" + path_core_xml_ + "\","
            + R"("created":")" + date_creation_ + "\","
            + R"("language":")" + language_ + "\","
            + R"("revision":")" + revision_ + "\","
            + R"("creator":")" + creator_ + "\","
            + R"("modified":")" + date_modification_ + "\","
            + R"("lastModifiedBy":")" + last_modified_by_ + "\","
            + R"("lastPrinted":")" + last_printed_ + "\"}"
          : "") << "]";
}

void docx_meta::Clear() {
  has_collected_from_app_xml_ = false;
  has_collected_from_core_xml_ = false;

  path_app_xml_ = "";
  path_core_xml_ = "";
}
