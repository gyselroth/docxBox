
#ifndef DOCXBOX_CLASS_DOCX_META_H
#define DOCXBOX_CLASS_DOCX_META_H

#include <string>
#include <utility>

#include "../../helper/helper_string.h"

class docx_meta {

 public:
  docx_meta(bool outputAsJson);

  void CollectFromAppXml(std::string path_app_xml, std::string app_xml);
  void CollectFromCoreXml(std::string path_core_xml_current, std::string core_xml);

  void Output();

 private:
  bool outputAsJson = false;

  bool hasCollectedFromAppXml = false;
  bool hasCollectedFromCoreXml = false;

  std::string path_app_xml;
  std::string path_core_xml;

  std::string creator;
  std::string date_creation;
  std::string date_modification;
  std::string language;
  std::string last_modified_by;
  std::string last_printed;
  std::string revision;
  std::string xml_schema;

  void OutputPlain();
  void OutputJson();
  void Clear();
};

#endif //DOCXBOX_CLASS_DOCX_META_H
