#include <zip.h>
#include <iostream>

std::string loadCompressedData() {
    int err = 0;

    zip *a = zip_open("package.zip", 0, &err);
    if (a == nullptr) {
        std::cerr << "Error opening zip archive: " << err << std::endl;
        return "";
    }

    struct zip_stat st;
    zip_stat_init(&st);
    if (zip_stat(a, "manifest.json", 0, &st) != 0) {
        zip_close(a);
        std::cerr << "Error reading manifest.json: " << err << std::endl;
        return "";
    }


}

std::string Xenia::AssetManager::loadCompressedData(std::string filename) {
    int err = 0;

    zip *a = zip_open(resPack, 0, &err);
    if (a == nullptr) {
        Xenia::Logger::logError("Failed to open Resources!", 1);
    }

    struct zip_stat st;
    zip_stat_init(&st);
    if (zip_stat(a, filename.c_str(), 0, &st) != 0) {
        zip_close(a);
        Xenia::Logger::logError( "Failed to find file details in archive, looked for: " + filename, 1);
    }

    zip_file *file = zip_fopen(a, filename.c_str(), 0);
    if (file == nullptr) {
        Xenia::Logger::logError("Failed to find file in archive", 1);
    }

    std::vector<char> buffer(st.size);

    zip_int64_t bytesRead = zip_fread(file, buffer.data(), st.size);
    if (bytesRead < 0 || (zip_uint64_t)bytesRead != st.size) {
        zip_fclose(file);
        zip_close(a);
        Xenia::Logger::logError("Failed to extract data", 1);
    }

    std::string content(buffer.begin(), buffer.end());

    zip_fclose(file);
    zip_close(a);

    return content;
}

int main(int argc, char* argv[]) {

}