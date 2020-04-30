// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/docx_archive_list.h>

docx_archive_list::docx_archive_list(
    int argc,
    char **argv) : docx_archive(argc, argv) {}

// List files inside DOCX archive and their attributes
bool docx_archive_list::ListFilesInDocx(bool as_json, bool images_only) {
  if (!docxbox::AppArguments::IsArgumentGiven(argc_, 2, "DOCX filename"))
    return false;

  try {
    InitPathDocxByArgV(3);
  } catch (std::string &message) {
    std::cerr << message;

    return false;
  }

  miniz_cpp::zip_file docx_file(path_docx_in_);

  std::string file_ending = ParseFileWildcard(3);

  std::string files_list =
      miniz_cpp_ext::PrintDir(docx_file, as_json, images_only, file_ending);

  if (file_ending.empty()
      && argc_ >= 4
      && helper::String::EndsWith(argv_[3], ".docx")
      && helper::File::FileExists(argv_[3])) {
    ListFilesInDocxCompare(as_json, images_only, file_ending, files_list);
  } else {
    // Output single DOCX files list
    std::cout << files_list;
  }

  return true;
}

// Output two DOCX file lists side-by-side
void docx_archive_list::ListFilesInDocxCompare(bool as_json,
                                               bool images_only,
                                               const std::string &file_ending,
                                               std::string &file_list_1) {
  const std::string &path_docx_in_2 =
      docxbox::AppArguments::ResolvePathFromArgument(
          path_working_directory_, argc_, argv_, 4);

  miniz_cpp::zip_file docx_file_2(path_docx_in_2);

  std::string file_list_2 = miniz_cpp_ext::PrintDir(
          docx_file_2, as_json, images_only, file_ending);

  docx_fileList::SortFileListByFilename(file_list_1);
  docx_fileList::SortFileListByFilename(file_list_2);

  // TODO(kay):
  //   3. detect items that are given only in 1 DOCX,
  //      and insert empty lines into the other list
  //   4. convert lists back to std::string

  std::cout << helper::String::RenderSideBySide(
      path_docx_in_ + "\n\n" + file_list_1,
      path_docx_in_2 + "\n\n" + file_list_2,
      8);
}

// List contained images and their attributes
bool docx_archive_list::ListImages(bool as_json) {
  if (as_json) return ListFilesInDocx(as_json, true);

  if (!UnzipDocx("-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  std::cout
      << miniz_cpp_ext::PrintDir(docx_file, false, true);

  return miniz_cpp_ext::RemoveExtract(path_extract_, docx_file.infolist());
}

bool docx_archive_list::LocateFilesContainingString(bool as_json) {
  std::string needle;
  InitLocateFilesContaining(as_json, needle);

  if (!UnzipDocx("", true, true)) return false;

  std::string grep = "grep -iRl \"" + needle + "\" " + path_extract_;

  auto files_located = helper::Cli::GetExecutionResponse(grep.c_str());
  helper::String::ReplaceAll(files_located, path_extract_ + "/", "");

  auto filenames = helper::String::Explode(files_located, '\n');

  Zip(true, path_extract_, "", true, true);

  miniz_cpp::zip_file docx_file(path_docx_in_);
  auto file_list = docx_file.infolist();

  if (!filenames.empty())
    std::cout
      << miniz_cpp_ext::PrintDir(docx_file, as_json, false, "", filenames);

  miniz_cpp_ext::RemoveExtract(path_extract_, file_list);

  return true;
}

void docx_archive_list::InitLocateFilesContaining(bool &as_json,
                                             std::string &needle) const {
  docxbox::AppArguments::EnsureArgumentGiven(
      argc_,
      3,
      "String or regular expression to be located");

    needle = argv_[3];

  if (needle == "-l" || needle == "--locate") {
    docxbox::AppArguments::EnsureArgumentGiven(
        argc_,
        4,
        "String or regular expression to be located");

    needle = argv_[4];
  }

  if (needle == "-lj") {
    docxbox::AppArguments::EnsureArgumentGiven(
        argc_,
        4,
        "String or regular expression to be located");

    needle = argv_[4];
    as_json = true;
  }

  if (needle == "-j" || needle == "--json") {
    docxbox::AppArguments::EnsureArgumentGiven(
        argc_,
        5,
        "String or regular expression to be located");

    as_json = true;
    needle = argv_[5];
  }

  if (needle == "-lj") {
    docxbox::AppArguments::EnsureArgumentGiven(
        argc_,
        4,
        "String or regular expression to be located");

    needle = argv_[4];
  }
}

// Output meta data from within given DOCX file:
// Creation date, revision, title, language, used fonts, contained media files
bool docx_archive_list::ListMeta(bool as_json) {
  if (!UnzipDocx("-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto *meta = new docx_meta(argc_, argv_);
  meta->SetPathExtract(path_extract_);

  if (as_json) meta->SetOutputAsJson(true);

  int index_app = 0;
  int index_core = 0;

  for (const auto &file_in_zip : file_list) {
    const char *path_file_within_docx = file_in_zip.filename.c_str();

    std::string
        path_file_absolute = path_extract_ + "/" + path_file_within_docx;

    // Extract meta data from app.xml and core.xml,
    // data is output when both are collected
    // or another of the same kind is found (e.g. merged documents)
    if (helper::String::EndsWith(file_in_zip.filename, "app.xml")) {
      meta->CollectFromAppXml(
          file_in_zip.filename,
          helper::File::GetFileContents(path_file_absolute));

      ++index_app;
    } else if (helper::String::EndsWith(file_in_zip.filename, "core.xml")) {
      meta->LoadCoreXml(path_file_absolute);
      meta->CollectFromCoreXml(file_in_zip.filename);

      ++index_core;
    }
  }

  // Output anything that hasn't been yet
  meta->Output();

  miniz_cpp_ext::RemoveExtract(path_extract_, file_list);

  delete meta;

  return true;
}

// List referenced fonts and their metrics
bool docx_archive_list::ListFonts(bool as_json) {
  if (!UnzipDocx("-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto *fontTable = new docx_fontTable();

  if (as_json) std::cout << "[";

  int index_font = 0;

  for (const auto &file_in_zip : file_list) {
    if (helper::String::EndsWith(file_in_zip.filename, "fontTable.xml")) {
      // Extract fonts data from all fontTable.xml files
      const char *path_file_within_docx = file_in_zip.filename.c_str();

      std::string path_file_absolute =
          path_extract_ + "/" + path_file_within_docx;

      fontTable->CollectFontsMetrics(
          helper::File::GetFileContents(path_file_absolute));

      if (as_json) {
        if (index_font > 0) std::cout << ",";

        fontTable->OutputAsJson(file_in_zip.filename);
      } else {
        fontTable->OutputPlain(file_in_zip.filename);
      }

      fontTable->Clear();

      ++index_font;
    }
  }

  delete fontTable;

  if (as_json) std::cout << "]";

  miniz_cpp_ext::RemoveExtract(path_extract_, file_list);

  return true;
}

bool docx_archive_list::ListFields(bool as_json) {
  if (!UnzipDocx("-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto parser = new docx_xml_field(argc_, argv_);

  for (const auto &file_in_zip : file_list) {
    // TODO(kay): fetch from all textual XML files, instead only document.xml

    if (!helper::String::EndsWith(file_in_zip.filename, "word/document.xml"))
      continue;

    parser->CollectFields(path_extract_ + "/" + file_in_zip.filename);
  }

  parser->Output(as_json);

  delete parser;

  miniz_cpp_ext::RemoveExtract(path_extract_, file_list);

  return true;
}
