#include "assetmate.h"
#include "ui_assetmate.h"
#include "validator.h"
#include "savingengine.h"
#include "mortgageengine.h"

AssetMate::AssetMate(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AssetMate)
{
    ui->setupUi(this);
}

AssetMate::~AssetMate()
{
    delete ui;
}

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// Karta oszczędności
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void AssetMate::on_calcSavingsButton_clicked()
{
    SavingEngine savingEngine;
    Validator validator;

    // Pobranie danych z UI
    double goalValue = ui->goalValueSpinBox->value();
    double monthlyWage = ui->wageSpinBox->value();
    double months = ui->monthsSpinBox->value();

    // 1. Tworzenie mapy danych (poprawna składnia C++)
    std::map<std::string, double> data = {
        {"goalValue", goalValue},
        {"monthlyWage", monthlyWage},
        {"months", months}
    };

    // 2. Walidacja
    if (validator.validateSavingData(data)) {
        if (months > 0) {
            // Liczymy wymaganą wpłatę na podstawie czasu
            double result = savingEngine.calcMonthlyWage(goalValue, months);
            ui->lcdNumber->display(result);

            ui->resultLabel->setText(QString("Aby zrealizować cel, musisz wpłacać %1 PLN przez %2 mies.")
                                         .arg(result, 0, 'f', 2)
                                         .arg(months));
        } else {
            // Liczymy czas na podstawie deklarowanej wpłaty
            double result = savingEngine.calcMonths(goalValue, monthlyWage);
            ui->lcdNumber->display(result);

            ui->resultLabel->setText(QString("Przy wpłatach %1 PLN, cel osiągniesz za ok. %2 mies.")
                                         .arg(monthlyWage, 0, 'f', 2)
                                         .arg(result, 0, 'f', 1));
        }
    } else {
        ui->resultLabel->setText("Błąd: Podaj cel oraz czas lub kwotę wpłaty!");
        ui->lcdNumber->display(0);
    }
}

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// Karta kredytów
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void AssetMate::on_calcMortgageButton_clicked()
{
    MortgageEngine mortgageEngine;

    double loanAmount = ui->loanAmountSpinBox->value();
    double annualRate = ui->interestRateSpinBox->value();
    int months = ui->loanMonthsSpinBox->value();
    double overpayment = ui->overpaymentSpinBox->value();

    if (loanAmount <= 0 || months <= 0) {
        ui->mortgageResultLabel->setText("Błąd: Wprowadź poprawną kwotę i okres kredytowania.");
        return;
    }

    // Obliczamy wariant podstawowy
    MortgageResult baseResult = mortgageEngine.calculateEqualInstallments(loanAmount, annualRate, months);

    QString reportText;

    // Jeśli użytkownik zadeklarował nadpłatę, odpalamy magię
    if (overpayment > 0.0) {
        MortgageResult overpayResult = mortgageEngine.calculateWithOverpayment(loanAmount, annualRate, months, overpayment);

        // Obliczamy oszczędności!
        double savedInterest = baseResult.totalInterest - overpayResult.totalInterest;
        int savedMonths = baseResult.actualMonths - overpayResult.actualMonths;
        int savedYears = savedMonths / 12;
        int remainingMonths = savedMonths % 12;

        QString savedTimeStr = QString("%1 lat i %2 mies.").arg(savedYears).arg(remainingMonths);
        if (savedYears == 0) savedTimeStr = QString("%1 mies.").arg(remainingMonths);

        reportText = QString(
                         "<b>Rata bazowa:</b> %1 PLN | <b>Nadpłata:</b> %2 PLN<br><br>"
                         "<span style='color:#10b981; font-size:16px; font-weight:bold;'>"
                         "🎉 Oszczędzasz na odsetkach: %3 PLN!</span><br>"
                         "<span style='color:#10b981; font-weight:bold;'>"
                         "⏳ Spłacisz kredyt szybciej o: %4</span><br><br>"
                         "<span style='color:#4a5568; font-size:12px;'>"
                         "Nowy całkowity koszt odsetek: %5 PLN<br>"
                         "Kredyt zniknie po: %6 miesiącach (zamiast %7)"
                         "</span>"
                         ).arg(baseResult.firstInstallment, 0, 'f', 2)
                         .arg(overpayment, 0, 'f', 2)
                         .arg(savedInterest, 0, 'f', 2)
                         .arg(savedTimeStr)
                         .arg(overpayResult.totalInterest, 0, 'f', 2)
                         .arg(overpayResult.actualMonths)
                         .arg(baseResult.actualMonths);

    } else {
        // Zwykły raport (bez nadpłaty)
        reportText = QString(
                         "<b>Miesięczna rata:</b> <span style='color:#3d5afe; font-size:16px;'>%1 PLN</span><br>"
                         "Całkowity koszt odsetek: %2 PLN<br>"
                         "Całkowita kwota do spłaty: %3 PLN"
                         ).arg(baseResult.firstInstallment, 0, 'f', 2)
                         .arg(baseResult.totalInterest, 0, 'f', 2)
                         .arg(baseResult.totalCost, 0, 'f', 2);
    }

    ui->mortgageResultLabel->setText(reportText);
}

