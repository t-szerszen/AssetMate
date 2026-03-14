#ifndef MORTGAGEENGINE_H
#define MORTGAGEENGINE_H

#include <vector>

/**
 * @brief Struktura przechowująca kompleksowe wyniki kalkulacji kredytowej.
 */
struct MortgageResult {
    double firstInstallment {0.0};    // Kwota pierwszej (bazowej) raty równej.
    double totalInterest {0.0};       // Suma wszystkich zapłaconych odsetek.
    double totalCost {0.0};           // Całkowity koszt (kapitał + odsetki).
    int actualMonths {0};             // Rzeczywista liczba miesięcy do spłaty (istotne przy nadpłatach).
    std::vector<double> principalHistory; // Historia salda zadłużenia miesiąc po miesiącu (do wykresu).
};

/**
 * @brief Klasa MortgageEngine
 * Silnik obliczeniowy realizujący matematykę bankową. Klasa jest bezstanowa,
 * co ułatwia jej testowanie i integrację z różnymi widokami.
 */
class MortgageEngine
{
public:
    MortgageEngine();

    /**
     * @brief Oblicza standardowy kredyt o ratach równych (anuitet).
     */
    MortgageResult calculateEqualInstallments(double loanAmount, double annualRate, int months) const;

    /**
     * @brief Symuluje spłatę kredytu z uwzględnieniem stałej miesięcznej nadpłaty.
     */
    MortgageResult calculateWithOverpayment(double loanAmount, double annualRate, int months, double monthlyOverpayment) const;
};

#endif // MORTGAGEENGINE_H
