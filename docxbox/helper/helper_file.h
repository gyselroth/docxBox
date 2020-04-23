// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_HELPER_HELPER_FILE_H_
#define DOCXBOX_HELPER_HELPER_FILE_H_

#include <dirent.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#ifdef __linux__
#include <unistd.h>
#include <algorithm>
#include <chrono>  // NOLINT [build/c++11]
#endif

#include <docxbox/helper/helper_string.h>

namespace helper {
namespace File {

extern bool IsDirectory(const std::string& file_path);

// Check whether given file exists
extern bool FileExists(const std::string &name);

// Resolve path: keep absolute or make relative from given (binary) path
extern std::string ResolvePath(
    const std::string &pwd,
    std::string path,
    bool must_exist = false);

extern std::streampos GetFileSize(std::ifstream &file);

extern std::string GetFileContents(const std::string &filename);
extern std::string GetFileContents(std::ifstream &file);

extern bool WriteToNewFile(
    const std::string &filename,
    const std::string &content);

extern void CopyFile(
    const std::string &path_image_original,
    const std::string &path_image_replacement);

extern bool Remove(const char *file_path);

extern std::string GetLastPathSegment(std::string path);

extern std::vector<std::string> ScanDir(const char *pathname);
extern std::vector<std::string> ScanDirRecursive(
    const char *pathname,
    std::vector<std::string> files,
    const std::string& remove_prefix = "");

extern std::string GetTmpName();

extern bool IsWordCompatibleImage(const std::string &filename);

}  // namespace File
}  // namespace helper

#endif  // DOCXBOX_HELPER_HELPER_FILE_H_
