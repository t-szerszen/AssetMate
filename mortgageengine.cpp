#include "mortgageengine.h"
#include <cmath>

MortgageEngine::MortgageEngine() {
    // Konstruktor bezstanowy - silnik nie przechowuje danych wewnętrznych.
}

/**
 * Obliczanie raty równej przy użyciu standardowego wzoru na anuitet.
 * Wzór: R = P * r * (1+r)^n / ((1+r)^n - 1)
 * Gdzie: P = kapitał, r = stopa miesięczna, n = liczba rat.
 */
MortgageResult MortgageEngine::calculateEqualInstallments(double loanAmount, double annualRate, int months) const {
    MortgageResult result;

    // 1. Konwersja oprocentowania rocznego (np. 8%) na stopę miesięczną (dziesiętną).
    double monthlyRate = (annualRate / 100.0) / 12.0;

    // 2. Obliczenie raty równej (kapitałowo-odsetkowej) przy pomocy funkcji potęgowej pow().
    double power = std::pow(1.0 + monthlyRate, months);
    result.firstInstallment = loanAmount * monthlyRate * power / (power - 1.0);

    // 3. Podstawowe statystyki dla planu standardowego.
    result.totalCost = result.firstInstallment * months;
    result.totalInterest = result.totalCost - loanAmount;
    result.actualMonths = months;

    // 4. Generowanie punktów dla wykresu - symulacja spadku kapitału.
    double remaining = loanAmount;
    result.principalHistory.push_back(remaining); // Stan początkowy
    for (int i = 0; i < months; ++i) {
        double interestPart = remaining * monthlyRate;      // Odsetki w tym miesiącu
        double capitalPart = result.firstInstallment - interestPart; // Spłacony kapitał
        remaining -= capitalPart;
        result.principalHistory.push_back(remaining > 0 ? remaining : 0);
    }

    return result;
}

/**
 * Symulacja spłaty z nadpłatą miesiąc po miesiącu.
 * Ta metoda nie używa jednego wzoru, lecz pętli odwzorowującej zachowanie banku.
 */
MortgageResult MortgageEngine::calculateWithOverpayment(double loanAmount, double annualRate, int months, double monthlyOverpayment) const {
    MortgageResult result;
    if (loanAmount <= 0 || months <= 0) return result;

    // 1. Obliczenie raty bazowej, którą i tak musimy płacić wg umowy.
    double monthlyRate = (annualRate / 100.0) / 12.0;
    double power = std::pow(1.0 + monthlyRate, months);
    double baseInstallment = loanAmount * monthlyRate * power / (power - 1.0);

    result.firstInstallment = baseInstallment; // Zapamiętujemy ratę z umowy.

    double remainingCapital = loanAmount;
    result.totalInterest = 0.0;
    result.actualMonths = 0;
    result.principalHistory.push_back(loanAmount);

    // 2. Pętla symulacyjna - działa dopóki kapitał nie spadnie do zera.
    while (remainingCapital > 0.0) {
        result.actualMonths++;

        // Obliczamy odsetki od aktualnego stanu zadłużenia.
        double currentInterest = remainingCapital * monthlyRate;
        result.totalInterest += currentInterest;

        // Klucz logiki: Spłacamy część kapitałową raty bazowej + całą zadeklarowaną nadpłatę.
        double capitalPayment = (baseInstallment - currentInterest) + monthlyOverpayment;

        // Zabezpieczenie dla ostatniego miesiąca, aby nie spłacić więcej niż zostało długu.
        if (capitalPayment > remainingCapital) {
            capitalPayment = remainingCapital;
        }

        remainingCapital -= capitalPayment;

        // Zapisujemy saldo po spłacie do historii wykresu.
        result.principalHistory.push_back(remainingCapital > 0 ? remainingCapital : 0);

        // Zabezpieczenie inżynierskie przed nieskończoną pętlą (np. przy błędnych danych).
        if (result.actualMonths > months * 2) break;
    }

    // 3. Obliczenie całkowitego kosztu po uwzględnieniu oszczędności z nadpłat.
    result.totalCost = loanAmount + result.totalInterest;
    return result;
}
