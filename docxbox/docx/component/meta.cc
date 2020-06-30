// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/component/meta.h>

// Meta attribute tag names possible in docProps/app.xml
const char meta::TAG_APP_APPLICATION[] = "Application";
const char meta::TAG_APP_APP_VERSION[] = "AppVersion";
const char meta::TAG_APP_COMPANY[] = "Company";
const char meta::TAG_APP_TEMPLATE[] = "Template";
const char meta::TAG_APP_XML_SCHEMA[] = "xmlSchema";

// Meta attribute tag names possible in docProps/core.xml
const char meta::TAG_CORE_CP_LAST_MODIFIED_BY[] = "cp:lastModifiedBy";
const char meta::TAG_CORE_CP_LAST_PRINTED[] = "cp:lastPrinted";
const char meta::TAG_CORE_CP_REVISION[] = "cp:revision";

const char meta::TAG_CORE_DC_CREATOR[] = "dc:creator";
const char meta::TAG_CORE_DC_DESCRIPTION[] = "dc:description";
const char meta::TAG_CORE_DC_KEYWORDS[] = "dc:keywords";
const char meta::TAG_CORE_DC_LANGUAGE[] = "dc:language";
const char meta::TAG_CORE_DC_SUBJECT[] = "dc:subject";
const char meta::TAG_CORE_DC_TITLE[] = "dc:title";
const char meta::TAG_CORE_DC_CREATED[] = "dcterms:created";
const char meta::TAG_CORE_DC_MODIFIED[] = "dcterms:modified";

const char meta::TAG_RHS_CORE_PROPERTIES[] = "</cp:coreProperties>";

const char meta::ATTR_CORE_CREATED[] = "created";
const char meta::ATTR_CORE_CREATOR[] = "creator";
const char meta::ATTR_CORE_DESCRIPTION[] = "description";
const char meta::ATTR_CORE_KEYWORDS[] = "keywords";
const char meta::ATTR_CORE_LANGUAGE[] = "language";
const char meta::ATTR_CORE_LAST_MODIFIED_BY[] = "lastModifiedBy";
const char meta::ATTR_CORE_LAST_PRINTED[] = "lastPrinted";
const char meta::ATTR_CORE_MODIFIED[] = "modified";
const char meta::ATTR_CORE_REVISION[] = "revision";
const char meta::ATTR_CORE_SUBJECT[] = "subject";
const char meta::ATTR_CORE_TITLE[] = "title";

const char meta::ATTR_DC_XSI[] = " xsi:type=\"dcterms:W3CDTF\"";

const char meta::TAG_LHS_XML_SCHEME[] = "<Properties xmlns=\"";
const char meta::TAG_RHS_XML_SCHEME[] = "\" ";

meta::meta(int argc, const std::vector<std::string>& argv) {
    argc_ = argc;
    argv_ = argv;
}

meta::AttributeType meta::GetAttribute() const {
  return attribute_type_;
}

const std::string &meta::GetValue() const {
  return attribute_value_;
}

void meta::SetAttribute(meta::AttributeType attribute) {
  attribute_type_ = attribute;
}

void meta::SetValue(const std::string &value) {
  attribute_value_ = value;
}

void meta::SetPathExtract(const std::string &path) {
  path_extract_ = path;
}

void meta::SetOutputAsJson(bool output_as_json) {
  output_as_json_ = output_as_json;
}

meta::AttributeType meta::ResolveAttribute(const std::string &attribute) {
  is_app_attribute_ = true;

  if (attribute == "Application") return Attr_App_Application;
  if (attribute == "AppVersion") return Attr_App_AppVersion;
  if (attribute == "Company") return Attr_App_Company;
  if (attribute == "Template") return Attr_App_Template;
  if (attribute == "xmlSchema") return Attr_App_XmlSchema;

  is_app_attribute_ = false;

  if (attribute == ATTR_CORE_CREATED) return Attr_Core_Created;
  if (attribute == ATTR_CORE_CREATOR) return Attr_Core_Creator;
  if (attribute == ATTR_CORE_DESCRIPTION) return Attr_Core_Description;
  if (attribute == ATTR_CORE_KEYWORDS) return Attr_Core_Keywords;
  if (attribute == ATTR_CORE_LANGUAGE) return Attr_Core_Language;
  if (attribute == ATTR_CORE_LAST_MODIFIED_BY) return Attr_Core_LastModifiedBy;
  if (attribute == ATTR_CORE_LAST_PRINTED) return Attr_Core_LastPrinted;
  if (attribute == ATTR_CORE_MODIFIED) return Attr_Core_Modified;
  if (attribute == ATTR_CORE_REVISION) return Attr_Core_Revision;
  if (attribute == ATTR_CORE_SUBJECT) return Attr_Core_Subject;
  if (attribute == ATTR_CORE_TITLE) return Attr_Core_Title;

  return Attr_Unknown;
}

