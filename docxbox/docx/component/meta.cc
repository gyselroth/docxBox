// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/component/meta.h>

meta::meta(int argc, char **argv) {
    argc_ = argc;
    argv_ = argv;
}

meta::Attribute meta::GetAttribute() const {
  return attribute_;
}

const std::string &meta::GetValue() const {
  return value_;
}

void meta::SetAttribute(meta::Attribute attribute) {
  attribute_ = attribute;
}

void meta::SetValue(const std::string &value) {
  value_ = value;
}

void meta::SetPathExtract(const std::string &path) {
  path_extract_ = path;
}

void meta::SetOutputAsJson(bool output_as_json) {
  output_as_json_ = output_as_json;
}

meta::Attribute meta::ResolveAttribute(const std::string &attribute) {
  is_app_attribute_ = true;

  if (attribute == "Application") return Attr_App_Application;
  if (attribute == "AppVersion") return Attr_App_AppVersion;
  if (attribute == "Company") return Attr_App_Company;
  if (attribute == "Template") return Attr_App_Template;
  if (attribute == "xmlSchema") return Attr_App_XmlSchema;

  is_app_attribute_ = false;

  if (attribute == kAttrCoreCreated) return Attr_Core_Created;
  if (attribute == kAttrCoreCreator) return Attr_Core_Creator;
  if (attribute == kAttrCoreDescription) return Attr_Core_Description;
  if (attribute == kAttrCoreKeywords) return Attr_Core_Keywords;
  if (attribute == kAttrCoreLanguage) return Attr_Core_Language;
  if (attribute == kAttrCoreLastModifiedBy) return Attr_Core_LastModifiedBy;
  if (attribute == kAttrCoreLastPrinted) return Attr_Core_LastPrinted;
  if (attribute == kAttrCoreModified) return Attr_Core_Modified;
  if (attribute == kAttrCoreRevision) return Attr_Core_Revision;
  if (attribute == kAttrCoreSubject) return Attr_Core_Subject;
  if (attribute == kAttrCoreTitle) return Attr_Core_Title;

  return Attr_Unknown;
}

// Known attribute is declared in app.xml? (or core.xml)
// Set within ResolveAttributeByName()
bool meta::IsAppAttribute() {
  return is_app_attribute_;
}

bool meta::IsDateAttribute(Attribute attribute) {
  return
      attribute == Attr_Core_LastPrinted
          || attribute == Attr_Core_Created
          || attribute == Attr_Core_Modified;
}

/**
 * @param tag_name
 * @param label     Optional, if empty: same as tag_name
 * @return Fetched value of given attribute
 */
std::string meta::FetchAttributeFromAppXml(
    const char *tag_name,
    const std::string &label) {

  return FetchAttributeFromAppXml(
      GetLhsTagByTagName(tag_name).c_str(),
      GetRhsTagByTagName(tag_name).c_str(),
      label.empty() ? tag_name : label);
}

std::string meta::GetRhsTagByTagName(const char *tag_name) {
  return std::string("</") + tag_name + ">";
}

std::string meta::GetLhsTagByTagName(const char *tag_name) {
  std::string attributes;

  if (helper::String::StartsWith(tag_name, "dcterms:"))
    attributes = kWmlAttributeDcTermsXsi;

  return std::string("<") + tag_name + attributes + ">";
}

