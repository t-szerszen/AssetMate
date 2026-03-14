#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <map>
#include <string>

/**
 * @brief Klasa Validator
 * Klasa typu Utility (narzędziowa), odpowiedzialna za weryfikację poprawności danych
 * przesyłanych z interfejsu użytkownika do silników obliczeniowych.
 * * Separacja walidacji od logiki biznesowej pozwala na zachowanie czystości kodu (Clean Architecture).
 */
class Validator
{
public:
    /**
     * @brief Walidacja danych dla kalkulatora oszczędności.
     * @param data Mapa zawierająca klucze: "goalValue", "monthlyWage", "months".
     * @return true jeśli dane są kompletne i logicznie poprawne, false w przeciwnym razie.
     */
    static bool validateSavingData(const std::map<std::string, double>& data);

    /**
     * @brief Walidacja danych dla kalkulatora kredytowego.
     * @param data Mapa zawierająca klucze: "loanAmount", "months".
     * @return true jeśli kwota i czas są dodatnie, false jeśli dane są błędne lub niekompletne.
     */
    static bool validateMortgageData(const std::map<std::string, double>& data);
};

#endif // VALIDATOR_H
