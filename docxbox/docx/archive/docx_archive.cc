// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/archive/docx_archive.h>

docx_archive::docx_archive(int argc, char **argv) {
  argc_ = argc;
  argv_ = argv;

  path_working_directory_ = getenv("PWD");
}

docx_archive::~docx_archive() {
  RemoveTemporaryFiles();
}

// Setup path to DOCX file,
// absolute or relative from execution path, from given argument
bool docx_archive::InitPathDocxByArgV(int index_path_argument) {
  path_docx_in_ = docxbox::AppArguments::ResolvePathFromArgument(
      path_working_directory_,
      argc_,
      argv_,
      index_path_argument);

  return !path_docx_in_.empty();
}

// Render path (string) where to extract given DOCX file
std::string docx_archive::InitExtractionPath(
    const std::string &path_docx,
    const std::string &path_extract_appendix,
    bool is_temporary,
    const std::string &path_extract_prefix) {
  std::string path = path_extract_prefix
                   + helper::File::GetLastPathSegment(path_docx)
                   + (path_extract_appendix.empty()
                     ? "-extracted"
                     : path_extract_appendix);

  if (is_temporary) RememberTemporaryExtractionPath(path);

  return path;
}

void docx_archive::RememberTemporaryExtractionPath(const std::string& path) {
  auto iterator_end = paths_temporary_.end();

  if (std::find(paths_temporary_.begin(), iterator_end, path) != iterator_end)
    return;

  paths_temporary_.push_back(path);
}

// TODO(kay): handle more variations of wildcards, not only file-ending
std::string docx_archive::ParseFileWildcard(int index_argument) const {
  return argc_ >= index_argument + 1
             && argv_[3][0] == '*'
             && argv_[3][1] == '.'
         ? std::string(argv_[3]).substr(2)
         : "";
}

void docx_archive::RemoveTemporaryFiles() {
  if (paths_temporary_.empty()) return;

  for (const auto& path : paths_temporary_) {
    if (helper::File::IsDirectory(path))
      helper::File::RemoveRecursive(path.c_str());
  }

  paths_temporary_.clear();
}

std::string docx_archive::UnzipDocx(
    const std::string &path_docx,
    const std::string &path_extract_appendix,
    const std::string &path_extract_prefix,
    bool is_temporary) {
  if (!IsZipArchive(path_docx)) return "";

  auto path_extract = InitExtractionPath(
      path_docx, path_extract_appendix, is_temporary, path_extract_prefix);

  miniz_cpp::zip_file docx_file(path_docx);

  auto file_list = docx_file.infolist();

  mkdir(path_extract.c_str(), 0777);
  miniz_cpp_ext::CreateSubDirectories(path_extract, file_list);

  std::string path_working_directory = getenv("PWD");
  docx_file.extractall(path_working_directory + "/" + path_extract);

  return path_extract;
}

// Unzip all files of DOCX file w/ file argument taken from argv_
bool docx_archive::UnzipDocxByArgv(bool is_temporary,
                                   const std::string &directory_appendix,
                                   bool ensure_is_docx,
                                   bool format_xml_files) {
  if (!docxbox::AppArguments::IsArgumentGiven(
      argc_, 2, "Filename of DOCX to be extracted")) return false;

  try {
    InitPathDocxByArgV(3);
  } catch (std::string &message) {
    return docxbox::AppStatus::Error(message);
  }

  if (!IsZipArchive(path_docx_in_)) return false;

  path_extract_ =
      InitExtractionPath(path_docx_in_, directory_appendix, is_temporary);

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  mkdir(path_extract_.c_str(), 0777);
  miniz_cpp_ext::CreateSubDirectories(path_extract_, file_list);

  docx_file.extractall(path_working_directory_ + "/" + path_extract_);

  if (ensure_is_docx && IsUnzippedDocx())
    // TODO(kay): check above condition, seems inverted
    return docxbox::AppStatus::Error(path_docx_in_ + " is not a DOCX document");

  return format_xml_files
         ? miniz_cpp_ext::IndentXmlFiles(path_extract_, file_list)
         : true;
}

// Ensure given file is a ZIP archive (must begin w/ "PK^C^D^T")
bool docx_archive::IsZipArchive(const std::string& path_file) {
  if (!helper::File::FileExists(path_file)) return false;

  return helper::String::StartsWith(
      helper::File::GetFileContents(path_file).c_str(),
      "PK\003\004\024")
         ? true
         : docxbox::AppStatus::Error("File is no ZIP archive: " + path_file);
}

