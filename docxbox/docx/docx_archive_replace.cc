// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/docx/docx_archive_replace.h>

docx_archive_replace::docx_archive_replace(
    int argc,
    char **argv) : docx_archive(argc, argv) {}

bool docx_archive_replace::ReplaceImage() {
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

bool docx_archive_replace::ReplaceText() {
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

bool docx_archive_replace::RemoveBetweenText() {
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

bool docx_archive_replace::ReplaceAllTextByLoremIpsum() {
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

bool docx_archive_replace::SetFieldValue() {
  if (!UnzipDocx("-" + helper::File::GetTmpName())) return false;

  if (!docxbox::AppArguments::IsArgumentGiven(
          argc_,
          3,
          "Field identifier")
      || !docxbox::AppArguments::IsArgumentGiven(
          argc_,
          4,
          "Value to be set")) return false;

  std::string field_identifier = argv_[3];
  std::string value = argv_[4];

  miniz_cpp::zip_file docx_file(path_docx_in_);

  auto file_list = docx_file.infolist();

  auto parser = new docx_xml_fields(argc_, argv_);

  for (const auto &file_in_zip : file_list) {
    // TODO(kay): fetch from all textual XML files, instead only document.xml

    if (!helper::String::EndsWith(file_in_zip.filename, "word/document.xml"))
      continue;

    if (!parser->SetFieldText(
        path_extract_ + "/" + file_in_zip.filename,
        field_identifier,
        value)) {
      delete parser;

      return false;
    }
  }

  delete parser;

  std::string path_docx_out =
      argc_ >= 5
      // Result filename is given as argument
      ? helper::File::ResolvePath(path_working_directory_, argv_[5])
      // Overwrite original DOCX
      : path_docx_in_;

  if (!Zip(false, path_extract_, path_docx_out + "tmp")) {
    std::cerr << "DOCX creation failed.\n";

    return false;
  }

  miniz_cpp_ext::RemoveExtract(path_extract_, file_list);

  std::rename(
      std::string(path_docx_out).append("tmp").c_str(),
      path_docx_out.c_str());

  return true;
}