// TODO(kay): merge GetLhsTagByAttribute + GetRhsTagByAttribute methods
std::string meta::GetLhsTagByAttribute(const meta::Attribute &attribute) {
  switch (attribute) {
    case Attr_App_Application:return GetLhsTagByTagName(kTagNameApplication);
    case Attr_App_AppVersion:return GetLhsTagByTagName(kTagNameAppVersion);
    case Attr_Core_Created:return GetLhsTagByTagName(kTagNameDcTermsCreated);
    case Attr_Core_Creator:return GetLhsTagByTagName(kTagNameDcCreator);
    case Attr_App_Company:return GetLhsTagByTagName(kTagNameCompany);
    case Attr_Core_Description:return GetLhsTagByTagName(kTagNameDcDescription);
    case Attr_Core_Title:return GetLhsTagByTagName(kTagNameDcTitle);
    case Attr_Core_Language:return GetLhsTagByTagName(kTagNameDcLanguage);
    case Attr_Core_Revision:return GetLhsTagByTagName(kTagNameCpRevision);
    case Attr_Core_LastModifiedBy:
      return GetLhsTagByTagName(kTagNameCpLastModifiedBy);
    case Attr_Core_Modified:return GetLhsTagByTagName(kTagNameDcTermsModified);
    case Attr_Core_LastPrinted:return GetLhsTagByTagName(kTagNameCpLastPrinted);
    case Attr_Core_Subject:return GetLhsTagByTagName(kTagNameDcSubject);
    case Attr_App_Template:return GetLhsTagByTagName(kTagNameTemplate);
    case Attr_App_XmlSchema:return GetLhsTagByTagName(kTagNameXmlSchema);
    default:
      docxbox::AppLog::Error(
          "Failed render opening tag. Unknown attribute: " + attribute);

      return "";
  }
}

std::string meta::GetRhsTagByAttribute(const meta::Attribute &attribute) {
  switch (attribute) {
    case Attr_App_Application:return GetRhsTagByTagName(kTagNameApplication);
    case Attr_App_AppVersion:return GetRhsTagByTagName(kTagNameAppVersion);
    case Attr_Core_Created:return GetRhsTagByTagName(kTagNameDcTermsCreated);
    case Attr_Core_Creator:return GetRhsTagByTagName(kTagNameDcCreator);
    case Attr_App_Company:return GetRhsTagByTagName(kTagNameCompany);
    case Attr_Core_Description:return GetRhsTagByTagName(kTagNameDcDescription);
    case Attr_Core_Title:return GetRhsTagByTagName(kTagNameDcTitle);
    case Attr_Core_Language:return GetRhsTagByTagName(kTagNameDcLanguage);
    case Attr_Core_Revision:return GetRhsTagByTagName(kTagNameCpRevision);
    case Attr_Core_LastModifiedBy:
      return GetRhsTagByTagName(kTagNameCpLastModifiedBy);
    case Attr_Core_Modified:return GetRhsTagByTagName(kTagNameDcTermsModified);
    case Attr_Core_LastPrinted:return GetRhsTagByTagName(kTagNameCpLastPrinted);
    case Attr_Core_Subject:return GetRhsTagByTagName(kTagNameDcSubject);
    case Attr_App_Template:return GetRhsTagByTagName(kTagNameTemplate);
    case Attr_App_XmlSchema:return GetRhsTagByTagName(kTagNameXmlSchema);
    default:
      docxbox::AppLog::Error(
          "Failed render closing tag. Unknown attribute: " + attribute);

      return "";
  }
}

std::string meta::FetchAttributeFromAppXml(
    const char* lhs_of_value,
    const char* rhs_of_value,
    const std::string &label) {
  if (!helper::String::Contains(app_xml_, lhs_of_value)) return "";

  std::string value =
      helper::String::GetSubStrBetween(app_xml_, lhs_of_value, rhs_of_value);

  if (!label.empty()) attributes_.emplace_back(label, value);

  return value;
}

std::string meta::FetchAttributeFromCoreXml(
    const char* tag_name,
    const std::string &label) {
  return FetchAttributeFromCoreXml(
      GetLhsTagByTagName(tag_name).c_str(),
      GetRhsTagByTagName(tag_name).c_str(),
      label);
}

