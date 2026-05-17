#pragma once
#include <string>
#include <ctime>

enum class NotificationType {
    LIMIT_REACHED,
    LOW_BALANCE,
    ACCOUNT_LOCKED,
    GENERAL
};

class Notification {
private:
    std::string id;
    std::string message;
    bool isRead;
    std::time_t timestamp;
    NotificationType type;

public:
    Notification(const std::string& id, const std::string& message, NotificationType type)
        : id(id), message(message), isRead(false), timestamp(std::time(nullptr)), type(type) {}

    void markAsRead() { isRead = true; }

    std::string getMessage() const { return message; }
    NotificationType getType() const { return type; }
    std::string getId() const { return id; }
    bool getIsRead() const { return isRead; }

    std::string typeToString() const {
        switch (type) {
            case NotificationType::LIMIT_REACHED:  return "LIMIT_REACHED";
            case NotificationType::LOW_BALANCE:    return "LOW_BALANCE";
            case NotificationType::ACCOUNT_LOCKED: return "ACCOUNT_LOCKED";
            case NotificationType::GENERAL:        return "GENERAL";
            default:                               return "UNKNOWN";
        }
    }
};