// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/docx_meta.h>

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

std::string docx_meta::FetchAttributeFromAppXml(
    const char* lhs_of_value,
    const char* rhs_of_value,
    const std::string &label) {
  if (!helper::String::Contains(app_xml_, lhs_of_value)) return "";

  std::string value =
      helper::String::GetSubStrBetween(app_xml_, lhs_of_value, rhs_of_value);

  if (!label.empty()) attributes_.emplace_back(label, value);

  return value;
}

std::string docx_meta::FetchAttributeFromCoreXml(
    const char* lhs_of_value,
    const char* rhs_of_value,
    const std::string &label) {
  if (!helper::String::Contains(core_xml_, lhs_of_value)) return "";

  std::string value =
      helper::String::GetSubStrBetween(core_xml_, lhs_of_value, rhs_of_value);

  if (!label.empty()) attributes_.emplace_back(label, value);

  return value;
}

docx_meta::Attribute docx_meta::ResolveAttributeByName(
    const std::string &attribute) {
  if (attribute == "created") return Attribute_Created;
  if (attribute == "creator") return Attribute_Creator;
  if (attribute == "description") return Attribute_Description;
  if (attribute == "keywords") return Attribute_Keywords;
  if (attribute == "language") return Attribute_Language;
  if (attribute == "lastModifiedBy") return Attribute_LastModifiedBy;
  if (attribute == "lastPrinted") return Attribute_LastPrinted;
  if (attribute == "modified") return Attribute_Modified;
  if (attribute == "revision") return Attribute_Revision;
  if (attribute == "subject") return Attribute_Subject;
  if (attribute == "title") return Attribute_Title;

  return Attribute_Unknown;
}

bool docx_meta::InitModificationArguments() {
  if (!docxbox::AppArguments::IsArgumentGiven(argc_, 2, "DOCX filename")
      || !docxbox::AppArguments::IsArgumentGiven(
          argc_,
          3,
          "Meta attribute to be set")) return false;

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
    ? UpdateCoreAttribute(attribute_, value_)
    : InsertCoreAttribute(attribute_, value_);
}

bool docx_meta::UpdateCoreAttribute(
    Attribute attribute,
    const std::string& value) {
  EnsureIsLoadedCoreXml();

  const char *lhs_of_value;
  const char *rhs_of_value;

  switch (attribute) {
    case Attribute_Title:lhs_of_value = kWordMlTitleLhs;
      rhs_of_value = kWordMlTitleRhs;
      break;
    case Attribute_Language:lhs_of_value = kWordMlLanguageLhs;
      rhs_of_value = kWordMlLanguageRhs;
      break;
    case Attribute_Revision:lhs_of_value = kWordMlRevisionLhs;
      rhs_of_value = kWordMlRevisionRhs;
      break;
    case Attribute_Creator:lhs_of_value = kWordMlCreatorLhs;
      rhs_of_value = kWordMlCreatorRhs;
      break;
    case Attribute_LastModifiedBy:lhs_of_value = kWordMlLastModifiedByLhs;
      rhs_of_value = kWordMlLastModifiedByRhs;
      break;
    case Attribute_Created:lhs_of_value = kWordMlCreatedLhs;
      rhs_of_value = kWordMlCreatedRhs;
      break;
    case Attribute_Modified:lhs_of_value = kWordMlModifiedLhs;
      rhs_of_value = kWordMlModifiedRhs;
      break;
    case Attribute_LastPrinted:lhs_of_value = kWordMlLastPrintedLhs;
      rhs_of_value = kWordMlLastPrintedRhs;
      break;
    default:return false;
  }

  core_xml_ = helper::String::Replace(
      core_xml_,
      (std::string(lhs_of_value)
          + FetchAttributeFromCoreXml(lhs_of_value, rhs_of_value, "")).c_str(),
      std::string(lhs_of_value).append(value).c_str());

  return true;
}

bool docx_meta::InsertCoreAttribute(
    Attribute attribute,
    const std::string& value) {
  EnsureIsLoadedCoreXml();

  const char *lhs_of_value;
  const char *rhs_of_value;

  switch (attribute) {
    case Attribute_Title:
      lhs_of_value = kWordMlTitleLhs;
      rhs_of_value = kWordMlTitleRhs;
      break;
    case Attribute_Language:
      lhs_of_value = kWordMlLanguageLhs;
      rhs_of_value = kWordMlLanguageRhs;
      break;
    case Attribute_Revision:
      lhs_of_value = kWordMlRevisionLhs;
      rhs_of_value = kWordMlRevisionRhs;
      break;
    case Attribute_Creator:
      lhs_of_value = kWordMlCreatorLhs;
      rhs_of_value = kWordMlCreatorRhs;
      break;
    case Attribute_LastModifiedBy:
      lhs_of_value = kWordMlLastModifiedByLhs;
      rhs_of_value = kWordMlLastModifiedByRhs;
      break;
    case Attribute_Created:
      lhs_of_value = kWordMlCreatedLhs;
      rhs_of_value = kWordMlCreatedRhs;
      break;
    case Attribute_Modified:
      lhs_of_value = kWordMlModifiedLhs;
      rhs_of_value = kWordMlModifiedRhs;
      break;
    case Attribute_LastPrinted:
      lhs_of_value = kWordMlLastPrintedLhs;
      rhs_of_value = kWordMlLastPrintedRhs;
      break;
    default:return false;
  }

  core_xml_ = helper::String::Replace(
      core_xml_,
      kWordMlCorePropertiesRhs,
      (lhs_of_value + value + rhs_of_value + kWordMlCorePropertiesRhs).c_str());
}