bool meta::IsAttributeFromAppXml(const AttributeType &attribute) {
  return attribute == Attr_App_Application
      || attribute == Attr_App_AppVersion
      || attribute == Attr_App_Company
      || attribute == Attr_App_Template
      || attribute == Attr_App_XmlSchema;
}

// Known attribute is declared in app.xml? (or core.xml)
// Set within ResolveAttributeByName()
bool meta::IsAppAttribute() {
  return is_app_attribute_;
}

bool meta::IsDateAttribute(AttributeType attribute) {
  return
      attribute == Attr_Core_LastPrinted
          || attribute == Attr_Core_Created
          || attribute == Attr_Core_Modified;
}

bool meta::HasModifiedModificationDate() {
  return has_modified_modification_date_;
}

/**
 * @param tag_name
 * @param label     Optional, if empty: same as tag_name
 * @return Fetched value of given attribute
 */
std::string meta::FetchAttributeFromAppXml(const char tag_name[],
                                           const std::string &label) {
  return FetchAttributeFromAppXml(
      GetLhsTagByTagName(tag_name).c_str(),
      GetRhsTagByTagName(tag_name).c_str(),
      label.empty() ? tag_name : label);
}

std::string meta::GetRhsTagByTagName(const char tag_name[]) {
  return std::string("</") + tag_name + ">";
}

std::string meta::GetLhsTagByTagName(const char tag_name[]) {
  std::string attributes;

  if (helper::String::StartsWith(tag_name, "dcterms:"))
    attributes = ATTR_DC_XSI;

  return std::string("<") + tag_name + attributes + ">";
}

// TODO(kay): merge GetLhsTagByAttribute + GetRhsTagByAttribute methods
std::string meta::GetLhsTagByAttribute(const meta::AttributeType &attribute) {
  switch (attribute) {
    case Attr_App_Application:return GetLhsTagByTagName(TAG_APP_APPLICATION);
    case Attr_App_AppVersion:return GetLhsTagByTagName(TAG_APP_APP_VERSION);
    case Attr_Core_Created:return GetLhsTagByTagName(TAG_CORE_DC_CREATED);
    case Attr_Core_Creator:return GetLhsTagByTagName(TAG_CORE_DC_CREATOR);
    case Attr_App_Company:return GetLhsTagByTagName(TAG_APP_COMPANY);
    case Attr_Core_Description:
      return GetLhsTagByTagName(TAG_CORE_DC_DESCRIPTION);
    case Attr_Core_Title:return GetLhsTagByTagName(TAG_CORE_DC_TITLE);
    case Attr_Core_Language:return GetLhsTagByTagName(TAG_CORE_DC_LANGUAGE);
    case Attr_Core_Revision:return GetLhsTagByTagName(TAG_CORE_CP_REVISION);
    case Attr_Core_LastModifiedBy:
      return GetLhsTagByTagName(TAG_CORE_CP_LAST_MODIFIED_BY);
    case Attr_Core_Modified:return GetLhsTagByTagName(TAG_CORE_DC_MODIFIED);
    case Attr_Core_LastPrinted:
      return GetLhsTagByTagName(TAG_CORE_CP_LAST_PRINTED);
    case Attr_Core_Subject:return GetLhsTagByTagName(TAG_CORE_DC_SUBJECT);
    case Attr_App_Template:return GetLhsTagByTagName(TAG_APP_TEMPLATE);
    case Attr_App_XmlSchema:return GetLhsTagByTagName(TAG_APP_XML_SCHEMA);
    default:
      docxbox::AppLog::NotifyError(
          "Failed render opening tag. Unknown attribute.");

      return "";
  }
}

