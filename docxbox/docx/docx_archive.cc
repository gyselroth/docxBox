// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/docx_archive.h>

docx_archive::docx_archive(int argc, char **argv) {
  argc_ = argc;
  argv_ = argv;

  path_working_directory_ = getenv("PWD");
}

// Setup path to DOCX file,
// absolute or relative from execution path, from given argument
bool docx_archive::InitPathDocxByArgV(int index_path_argument) {
  path_docx_in_ = docxbox::AppArguments::ResolvePathFromArgument(
      path_working_directory_,
      argc_,
      argv_,
      index_path_argument);

  if (!helper::File::FileExists(path_docx_in_))
    throw "Error - File not found: " + path_docx_in_ + "\n";

  return true;
}

// Render path (string) where to extract given DOCX file
void docx_archive::InitExtractionPath(const std::string &directory_appendix,
                                      const std::string &path_docx) {
  path_extract_ =
      helper::File::GetLastPathSegment(path_docx)
          + (directory_appendix.empty()
             ? "-extracted"
             : directory_appendix);
}

std::string docx_archive::ParseFileWildcard(int index_argument) const {
  return argc_ >= index_argument + 1
             && argv_[3][0] == '*'
             && argv_[3][1] == '.'
         ? std::string(argv_[3]).substr(2)
         : "";
}

bool docx_archive::ExecuteUserCommand() {
  // TODO(kay): add safeguard: verify all arguments being given + valid

  if (!UnzipDocx("", true, true)) return false;

  std::string command = argv_[3];
  helper::String::ReplaceAll(command, "*DOCX*", path_extract_);

  helper::Cli::Execute(command.c_str());

  std::cout << "\nHit [Enter] when done.";

  getchar();

  Zip(true, path_extract_, "", true, true);

  std::cout << "\n";

  miniz_cpp::zip_file docx_file(path_docx_in_);
  auto file_list = docx_file.infolist();
  miniz_cpp_ext::RemoveExtract(path_extract_, file_list);

  return true;
}

bool docx_archive::ViewFilesDiff() {
  // TODO(kay): add safeguard: verify all arguments being given + valid

  if (!UnzipDocx("", true, true)) return false;

  std::string path_in_left = path_docx_in_;
  std::string path_extract_left = path_extract_;

  argv_[2] = argv_[3];

  if (!UnzipDocx("", true, true)) return false;

  std::string path_in_right = path_docx_in_;
  std::string path_extract_right = path_extract_;

  std::string file = argv_[4];

  int width = helper::File::GetLongestLineLength(path_extract_left + "/" + file);
  int width2 = helper::File::GetLongestLineLength(path_extract_right + "/" + file);

  if (width2 > width) width = width2;

  if (width > 200) width = 200;

  helper::Cli::Execute(
      std::string("diff -y "
                  "--width=" + std::to_string(width) + " "
                  + path_extract_left + "/" + file + " "
                  + path_extract_right + "/" + file).c_str());

  std::cout << "\nHit [Enter] when done.";

  getchar();

  std::cout << "\n";

  miniz_cpp::zip_file docx_file_left(path_in_left);
  auto file_list = docx_file_left.infolist();
  miniz_cpp_ext::RemoveExtract(path_extract_left, file_list);

  miniz_cpp::zip_file docx_file_right(path_in_left);
  file_list = docx_file_right.infolist();
  miniz_cpp_ext::RemoveExtract(path_extract_right, file_list);

  return true;
}
// Unzip all files of DOCX file
bool docx_archive::UnzipDocx(const std::string &directory_appendix,
                             bool ensure_is_docx,
                             bool format_xml_files) {
  if (!docxbox::AppArguments::IsArgumentGiven(
      argc_,
      2,
      "Filename of DOCX to be extracted"))
    return false;

  try {
    InitPathDocxByArgV(3);
  } catch (std::string &message) {
    std::cerr << message << "\n";

    return false;
  }

  InitExtractionPath(directory_appendix, path_docx_in_);

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  mkdir(path_extract_.c_str(), 0777);
  miniz_cpp_ext::CreateSubDirectories(path_extract_, file_list);

  docx_file.extractall(path_working_directory_ + "/" + path_extract_);

  if (ensure_is_docx && IsDocx()) {
    std::cerr << "Error: " << path_docx_in_ << " is not a DOCX document.\n";

    return false;
  }

  return format_xml_files
    ? miniz_cpp_ext::IndentXmlFiles(path_extract_, file_list)
    : true;
}

// Check formal structure of DOCX archive - mandatory files given?
bool docx_archive::IsDocx() {
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
  if (!UnzipDocx("-media")) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  miniz_cpp_ext::ReduceExtractToImages(path_extract_, file_list);

  return true;
}

