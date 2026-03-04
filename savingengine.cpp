#include "savingengine.h"

SavingEngine::SavingEngine() {
    // Pusty konstruktor - silnik nie potrzebuje początkowego stanu
}

// Definicje metod poza konstruktorem!
double SavingEngine::calcMonthlyWage(double goalValue, int months) const {
    if (months <= 0) return 0.0; // Zabezpieczenie przed dzieleniem przez zero
    return goalValue / months;
}

double SavingEngine::calcMonths(double goalValue, double monthlyWage) const {
    if (monthlyWage <= 0) return 0.0;
    return goalValue / monthlyWage;
}
