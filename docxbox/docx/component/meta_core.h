// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_DOCX_COMPONENT_META_CORE_H_
#define DOCXBOX_DOCX_COMPONENT_META_CORE_H_

// Tag names possible in docProps/core.xml
static const char *const kTagNameCpLastModifiedBy = "cp:lastModifiedBy";
static const char *const kTagNameCpLastPrinted = "cp:lastPrinted";
static const char *const kTagNameCpRevision = "cp:revision";

static const char *const kTagNameDcCreator = "dc:creator";
static const char *const kTagNameDcDescription = "dc:description";
static const char *const kTagNameDcKeywords = "dc:keywords";
static const char *const kTagNameDcLanguage = "dc:language";
static const char *const kTagNameDcSubject = "dc:subject";
static const char *const kTagNameDcTitle = "dc:title";

static const char *const kTagNameDcTermsCreated = "dcterms:created";
static const char *const kTagNameDcTermsModified = "dcterms:modified";

static const char *const kWordMlCorePropertiesRhs = "</cp:coreProperties>";

static const char *const kAttrCoreCreated = "created";
static const char *const kAttrCoreCreator = "creator";
static const char *const kAttrCoreDescription = "description";
static const char *const kAttrCoreKeywords = "keywords";
static const char *const kAttrCoreLanguage = "language";
static const char *const kAttrCoreLastModifiedBy = "lastModifiedBy";
static const char *const kAttrCoreLastPrinted = "lastPrinted";
static const char *const kAttrCoreModified = "modified";
static const char *const kAttrCoreRevision = "revision";
static const char *const kAttrCoreSubject = "subject";
static const char *const kAttrCoreTitle = "title";

#endif  // DOCXBOX_DOCX_COMPONENT_META_CORE_H_
