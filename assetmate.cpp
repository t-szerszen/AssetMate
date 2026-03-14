#include "assetmate.h"
#include "ui_assetmate.h"
#include "validator.h"
#include "savingengine.h"
#include "mortgageengine.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

AssetMate::AssetMate(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AssetMate)
{
    ui->setupUi(this); // Budowa interfejsu graficznego.
}

AssetMate::~AssetMate()
{
    delete ui; // Zwolnienie pamięci interfejsu.
}

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// Karta oszczędności
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void AssetMate::on_calcSavingsButton_clicked()
{
    SavingEngine savingEngine;
    Validator validator;

    // 1. Pobranie surowych danych z widżetów typu SpinBox.
    double goalValue = ui->goalValueSpinBox->value();
    double monthlyWage = ui->wageSpinBox->value();
    double months = ui->monthsSpinBox->value();

    // 2. Przygotowanie paczki danych do walidacji.
    std::map<std::string, double> data = {
        {"goalValue", goalValue},
        {"monthlyWage", monthlyWage},
        {"months", months}
    };

    // 3. Walidacja logiczna danych.
    if (validator.validateSavingData(data)) {
        if (months > 0) {
            // TRYB A: Użytkownik podał czas, liczymy wymaganą wpłatę.
            double result = savingEngine.calcMonthlyWage(goalValue, months);
            ui->lcdNumber->display(result);

            ui->resultLabel->setText(QString("Aby zrealizować cel, musisz wpłacać %1 PLN przez %2 mies.")
                                         .arg(result, 0, 'f', 2)
                                         .arg(months));
        } else {
            // TRYB B: Użytkownik podał kwotę, liczymy czas potrzebny na osiągnięcie celu.
            double result = savingEngine.calcMonths(goalValue, monthlyWage);
            ui->lcdNumber->display(result);

            ui->resultLabel->setText(QString("Przy wpłatach %1 PLN, cel osiągniesz za ok. %2 mies.")
                                         .arg(monthlyWage, 0, 'f', 2)
                                         .arg(result, 0, 'f', 1));
        }
    } else {
        // Obsługa błędnych danych wejściowych.
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

    // 1. Walidacja "na miejscu" z wykorzystaniem anonimowej mapy.
    if (!Validator::validateMortgageData({
            {"loanAmount", ui->loanAmountSpinBox->value()},
            {"months", static_cast<double>(ui->loanYearsSpinBox->value())}
        })) {
        ui->mortgageResultLabel->setText("<span style='color:#ef4444;'>Błąd: Wprowadź poprawne dane.</span>");
        return;
    }

    // 2. Pobranie parametrów kredytu.
    double loanAmount = ui->loanAmountSpinBox->value();
    double annualRate = ui->interestRateSpinBox->value();
    int years = ui->loanYearsSpinBox->value();
    double overpayment = ui->overpaymentSpinBox->value();

    // 2.1 Przeliczenie lat na miesiące:
    int months = years * 12;

    // 3. Obliczenia bazowe (bez nadpłat).
    MortgageResult base = mortgageEngine.calculateEqualInstallments(loanAmount, annualRate, months);

    // 4. Obliczenia wariantu z nadpłatą (jeśli zadeklarowana).
    MortgageResult overpay = (overpayment > 0)
                                 ? mortgageEngine.calculateWithOverpayment(loanAmount, annualRate, months, overpayment)
                                 : MortgageResult();

    // 5. Aktualizacja widoku: raport tekstowy oraz wykres.
    ui->mortgageResultLabel->setText(formatMortgageReport(base, overpay, overpayment));
    updateMortgageChart(base, overpay);
}

/**
 * Generowanie ustandaryzowanego raportu HTML.
 * Ta metoda separuje wygląd raportu od logiki przycisku.
 */
QString AssetMate::formatMortgageReport(const MortgageResult& base, const MortgageResult& overpay, double overpaymentValue)
{
    QString report = "<b>Podsumowanie kredytu:</b><br>";
    report += QString("Rata bazowa: <span style='color:#3b82f6;'>%1 PLN</span><br>").arg(base.firstInstallment, 0, 'f', 2);

    if (overpaymentValue > 0) {
        // Obliczanie różnic wynikających z nadpłaty.
        double savedInterest = base.totalInterest - overpay.totalInterest;
        int savedMonths = base.actualMonths - overpay.actualMonths;
        int years = savedMonths / 12;
        int remMonths = savedMonths % 12;
        QString timeSaved = (years > 0) ? QString("%1 lat i %2 mies.").arg(years).arg(remMonths) : QString("%1 mies.").arg(remMonths);

        report += QString("Nadpłata: <span style='color:#34d399;'>+%1 PLN/mies.</span><br><br>").arg(overpaymentValue, 0, 'f', 2);

        report += QString("<div style='background-color:#064e3b; padding:12px; border: 1px solid #059669; border-radius:6px;'>");
        report += QString("<b style='color:#6ee7b7;'>🎉 Oszczędność: %1 PLN</b><br>").arg(savedInterest, 0, 'f', 2);
        report += QString("<b style='color:#6ee7b7;'>⏳ Czas spłaty krótszy o: %1</b>").arg(timeSaved);
        report += "</div><br>";
    } else {
        // Raport dla braku nadpłaty.
        report += QString("Całkowity koszt odsetek: %1 PLN<br>").arg(base.totalInterest, 0, 'f', 2);
        report += QString("Suma wszystkich wpłat: <b>%1 PLN</b>").arg(base.totalCost, 0, 'f', 2);
    }

    return report;
}

/**
 * Wizualizacja danych przy pomocy modułu Qt Charts.
 * Przekształca wektory historii zadłużenia w serie punktów na wykresie liniowym.
 */
void AssetMate::updateMortgageChart(const MortgageResult& base, const MortgageResult& overpay) {
    auto seriesBase = new QLineSeries();
    seriesBase->setName("Plan standardowy");
    for (int i = 0; i < base.principalHistory.size(); ++i) {
        seriesBase->append(i, base.principalHistory[i]); // Dodawanie punktów (miesiąc, saldo).
    }

    auto seriesOverpay = new QLineSeries();
    seriesOverpay->setName("Z nadpłatą");
    for (int i = 0; i < overpay.principalHistory.size(); ++i) {
        seriesOverpay->append(i, overpay.principalHistory[i]);
    }

    auto chart = new QChart();
    chart->addSeries(seriesBase);
    chart->addSeries(seriesOverpay);
    chart->setTitle("Tempo spłaty kapitału");

    // Konfiguracja osi wykresu.
    chart->createDefaultAxes();
    if (!chart->axes(Qt::Horizontal).isEmpty()) {
        chart->axes(Qt::Horizontal).first()->setTitleText("Miesiące");
    }
    if (!chart->axes(Qt::Vertical).isEmpty()) {
        chart->axes(Qt::Vertical).first()->setTitleText("Pozostały kapitał (PLN)");
    }
    chart->setAnimationOptions(QChart::SeriesAnimations); // Efektowna animacja rysowania.

    // Stylizacja serii nadpłaty na kolor zielony.
    seriesOverpay->setPen(QPen(QColor("#10b981"), 3));

    // Przypisanie gotowego wykresu do widżetu QChartView w UI.
    ui->mortgageChartView->setChart(chart);
    ui->mortgageChartView->setRenderHint(QPainter::Antialiasing); // Wygładzanie krawędzi linii.
}