// Check formal structure of DOCX archive - mandatory files given?
bool docx_archive::IsUnzippedDocx() {
  return
      helper::File::IsDirectory(path_extract_ + "/rels")
          && helper::File::IsDirectory(path_extract_ + "/docProps")
          && helper::File::IsDirectory(path_extract_ + "/word")
          && helper::File::FileExists(path_extract_ + "/[Content_Types].xml")
          && helper::File::FileExists(path_extract_ + "/_rels/.rels")
          && helper::File::FileExists(path_extract_ + "/docProps/app.xml")
          && helper::File::FileExists(path_extract_ + "/docProps/core.xml")
          && helper::File::FileExists(path_extract_ + "/word/document.xml");
}

// Unzip all (than remove everything but) media files from DOCX file
bool docx_archive::UnzipMedia() {
  if (!UnzipDocxByArgv(false, "-media-extracted")) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  miniz_cpp_ext::ReduceExtractToImages(path_extract_, file_list);

  return true;
}

bool docx_archive::CreateDocxFromExtract(
    const std::string &path_docx_out, bool overwrite_source_docx) {
  if (!Zip(false, path_extract_, path_docx_out + "tmp"))
    return docxbox::AppStatus::Error("DOCX creation failed.");

  if (overwrite_source_docx) helper::File::Remove(path_docx_in_.c_str());

  return 0 == rename(
      std::string(path_docx_out).append("tmp").c_str(),
      path_docx_out.c_str());
}

// Zip files into given path into DOCX of given filename
// Optionally update "creation" and "modified" meta attributes (core.xml)
// to current date-time value
bool docx_archive::Zip(
    bool compress_xml,
    std::string path_directory,
    std::string path_docx_result,
    bool set_date_modified_to_now,
    bool set_date_created_to_now,
    const std::string& date_created,
    const std::string& date_modified) {
  if (path_directory.empty()) {
    if (!docxbox::AppArguments::AreArgumentsGiven(
        argc_,
        2, "Path of directory to be zipped",
        3, "Filename of docx to be created")) return false;

    path_directory =
        helper::File::ResolvePath(path_working_directory_, argv_[2]);

    path_docx_result =
        helper::File::ResolvePath(path_working_directory_, argv_[3], false);
  } else {
    if (!helper::File::IsDirectory(path_directory))
      return docxbox::AppStatus::Error("Not a directory: " + path_directory);
  }

  if (path_docx_result.empty() && !path_docx_in_.empty())
    // Overwrite source DOCX
    path_docx_result = path_docx_in_;

  if (!date_created.empty()) {
    if (!UpdateCoreXmlDate(
        meta::Attribute::Attribute_Created, date_created)) return false;

  } else {
    if (set_date_created_to_now)
      if (!UpdateCoreXmlDate(
          meta::Attribute::Attribute_Created)) return false;
  }

  if (!date_modified.empty()) {
    if (!UpdateCoreXmlDate(
        meta::Attribute::Attribute_Modified, date_modified)) return false;
  } else {
    if (set_date_modified_to_now)
      if (!UpdateCoreXmlDate(
          meta::Attribute::Attribute_Modified)) return false;
  }

// ZipUsingMinizCpp(compress_xml, path_directory, path_docx_result);

  ZipUsingCLi(path_directory, path_docx_result, compress_xml);

  return helper::File::FileExists(path_docx_result);
}

