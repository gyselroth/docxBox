// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_COMPONENT_PARTNAMES_H_
#define DOCXBOX_DOCX_COMPONENT_PARTNAMES_H_

class partNames {
 public:
  enum PartName {
    PartName_CoreXml,
    PartName_AppXml,
    PartName_WebSettingsXml,
    PartName_SettingsXml,
    PartName_FontTableXml,
    PartName_StylesXml,
    PartName_StylesWithEffectsXml,
    PartName_NumberingXml,
    PartName_DocumentXml
  };
};

#endif  // DOCXBOX_DOCX_COMPONENT_PARTNAMES_H_
