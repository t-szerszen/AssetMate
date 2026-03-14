#include "validator.h"

/**
 * Metoda sprawdza, czy użytkownik podał cel oszczędnościowy oraz
 * parametry pozwalające na wykonanie obliczeń (czas lub kwotę wpłaty).
 */
bool Validator::validateSavingData(const std::map<std::string, double>& data) {
    // Pobranie wartości z mapy przy użyciu metody .at(), która rzuca wyjątek przy braku klucza.
    // Mapy są przekazywane przez stałą referencję, co optymalizuje zużycie pamięci.
    double goal = data.at("goalValue");
    double wage = data.at("monthlyWage");
    double months = data.at("months");

    // Warunek 1: Cel oszczędnościowy musi być wartością dodatnią.
    // Bez celu finansowego algorytm nie ma punktu odniesienia.
    if (goal <= 0) {
        return false;
    }

    // Warunek 2: Logika "Dostępności danych".
    // Aby obliczyć plan, musimy znać albo deklarowaną wpłatę (wage), albo czas (months).
    // Jeśli oba parametry są zerowe lub ujemne, obliczenia są niemożliwe.
    if (wage <= 0 && months <= 0) {
        return false;
    }

    // Jeśli wszystkie powyższe testy przeszły pomyślnie, dane są poprawne.
    return true;
}

/**
 * Metoda weryfikuje dane wejściowe dla kredytu hipotecznego.
 * Wykorzystuje blok try-catch dla zwiększenia odporności aplikacji na błędy danych.
 */
bool Validator::validateMortgageData(const std::map<std::string, double>& data) {
    try {
        // Próba odczytu kluczowych parametrów kredytu.
        double loanAmount = data.at("loanAmount");
        double months = data.at("months");

        // Walidacja logiczna:
        // Kwota kredytu (loanAmount) musi być większa od zera.
        // Czas trwania kredytu (months) musi być co najmniej 1 miesiącem.
        // Zapobiega to błędom matematycznym, np. dzieleniu przez zero w wzorze na raty.
        if (loanAmount <= 0 || months <= 0) {
            return false;
        }

        return true;
    } catch (...) {
        // Blok catch przechwytuje ewentualne wyjątki (np. std::out_of_range),
        // gdyby w przekazanej mapie zabrakło któregoś z kluczy.
        // Zapewnia to, że aplikacja nie "scrashuje" przy błędnym przepływie danych.
        return false;
    }
}