std::string meta::GetRhsTagByAttribute(const meta::AttributeType &attribute) {
  switch (attribute) {
    case Attr_App_Application:return GetRhsTagByTagName(TAG_APP_APPLICATION);
    case Attr_App_AppVersion:return GetRhsTagByTagName(TAG_APP_APP_VERSION);
    case Attr_Core_Created:return GetRhsTagByTagName(TAG_CORE_DC_CREATED);
    case Attr_Core_Creator:return GetRhsTagByTagName(TAG_CORE_DC_CREATOR);
    case Attr_App_Company:return GetRhsTagByTagName(TAG_APP_COMPANY);
    case Attr_Core_Description:
      return GetRhsTagByTagName(TAG_CORE_DC_DESCRIPTION);
    case Attr_Core_Title:return GetRhsTagByTagName(TAG_CORE_DC_TITLE);
    case Attr_Core_Language:return GetRhsTagByTagName(TAG_CORE_DC_LANGUAGE);
    case Attr_Core_Revision:return GetRhsTagByTagName(TAG_CORE_CP_REVISION);
    case Attr_Core_LastModifiedBy:
      return GetRhsTagByTagName(TAG_CORE_CP_LAST_MODIFIED_BY);
    case Attr_Core_Modified:return GetRhsTagByTagName(TAG_CORE_DC_MODIFIED);
    case Attr_Core_LastPrinted:
      return GetRhsTagByTagName(TAG_CORE_CP_LAST_PRINTED);
    case Attr_Core_Subject:return GetRhsTagByTagName(TAG_CORE_DC_LANGUAGE);
    case Attr_App_Template:return GetRhsTagByTagName(TAG_APP_TEMPLATE);
    case Attr_App_XmlSchema:return GetRhsTagByTagName(TAG_APP_XML_SCHEMA);
    default:
      docxbox::AppLog::NotifyError(
          "Failed render closing tag. Unknown attribute: " + attribute);

      return "";
  }
}

std::string meta::FetchAttributeFromAppXml(const char* lhs_of_value,
                                           const char *rhs_of_value,
                                           const std::string &label) {
  if (!helper::String::Contains(app_xml_, lhs_of_value)) return "";

  std::string value =
      helper::String::GetSubStrBetween(app_xml_, lhs_of_value, rhs_of_value);

  if (!label.empty()) attributes_.emplace_back(label, value);

  return value;
}

std::string meta::FetchAttributeFromCoreXml(const char tag_name[],
                                            const std::string &label) {
  return FetchAttributeFromCoreXml(
      GetLhsTagByTagName(tag_name).c_str(),
      GetRhsTagByTagName(tag_name).c_str(),
      label);
}

std::string meta::FetchAttributeFromCoreXml(const char lhs_of_value[],
                                            const char rhs_of_value[],
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

  if (helper::Json::IsJson(argv_[3])) {
    is_json_ = true;
    json_ = argv_[3];

    return InitFromJson();
  }

  attribute_type_ = ResolveAttribute(argv_[3]);

  if (attribute_type_ == AttributeType::Attr_Unknown)
    return docxbox::AppLog::NotifyError(
        std::string(
            "Invalid argument: Unknown or unsupported attribute: ") + argv_[3]);

  if (!docxbox::AppArgument::IsArgumentGiven(
      argc_, 4, "Value to set attribute to")) return false;

  attribute_value_ = argv_[4];

  return true;
}

bool meta::UpsertAttribute(bool saveXml) {
  if (is_json_) return UpsertAttributesFromJson();

    return IsAppAttribute()
      ? UpsertAttributeInAppXml(saveXml)
      : UpsertAttributeInCoreXml(saveXml);
}

bool meta::UpsertAttributesFromJson() {
  for (std::tuple<AttributeType, std::string>
        attribute_tuple : attributes_from_json_) {
    attribute_type_ = std::get<0>(attribute_tuple);
    attribute_value_ = std::get<1>(attribute_tuple);

    if (IsAttributeFromAppXml(attribute_type_))
      UpsertAttributeInAppXml();
    else if (UpsertAttributeInCoreXml()
        && attribute_type_ == Attr_Core_Modified)
        // When having explicitly updated the modification date,
        // Upon re-zipping the result, the mod-date is not updated again
        has_modified_modification_date_ = true;
  }

  return SaveXml();
}

bool meta::InitFromJson() {
  auto json_outer = nlohmann::json::parse(json_);

  for (auto &json_inner : json_outer) {
    for (nlohmann::json::iterator it = json_inner.begin();
         it != json_inner.end();
         ++it) {
      AttributeType attribute_type = ResolveAttribute(it.key());

      if (AttributeType::Attr_Unknown == attribute_type) {
        docxbox::AppLog::NotifyWarning(
            "Invalid argument - failed parse attributes from JSON");
        continue;
      }

      attributes_from_json_.emplace_back(
          std::make_tuple(attribute_type, it.value()));
    }
  }

  return attributes_from_json_.empty()
         ? docxbox::AppLog::NotifyError(
             "Invalid argument - failed parse attributes from JSON")
         : true;
}