std::string meta::FetchAttributeFromCoreXml(
    const char* lhs_of_value,
    const char* rhs_of_value,
    const std::string &label) {
  if (!helper::String::Contains(core_xml_, lhs_of_value)) return "";

  std::string value = helper::String::GetSubStrBetween(
      core_xml_, lhs_of_value, rhs_of_value);

  if (!label.empty()) attributes_.emplace_back(label, value);

  return value;
}

// Explicit meta modification CLI call:
// Validate CLI arguments and initialize rel. properties
bool meta::InitModificationArguments() {
  if (!docxbox::AppArgument::AreArgumentsGiven(
      argc_,
      2, "DOCX filename",
      3, "Meta attribute to be set")) return false;

  attribute_ = ResolveAttribute(argv_[3]);

  if (attribute_ == Attribute::Attr_Unknown)
    return docxbox::AppLog::Error(
        std::string(
            "Invalid argument: Unknown or unsupported attribute: ") + argv_[3]);

  if (!docxbox::AppArgument::IsArgumentGiven(
      argc_, 4, "Value to set attribute to")) return false;

  value_ = argv_[4];

  return true;
}

bool meta::UpsertAttribute(bool saveXml) {
    return IsAppAttribute()
      ? UpsertAttributeInAppXml(saveXml)
      : UpsertAttributeInCoreXml(saveXml);
}

bool meta::UpsertAttributeInCoreXml(bool saveXml) {
  path_core_xml_ = path_extract_ + "/docProps/core.xml";

  LoadCoreXml(path_core_xml_);

  bool result;

  try {
    if (IsDateAttribute(attribute_)
        && !helper::DateTime::IsIso8601Date(value_))
      return docxbox::AppLog::Error(
          "Invalid date (must be given as ISO 8601): " + value_);

    bool attribute_exists = AttributeExistsInCoreXml(attribute_);

    result = attribute_exists
           ? UpdateCoreAttribute(attribute_, value_)
           : InsertCoreAttribute(attribute_, value_);
  } catch (std::string &message) {
    return docxbox::AppLog::Error(message);
  }

  return result && saveXml
    ? SaveCoreXml()
    : result;
}

bool meta::UpsertAttributeInAppXml(bool saveXml) {
  path_app_xml_ = path_extract_ + "/docProps/app.xml";

  LoadAppXml(path_app_xml_);

  bool result;

  try {
    if (IsDateAttribute(attribute_)
        && !helper::DateTime::IsIso8601Date(value_))
      return docxbox::AppLog::Error(
          "Invalid date (must be given as ISO 8601): " + value_);

    bool attribute_exists = AttributeExistsInAppXml(attribute_);

    result = attribute_exists
           ? UpdateAppAttribute(attribute_, value_)
           : InsertAppAttribute(attribute_, value_);
  } catch (std::string &message) {
    return docxbox::AppLog::Error(message);
  }

  return result && saveXml
    ? SaveAppXml()
    : result;
}

bool meta::UpdateAppAttribute(Attribute attribute, const std::string& value) {
  EnsureIsLoadedAppXml();

  std::string lhs_of_value = GetLhsTagByAttribute(attribute);
  std::string rhs_of_value = GetRhsTagByAttribute(attribute);

  if (lhs_of_value.empty() || rhs_of_value.empty()) return false;

  helper::String::Replace(
      app_xml_,
      (std::string(lhs_of_value)
          + FetchAttributeFromAppXml(
              lhs_of_value.c_str(), rhs_of_value.c_str(), "")).c_str(),
      std::string(lhs_of_value).append(value).c_str());

  return true;
}

bool meta::UpdateCoreAttribute(
    Attribute attribute,
    const std::string& value) {
  EnsureIsLoadedCoreXml();

  std::string lhs_of_value = GetLhsTagByAttribute(attribute);
  std::string rhs_of_value = GetRhsTagByAttribute(attribute);

  if (lhs_of_value.empty() || rhs_of_value.empty()) return false;

  helper::String::Replace(
      core_xml_,
      (std::string(lhs_of_value)
          + FetchAttributeFromCoreXml(
              lhs_of_value.c_str(),
              rhs_of_value.c_str(),
              "")).c_str(),
      std::string(lhs_of_value).append(value).c_str());

  return true;
}

