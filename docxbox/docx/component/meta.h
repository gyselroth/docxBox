// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_COMPONENT_META_H_
#define DOCXBOX_DOCX_COMPONENT_META_H_

#include <docxbox/app/app_argument.h>
#include <docxbox/helper/helper_dateTime.h>
#include <docxbox/helper/helper_string.h>
#include <docxbox/docx/xml/docx_xml.h>

#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// wordMl meta attribute tag names
static const char *const kWmlTagApplication = "Application";
static const char *const kWmlTagAppVersion = "AppVersion";
static const char *const kWmlTagCompany = "Company";
static const char *const kWmlTagTemplate = "Template";

static const char *const kWmlTagCpLastModifiedBy = "cp:lastModifiedBy";
static const char *const kWmlTagCpLastPrinted = "cp:lastPrinted";
static const char *const kWmlTagCpRevision = "cp:revision";

static const char *const kWmlTagDcCreator = "dc:creator";
static const char *const kWmlTagDcDescription = "dc:description";
static const char *const kWmlTagDcKeywords = "dc:keywords";
static const char *const kWmlTagDcLanguage = "dc:language";
static const char *const kWmlTagDcSubject = "dc:subject";
static const char *const kWmlTagDcTitle = "dc:title";

static const char *const kWmlAttributeDcTermsXsi =
    " xsi:type=\"dcterms:W3CDTF\"";

static const char *const kWmlTagDcTermsCreated = "dcterms:created";
static const char *const kWmlTagDcTermsModified = "dcterms:modified";

static const char *const kWordMlXmlSchemeLhs = "<Properties xmlns=\"";
static const char *const kWordMlXmlSchemeRhs = "\" ";

static const char *const kWordMlCorePropertiesRhs = "</cp:coreProperties>";

class meta {
 public:
  // Known (supported for modification) attributes
  enum Attribute {
    Attribute_Created,
    Attribute_Creator,
    Attribute_Description,
    Attribute_Keywords,
    Attribute_Language,
    Attribute_LastModifiedBy,
    Attribute_LastPrinted,
    Attribute_Modified,
    Attribute_Revision,
    Attribute_Subject,
    Attribute_Title,
    Attribute_Unknown
  };

  meta(int argc, char **argv);

  Attribute GetAttribute() const;

  const std::string &GetValue() const;

  void SetAttribute(Attribute attribute);
  void SetValue(const std::string &value);

  void SetPathExtract(const std::string &path_extract);
  void SetOutputAsJson(bool output_as_json);

  void LoadCoreXml(const std::string& path);
  bool SaveCoreXml();

  static bool IsDateAttribute(Attribute);

  std::string FetchAttributeFromAppXml(
      const char* tag_name,
      const std::string &label = "");

  std::string FetchAttributeFromAppXml(
      const char* lhs_of_value,
      const char* rhs_of_value,
      const std::string &label);

  std::string FetchAttributeFromCoreXml(
      const char* tag_name,
      const std::string &label);

  std::string FetchAttributeFromCoreXml(
      const char* lhs_of_value,
      const char* rhs_of_value,
      const std::string &label);

  // Explicit meta modification CLI call:
  // Validate CLI arguments and initialize rel. properties
  bool InitModificationArguments();

  bool UpsertAttribute(bool saveXml = false);

  void CollectFromAppXml(std::string path_app_xml, std::string app_xml);
  void CollectFromCoreXml(std::string path_core_xml_current);

  void Output();

 private:
  int argc_;
  char **argv_;

  std::string path_extract_;

  bool output_as_json_ = false;

  bool has_collected_from_app_xml_ = false;
  bool has_collected_from_core_xml_ = false;

  std::string path_app_xml_;
  std::string path_core_xml_;

  std::string app_xml_;
  std::string core_xml_;

  // Attribute + value for single modification
  Attribute attribute_ = Attribute::Attribute_Unknown;
  std::string value_;

  // Extracted meta values, string if contained / nullptr if not contained
  std::vector<std::tuple<std::string, std::string>> attributes_;

  void OutputPlain();
  void OutputJson();

  void Clear();

  static meta::Attribute ResolveAttributeByName(
      const std::string &attribute);

  bool AttributeExistsInCoreXml(Attribute attribute);

  bool UpdateCoreAttribute(Attribute attribute, const std::string& value);
  bool InsertCoreAttribute(Attribute attribute, const std::string& value);

  void EnsureIsLoadedCoreXml();

  static std::string GetLhsTagByTagName(const char *tag_name);
  static std::string GetRhsTagByTagName(const char *tag_name);

  static std::string GetLhsTagByAttribute(const Attribute &attribute);
  static std::string GetRhsTagByAttribute(const Attribute &attribute);

  std::string ExtractXmlSchemaFromAppXml(std::string &app_xml) const;
};

#endif  // DOCXBOX_DOCX_COMPONENT_META_H_
