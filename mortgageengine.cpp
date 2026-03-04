#include "mortgageengine.h"
#include <cmath>

MortgageEngine::MortgageEngine() {
    // Silnik jest bezstanowy (stateless) - brak inicjalizacji pól
}

MortgageResult MortgageEngine::calculateEqualInstallments(double loanAmount, double annualRate, int months) const {
    MortgageResult result;
    if (loanAmount <= 0 || months <= 0) return result;

    if (annualRate == 0.0) {
        result.firstInstallment = loanAmount / months;
        result.totalCost = loanAmount;
        return result;
    }

    // Oprocentowanie miesięczne (np. 8% w skali roku to 0.08 / 12)
    double monthlyRate = (annualRate / 100.0) / 12.0;

    // Matematyczny wzór na ratę równą
    double power = std::pow(1.0 + monthlyRate, months);
    result.firstInstallment = loanAmount * monthlyRate * power / (power - 1.0);

    result.totalCost = result.firstInstallment * months;
    result.totalInterest = result.totalCost - loanAmount;

    return result;
}

MortgageResult MortgageEngine::calculateDecreasingInstallments(double loanAmount, double annualRate, int months) const {
    MortgageResult result;
    if (loanAmount <= 0 || months <= 0) return result;

    double capitalPart = loanAmount / months; // Część kapitałowa jest stała
    double monthlyRate = (annualRate / 100.0) / 12.0;

    double remainingCapital = loanAmount;
    result.totalInterest = 0.0;

    // Symulacja miesiąc po miesiącu - najlepsza metoda dla rat malejących
    for (int i = 0; i < months; ++i) {
        double currentInterest = remainingCapital * monthlyRate;
        result.totalInterest += currentInterest;

        // Zapisujemy pierwszą ratę, by pokazać ją użytkownikowi w UI
        if (i == 0) {
            result.firstInstallment = capitalPart + currentInterest;
        }

        remainingCapital -= capitalPart;
    }

    result.totalCost = loanAmount + result.totalInterest;

    return result;
}

MortgageResult MortgageEngine::calculateWithOverpayment(double loanAmount, double annualRate, int months, double monthlyOverpayment) const {
    MortgageResult result;
    if (loanAmount <= 0 || months <= 0) return result;

    // Najpierw obliczamy bazową ratę równą (wymaganą przez bank)
    double monthlyRate = (annualRate / 100.0) / 12.0;
    double power = std::pow(1.0 + monthlyRate, months);
    double baseInstallment = loanAmount * monthlyRate * power / (power - 1.0);

    result.firstInstallment = baseInstallment; // Rata z umowy (bez nadpłaty)

    double remainingCapital = loanAmount;
    result.totalInterest = 0.0;
    result.actualMonths = 0;

    // Symulacja miesiąc po miesiącu
    while (remainingCapital > 0.0) {
        result.actualMonths++;

        // Odsetki za dany miesiąc od pozostałego kapitału
        double currentInterest = remainingCapital * monthlyRate;
        result.totalInterest += currentInterest;

        // Ile kapitału spłacamy w tym miesiącu? (Rata bazowa - odsetki + nasza stówka)
        double capitalPayment = (baseInstallment - currentInterest) + monthlyOverpayment;

        // Ostatni miesiąc - upewniamy się, że nie spłacimy więcej niż zostało
        if (capitalPayment > remainingCapital) {
            capitalPayment = remainingCapital;
        }

        remainingCapital -= capitalPayment;

        // Zabezpieczenie przed nieskończoną pętlą (w idealnym świecie niepotrzebne, ale to dobra praktyka inżynierska)
        if (result.actualMonths > months * 2) break;
    }

    result.totalCost = loanAmount + result.totalInterest;
    return result;
}