bool meta::UpsertAttributeInCoreXml(bool saveXml) {
  EnsureIsLoadedCoreXml();

  bool result;

  try {
    if (IsDateAttribute(attribute_type_)
        && !helper::DateTime::IsIso8601Date(attribute_value_))
      return docxbox::AppLog::NotifyError(
          "Invalid date (must be given as ISO 8601): " + attribute_value_);

    bool attribute_exists = AttributeExistsInCoreXml(attribute_type_);

    result = attribute_exists
           ? UpdateCoreAttribute(attribute_type_, attribute_value_)
           : InsertCoreAttribute(attribute_type_, attribute_value_);
  } catch (std::string &message) {
    return docxbox::AppLog::NotifyError(message);
  }

  return result && saveXml
    ? SaveCoreXml()
    : result;
}

bool meta::UpsertAttributeInAppXml(bool saveXml) {
  EnsureIsLoadedAppXml();

  bool result;

  try {
    if (IsDateAttribute(attribute_type_)
        && !helper::DateTime::IsIso8601Date(attribute_value_))
      return docxbox::AppLog::NotifyError(
          "Invalid date (must be given as ISO 8601): " + attribute_value_);

    bool attribute_exists = AttributeExistsInAppXml(attribute_type_);

    result = attribute_exists
           ? UpdateAppAttribute(attribute_type_, attribute_value_)
           : InsertAppAttribute(attribute_type_, attribute_value_);
  } catch (std::string &message) {
    return docxbox::AppLog::NotifyError(message);
  }

  return result && saveXml
    ? SaveAppXml()
    : result;
}

bool meta::UpdateAppAttribute(AttributeType attribute,
                              const std::string& value) {
  EnsureIsLoadedAppXml();

  std::string lhs_of_value = GetLhsTagByAttribute(attribute);
  std::string rhs_of_value = GetRhsTagByAttribute(attribute);

  if (lhs_of_value.empty() || rhs_of_value.empty()) return false;

  helper::String::Replace(
      &app_xml_,
      (std::string(lhs_of_value)
          + FetchAttributeFromAppXml(
              lhs_of_value.c_str(), rhs_of_value.c_str(), "")).c_str(),
      std::string(lhs_of_value).append(value).c_str());

  return true;
}

bool meta::UpdateCoreAttribute(
    AttributeType attribute,
    const std::string& value) {
  EnsureIsLoadedCoreXml();

  std::string lhs_of_value = GetLhsTagByAttribute(attribute);
  std::string rhs_of_value = GetRhsTagByAttribute(attribute);

  if (lhs_of_value.empty() || rhs_of_value.empty()) return false;

  helper::String::Replace(
      &core_xml_,
      (std::string(lhs_of_value)
          + FetchAttributeFromCoreXml(
              lhs_of_value.c_str(),
              rhs_of_value.c_str(),
              "")).c_str(),
      std::string(lhs_of_value).append(value).c_str());

  return true;
}

bool meta::InsertAppAttribute(AttributeType attribute,
                              const std::string& value) {
  EnsureIsLoadedAppXml();

  const std::string &kLhsTag = GetLhsTagByAttribute(attribute);
  const std::string &kRhs = GetRhsTagByAttribute(attribute);

  if (kLhsTag.empty() || kRhs.empty()) return false;

  helper::String::Replace(
      &app_xml_,
      meta::TAG_RHS_CORE_PROPERTIES,
      (kLhsTag + value + kRhs + meta::TAG_RHS_CORE_PROPERTIES).c_str());

  return true;
}

bool meta::InsertCoreAttribute(AttributeType attribute,
                               const std::string& value) {
  EnsureIsLoadedCoreXml();

  const std::string &kLhsTag = GetLhsTagByAttribute(attribute);
  const std::string &kRhs = GetRhsTagByAttribute(attribute);

  if (kLhsTag.empty() || kRhs.empty()) return false;

  helper::String::Replace(
      &core_xml_,
      meta::TAG_RHS_CORE_PROPERTIES,
      (kLhsTag + value + kRhs + meta::TAG_RHS_CORE_PROPERTIES).c_str());

  return true;
}

// Check whether core.xml of current DOCX contains given attribute
bool meta::AttributeExistsInAppXml(AttributeType attribute) {
  EnsureIsLoadedAppXml();

  std::string lhs_of_tag = GetLhsTagByAttribute(attribute);

  return lhs_of_tag.empty()
    ? false
    : helper::String::Contains(app_xml_, lhs_of_tag);
}

// Check whether core.xml of current DOCX contains given attribute
bool meta::AttributeExistsInCoreXml(AttributeType attribute) {
  EnsureIsLoadedCoreXml();

  std::string lhs_of_tag = GetLhsTagByAttribute(attribute);

  return lhs_of_tag.empty()
    ? false
    : helper::String::Contains(core_xml_, lhs_of_tag);
}

