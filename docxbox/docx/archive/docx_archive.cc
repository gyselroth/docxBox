// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/docx/archive/docx_archive.h>

docx_archive::docx_archive(int argc,
                           const std::vector<std::string>& argv,
                           bool is_batch_mode = false) {
  argc_ = argc;
  argv_ = argv;
  is_batch_mode_ = is_batch_mode;

  path_working_dir_ = std::getenv("PWD");
}

docx_archive::~docx_archive() {
  RemoveTemporaryFiles();
}

const std::vector<std::string> &docx_archive::GetArgv() const {
  return argv_;
}

std::string docx_archive::GetArgValueAt(int index) {
  return argv_[index];
}

std::string docx_archive::GetLastArgValue() {
  return argv_[argc_ - 1];
}

void docx_archive::SetPathDocxIn(const std::string &path) {
  path_docx_in_ = path;
}

void docx_archive::SetPathDocxOut(const std::string &path) {
  path_docx_out_ = path;
}

void docx_archive::SetPathExtract(const std::string &path) {
  path_extract_ = path;
}

void docx_archive::SetIsFinalBatchStep(bool is_final_batch_step) {
  is_final_batch_step_ = is_final_batch_step;
}

const std::string &docx_archive::GetPathDocxIn() const {
  return path_docx_in_;
}

const std::string &docx_archive::GetPathExtract() const {
  return path_extract_;
}

