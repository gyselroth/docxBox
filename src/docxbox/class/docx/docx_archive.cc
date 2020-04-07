
#include "docx_archive.h"

#include "../../../vendor/miniz-cpp/zip_file.hpp"

/**
 * Static extension methods to miniz-cpp
 *
 * Necessarily inlined here: this is the single place of "zip_file.hpp" inclusion.
 * Multiple inclusion would fail w/ a "multiple definitions" linker error,
 * but including these methods within docx_archive.h would make it necessary to including "zip_file.hpp" also there.
 */
class miniz_cpp_ext {
 public:
  static void CreateSubDirectories(
      const std::string& path_extract,
      const std::vector<miniz_cpp::zip_info> &file_list
  ) {
    for (auto file_in_zip : file_list) {
      if (helper::String::Contains(file_in_zip.filename, "/")) {
        auto directories = helper::String::Explode(file_in_zip.filename, '/');

        std::string path = path_extract;
        unsigned long amount_directories = directories.size();

        for (unsigned long i = 0; i < amount_directories - 1; i++) {
          path += "/" + directories[i];
          mkdir(path.c_str(), 0777);
        }
      }
    }
  }

  static void ReduceExtractToImages(std::string& path_extract, std::vector<miniz_cpp::zip_info> &file_list) {
    for (const auto& file_in_zip : file_list) {
      const char *path_file_within_docx = file_in_zip.filename.c_str();

      const std::basic_string<char, std::char_traits<char>, std::allocator<char>>
          &path_file_absolute = path_extract + "/" + path_file_within_docx;

      if (
          helper::String::EndsWith(file_in_zip.filename, ".xml")
              || helper::String::EndsWith(file_in_zip.filename, ".rels")
          )
        // Remove all .xml and .rels files
      {
        helper::File::Remove(path_file_absolute.c_str());
      } else {
        // Move all other (=media) files into root of extraction directory
        std::string path_extract_file = path_extract + "/" + helper::File::GetLastPathSegment(file_in_zip.filename);

        rename(path_file_absolute.c_str(), path_extract_file.c_str());
      }
    }

    // Remove all (now empty) sub-directories
    miniz_cpp_ext::RemoveSubDirectories(path_extract, file_list);
  }

  // Precondition: the directories contain no (more) files
  static void RemoveSubDirectories(const std::string& path_extract, const std::vector<miniz_cpp::zip_info> &file_list) {
    for (auto file_in_zip : file_list) {
      if (helper::String::Contains(file_in_zip.filename, "/")) {
        auto directories = helper::String::Explode(file_in_zip.filename, '/');

        unsigned long amount_directories = directories.size();

        for (unsigned long i=amount_directories-1; i> 0; i--) {
          std::string path_remove = path_extract;

          for (unsigned long j=0; j<i; j++) {
            path_remove += "/" + directories[j];
          }

          helper::File::Remove(path_remove.c_str());
        }
      }
    }
  }

  static void RemoveExtract(const std::string& path_extract, const std::vector<miniz_cpp::zip_info>& file_list) {
    // Remove all files
    for (const auto& file_in_zip : file_list)
      helper::File::Remove(std::string(path_extract + "/" + file_in_zip.filename).c_str());

    RemoveSubDirectories(path_extract, file_list);

    helper::File::Remove(path_extract.c_str());
  }

  static void PrintDirAsJson(miniz_cpp::zip_file &docx_file) {
    std::cout << "[";

    int index_file = 0;

    for(auto &member : docx_file.infolist()) {
      std::cout
          << (index_file == 0 ? "" : ",") << "{"
          << R"("file":")" << member.filename << "\","
          << R"("length":)" << member.file_size << ","
          << R"("date":")"
          << member.date_time.month << "/" << member.date_time.day << "/" << member.date_time.year
          << "\","
          << R"("time":")" << member.date_time.hours << ":" << member.date_time.minutes << "\""
          << "}";

      index_file++;
    }

    std::cout << "]";
  }
};

docx_archive::docx_archive(int argc, char **argv) {
  this->argc = argc;
  this->argv = argv;

  this->path_working_directory = getenv("PWD");
}

// Setup path to DOCX file, absolute or relative from execution path, from given argument
void docx_archive::InitPathDocxByArgV(int index_path_argument) {
  path_docx = docxbox::AppArguments::ResolvePathFromArgument(path_working_directory, argc, argv, index_path_argument);
}

// Output paths of files (and directories) within DOCX file
bool docx_archive::ListFiles(bool asJson) {
  if (argc <= 2) {
    std::cout << "Missing argument: DOCX filename\n";

    return false;
  }

  try {
    InitPathDocxByArgV(3);
  } catch (std::string &message) {
    std::cout << message;

    return false;
  }

  miniz_cpp::zip_file docx_file(path_docx);

  if (asJson) miniz_cpp_ext::PrintDirAsJson(docx_file);
  else docx_file.printdir();

  return true;
}

