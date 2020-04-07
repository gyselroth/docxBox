
#ifndef DOCXBOX_CLASS_DOCX_META_H
#define DOCXBOX_CLASS_DOCX_META_H

#include <string>
#include <utility>

#include "../../helper/helper_string.h"

class docx_meta {

 public:
  // Known (supported for modification) attributes
  enum Attribute {
    Attribute_Title,
    Attribute_Language,
    Attribute_Revision,
    Attribute_Creator,
    Attribute_LastModifiedBy,
    Attribute_Created,
    Attribute_Modified,
    Attribute_LastPrinted,
    Attribute_Unknown
  };

  docx_meta(int argc, char **argv);

  void SetOutputAsJson(bool output_as_json);

  bool AreModificationArgumentsValid();

  void CollectFromAppXml(std::string path_app_xml, std::string app_xml);
  void CollectFromCoreXml(std::string path_core_xml_current, std::string core_xml);

  void Output();

 private:
  int argc;
  char **argv;
  bool outputAsJson = false;

  bool has_collected_from_app_xml = false;
  bool has_collected_from_core_xml = false;

  std::string path_app_xml;
  std::string path_core_xml;

  std::string creator;
  std::string date_creation;
  std::string date_modification;
  std::string language;
  std::string last_modified_by;
  std::string last_printed;
  std::string revision;
  std::string title;
  std::string xml_schema;

  static docx_meta::Attribute ResolveAttributeByName(const std::string &attribute);
  static bool IsSupportedAttribute(const std::string& attribute);

  void OutputPlain();
  void OutputJson();
  void Clear();
};

#endif //DOCXBOX_CLASS_DOCX_META_H
