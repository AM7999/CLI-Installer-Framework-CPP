#ifndef INSTALLER_PACKAGE_H
#define INSTALLER_PACKAGE_H

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

struct Package {
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::string url;
    std::string license;
};

// as big as this is lowkey super helpful
namespace nlohmann {
    template<>
    struct adl_serializer<Package> {
        static void to_json(json &j, const Package &package) {
            j = json{
                {"name", package.name},
                {"version", package.version},
                {"author", package.author},
                {"description", package.description},
                {"url", package.url},
                {"license", package.license}
            };
        }
        static void from_json(const json &j, Package &package) {
            try {
                package.name = j.at("name").get<std::string>();
                package.version = j.at("version").get<std::string>();
                package.author = j.at("author").get<std::string>();
                package.description = j.at("description").get<std::string>();
                package.url = j.at("url").get<std::string>();
                package.license = j.at("license").get<std::string>();
            }
            catch (std::exception &e) {
                std::cerr << e.what() << '\n';
            }
        }
    };
}

#endif