// Render path (string) where to extract given DOCX file
void docx_archive::InitExtractionPath(const std::string &directory_appendix, const std::string &path_docx) {
  path_extract =
      helper::File::GetLastPathSegment(path_docx)
      + (directory_appendix.empty()
         ? "-extracted"
         : directory_appendix
      );
}

// Unzip all files of DOCX file
bool docx_archive::Unzip(const std::string& directory_appendix) {
  if (argc <= 2) {
    std::cout << "Missing argument: filename of DOCX to unzip\n";

    return false;
  }

  try {
    InitPathDocxByArgV(3);
  } catch (std::string &message) {
    std::cout << message << "\n";

    return false;
  }

  InitExtractionPath(directory_appendix, path_docx);

  miniz_cpp::zip_file docx_file(path_docx);

  auto file_list = docx_file.infolist();

  mkdir(path_extract.c_str(), 0777);
  miniz_cpp_ext::CreateSubDirectories(path_extract, file_list);

  docx_file.extractall(path_working_directory + "/" + path_extract);

  return true;
}

// Unzip all (than remove everything but) media files from DOCX file
bool docx_archive::UnzipMedia() {
  if (!Unzip("-media")) return false;

  miniz_cpp::zip_file docx_file(path_docx);

  auto file_list = docx_file.infolist();

  miniz_cpp_ext::ReduceExtractToImages(path_extract, file_list);

  return true;
}

