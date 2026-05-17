#include <iostream>
#include <stdexcept>
#include "ClientManager.h"

// =============================================================
//  main.cpp — демо на Функционалност 1: Управление на клиенти
//  Показва регистрация, редактиране, деактивиране и валидация.
// =============================================================

int main() {
    std::cout << "========================================\n";
    std::cout << "   NeoBank Pro — Управление на клиенти  \n";
    std::cout << "========================================\n\n";

    ClientManager manager;

    // ── 1. Регистрация на физическо лице ─────────────────────
    std::cout << ">> Регистрация на физическо лице...\n";
    IndividualClient* ivan = nullptr;
    try {
        ivan = manager.registerIndividual(
            "Иван Петров",
            "ivan@example.com",
            "+359888123456",
            "9001011234",   // ЕГН — 10 цифри
            "1990-01-01",   // дата на раждане
            "ул. Витоша 5, София"
        );
        std::cout << "Успешно регистриран: " << ivan->getName()
                  << " (ID: " << ivan->getId() << ", Възраст: " << ivan->getAge() << ")\n\n";
    } catch (const std::exception& e) {
        std::cerr << "Грешка: " << e.what() << "\n\n";
    }

    // ── 2. Регистрация на юридическо лице ────────────────────
    std::cout << ">> Регистрация на юридическо лице...\n";
    CorporateClient* firma = nullptr;
    try {
        firma = manager.registerCorporate(
            "Иван Петров (представляващ)",
            "office@techbg.com",
            "+35929123456",
            "123456789",         // ЕИК — 9 цифри
            "TechBG ЕООД",
            "BG123456789",       // ДДС номер
            "Иван Петров"
        );
        std::cout << "Успешно регистрирана: " << firma->getCompanyName()
                  << " (ID: " << firma->getId()
                  << ", ДДС: " << (firma->isVatRegistered() ? "Да" : "Не") << ")\n\n";
    } catch (const std::exception& e) {
        std::cerr << "Грешка: " << e.what() << "\n\n";
    }

    // ── 3. Редактиране на профил ──────────────────────────────
    if (ivan) {
        std::cout << ">> Редактиране на профила на Иван...\n";
        ivan->setEmail("ivan.petrov@newmail.com");
        ivan->setAddress("бул. България 10, Пловдив");
        std::cout << "Email обновен: " << ivan->getEmail() << "\n";
        std::cout << "Адрес обновен: " << ivan->getAddress() << "\n\n";
    }

    // ── 4. Добавяне на известие (Notification) ────────────────
    if (ivan) {
        std::cout << ">> Добавяне на известие към клиента...\n";
        ivan->addNotification(Notification("N001", "Добре дошъл в NeoBank Pro!", NotificationType::GENERAL));
        std::cout << "Известия за " << ivan->getName() << ": "
                  << ivan->getNotifications().size() << "\n";
        std::cout << "Съобщение: " << ivan->getNotifications()[0].getMessage() << "\n\n";
    }

    // ── 5. Листване на всички клиенти ────────────────────────
    std::cout << ">> Всички клиенти в системата:\n";
    manager.listAll();

    // ── 6. Деактивиране на акаунт ────────────────────────────
    if (ivan) {
        std::cout << "\n>> Деактивиране на акаунта на Иван...\n";
        ivan->deactivate();
        std::cout << "Статус: " << (ivan->isActive() ? "Активен" : "Деактивиран") << "\n";

        // Опит за повторно деактивиране — очаква се грешка
        try {
            ivan->deactivate();
        } catch (const std::runtime_error& e) {
            std::cout << "Очаквана грешка: " << e.what() << "\n";
        }
    }

    // ── 7. Валидация с невалидни данни ───────────────────────
    std::cout << "\n>> Тест: регистрация с невалидно ЕГН (само 5 цифри)...\n";
    try {
        manager.registerIndividual(
            "Грешен Клиент", "bad@example.com", "+359000",
            "12345",        // невалидно ЕГН
            "2010-06-15",   // под 18 г.
            "Непозната"
        );
    } catch (const std::exception& e) {
        std::cout << "Хваната грешка: " << e.what() << "\n";
    }

    // ── 8. Намиране на клиент по ID ───────────────────────────
    if (firma) {
        std::cout << "\n>> Намиране на клиент по ID: " << firma->getId() << "\n";
        Person* found = manager.findById(firma->getId());
        if (found)
            std::cout << "Намерен: " << found->getName()
                      << " [" << found->getClientType() << "]\n";
    }

    std::cout << "\n========================================\n";
    std::cout << "   Демото приключи успешно.\n";
    std::cout << "========================================\n";
    return 0;
}