// Setup path to DOCX file,
// absolute or relative from execution path, from given argument
bool docx_archive::InitPathDocxByArgV(int index_path_argument) {
  path_docx_in_ = docxbox::AppArgument::ResolvePathFromArgument(
      path_working_dir_,
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

// Detect globbing wildcard and convert to regular expression if any
std::string docx_archive::ParseFileWildcard(int index_argument) const {
  if (argc_ < index_argument + 1) return "";

  std::string glob_pattern = argv_[index_argument];

  if (docxbox::AppArgument::IsKnownOption(glob_pattern)) return "";

  return helper::File::GlobPatternToRegEx(glob_pattern);
}

void docx_archive::RemoveTemporaryFiles() {
  if (paths_temporary_.empty()) return;

  for (const auto& path : paths_temporary_) {
    if (helper::File::IsDir(path))
      helper::File::RemoveRecursive(path.c_str());
  }

  paths_temporary_.clear();
}

std::string docx_archive::UnzipDocx(const std::string &path_docx,
                                    const std::string &path_extract_appendix,
                                    const std::string &path_extract_prefix,
                                    bool is_temporary) {
  if (!IsZipArchive(path_docx)) return "";

  auto path_extract = InitExtractionPath(
      path_docx, path_extract_appendix, is_temporary, path_extract_prefix);

  miniz_cpp::zip_file docx_file(path_docx);

  auto file_list = docx_file.infolist();

  mkdir(path_extract.c_str(), 0777);
  miniz_cpp_ext::CreateSubDirs(path_extract, file_list);

  std::string path_working_dir = getenv("PWD");
  docx_file.extractall(path_working_dir + "/" + path_extract);

  return path_extract;
}

// Unzip all files of DOCX file w/ file argument taken from argv_
bool docx_archive::UnzipDocxByArgv(bool is_temporary,
                                   const std::string &dir_appendix,
                                   bool ensure_is_docx,
                                   bool indent_xml_files) {
  if (!docxbox::AppArgument::IsArgumentGiven(
      argc_, 2, "Filename of DOCX to be extracted")) return false;

  try {
    InitPathDocxByArgV(3);
  } catch (std::string &message) {
    return docxbox::AppLog::NotifyError(message);
  }

  if (!IsZipArchive(path_docx_in_)) return false;

  path_extract_ =
      InitExtractionPath(path_docx_in_, dir_appendix, is_temporary);

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  mkdir(path_extract_.c_str(), 0777);
  miniz_cpp_ext::CreateSubDirs(path_extract_, file_list);

  docx_file.extractall(path_working_dir_ + "/" + path_extract_);

  if (ensure_is_docx && !IsUnzippedDocx())
    return docxbox::AppLog::NotifyError(
        path_docx_in_ + " is not a DOCX document");

  docxbox::AppLog::NotifyInfo(
      "Unzipped DOCX: " + path_docx_in_
          + " to: " + path_working_dir_ + "/" + path_extract_);

  return indent_xml_files
         ? miniz_cpp_ext::IndentXmlFiles(path_extract_, file_list)
         : true;
}

// Ensure given file is a ZIP archive (must begin w/ "PK^C^D^T")
bool docx_archive::IsZipArchive(const std::string& path_file) {
  std::string file_contents;

  if (!helper::File::GetFileContents(path_file, &file_contents))
    return false;

  return helper::String::StartsWith(&file_contents, "PK\003\004\024")
         ? true
         : docxbox::AppLog::NotifyError("File is no ZIP archive: " + path_file);
}

// Check formal structure of DOCX archive - mandatory files given?
bool docx_archive::IsUnzippedDocx() {
  return
      helper::File::IsDir(path_extract_ + "/docProps")
          && helper::File::IsDir(path_extract_ + "/word")
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

bool docx_archive::CreateDocxFromExtract(const std::string &path_docx_out,
                                         bool overwrite_source_docx) {
  if (!Zip(false, path_extract_, path_docx_out + "tmp"))
    return docxbox::AppLog::NotifyError("DOCX creation failed.");

  if (overwrite_source_docx) helper::File::Remove(path_docx_in_.c_str());

  return 0 == std::rename(
      std::string(path_docx_out).append("tmp").c_str(),
      path_docx_out.c_str())
         ? docxbox::AppLog::NotifyInfo("Saved DOCX: " + path_docx_out_)
         : docxbox::AppLog::NotifyError(
          "Failed rename temporary DOCX to: " + path_docx_out_);
}

// Zip files into given path into DOCX of given filename
// Optionally update "creation" and "modified" meta attributes (core.xml)
// to current date-time value
bool docx_archive::Zip(bool compress_xml,
                       std::string path_dir,
                       std::string path_docx_result,
                       bool set_date_modified_to_now) {
  if (path_dir.empty()) {
    if (!docxbox::AppArgument::AreArgumentsGiven(
        argc_,
        2, "Path of directory to be zipped",
        3, "Filename of docx to be created")) return false;

    path_dir = argv_[2];
    helper::File::ResolvePath(path_working_dir_, &path_dir);

    path_docx_result = argv_[3];
    helper::File::ResolvePath(path_working_dir_,
                              &path_docx_result,
                              false);
  } else {
    if (!helper::File::IsDir(path_dir))
      return docxbox::AppLog::NotifyError("Not a directory: " + path_dir);
  }

  if (path_docx_result.empty() && !path_docx_in_.empty())
    // Overwrite source DOCX
    path_docx_result = path_docx_in_;

  if (set_date_modified_to_now
      && !UpdateCoreXmlDate(meta::AttributeType::Attr_Core_Modified))
    return false;

// ZipUsingMinizCpp(compress_xml, path_dir, path_docx_result);

  ZipUsingCLi(path_dir, path_docx_result, compress_xml);

  return helper::File::FileExists(path_docx_result);
}

bool docx_archive::GetText(bool newline_at_segments) {
  if (!UnzipDocxByArgv(true, "-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto parser = new docx_xml_to_plaintext(argc_, argv_);

  for (const auto &file_in_zip : file_list) {
    if (!docx_xml::IsXmlFileContainingText(file_in_zip.filename)) continue;

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
      if (!docxbox::AppArgument::AreArgumentsGiven(
        argc_,
        2, "DOCX filename",
        3, "Command")) return false;

    command = argv_[3];
  }

  if (!UnzipDocxByArgv(true, "", true, true)) return false;

  helper::String::ReplaceAll(&command, "*DOCX*", path_extract_);

  helper::Cli::Execute(command.c_str());

  Zip(true, path_extract_, "");

  if (!docxbox::AppLog::IsSilent()) std::cout << "\n";

  return true;
}

bool docx_archive::Batch() {
  if (!docxbox::AppArgument::AreArgumentsGiven(
      argc_,
      2, "DOCX file",
      3, "Batch commands JSON")) return false;

  if (!UnzipDocxByArgv(true, "", true, true)) return false;

  auto batch = new docx_batch(this, std::string(argv_[3]));
  bool result = batch->ProcessSequence();

  if (!result) {
    delete batch;

    return false;
  }

  std::string path_docx_out;

  if (argc_ >= 4) {
    // Result filename is given as argument
    path_docx_out = argv_[4];
    helper::File::ResolvePath(path_working_dir_, &path_docx_out);
  } else {
    // Overwrite original DOCX
    path_docx_out = path_docx_in_;
  }

  if (!Zip(false, path_extract_, path_docx_out + "tmp")) {
    // TODO(kay): improve date modified handling during batch processing
    delete batch;

    return docxbox::AppLog::NotifyError("DOCX creation failed.");
  }

  delete batch;

  if (argc_ < 4) helper::File::Remove(path_docx_in_.c_str());

  return 0 == std::rename(
      std::string(path_docx_out).append("tmp").c_str(),
      path_docx_out.c_str())
         ? docxbox::AppLog::NotifyInfo("Saved DOCX: " + path_docx_out_)
         : docxbox::AppLog::NotifyError(
          "Failed rename temporary DOCX to: " + path_docx_out_);
}

bool docx_archive::CatFile() {
  if (!docxbox::AppArgument::AreArgumentsGiven(
      argc_,
      2, "DOCX filename",
      3, "File to be displayed")) return false;

  if (!UnzipDocxByArgv(true, "", true, true)) return false;

  std::string path_file_relative = argv_[3];

  if (path_file_relative.empty())
    return docxbox::AppLog::NotifyError(
        "Invalid file path: " + path_file_relative);

  if (path_file_relative[0] == '/')
    path_file_relative = path_file_relative.substr(1);

  std::string path_file = path_extract_ + "/" + path_file_relative;
  std::string file_contents;

  if (!helper::File::GetFileContents(path_file, &file_contents))
    return false;

  std::cout << file_contents << "\n";

  return true;
}

bool docx_archive::ViewFilesDiff() {
  if (!docxbox::AppArgument::AreArgumentsGiven(
      argc_,
      3, "DOCX file to compare with",
      4, "File within DOCX archives to be compared")) return false;

  if (!UnzipDocxByArgv(true, "", true, true)) return false;

  std::string path_extract_left = path_extract_;

  argv_[2] = argv_[3];

  if (!UnzipDocxByArgv(true, "", true, true)) return false;

  std::string path_extract_right = path_extract_;

  std::string file = argv_[4];

  if (!helper::File::FileExists(path_extract_left + "/" + file)
      || !helper::File::FileExists(path_extract_right + "/" + file))
    return docxbox::AppLog::NotifyError(
        "File not given in both DOCX archives: " + file);

  if (argc_ > 5
      && (argv_[5] == "-u" || argv_[5] == "--unified"))
    docx_diff::OutputUnified(path_extract_left, path_extract_right, file);
  else
    docx_diff::OutputSideBySide(path_extract_left, path_extract_right, file);

  std::cout << "\n";

  return true;
}

// TODO(kay): move into class of its own (docx_archive_meta)
//            along w/ rel. sub-methods
bool docx_archive::ModifyMeta() {
  auto *meta_component = new meta(argc_, argv_);

  if (!meta_component->InitModificationArguments(is_batch_mode_)
      || (!is_batch_mode_
          && !UnzipDocxByArgv(true, "-" + helper::File::GetTmpName()))) {
    delete meta_component;

    return docxbox::AppLog::NotifyError(
        "Initialization for meta modification failed.");
  }

  miniz_cpp::zip_file docx_file(path_docx_in_);
  auto file_list = docx_file.infolist();

  meta_component->SetPathExtract(path_extract_);

  if (!meta_component->UpsertAttribute()) {
    delete meta_component;

    return docxbox::AppLog::NotifyError("Update/Insert meta attribute failed.");
  }

  // Modifiable meta attributes are in docProps/app.xml or docProps/core.xml
  meta_component->SaveXml();

  // Create resulting DOCX from files during non-batch mode
  // or at final step of batch sequence
  if (is_batch_mode_) {
    if (!is_final_batch_step_) {
      delete meta_component;

      return true;
    }
  } else {
    if (argc_ >= 6) {
      // Result filename is given as argument
      path_docx_out_ = argv_[5];
      helper::File::ResolvePath(path_working_dir_, &path_docx_out_);
    } else {
      // Overwrite original DOCX
      path_docx_out_ = path_docx_in_;
    }
  }

  auto attribute = meta_component->GetAttribute();

  if (!Zip(false,
           path_extract_,
           path_docx_out_ + "tmp",
           (attribute != meta::Attr_Core_Modified
               && !is_batch_mode_
               && !meta_component->HasModifiedModificationDate()))) {
    delete meta_component;

    return docxbox::AppLog::NotifyError("DOCX creation failed.");
  }

  delete meta_component;

  if (path_docx_in_ == path_docx_out_)
    helper::File::Remove(path_docx_in_.c_str());

  return 0 == std::rename(
      std::string(path_docx_out_).append("tmp").c_str(),
      path_docx_out_.c_str())
         ? docxbox::AppLog::NotifyInfo("Saved DOCX: " + path_docx_out_)
         : docxbox::AppLog::NotifyError(
          "Failed rename temporary DOCX to: " + path_docx_out_);
}

// Update given meta date attribute and immediately save updated core.xml
bool docx_archive::UpdateCoreXmlDate(meta::AttributeType attribute,
                                     const std::string &value) {
  auto meta_component = new meta(argc_, argv_);

  meta_component->SetPathExtract(path_extract_);
  meta_component->SetAttribute(attribute);

  if (value.empty()) {
    meta_component->SetValue(helper::DateTime::GetCurrentDateTimeInIso8601());
  } else {
    if (!helper::DateTime::IsIso8601Date(value)) {
      delete meta_component;

      return docxbox::AppLog::NotifyError(
          "Invalid date (must be ISO 8601): " + value);
    }

    meta_component->SetValue(value);
  }

  auto result = meta_component->UpsertAttributeInCoreXml(true);

  delete meta_component;

  return result;
}

void docx_archive::ZipUsingCLi(const std::string &path_dir,
                               const std::string &path_docx_result,
                               bool compress_xml) const {
  if (compress_xml) CompressXmlFiles(path_dir);

  std::string cmd = "cd " + path_dir + ";zip tmp.zip -rq *;";

  if (!path_docx_result.empty()) {
    if (helper::File::FileExists(path_docx_result))
      helper::File::Remove(path_docx_result.c_str());

    cmd += "mv tmp.zip " + path_docx_result;
  }

  helper::Cli::Execute(cmd.c_str());
}

void docx_archive::ZipUsingMinizCpp(bool compress_xml,
                                    const std::string &path_dir,
                                    const std::string &path_docx_result) const {
  // Get relative paths of all files to be zipped
  std::vector<std::string> files_in_zip;

  files_in_zip = helper::File::ScanDirRecursive(
      path_dir.c_str(), files_in_zip, path_dir + "/");

  miniz_cpp::zip_file file;

  for (const auto &file_in_zip : files_in_zip) {
    std::string path_file_absolute =
        std::string(path_dir + "/").append(file_in_zip);

    std::string xml;
    helper::File::GetFileContents(path_file_absolute, &xml);

    if (compress_xml) helper::Xml::CompressXml(&xml);

    file.writestr(file_in_zip, xml);
  }

  file.save(path_docx_result);
}

void docx_archive::CompressXmlFiles(const std::string &path_dir) const {
  std::vector<std::string> files;

  files = helper::File::ScanDirRecursive(
      path_dir.c_str(), files, path_dir + "/");

  for (const auto &file_in_zip : files) {
    std::string path_file_absolute =
        std::string(path_dir + "/").append(file_in_zip);

    std::string xml;
    helper::File::GetFileContents(path_file_absolute, &xml);

    helper::Xml::CompressXml(&xml);

    helper::File::WriteToNewFile(path_file_absolute, xml);
  }
}
