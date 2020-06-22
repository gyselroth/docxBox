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
    Attr_App_Application,
    Attr_App_AppVersion,
    Attr_App_Company,
    Attr_App_Template,
    Attr_App_XmlSchema,
    Attr_Core_Created,
    Attr_Core_Creator,
    Attr_Core_Description,
    Attr_Core_Keywords,
    Attr_Core_Language,
    Attr_Core_LastModifiedBy,
    Attr_Core_LastPrinted,
    Attr_Core_Modified,
    Attr_Core_Revision,
    Attr_Core_Subject,
    Attr_Core_Title,
    Attr_Unknown
  };

  // Tag names possible in docProps/core.xml
  /*static const char *const kTagNameCpLastModifiedBy = "cp:lastModifiedBy";
  static const char *const kTagNameCpLastPrinted = "cp:lastPrinted";
  static const char *const kTagNameCpRevision = "cp:revision";

  static const char *const kTagNameDcCreator = "dc:creator";
  static const char *const kTagNameDcDescription = "dc:description";
  static const char *const kTagNameDcKeywords = "dc:keywords";
  static const char *const kTagNameDcLanguage = "dc:language";
  static const char *const kTagNameDcSubject = "dc:subject";
  static const char *const kTagNameDcTitle = "dc:title";

  static const char *const kTagNameDcTermsCreated = "dcterms:created";
  static const char *const kTagNameDcTermsModified = "dcterms:modified";*/

  static const char TAG_RHS_CORE_PROPERTIES[];

  static const char ATTR_CORE_CREATED[];
  static const char ATTR_CORE_CREATOR[];
  static const char ATTR_CORE_DESCRIPTION[];
  static const char ATTR_CORE_KEYWORDS[];
  static const char ATTR_CORE_LANGUAGE[];
  static const char ATTR_CORE_LAST_MODIFIED_BY[];
  static const char ATTR_CORE_LAST_PRINTED[];
  static const char ATTR_CORE_MODIFIED[];
  static const char ATTR_CORE_REVISION[];
  static const char ATTR_CORE_SUBJECT[];
  static const char ATTR_CORE_TITLE[];

  meta(int argc, const std::vector<std::string>& argv);

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

  std::string FetchAttributeFromAppXml(const char* tag_name,
                                       const std::string &label = "");

  std::string FetchAttributeFromAppXml(const char* lhs_of_value,
                                       const char *rhs_of_value,
                                       const std::string &label);

  std::string FetchAttributeFromCoreXml(const char* tag_name,
                                        const std::string &label);

  std::string FetchAttributeFromCoreXml(const char* lhs_of_value,
                                        const char *rhs_of_value,
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
  std::vector<std::string> argv_;

  std::string path_extract_;

  bool output_as_json_ = false;

  bool has_collected_from_app_xml_ = false;
  bool has_collected_from_core_xml_ = false;

  std::string path_app_xml_;
  std::string path_core_xml_;

  std::string app_xml_;
  std::string core_xml_;

  // Attribute + value for single modification
  Attribute attribute_ = Attribute::Attr_Unknown;

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
