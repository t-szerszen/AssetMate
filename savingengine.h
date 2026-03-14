#ifndef SAVINGENGINE_H
#define SAVINGENGINE_H

/**
 * @brief Klasa SavingEngine
 * Odpowiada za czystą logikę matematyczną związaną z planowaniem oszczędności.
 * Silnik jest zaprojektowany jako "stateless" (bezstanowy), co oznacza, że nie przechowuje
 * danych użytkownika, a jedynie przetwarza parametry wejściowe i zwraca wynik.
 */
class SavingEngine
{
public:
    /**
     * @brief Konstruktor domyślny.
     */
    SavingEngine();

    /**
     * @brief Oblicza wymaganą miesięczną wpłatę, aby osiągnąć cel w danym czasie.
     * @param goalValue Kwota docelowa (np. 10 000 PLN).
     * @param months Czas oszczędzania w miesiącach.
     * @return Wymagana miesięczna kwota wpłaty.
     */
    double calcMonthlyWage(double goalValue, int months) const;

    /**
     * @brief Oblicza czas potrzebny na osiągnięcie celu przy stałej miesięcznej wpłacie.
     * @param goalValue Kwota docelowa (np. 10 000 PLN).
     * @param monthlyWage Deklarowana miesięczna wpłata.
     * @return Przybliżona liczba miesięcy potrzebna na uzbieranie kwoty.
     */
    double calcMonths(double goalValue, double monthlyWage) const;
};

#endif // SAVINGENGINE_H
