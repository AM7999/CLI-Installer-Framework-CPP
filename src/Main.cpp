#include <zip.h>
#include <iostream>
#include <vector>

#include "Package.h"

#define WHITE "\x1b[0;38;2;255;255;255;49m"
#define GREEN "\x1b[0;38;2;28;220;154;49m"

std::string getOperatingSystem() {
#ifdef _WIN32
    return "win32";
#elif _WIN64
    return "win64";
#elif __APPLE_ || __MACH__
    return "macOS"
#elif __linux__
    return "linux";
#elif __FreeBSD__
    return "freebsd";
#elif __unix || __unix__
    rturn "unix";
#else
    return "other"
#endif
}


std::string loadManifest(const std::string& zipName) {
    int err = 0;

    zip* a = zip_open(zipName.c_str(), 0, &err);
    if (a == nullptr) {
        zip_error_t ziperr;
        zip_error_init_with_code(&ziperr, err);
        std::cerr << "Failed to open ZIP archive: " << zip_error_strerror(&ziperr) << std::endl;
        zip_error_fini(&ziperr);
        return "No Data.";
    }

    struct zip_stat st;
    zip_stat_init(&st);
    if (zip_stat(a, "manifest.json", 0, &st) != 0 || !(st.valid & ZIP_STAT_SIZE)) {
        std::cerr << "Failed to stat file in archive: " << zip_strerror(a) << std::endl;
        zip_close(a);
        return "No Data.";
    }

    zip_file* file = zip_fopen(a, "manifest.json", 0);
    if (file == nullptr) {
        std::cerr << "Failed to open file in archive: " << zip_strerror(a) << std::endl;
        zip_close(a);
        return "No Data.";
    }

    std::string content(st.size, '\0');
    zip_int64_t bytesRead = zip_fread(file, content.data(), st.size);

    zip_fclose(file);
    zip_close(a);

    if (bytesRead < 0 || static_cast<zip_uint64_t>(bytesRead) != st.size) {
        std::cerr << "Failed to extract data" << std::endl;
        return "No Data.";
    }

    return content;
}

int main(int argc, char* argv[]) {
    std::string os = getOperatingSystem();

    if (argc < 2) {
        std::cout << "Usage: installer <zip archive>" << os << std::endl;
        return 1;
    }

    int err = 0;

    std::string manifest_str = loadManifest(argv[1]);
    nlohmann::json manifest = nlohmann::json::parse(manifest_str);
    auto p = manifest.get<Package>();

    std::cout << "Are you sure you want to install: " << p.name << '\n';
    std::cout << GREEN << "  Author: " << WHITE << p.author << '\n';
    std::cout << GREEN << "  Version: " << WHITE << p.version << '\n';
    std::cout << GREEN << "  Description: " << WHITE << p.description << '\n';
    std::cout << GREEN << "  Source URL: " << WHITE << p.url << '\n';
    std::cout << GREEN << "  License: "<< WHITE << p.license << std::endl;

    std::cout << WHITE << "Install? [Y/n]: ";


    return 0;
}