bool meta::InsertAppAttribute(Attribute attribute, const std::string& value) {
  EnsureIsLoadedAppXml();

  const std::string &kLhsTag = GetLhsTagByAttribute(attribute);
  const std::string &kRhs = GetRhsTagByAttribute(attribute);

  if (kLhsTag.empty() || kRhs.empty()) return false;

  helper::String::Replace(
      app_xml_,
      kWordMlCorePropertiesRhs,
      (kLhsTag + value + kRhs).c_str());

  return true;
}

bool meta::InsertCoreAttribute(Attribute attribute, const std::string& value) {
  EnsureIsLoadedCoreXml();

  const std::string &kLhsTag = GetLhsTagByAttribute(attribute);
  const std::string &kRhs = GetRhsTagByAttribute(attribute);

  if (kLhsTag.empty() || kRhs.empty()) return false;

  helper::String::Replace(
      core_xml_,
      kWordMlCorePropertiesRhs,
      (kLhsTag + value + kRhs + kWordMlCorePropertiesRhs).c_str());

  return true;
}

// Check whether core.xml of current DOCX contains given attribute
bool meta::AttributeExistsInAppXml(Attribute attribute) {
  EnsureIsLoadedAppXml();

  std::string lhs_of_tag = GetLhsTagByAttribute(attribute);

  return lhs_of_tag.empty()
    ? false
    : helper::String::Contains(app_xml_, lhs_of_tag.c_str());
}

// Check whether core.xml of current DOCX contains given attribute
bool meta::AttributeExistsInCoreXml(Attribute attribute) {
  EnsureIsLoadedCoreXml();

  std::string lhs_of_tag = GetLhsTagByAttribute(attribute);

  return lhs_of_tag.empty()
    ? false
    : helper::String::Contains(core_xml_, lhs_of_tag.c_str());
}

void meta::EnsureIsLoadedAppXml() {
  if (app_xml_.empty()) {
    path_app_xml_ = path_extract_ + "/docProps/app.xml";
    app_xml_ = helper::File::GetFileContents(path_app_xml_);
  }
}

void meta::EnsureIsLoadedCoreXml() {
  if (core_xml_.empty()) {
    path_core_xml_ = path_extract_ + "/docProps/core.xml";
    core_xml_ = helper::File::GetFileContents(path_core_xml_);
  }
}

void meta::LoadAppXml(const std::string& path) {
  app_xml_ = helper::File::GetFileContents(path);
}

void meta::LoadCoreXml(const std::string& path) {
  core_xml_ = helper::File::GetFileContents(path);
}

bool meta::SaveAppXml() {
  return helper::File::WriteToNewFile(path_app_xml_, app_xml_)
         ? true
         : docxbox::AppLog::Error("Failed saving app.xml: " + path_app_xml_);
}

bool meta::SaveXml() {
  if (!path_app_xml_.empty() && !app_xml_.empty()) return SaveAppXml();

  if (!path_core_xml_.empty() && !core_xml_.empty()) return SaveCoreXml();

  return false;
}

bool meta::SaveCoreXml() {
  return helper::File::WriteToNewFile(path_core_xml_, core_xml_)
    ? true
    : docxbox::AppLog::Error("Failed saving core.xml: " + path_core_xml_);
}

