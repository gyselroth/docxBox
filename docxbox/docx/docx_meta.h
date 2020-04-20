// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_META_H_
#define DOCXBOX_DOCX_DOCX_META_H_

#include <docxbox/app/app_argument.h>
#include <docxbox/helper/helper_string.h>
#include <docxbox/docx/docx_xml.h>

#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// wordMl meta attribute tag names
static const char *const kWmlTagApplication = "Application";
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

class docx_meta {
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

  docx_meta(int argc, char **argv);

  void SetPathExtract(const std::string &path_extract);
  void SetOutputAsJson(bool output_as_json);

  void LoadCoreXml(std::string path);
  bool SaveCoreXml();

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

  bool InitModificationArguments();
  bool UpsertAttribute();

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

  static docx_meta::Attribute ResolveAttributeByName(
      const std::string &attribute);

  bool AttributeExistsInCoreXml(Attribute attribute);

  bool UpdateCoreAttribute(Attribute attribute, const std::string& value);
  bool InsertCoreAttribute(Attribute attribute, const std::string& value);

  void EnsureIsLoadedCoreXml();

  static std::basic_string<char> GetLhsTagByTagName(const char *tag_name);
  static std::basic_string<char> GetRhsTagByTagName(const char *tag_name);

  static void GetLhsTagByAttribute(
      const Attribute &attribute,
      const char *&lhs_of_value);

  static void GetRhsTagByAttribute(
      const Attribute &attribute,
      const char *&rhs_of_value);

  std::string ExtractXmlSchemaFromAppXml(std::string &app_xml) const;
};

#endif  // DOCXBOX_DOCX_DOCX_META_H_
