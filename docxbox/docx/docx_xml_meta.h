// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_DOCX_DOCX_META_H_
#define DOCXBOX_DOCX_DOCX_META_H_

#include <string>
#include <utility>
#include <vector>

#include "../app/app_argument.h"
#include "../helper/helper_string.h"
#include "docx_xml.h"

// wordMl meta-attribute tag-parts
// (left- and right-hand-side strings surrounding the value)
static const char *const kWordMlCorePropertiesRhs = "</cp:coreProperties>";

static const char
    *const kWordMlCreatedLhs = "<dcterms:created xsi:type=\"dcterms:W3CDTF\">";
static const char *const kWordMlCreatedRhs = "</dcterms:created>";

static const char *const kWordMlCreatorLhs = "<dc:creator>";
static const char *const kWordMlCreatorRhs = "</dc:creator>";

static const char *const kWordMlDescriptionLhs = "<dc:Description>";
static const char *const kWordMlDescriptionRhs = "</dc:Description>";

static const char *const kWordMlKeywordsLhs = "<dc:keywords>";
static const char *const kWordMlKeywordsRhs = "</dc:keywords>";

static const char *const kWordMlLastPrintedLhs = "<cp:lastPrinted>";
static const char *const kWordMlLastPrintedRhs = "</cp:lastPrinted>";

static const char *const kWordMlLanguageLhs = "<dc:language>";
static const char *const kWordMlLanguageRhs = "</dc:language>";

static const char *const kWordMlLastModifiedByLhs = "<cp:lastModifiedBy>";
static const char *const kWordMlLastModifiedByRhs = "</cp:lastModifiedBy>";

static const char *const kWordMlModifiedLhs =
    "<dcterms:modified xsi:type=\"dcterms:W3CDTF\">";
static const char *const kWordMlModifiedRhs = "</dcterms:modified>";

static const char *const kWordMlRevisionLhs = "<cp:revision>";
static const char *const kWordMlRevisionRhs = "</cp:revision>";

static const char *const kWordMlSubjectLhs = "<dc:subject>";
static const char *const kWordMlSubjectRhs = "</dc:subject>";

static const char *const kWordMlTitleLhs = "<dc:title>";
static const char *const kWordMlTitleRhs = "</dc:title>";

static const char *const kWordMlXmlSchemeLhs = "<Properties xmlns=\"";
static const char *const kWordMlXmlSchemeRhs = "\" ";

class docx_xml_meta:docx_xml {
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

  docx_xml_meta(int argc, char **argv);

  void SetPathExtract(const std::string &path_extract);
  void SetOutputAsJson(bool output_as_json);

  void LoadCoreXml(std::string path);
  bool SaveCoreXml();

  std::string SetCreatedFromCoreXml();
  std::string SetCreatorFromCoreXml();
  std::string SetDescriptionFromCoreXml();
  std::string SetKeywordsFromCoreXml();
  std::string SetLanguageFromCoreXml();
  std::string SetLastModifiedByFromCoreXml();
  std::string SetLastPrintedFromCoreXml();
  std::string SetModifiedFromCoreXml();
  std::string SetRevisionFromCoreXml();
  std::string SetSubjectFromCoreXml();
  std::string SetTitleFromCoreXml();

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

  std::string core_xml_;

  // Attribute + value for single modification
  Attribute attribute_ = Attribute::Attribute_Unknown;
  std::string value_;

  // Extracted meta values, string if contained / nullptr if not contained
  std::string creator_;
  std::string date_creation_;
  std::string date_modification_;
  std::string description_;
  std::string keywords_;
  std::string language_;
  std::string last_modified_by_;
  std::string last_printed_;
  std::string revision_;
  std::string subject_;
  std::string title_;
  std::string xml_schema_;

  void OutputPlain();
  void OutputJson();
  void Clear();

  static docx_xml_meta::Attribute ResolveAttributeByName(
      const std::string &attribute);

  bool AttributeExistsInCoreXml(Attribute attribute);
  bool UpdateAttribute(Attribute attribute, const std::string& value);
  bool InsertAttribute(Attribute attribute, const std::string& value);
  void EnsureIsLoadedCoreXml();
};

#endif  // DOCXBOX_DOCX_DOCX_META_H_