bool docx_archive::GetText(bool newline_at_segments) {
  if (!UnzipDocxByArgv(true, "-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto parser = new docx_xml_to_plaintext(argc_, argv_);

  for (const auto &file_in_zip : file_list) {
    // TODO(kay): fetch from all textual XML files, instead only document.xml

    if (!helper::String::EndsWith(file_in_zip.filename, "word/document.xml"))
      continue;

    parser->GetTextFromXmlFile(
        path_extract_ + "/" + file_in_zip.filename,
        newline_at_segments);
  }

  parser->Output();

  delete parser;

  return true;
}

bool docx_archive::ExecuteUserCommand(std::string command) {
  if (command.empty()) {
      if (!docxbox::AppArguments::AreArgumentsGiven(
        argc_,
        2, "DOCX filename",
        3, "Command")) return false;

    command = argv_[3];
  }

  if (!UnzipDocxByArgv(true, "", true, true)) return false;

  helper::String::ReplaceAll(command, "*DOCX*", path_extract_);

  helper::Cli::Execute(command.c_str());

// TODO(kay): TBD - implement optional wait for keypress
//  std::cout << "\nHit [Enter] when done.";
//  getchar();

  Zip(true, path_extract_, "", true, true);

  std::cout << "\n";

  return true;
}

bool docx_archive::Batch() {
  if (!docxbox::AppArguments::AreArgumentsGiven(
      argc_,
      2, "DOCX file",
      3, "Batch commands JSON")) return false;

  if (!UnzipDocxByArgv(true, "", true, true)) return false;

  auto batch = new docx_batch(path_extract_, std::string(argv_[3]));

  if (!batch->ProcessBatch()) {
    return false;
  }

  std::string path_docx_out = argc_ >= 4
    // Result filename is given as argument
    ? helper::File::ResolvePath(path_working_directory_, argv_[4])
    // Overwrite original DOCX
    : path_docx_in_;

  if (!Zip(false,
           path_extract_,
           path_docx_out + "tmp",
           true,
           false)) {
    delete batch;

    return docxbox::AppStatus::Error("DOCX creation failed.");
  }

  delete batch;

  if (argc_ < 4) helper::File::Remove(path_docx_in_.c_str());

  return 0 == std::rename(
      std::string(path_docx_out).append("tmp").c_str(),
      path_docx_out.c_str());
}

bool docx_archive::CatFile() {
  if (!docxbox::AppArguments::AreArgumentsGiven(
      argc_,
      2, "DOCX filename",
      3, "File to be displayed")) return false;

  if (!UnzipDocxByArgv(true, "", true, true)) return false;

  std::string path_file_relative = argv_[3];

  if (path_file_relative.empty())
    return docxbox::AppStatus::Error(
        "Invalid file path: " + path_file_relative);

  if (path_file_relative[0] == '/')
    path_file_relative = path_file_relative.substr(1);

  std::string path_file = path_extract_ + "/" + path_file_relative;

  if (!helper::File::FileExists(path_file))
    return
        docxbox::AppStatus::Error(std::string("File not found: ") + argv_[3]);

  std::cout << helper::File::GetFileContents(path_file) << "\n";

  return true;
}

bool docx_archive::ViewFilesDiff() {
  if (!docxbox::AppArguments::AreArgumentsGiven(
      argc_,
      3, "DOCX file to compare with",
      4, "File within DOCX archives to be compared")) return false;

  if (!UnzipDocxByArgv(true, "", true, true)) return false;

  std::string path_in_left = path_docx_in_;
  std::string path_extract_left = path_extract_;

  argv_[2] = argv_[3];

  if (!UnzipDocxByArgv(true, "", true, true)) return false;

  std::string path_in_right = path_docx_in_;
  std::string path_extract_right = path_extract_;

  std::string file = argv_[4];

  if (!helper::File::FileExists(path_extract_left + "/" + file)
      || !helper::File::FileExists(path_extract_right + "/" + file))
    return docxbox::AppStatus::Error(
        "File not given in both DOCX archives: " + file);

  if (argc_ > 5
      && (0 == strcmp(argv_[5], "-u") || 0 == strcmp(argv_[5], "--unified")))
    docx_diff::OutputUnified(path_extract_left, path_extract_right, file);
  else
    docx_diff::OutputSideBySide(path_extract_left, path_extract_right, file);

// TODO(kay): TBD - implement optional wait for keypress
//  std::cout << "\nHit [Enter] when done.";
//  getchar();

  std::cout << "\n";

  return true;
}

bool docx_archive::ModifyMeta() {
  auto *meta_component = new meta(argc_, argv_);

  if (!meta_component->InitModificationArguments()
      || !UnzipDocxByArgv(true, "-" + helper::File::GetTmpName())) {
    delete meta_component;

    return docxbox::AppStatus::Error(
        "Initialization for meta modification failed.");
  }

  miniz_cpp::zip_file docx_file(path_docx_in_);
  auto file_list = docx_file.infolist();

  meta_component->SetPathExtract(path_extract_);

  if (!meta_component->UpsertAttribute()) {
    delete meta_component;

    return docxbox::AppStatus::Error("Update/Insert meta attribute failed.");
  }

  // Modifiable meta attributes are in docProps/core.xml
  try {
    meta_component->SaveCoreXml();
  } catch (std::string &message) {
    std::cerr << message;
  }

  std::string path_docx_out;

  if (argc_ >= 6) {
    // Result filename is given as argument
    path_docx_out =
        helper::File::ResolvePath(path_working_directory_, argv_[5]);
  } else {
    // Overwrite original DOCX
    path_docx_out = path_docx_in_;
  }

  auto attribute = meta_component->GetAttribute();

  /*if (attribute == docx_meta::Attribute_LastPrinted) {
    std::string date = meta->GetValue();

    if (!Zip(false, path_extract_, path_docx_out + "tmp",
             false,
             false,
             date,
             date)) {
      std::cerr << "DOCX creation failed.\n";

      delete meta;

      return false;
    }
  } else {*/
    if (!Zip(false,
             path_extract_,
             path_docx_out + "tmp",
             attribute != meta::Attribute_Modified,
             attribute != meta::Attribute_Created)) {
      delete meta_component;

      return docxbox::AppStatus::Error("DOCX creation failed.");
    }
  /*}*/

  delete meta_component;

  if (argc_ < 6) helper::File::Remove(path_docx_in_.c_str());

  return 0 == std::rename(
      std::string(path_docx_out).append("tmp").c_str(),
      path_docx_out.c_str());
}

// Update given meta date attribute and immediately save updated core.xml
// TODO(kay): add multi-attributes variation of method
//  to load/save only once than
bool docx_archive::UpdateCoreXmlDate(
    meta::Attribute attribute,
    const std::string& value) {
  auto meta_component = new meta(argc_, argv_);

  meta_component->SetPathExtract(path_extract_);
  meta_component->SetAttribute(attribute);

  if (value.empty()) {
    meta_component->SetValue(helper::DateTime::GetCurrentDateTimeInIso8601());
  } else {
    if (!helper::DateTime::IsIso8601Date(value))
      return docxbox::AppStatus::Error(
          "Invalid date (must be ISO 8601): " + value);

    meta_component->SetValue(value);
  }

  auto result = meta_component->UpsertAttribute(true);

  delete meta_component;

  return result;
}

void docx_archive::ZipUsingCLi(const std::string &path_directory,
                               const std::string &path_docx_result,
                               bool compress_xml) const {
  if (compress_xml) CompressXmlFiles(path_directory);

  std::string cmd =
    "cd " + path_directory + ";"
    "zip tmp.zip -rq *;";

  if (!path_docx_result.empty()) {
    if (helper::File::FileExists(path_docx_result))
      helper::File::Remove(path_docx_result.c_str());

    cmd += "mv tmp.zip " + path_docx_result;
  }

  helper::Cli::Execute(cmd.c_str());
}

void docx_archive::ZipUsingMinizCpp(bool compress_xml,
                                    const std::string &path_directory,
                                    const std::string &path_docx_result) const {
  // Get relative paths of all files to be zipped
  std::vector<std::string> files_in_zip;

  files_in_zip = helper::File::ScanDirRecursive(
      path_directory.c_str(),
      files_in_zip,
      path_directory + "/");

  miniz_cpp::zip_file file;

  for (const auto &file_in_zip : files_in_zip) {
    std::string path_file_absolute =
        std::string(path_directory + "/").append(file_in_zip);

    std::string xml = helper::File::GetFileContents(path_file_absolute);

    if (compress_xml) helper::Xml::CompressXml(xml);

    file.writestr(file_in_zip, xml);
  }

  file.save(path_docx_result);
}

void docx_archive::CompressXmlFiles(const std::string &path_directory) const {
  std::vector<std::string> files;

  files = helper::File::ScanDirRecursive(
      path_directory.c_str(),
      files,
      path_directory + "/");

  for (const auto &file_in_zip : files) {
    std::string path_file_absolute =
        std::string(path_directory + "/").append(file_in_zip);

    std::string xml = helper::File::GetFileContents(path_file_absolute);

    helper::Xml::CompressXml(xml);

    helper::File::WriteToNewFile(path_file_absolute, xml);
  }
}
