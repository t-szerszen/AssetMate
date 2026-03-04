#ifndef MORTGAGEENGINE_H
#define MORTGAGEENGINE_H

struct MortgageResult {
    double firstInstallment {0.0};
    double totalInterest {0.0};
    double totalCost {0.0};
    int actualMonths {0}; // NOWE: Rzeczywisty czas spłaty
};

class MortgageEngine
{
public:
    MortgageEngine();

    MortgageResult calculateEqualInstallments(double loanAmount, double annualRate, int months) const;
    MortgageResult calculateDecreasingInstallments(double loanAmount, double annualRate, int months) const;
    MortgageResult calculateWithOverpayment(double loanAmount, double annualRate, int months, double monthlyOverpayment) const;
};

#endif // MORTGAGEENGINE_H
