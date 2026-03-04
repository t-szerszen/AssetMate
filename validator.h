// AssetMate/validator.h
#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <map>
#include <string>

class Validator
{
public:
    // Metoda statyczna - można ją wywołać bez tworzenia obiektu
    static bool validateSavingData(const std::map<std::string, double>& data);
};

#endif // VALIDATOR_H
