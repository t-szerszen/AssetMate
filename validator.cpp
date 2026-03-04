#include "validator.h"

bool Validator::validateSavingData(const std::map<std::string, double>& data) {
    // Sprawdzamy czy cel jest podany (zakładamy, że mapa zawsze ma te klucze,
    // ale sprawdzamy ich wartości)

    double goal = data.at("goalValue");
    double wage = data.at("monthlyWage");
    double months = data.at("months");

    // Logika: Cel musi być > 0
    if (goal <= 0) {
        return false;
    }

    // Musi być podane co najmniej jedno z dwóch: wage lub months
    if (wage <= 0 && months <= 0) {
        return false;
    }

    return true;
}