bool docx_archive::ModifyMeta() {
  auto *meta = new docx_meta(argc_, argv_);

  if (!meta->InitModificationArguments()
      || !UnzipDocx("-" + helper::File::GetTmpName())) {
    std::cerr << "Initialization for meta modification failed.\n";

    delete meta;

    return false;
  }

  miniz_cpp::zip_file docx_file(path_docx_in_);
  auto file_list = docx_file.infolist();

  meta->SetPathExtract(path_extract_);

  if (!meta->UpsertAttribute()) {
    std::cerr << "Update/Insert meta attribute failed.\n";

    delete meta;

    return false;
  }

  // Modifiable meta attributes are in docProps/core.xml
  try {
    meta->SaveCoreXml();
  } catch (std::string &message) {
    std::cerr << message;
  }

  delete meta;

  std::string path_docx_out;

  if (argc_ >= 6) {
    // Result filename is given as argument
    path_docx_out =
        helper::File::ResolvePath(path_working_directory_, argv_[5]);
  } else {
    // Overwrite original DOCX
    path_docx_out = path_docx_in_;
  }

  auto attribute = meta->GetAttribute();

  if (!Zip(false, path_extract_, path_docx_out + "tmp",
      attribute != docx_meta::Attribute_Created,
      attribute != docx_meta::Attribute_Modified)) {
    std::cerr << "DOCX creation failed.\n";

    return false;
  }

  if (argc_ < 6) helper::File::Remove(path_docx_in_.c_str());

  std::rename(
      std::string(path_docx_out).append("tmp").c_str(),
      path_docx_out.c_str());

  return miniz_cpp_ext::RemoveExtract(path_extract_, file_list);
}

bool docx_archive::GetText(bool newline_at_segments) {
  if (!UnzipDocx("-" + helper::File::GetTmpName())) return false;

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

  miniz_cpp_ext::RemoveExtract(path_extract_, file_list);

  return true;
}

bool docx_archive::ReplaceImage() {
  if (
      !docxbox::AppArguments::IsArgumentGiven(
          argc_,
          2,
          "Filename of DOCX")
          || !docxbox::AppArguments::IsArgumentGiven(
          argc_,
          3,
          "Filename of image to be replaced")
          || !docxbox::AppArguments::IsArgumentGiven(
              argc_,
              4,
              "Filename of replacement image")
      )
    return false;

  if (!UnzipDocx("-" + helper::File::GetTmpName())) return false;

  std::string image_original = argv_[3];

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  try {
    bool found = false;

    for (const auto &file_in_zip : file_list) {
      if (!helper::String::EndsWith(file_in_zip.filename, image_original))
        continue;

      found = true;

      std::string
          path_image_original = path_extract_ + "/" + file_in_zip.filename;

      if (!helper::File::Remove(path_image_original.c_str()))
        throw "Failed replace " + image_original + "\n";

      std::string path_image_replacement =
          helper::File::ResolvePath(path_working_directory_, argv_[4]);

      helper::File::CopyFile(path_image_original, path_image_replacement);

      break;
    }

    if (!found)
      throw "Cannot replace " + image_original
            + " - no such image within " + path_docx_in_ + "\n";

    std::string path_docx_out =
        argc_ >= 6
        // Result filename is given as argument
        ? helper::File::ResolvePath(
            path_working_directory_,
            argv_[5])
        // Overwrite original DOCX
        : path_docx_in_;

    if (!Zip(false, path_extract_, path_docx_out + "tmp"))
      throw "DOCX creation failed.\n";

    if (argc_ < 6) helper::File::Remove(path_docx_in_.c_str());

    std::rename(
        std::string(path_docx_out).append("tmp").c_str(),
        path_docx_out.c_str());
  } catch (std::string &message) {
    std::cerr << message;

    miniz_cpp_ext::RemoveExtract(path_extract_, file_list);

    return false;
  }

  return miniz_cpp_ext::RemoveExtract(path_extract_, file_list);
}

