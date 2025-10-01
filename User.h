#pragma once
#include <string>
#include <map>


class User {
public:
    std::string id;
    std::string name;
    std::string email;
    std::map<std::string, std::string> userPrefs; // key/value
    std::string defaultCurrency = "USD";


    // Methods
    void encryptData(const std::string & /*passphrase*/);
    std::string backup() const;
    bool restore(const std::string & /*backupId*/);
};


inline void User::encryptData(const std::string & /*passphrase*/) { /* placeholder */ }
inline std::string User::backup() const { return "backup-id-placeholder"; }
inline bool User::restore(const std::string & /*backupId*/) { return true; }