#include "savingengine.h"

/**
 * Konstruktor silnika.
 * W obecnej architekturze jest pusty, ponieważ silnik nie wymaga
 * wstępnej konfiguracji ani połączenia z bazą danych.
 */
SavingEngine::SavingEngine() {
    // Pusty konstruktor - silnik nie potrzebuje początkowego stanu
}

/**
 * Metoda: calcMonthlyWage
 * Wykonuje proste dzielenie celu przez liczbę miesięcy.
 */
double SavingEngine::calcMonthlyWage(double goalValue, int months) const {
    // Zabezpieczenie przed błędem "division by zero" (dzielenie przez zero).
    // Jeśli użytkownik poda 0 miesięcy, funkcja zwraca 0.0 zamiast wywołać błąd krytyczny.
    if (months <= 0) return 0.0;

    // Zwraca iloraz kwoty przez czas.
    return goalValue / months;
}

/**
 * Metoda: calcMonths
 * Wykonuje obliczenie czasu na podstawie deklarowanej wpłaty.
 */
double SavingEngine::calcMonths(double goalValue, double monthlyWage) const {
    // Zabezpieczenie przed dzieleniem przez zero oraz wpłatami ujemnymi.
    // Jeśli wpłata wynosi 0, czas potrzebny na osiągnięcie celu byłby nieskończony.
    if (monthlyWage <= 0) return 0.0;

    // Zwraca iloraz celu przez miesięczną wpłatę.
    return goalValue / monthlyWage;
}