bool docx_archive::ReplaceText() {
  if (!docxbox::AppArguments::IsArgumentGiven(argc_, 2, "DOCX Filename")
      || !docxbox::AppArguments::IsArgumentGiven(
          argc_,
          3,
          "String to be found (and replaced)")
      || !docxbox::AppArguments::IsArgumentGiven(
          argc_,
          4,
          "Replacement")) return false;

  std::string search = argv_[3];
  std::string replacement = argv_[4];

  if (!UnzipDocx("-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto parser = new docx_xml_replace(argc_, argv_);

  for (const auto &file_in_zip : file_list) {
    if (!docx_xml::IsXmlFileContainingText(file_in_zip.filename)) continue;

    std::string path_file_absolute = path_extract_ + "/" + file_in_zip.filename;

    if (!parser->ReplaceInXml(path_file_absolute, search, replacement)) {
      std::cerr << "Error: Failed replace string in: "
                << file_in_zip.filename << "\n";

      delete parser;

      return false;
    }
  }

  delete parser;

  std::string path_docx_out =
      argc_ >= 6
      // Result filename is given as argument
      ? helper::File::ResolvePath(
          path_working_directory_,
          argv_[5])
      // Overwrite original DOCX
      : path_docx_in_;

  if (!Zip(false, path_extract_, path_docx_out + "tmp")) {
    std::cerr << "DOCX creation failed.\n";

    return false;
  }

  if (argc_ < 6) helper::File::Remove(path_docx_in_.c_str());

  std::rename(
      std::string(path_docx_out).append("tmp").c_str(),
      path_docx_out.c_str());

  return miniz_cpp_ext::RemoveExtract(path_extract_, file_list);
}

bool docx_archive::RemoveBetweenText() {
  if (!docxbox::AppArguments::IsArgumentGiven(argc_, 2, "DOCX Filename")
      || !docxbox::AppArguments::IsArgumentGiven(
          argc_,
          3,
          "String left-hand-side of part to be removed")
      || !docxbox::AppArguments::IsArgumentGiven(
          argc_,
          4,
          "String right-hand-side of part to be removed")) return false;

  std::string lhs = argv_[3];
  std::string rhs = argv_[4];

  if (!UnzipDocx("-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto parser = new docx_xml_remove(argc_, argv_);

  for (const auto &file_in_zip : file_list) {
    if (!docx_xml::IsXmlFileContainingText(file_in_zip.filename)) continue;

    std::string path_file_absolute = path_extract_ + "/" + file_in_zip.filename;

    if (!parser->RemoveBetweenStringsInXml(path_file_absolute, lhs, rhs)) {
      std::cerr << "Error: Failed to remove content from: "
                << file_in_zip.filename << "\n";

      delete parser;

      return false;
    }
  }

  delete parser;

  std::string path_docx_out =
      argc_ >= 7
      // Result filename is given as argument
      ? helper::File::ResolvePath(
          path_working_directory_,
          argv_[6])
      // Overwrite original DOCX
      : path_docx_in_;

  if (!Zip(false, path_extract_, path_docx_out + "tmp")) {
    std::cerr << "DOCX creation failed.\n";

    return false;
  }

  if (argc_ < 7) helper::File::Remove(path_docx_in_.c_str());

  std::rename(
      std::string(path_docx_out).append("tmp").c_str(),
      path_docx_out.c_str());

  return miniz_cpp_ext::RemoveExtract(path_extract_, file_list);
}

bool docx_archive::ReplaceAllTextByLoremIpsum() {
  if (!UnzipDocx("-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto parser = new docx_xml_lorem(argc_, argv_);

  for (const auto &file_in_zip : file_list) {
    if (!docx_xml::IsXmlFileContainingText(file_in_zip.filename)) continue;

    std::string path_file_absolute = path_extract_ + "/" + file_in_zip.filename;

    if (!parser->RandomizeAllTextInXml(path_file_absolute)) {
      std::cerr << "Error: Failed insert lorem ipsum in: "
                << file_in_zip.filename << "\n";

      delete parser;

      return false;
    }
  }

  delete parser;

  std::string path_docx_out =
      argc_ >= 4
      // Result filename is given as argument
      ? helper::File::ResolvePath(path_working_directory_, argv_[3])
      // Overwrite original DOCX
      : path_docx_in_;

  if (!Zip(false, path_extract_, path_docx_out + "tmp")) {
    std::cerr << "DOCX creation failed.\n";

    return false;
  }

  if (argc_ < 6) helper::File::Remove(path_docx_in_.c_str());

  std::rename(
      std::string(path_docx_out).append("tmp").c_str(),
      path_docx_out.c_str());

  return miniz_cpp_ext::RemoveExtract(path_extract_, file_list);
}

// Zip files into given path into DOCX of given filename
// Optionally update "creation" and "modified" meta attributes (core.xml)
// to current date-time value
bool docx_archive::Zip(
    bool compress_xml,
    std::string path_directory,
    std::string path_docx_result,
    bool update_created,
    bool update_modified) {
  if (path_directory.empty()) {
    if (argc_ <= 2) {
      std::cerr << "Missing argument: path of directory to be zipped\n";

      return false;
    }

    if (argc_ <= 3) {
      std::cerr << "Missing argument: filename of docx to be created\n";

      return false;
    }

    path_directory =
        helper::File::ResolvePath(path_working_directory_, argv_[2]);

    path_docx_result =
        helper::File::ResolvePath(path_working_directory_, argv_[3], false);
  } else {
    if (!helper::File::IsDirectory(path_directory)) {
      std::cerr << "Not a directory: " << path_directory;

      return false;
    }
  }

  if (update_created)
    UpdateCoreXmlDate(docx_meta::Attribute::Attribute_Created);

  if (update_modified)
    UpdateCoreXmlDate(docx_meta::Attribute::Attribute_Modified);

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

    if (compress_xml) docx_xml_indent::CompressXml(xml);

    file.writestr(file_in_zip, xml);
  }

  file.save(path_docx_result);

  return helper::File::FileExists(path_docx_result);
}

// Update given meta date attribute and immediately save updated core.xml
// TODO(kay): add multi-attributes variation of method
//  to load/save only once than
bool docx_archive::UpdateCoreXmlDate(docx_meta::Attribute attribute) {
  // TODO(kay): add gate - ensure attribute is a date

  auto meta = new docx_meta(argc_, argv_);

  meta->SetPathExtract(path_extract_);

  meta->SetAttribute(attribute);
  meta->SetValue(helper::DateTime::GetCurrentDateTimeInIso8601());

  auto result = meta->UpsertAttribute(true);

  delete meta;

  return result;
}