void meta::CollectFromAppXml(std::string path_app_xml_current,
                             std::string app_xml) {
  // Attempt output after collecting meta data from app.xml and core.xml,
  // but when parsing the 2nd app.xml w/o having parsed a rel. core.xml:
  // output prematurely
  if (has_collected_from_app_xml_) Output();

  path_app_xml_ = std::move(path_app_xml_current);

  app_xml_ = app_xml;

  FetchAttributeFromAppXml(kTagNameApplication);
  FetchAttributeFromAppXml(kTagNameAppVersion);
  FetchAttributeFromAppXml(kTagNameCompany);

  attributes_.emplace_back("xmlSchema", ExtractXmlSchemaFromAppXml(app_xml));

  app_xml_ = app_xml;

  FetchAttributeFromAppXml(kTagNameTemplate);

  has_collected_from_app_xml_ = true;

  if (has_collected_from_app_xml_ && has_collected_from_core_xml_) Output();
}

std::string meta::ExtractXmlSchemaFromAppXml(std::string &app_xml) const {
  u_int32_t offset = 0;

  std::string xml_schema = helper::String::GetSubStrBetween(
      app_xml,
      kWordMlXmlSchemeLhs,
      kWordMlXmlSchemeRhs,
      offset);

  // Remove last segment from schema,
  // leaving:  http://schemas.openxmlformats.org/officeDocument/<year>
  std::vector<std::string> segments = helper::String::Explode(xml_schema, '/');

  helper::String::Replace(
      xml_schema,
      (std::string("/") + segments[ segments.size() - 1]).c_str(),
      "");

  return xml_schema;
}

void meta::CollectFromCoreXml(std::string path_core_xml_current) {
  // Attempt output after collecting meta data from app.xml and core.xml,
  // but when parsing the 2nd core.xml w/o having parsed a rel. app.xml:
  // output prematurely
  if (has_collected_from_core_xml_) Output();

  path_core_xml_ = std::move(path_core_xml_current);

  FetchAttributeFromCoreXml(kTagNameDcTermsCreated, kAttrCoreCreated);
  FetchAttributeFromCoreXml(kTagNameDcCreator, kAttrCoreCreator);
  FetchAttributeFromCoreXml(kTagNameDcDescription, kAttrCoreDescription);
  FetchAttributeFromCoreXml(kTagNameDcKeywords, kAttrCoreKeywords);
  FetchAttributeFromCoreXml(kTagNameDcLanguage, kAttrCoreLanguage);
  
  FetchAttributeFromCoreXml(
      kTagNameCpLastModifiedBy, kAttrCoreLastModifiedBy);
  
  FetchAttributeFromCoreXml(kTagNameCpLastPrinted, kAttrCoreLastPrinted);
  FetchAttributeFromCoreXml(kTagNameDcTermsModified, kAttrCoreModified);
  FetchAttributeFromCoreXml(kTagNameCpRevision, kAttrCoreRevision);
  FetchAttributeFromCoreXml(kTagNameDcSubject, kAttrCoreSubject);
  FetchAttributeFromCoreXml(kTagNameDcTitle, kAttrCoreTitle);

  has_collected_from_core_xml_ = true;

  if (has_collected_from_app_xml_ && has_collected_from_core_xml_) Output();
}

void meta::Output() {
  if (has_collected_from_app_xml_ || has_collected_from_core_xml_) {
    if (output_as_json_)
      OutputJson();
    else
      OutputPlain();
  }

  Clear();
}

void meta::OutputPlain() {
  for (std::tuple<std::string, std::string> attribute : attributes_) {
    std::cout << std::get<0>(attribute) << ": "
              << std::get<1>(attribute) << "\n";
  }
}

void meta::OutputJson() {
  std::cout << "{";

  int i = 0;

  for (std::tuple<std::string, std::string> attribute : attributes_) {
    if (i > 0) std::cout << ",";

    std::cout << "\"" << std::get<0>(attribute) << "\": "
              << "\"" << std::get<1>(attribute) << "\"";
    i++;
  }

  std::cout << "}";
}

void meta::Clear() {
  has_collected_from_app_xml_ = false;
  has_collected_from_core_xml_ = false;

  path_app_xml_ = "";
  path_core_xml_ = "";

  attributes_.clear();
}