void meta::EnsureIsLoadedAppXml() {
  if (app_xml_.empty()) {
    path_app_xml_ = path_extract_ + "/docProps/app.xml";

    helper::File::GetFileContents(path_app_xml_, &app_xml_);
  }
}

void meta::EnsureIsLoadedCoreXml() {
  if (core_xml_.empty()) {
    path_core_xml_ = path_extract_ + "/docProps/core.xml";

    helper::File::GetFileContents(path_core_xml_, &core_xml_);
  }
}

void meta::LoadAppXml(const std::string& path) {
  helper::File::GetFileContents(path, &app_xml_);
}

void meta::LoadCoreXml(const std::string& path) {
  helper::File::GetFileContents(path, &core_xml_);
}

bool meta::SaveAppXml() {
  return helper::File::WriteToNewFile(path_app_xml_, app_xml_)
         ? docxbox::AppLog::NotifyInfo("Saved modified docProps/app.xml", true)
         : docxbox::AppLog::NotifyError(
          "Failed saving app.xml: " + path_app_xml_);
}

bool meta::SaveXml() {
  if (!path_app_xml_.empty() && !app_xml_.empty()) return SaveAppXml();

  if (!path_core_xml_.empty() && !core_xml_.empty()) return SaveCoreXml();

  return false;
}

bool meta::SaveCoreXml() {
  return helper::File::WriteToNewFile(path_core_xml_, core_xml_)
    ? docxbox::AppLog::NotifyInfo("Saved modified docProps/core.xml", true)
    : docxbox::AppLog::NotifyError("Failed saving core.xml: " + path_core_xml_);
}

void meta::CollectFromAppXml(std::string path_app_xml_current,
                             const std::string& app_xml) {
  // Attempt output after collecting meta data from app.xml and core.xml,
  // but when parsing the 2nd app.xml w/o having parsed a rel. core.xml:
  // output prematurely
  if (has_collected_from_app_xml_) Output();

  path_app_xml_ = std::move(path_app_xml_current);

  app_xml_ = app_xml;

  FetchAttributeFromAppXml(TAG_APP_APPLICATION);
  FetchAttributeFromAppXml(TAG_APP_APP_VERSION);
  FetchAttributeFromAppXml(TAG_APP_COMPANY);

  attributes_.emplace_back("xmlSchema", ExtractXmlSchemaFromAppXml(app_xml));

  app_xml_ = app_xml;

  FetchAttributeFromAppXml(TAG_APP_TEMPLATE);

  has_collected_from_app_xml_ = true;

  if (has_collected_from_app_xml_ && has_collected_from_core_xml_) Output();
}

std::string meta::ExtractXmlSchemaFromAppXml(const std::string &app_xml) const {
  u_int32_t offset = 0;

  std::string xml_schema = helper::String::GetSubStrBetween(
      app_xml, TAG_LHS_XML_SCHEME, TAG_RHS_XML_SCHEME, &offset);

  // Remove last segment from schema,
  // leaving:  http://schemas.openxmlformats.org/officeDocument/<year>
  std::vector<std::string> segments = helper::String::Explode(xml_schema, '/');

  helper::String::Replace(
      &xml_schema,
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

  FetchAttributeFromCoreXml(TAG_CORE_DC_CREATED, ATTR_CORE_CREATED);
  FetchAttributeFromCoreXml(TAG_CORE_DC_CREATOR, ATTR_CORE_CREATOR);
  FetchAttributeFromCoreXml(TAG_CORE_DC_DESCRIPTION, ATTR_CORE_DESCRIPTION);
  FetchAttributeFromCoreXml(TAG_CORE_DC_KEYWORDS, ATTR_CORE_KEYWORDS);
  FetchAttributeFromCoreXml(TAG_CORE_DC_LANGUAGE, ATTR_CORE_LANGUAGE);

  FetchAttributeFromCoreXml(
      TAG_CORE_CP_LAST_MODIFIED_BY, ATTR_CORE_LAST_MODIFIED_BY);

  FetchAttributeFromCoreXml(TAG_CORE_CP_LAST_PRINTED, ATTR_CORE_LAST_PRINTED);
  FetchAttributeFromCoreXml(TAG_CORE_DC_MODIFIED, ATTR_CORE_MODIFIED);
  FetchAttributeFromCoreXml(TAG_CORE_CP_REVISION, ATTR_CORE_REVISION);
  FetchAttributeFromCoreXml(TAG_CORE_DC_SUBJECT, ATTR_CORE_SUBJECT);
  FetchAttributeFromCoreXml(TAG_CORE_DC_TITLE, ATTR_CORE_TITLE);

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
