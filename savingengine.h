// AssetMate/savingengine.h
#ifndef SAVINGENGINE_H
#define SAVINGENGINE_H

class SavingEngine
{
public:
    SavingEngine();

    // Metody operują na zewnętrznym modelu danych, zwracając wynik
    double calcMonthlyWage(double goalValue, int months) const;
    double calcMonths(double goalValue, double monthlyWage) const;
};

#endif // SAVINGENGINE_H