// Check whether core.xml of current DOCX contains given attribute
bool docx_meta::AttributeExistsInCoreXml(Attribute attribute) {
  EnsureIsLoadedCoreXml();

  switch (attribute) {
    case Attribute_Created:
      return helper::String::Contains(core_xml_, kWordMlCreatedLhs);
    case Attribute_Creator:
      return helper::String::Contains(core_xml_, kWordMlCreatorLhs);
    case Attribute_Description:
      return helper::String::Contains(core_xml_, kWordMlDescriptionLhs);
    case Attribute_Language:
      return helper::String::Contains(core_xml_, kWordMlLanguageLhs);
    case Attribute_Keywords:
      return helper::String::Contains(core_xml_, kWordMlKeywordsLhs);
    case Attribute_LastModifiedBy:
      return helper::String::Contains(core_xml_, kWordMlLastModifiedByLhs);
    case Attribute_LastPrinted:
      return helper::String::Contains(core_xml_, kWordMlLastPrintedLhs);
    case Attribute_Modified:
      return helper::String::Contains(core_xml_, kWordMlModifiedLhs);
    case Attribute_Revision:
      return helper::String::Contains(core_xml_, kWordMlRevisionLhs);
    case Attribute_Title:
      return helper::String::Contains(core_xml_, kWordMlTitleLhs);
    case Attribute_Subject:
      return helper::String::Contains(core_xml_, kWordMlSubjectLhs);
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

  app_xml_ = app_xml;
  FetchAttributeFromAppXml(
      kWordMlApplicationLhs,
      kWordMlApplicationRhs,
      "Application");

  attributes_.emplace_back("xmlSchema", ExtractXmlSchemaFromAppXml(app_xml));

  app_xml_ = app_xml;
  FetchAttributeFromAppXml(
      kWordMlTemplateLhs,
      kWordMlTemplateRhs,
      "Template");

  has_collected_from_app_xml_ = true;

  if (has_collected_from_app_xml_ && has_collected_from_core_xml_) Output();
}

std::string docx_meta::ExtractXmlSchemaFromAppXml(std::string &app_xml) const {
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

void docx_meta::CollectFromCoreXml(std::string path_core_xml_current) {
  // Attempt output after collecting meta data from app.xml and core.xml,
  // but when parsing the 2nd core.xml w/o having parsed a rel. app.xml:
  // output prematurely
  if (has_collected_from_core_xml_) Output();

  path_core_xml_ = std::move(path_core_xml_current);

  FetchAttributeFromCoreXml(kWordMlCreatedLhs, kWordMlCreatedRhs, "created");
  FetchAttributeFromCoreXml(kWordMlCreatorLhs, kWordMlCreatorRhs, "creator");

  FetchAttributeFromCoreXml(
      kWordMlDescriptionLhs,
      kWordMlDescriptionRhs,
      "description");

  FetchAttributeFromCoreXml(kWordMlKeywordsLhs, kWordMlKeywordsRhs, "keywords");
  FetchAttributeFromCoreXml(kWordMlLanguageLhs, kWordMlLanguageRhs, "language");

  FetchAttributeFromCoreXml(
      kWordMlLastModifiedByLhs,
      kWordMlLastModifiedByRhs,
      "lastModifiedBy");

  FetchAttributeFromCoreXml(
      kWordMlLastPrintedLhs,
      kWordMlLastPrintedRhs,
      "lastPrinted");

  FetchAttributeFromCoreXml(kWordMlModifiedLhs, kWordMlModifiedRhs, "modified");
  FetchAttributeFromCoreXml(kWordMlRevisionLhs, kWordMlRevisionRhs, "revision");
  FetchAttributeFromCoreXml(kWordMlSubjectLhs, kWordMlSubjectRhs, "subject");
  FetchAttributeFromCoreXml(kWordMlTitleLhs, kWordMlTitleRhs, "title");

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
  for (std::tuple<std::string, std::string> attribute : attributes_) {
    std::cout
      << std::get<0>(attribute) << ": "
      << std::get<1>(attribute) << "\n";
  }
}

void docx_meta::OutputJson() {
  std::cout << "{";

  int i = 0;

  for (std::tuple<std::string, std::string> attribute : attributes_) {
    if (i > 0) std::cout << ",";

    std::cout
      << "\"" << std::get<0>(attribute) << "\": "
      << "\"" << std::get<1>(attribute) << "\"";

    i++;
  }

  std::cout << "}";
}

void docx_meta::Clear() {
  has_collected_from_app_xml_ = false;
  has_collected_from_core_xml_ = false;

  path_app_xml_ = "";
  path_core_xml_ = "";

  attributes_.clear();
}
