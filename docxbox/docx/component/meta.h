// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_COMPONENT_META_H_
#define DOCXBOX_DOCX_COMPONENT_META_H_

#include <docxbox/app/app_argument.h>
#include <docxbox/docx/component/meta_app.h>
#include <docxbox/docx/component/meta_core.h>
#include <docxbox/docx/xml/docx_xml.h>
#include <docxbox/helper/helper_dateTime.h>
#include <docxbox/helper/helper_string.h>

#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

static const char *const kWmlAttributeDcTermsXsi =
    " xsi:type=\"dcterms:W3CDTF\"";

static const char *const kWordMlXmlSchemeLhs = "<Properties xmlns=\"";
static const char *const kWordMlXmlSchemeRhs = "\" ";

class meta {
 public:
  // Known (supported for modification) attributes
  enum Attribute {
    Attribute_App_Application,
    Attribute_App_AppVersion,
    Attribute_Core_Created,
    Attribute_Core_Creator,
    Attribute_App_Company,
    Attribute_Core_Description,
    Attribute_Core_Keywords,
    Attribute_Core_Language,
    Attribute_Core_LastModifiedBy,
    Attribute_Core_LastPrinted,
    Attribute_Core_Modified,
    Attribute_Core_Revision,
    Attribute_Core_Subject,
    Attribute_App_Template,
    Attribute_Core_Title,
    Attribute_App_XmlSchema,
    Attribute_Unknown
  };

  meta(int argc, char **argv);

  Attribute GetAttribute() const;

  const std::string &GetValue() const;

  void SetAttribute(Attribute attribute);
  void SetValue(const std::string &value);

  void SetPathExtract(const std::string &path_extract);
  void SetOutputAsJson(bool output_as_json);

  void LoadAppXml(const std::string& path);
  void LoadCoreXml(const std::string& path);

  // Save modified XML (core.xml or app.xml)
  bool SaveXml();
  bool SaveAppXml();
  bool SaveCoreXml();

  // Known attribute is declared in app.xml? (or core.xml)
  // Set within ResolveAttributeByName()
  bool IsAppAttribute();

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
  bool UpsertAttributeInCoreXml(bool saveXml = false);
  bool UpsertAttributeInAppXml(bool saveXml = false);

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

  // For known attributes: Is declared in app.xml (otherwise: core.xml)
  bool is_app_attribute_ = false;

  std::string value_;

  // Extracted meta values, string if contained / nullptr if not contained
  std::vector<std::tuple<std::string, std::string>> attributes_;

  void OutputPlain();
  void OutputJson();

  void Clear();

  meta::Attribute ResolveAttribute(const std::string &attribute);

  bool AttributeExistsInAppXml(Attribute attribute);
  bool AttributeExistsInCoreXml(Attribute attribute);

  // TODO(kay): extract duplications from app.xml / core.xml - related methods
  bool UpdateAppAttribute(Attribute attribute, const std::string& value);
  bool UpdateCoreAttribute(Attribute attribute, const std::string& value);
  bool InsertAppAttribute(Attribute attribute, const std::string& value);
  bool InsertCoreAttribute(Attribute attribute, const std::string& value);

  void EnsureIsLoadedAppXml();
  void EnsureIsLoadedCoreXml();

  static std::string GetLhsTagByTagName(const char *tag_name);
  static std::string GetRhsTagByTagName(const char *tag_name);

  static std::string GetLhsTagByAttribute(const Attribute &attribute);
  static std::string GetRhsTagByAttribute(const Attribute &attribute);

  std::string ExtractXmlSchemaFromAppXml(std::string &app_xml) const;
};

#endif  // DOCXBOX_DOCX_COMPONENT_META_H_
