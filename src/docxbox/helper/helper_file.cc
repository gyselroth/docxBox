
#include "helper_file.h"

namespace helper {

bool File::IsDirectory(const std::string file_path) {
  struct stat buffer;

  stat(file_path.c_str(), &buffer);

  return S_ISDIR(buffer.st_mode);
}

bool File::FileExists(const std::string &name) {
  return access(name.c_str(), F_OK)!=-1;
}

std::string File::GetFileContents(std::string &filename) {
  std::ifstream file(filename);

  return GetFileContents(file);
}

std::string File::GetFileContents(std::ifstream &file) {
  std::streampos length = GetFileSize(file);

  // Read the whole file into the buffer
  std::vector<char> buffer(static_cast<unsigned long>(length));
  file.read(&buffer[0], length);

  std::string str(buffer.begin(), buffer.end());

  file.close();

  return str;
}

// Resolve path: keep absolute or make relative from given (binary) path
std::string File::ResolvePath(const std::string &pwd, std::string path, bool must_exist) {
  helper::String::Trim(path);

  if (!helper::String::StartsWith(path.c_str(), "/"))
    path = helper::String::EndsWith(pwd, "/")
        ? pwd + path
        : pwd + "/" + path;

  if (must_exist
    && (!helper::File::IsDirectory(path) && !helper::File::FileExists(path))
  ) throw "File not found: " + path;

  return path;
}

std::streampos File::GetFileSize(std::ifstream &file) {
  file.seekg(0, std::ios::end);
  std::streampos length = file.tellg();
  file.seekg(0, std::ios::beg);

  return length;
}

bool File::WriteToNewFile(const std::string &filename, std::string &content) {
  std::ofstream outfile(filename);
  outfile << content;
  outfile.close();

  return File::FileExists(filename);
}

void File::CopyFile(const std::string &path_image_original, const std::string &path_image_replacement) {
  int source = open(path_image_replacement.c_str(), O_RDONLY, 0);
  int dest = open(path_image_original.c_str(), O_WRONLY | O_CREAT /*| O_TRUNC/**/, 0644);

  // struct required, rationale: function stat() exists also
  struct stat stat_source;
  fstat(source, &stat_source);

  sendfile(dest, source, 0, stat_source.st_size);

  close(source);
  close(dest);
}

bool File::Remove(const char *file_path) {
  return remove(file_path) == 0;
}

std::string File::GetLastPathSegment(std::string path) {
  if (helper::String::Contains(path, "/")) {
    std::vector<std::string> parts = helper::String::Explode(path, '/');

    return parts[parts.size()-1];
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

      if (namelist[i]->d_name[0]!='.') {
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

      if (namelist[i]->d_name[0]!='.') {
        path_file += pathname;
        path_file  += "/";
        path_file  += namelist[i]->d_name;

        if (IsDirectory(path_file))
          files = helper::File::ScanDirRecursive(path_file.c_str(), files, remove_prefix);
        else {
          if (do_remove_prefix)
            path_file = helper::String::Replace(path_file, remove_prefix.c_str(), "");

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

  int timestamp = (int) time(nullptr);

  max_len = snprintf(nullptr, 0, "%d", timestamp);
  buffer = (char *)malloc(max_len + 1);
  snprintf(buffer, max_len+1, "%d", timestamp);

  return std::string(buffer);
}

} // namespace helper