// Output meta data from within given DOCX file:
// Creation date, revision, title, language, used fonts, contained media files
bool docx_archive::ListMeta(bool asJson) {
  if (!Unzip("-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx);

  auto file_list = docx_file.infolist();

  auto *meta = new docx_meta(argc, argv);

  if (asJson) {
    meta->SetOutputAsJson(true);

    std::cout << "[";
  }

  int index_app = 0;
  int index_core = 0;

  for (const auto& file_in_zip : file_list) {
    const char *path_file_within_docx = file_in_zip.filename.c_str();

    std::string path_file_absolute = path_extract + "/" + path_file_within_docx;

    // Extract meta data from app.xml and core.xml,
    // data is output when both are collected or another of the same kind is found (e.g. merged documents)
    if (helper::String::EndsWith(file_in_zip.filename, "app.xml")) {
      meta->CollectFromAppXml(file_in_zip.filename, helper::File::GetFileContents(path_file_absolute));

      ++index_app;
    } else if (helper::String::EndsWith(file_in_zip.filename, "core.xml")) {
      meta->CollectFromCoreXml(file_in_zip.filename, helper::File::GetFileContents(path_file_absolute));

      ++index_core;
    }
  }

  // Output anything that hasn't been yet
  meta->Output();

  if (asJson) std::cout << "]";

  miniz_cpp_ext::RemoveExtract(path_extract, file_list);

  return true;
}

bool docx_archive::ModifyMeta() {
  auto *meta = new docx_meta(argc, argv);

  if (!meta->AreModificationArgumentsValid()) return false;

  // @todo implement attribute insertion/modification

  return true;
}

// List contained images and their attributes and exif data
bool docx_archive::ListImages(bool asJson) {
  if (!Unzip("-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx);

  auto file_list = docx_file.infolist();

  miniz_cpp_ext::ReduceExtractToImages(path_extract, file_list);

  auto images = helper::File::ScanDir(path_extract.c_str());

  if (asJson) std::cout << "[";

  int index_image = 0;

  for (auto path_image : images) {
    std::string filename = helper::File::GetLastPathSegment(path_image);

    std::string path_jpeg = path_extract + "/" + path_image;

    if (asJson) {
      std::cout << (index_image > 0 ? "," : "") << "\"" << filename << "\"\n";
    } else std::cout << filename << "\n";

    // @todo output file size, image dimension, aspect ration, used scaled size(s), exif data
    //std::cout << helper::String::Repeat("-", filename.length()+1) << "\n";

    //int *height;
    //int *width;
    //if (helper::Image::GetDimension(path_jpeg.c_str(), width, height))
    //  std::cout << "Dimension:" << &width << " x " << &height << " pixels\n";
    //std::cout << "\n";

    index_image++;
  }

  if (asJson) std::cout << "]";
  else std::cout << "\n";

  for (const auto& path_image : images) helper::File::Remove(path_image.c_str());

  helper::File::Remove(path_extract.c_str());

  return true;
}

// List referenced fonts and their metrics
bool docx_archive::ListFonts(bool asJson) {
  if (!Unzip("-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx);

  auto file_list = docx_file.infolist();

  auto *fontTable = new docx_fontTable();

  if (asJson) std::cout << "[";

  int index_font = 0;

  for (const auto& file_in_zip : file_list) {
    const char *path_file_within_docx = file_in_zip.filename.c_str();

    std::string path_file_absolute = path_extract + "/" + path_file_within_docx;

    // Extract fonts data from all fontTable.xml files
    if (helper::String::EndsWith(file_in_zip.filename, "fontTable.xml")) {
      fontTable->CollectFontsMetrics(helper::File::GetFileContents(path_file_absolute));

      if (asJson) {
        if (index_font > 0) std::cout << ",";

        fontTable->OutputAsJson(file_in_zip.filename);
      } else fontTable->OutputPlain(file_in_zip.filename);

      fontTable->Clear();

      ++index_font;
    }
  }

  if (asJson) std::cout << "]";

  miniz_cpp_ext::RemoveExtract(path_extract, file_list);

  return true;
}

bool docx_archive::GetText(bool newlineAtSegments) {
  if (!Unzip("-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx);

  auto file_list = docx_file.infolist();

  auto parser = new docx_xml(argc, argv);

  for (const auto& file_in_zip : file_list) {
    //if (!docx_wordParser::IsXmlFileContainingText(file_in_zip.filename)) continue;
    if (!helper::String::EndsWith(file_in_zip.filename, "word/document.xml")) continue;

    parser->GetTextFromXmlFile(
        path_extract + "/" + file_in_zip.filename,
        newlineAtSegments
    );
  }

  parser->Output();

  miniz_cpp_ext::RemoveExtract(path_extract, file_list);

  return true;
}

bool docx_archive::ReplaceImage() {
  if (!Unzip("-" + helper::File::GetTmpName())) return false;

  if (argc <= 3) {
    std::cout << "Missing argument: filename of image to be replaced\n";

    return false;
  }

  if (argc <= 4) {
    std::cout << "Missing argument: filename of replacement image\n";

    return false;
  }

  if (helper::String::StartsWith(argv[3], "[")) {
    // JSON array = multiple images
    return ReplaceImages();
  }

  std::string image_original = argv[3];

  miniz_cpp::zip_file docx_file(path_docx);

  auto file_list = docx_file.infolist();

  for (const auto& file_in_zip : file_list) {
    if (!helper::String::EndsWith(file_in_zip.filename, image_original)) continue;

    std::string path_image_original = path_extract + "/" + file_in_zip.filename;
    helper::File::Remove(path_image_original.c_str());

    std::string path_image_replacement = helper::File::ResolvePath(path_working_directory, argv[4]);

    helper::File::CopyFile(path_image_original, path_image_replacement);

    break;
  }

  std::string path_docx_out;

  if (argc >= 6) {
    // Result filename is given as argument
    path_docx_out = helper::File::ResolvePath(path_working_directory, argv[5]);
  } else {
    // Overwrite original DOCX
    helper::File::Remove(path_docx.c_str());
    path_docx_out = path_docx;
  }

  return Zip(path_extract, path_docx_out);
}

bool docx_archive::ReplaceImages() {
  return false;
}

bool docx_archive::ReplaceText() {
  if (!Unzip("-" + helper::File::GetTmpName())) return false;

  miniz_cpp::zip_file docx_file(path_docx);

  auto file_list = docx_file.infolist();

  auto parser = new docx_xml(argc, argv);

  for (const auto& file_in_zip : file_list) {
    if (!docx_xml::IsXmlFileContainingText(file_in_zip.filename)) continue;

    std::string path_file_absolute = path_extract + "/" + file_in_zip.filename;

    // @todo implement text replace
  }

  return true;
}

// Zip files into given path into DOCX of given filename
bool docx_archive::Zip(std::string path_directory, std::string path_docx_result) {
  if (path_directory.empty()) {
    if (argc <= 2) {
      std::cout << "Missing argument: path of directory to be zipped\n";

      return false;
    }

    if (argc <= 3) {
      std::cout << "Missing argument: filename of docx to be created\n";

      return false;
    }

    path_directory = helper::File::ResolvePath(path_working_directory, argv[2]);
    path_docx_result = helper::File::ResolvePath(path_working_directory, argv[3], false);
  } else {
    if (!helper::File::IsDirectory(path_directory)) return false;
  }

  // Get relative paths of all files to be zipped
  std::vector<std::string> files_in_zip;
  files_in_zip = helper::File::ScanDirRecursive(
      path_directory.c_str(),
      files_in_zip,
      path_directory + "/"
  );

  miniz_cpp::zip_file file;

  for (const auto& file_in_zip : files_in_zip) {
    std::string path_file_absolute = std::string(path_directory + "/").append(file_in_zip);

    file.writestr(file_in_zip, helper::File::GetFileContents(path_file_absolute));
  }

  file.save(path_docx_result);

  return helper::File::FileExists(path_docx_result);
}