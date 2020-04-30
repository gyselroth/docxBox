// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/helper/helper_file.h>

namespace helper {

bool File::IsDirectory(const std::string& file_path) {
  struct stat buffer;

  stat(file_path.c_str(), &buffer);

  return S_ISDIR(buffer.st_mode);
}

bool File::FileExists(const std::string &name) {
  return access(name.c_str(), F_OK) != -1;
}

std::string File::GetFileContents(const std::string &filename) {
  std::ifstream file(filename);

  return GetFileContents(file);
}

std::string File::GetFileContents(std::ifstream &file) {
  std::streampos length = GetFileSize(file);

  // Read whole file into the buffer
  std::vector<char> buffer(static_cast<u_int32_t>(length));
  file.read(&buffer[0], length);

  std::string str(buffer.begin(), buffer.end());

  file.close();

  return str;
}

u_int32_t File::GetLongestLineLength(const std::string &filename) {
  // TODO(kay) add variation w/ arbitrary amount of filenames as vector
  std::string contents = GetFileContents(filename);
  std::vector<std::string> lines = helper::String::Explode(contents, '\n');

  return helper::String::GetMaxLength(lines);
}

// Resolve path: keep absolute or make relative from given (binary) path
std::string File::ResolvePath(
    const std::string &pwd,
    std::string path,
    bool must_exist) {
  helper::String::Trim(path);

  if (!helper::String::StartsWith(path.c_str(), "/"))
    path = helper::String::EndsWith(pwd, "/")
        ? pwd + path
        : pwd + "/" + path;

  if (must_exist
      && (!helper::File::IsDirectory(path)
          && !helper::File::FileExists(path))) {
    throw "File not found: " + path;
  }

  return path;
}

std::streampos File::GetFileSize(std::ifstream &file) {
  file.seekg(0, std::ios::end);
  std::streampos length = file.tellg();
  file.seekg(0, std::ios::beg);

  return length;
}

bool File::WriteToNewFile(
    const std::string &filename,
    const std::string &content) {
  std::ofstream outfile(filename);
  outfile << content;
  outfile.close();

  return File::FileExists(filename);
}

void File::CopyFile(
    const std::string &path_image_original,
    const std::string &path_image_replacement) {
  int source = open(path_image_replacement.c_str(), O_RDONLY, 0);
  int dest = open(path_image_original.c_str(), O_WRONLY | O_CREAT, 0644);

  // struct required, rationale: function stat() exists also
  struct stat stat_source;
  fstat(source, &stat_source);

  sendfile(dest, source, 0, stat_source.st_size);

  close(source);
  close(dest);
}

bool File::Remove(const char *path) {
  return remove(path) == 0;
}

bool File::RemoveRecursive(const char *path) {
  if (!IsDirectory(path)) return Remove(path);

  DIR *d = opendir(path);
  size_t path_len = strlen(path);

  int result = -1;

  if (d) {
    struct dirent *p;

    result = 0;
    while (!result && (p=readdir(d))) {
      int r2 = -1;
      char *buffer;
      size_t len;

      if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
        continue;

      len = path_len + strlen(p->d_name) + 2;
      buffer = static_cast<char *>(malloc(len));

      if (buffer) {
        struct stat stat_buffer{};

        snprintf(buffer, len, "%s/%s", path, p->d_name);

        if (!stat(buffer, &stat_buffer)) {
          if (S_ISDIR(stat_buffer.st_mode))
            r2 = RemoveRecursive(buffer);
          else
            r2 = unlink(buffer);
        }

        free(buffer);
      }

      result = r2;
    }

    closedir(d);
  }

  if (!result) result = rmdir(path);

  return result;
}

std::string File::GetLastPathSegment(std::string path) {
  if (helper::String::Contains(path, "/")) {
    std::vector<std::string> parts = helper::String::Explode(path, '/');

    return parts[parts.size() - 1];
  }

  return path;
}

std::vector<std::string> File::ScanDir(const char *pathname) {
  struct dirent **namelist;
  int filecount;

  std::vector<std::string> files;

  filecount = scandir(pathname, &namelist, nullptr, alphasort);

  if (filecount > 0) {
    for (int i = 0; i < filecount; i++) {
      std::string path_file;

      if (namelist[i]->d_name[0] != '.') {
        path_file += pathname;
        path_file  += "/";
        path_file  += namelist[i]->d_name;

        files.push_back(path_file);
      }
    }

    while (filecount--) {
      free(namelist[filecount]);
    }

    free(namelist);
  }

  return files;
}

std::vector<std::string> File::ScanDirRecursive(
    const char *pathname,
    std::vector<std::string> files,
    const std::string& remove_prefix
) {
  struct dirent **namelist;
  int filecount;

  bool do_remove_prefix = !remove_prefix.empty();

  filecount = scandir(pathname, &namelist, nullptr, alphasort);

  if (filecount > 0) {
    for (int i = 0; i < filecount; i++) {
      std::string path_file;

      if (namelist[i]->d_name[0] != '.') {
        path_file += pathname;
        path_file  += "/";
        path_file  += namelist[i]->d_name;

        if (IsDirectory(path_file)) {
          files = helper::File::ScanDirRecursive(
              path_file.c_str(),
              files,
              remove_prefix);
        } else {
          if (do_remove_prefix)
            helper::String::Replace(path_file, remove_prefix.c_str(), "");

          files.push_back(path_file);
        }
      }
    }

    while (filecount--) {
      free(namelist[filecount]);
    }

    free(namelist);
  }

  return files;
}

std::string File::GetTmpName() {
  char *buffer;
  size_t max_len;

  int timestamp = static_cast<int>(time(nullptr));

  max_len = snprintf(nullptr, 0, "%d", timestamp);
  buffer = reinterpret_cast<char *>(malloc(max_len + 1));
  snprintf(buffer, max_len+1, "%d", timestamp);

  return std::string(buffer);
}

bool File::IsWordCompatibleImage(const std::string &filename) {
  auto str = helper::String::ToLower(filename);

  return (helper::String::EndsWith(str, ".bmp")
      || helper::String::EndsWith(str, ".emg")
      || helper::String::EndsWith(str, ".gif")
      || helper::String::EndsWith(str, ".jpeg")
      || helper::String::EndsWith(str, ".jpg")
      || helper::String::EndsWith(str, ".png")
      || helper::String::EndsWith(str, ".tif")
      || helper::String::EndsWith(str, ".tiff")
      || helper::String::EndsWith(str, ".wmf"));
}

}  // namespace helper
