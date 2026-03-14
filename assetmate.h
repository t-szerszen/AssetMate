#ifndef ASSETMATE_H
#define ASSETMATE_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include "mortgageengine.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class AssetMate;
}
QT_END_NAMESPACE

/**
 * @brief Klasa AssetMate
 * Główny kontroler aplikacji (warstwa Controller w wzorcu MVC).
 * Zarządza interakcją z użytkownikiem, przekazuje dane do silników (Model)
 * oraz aktualizuje widżety i wykresy (View).
 */
class AssetMate : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor okna głównego.
     * Inicjalizuje interfejs użytkownika zdefiniowany w pliku .ui.
     */
    AssetMate(QWidget *parent = nullptr);
    ~AssetMate();

private slots:
    /**
     * @brief Obsługa kliknięcia przycisku obliczeń w karcie oszczędności.
     */
    void on_calcSavingsButton_clicked();

    /**
     * @brief Obsługa kliknięcia przycisku obliczeń w karcie kredytów.
     */
    void on_calcMortgageButton_clicked();

    /**
     * @brief Metoda aktualizująca wykres spłaty kapitału.
     * @param base Wyniki dla kredytu bez nadpłat.
     * @param overpay Wyniki dla kredytu z nadpłatami.
     */
    void updateMortgageChart(const MortgageResult& base, const MortgageResult& overpay);

private:
    Ui::AssetMate *ui; // Wskaźnik na wygenerowaną klasę interfejsu.

    /**
     * @brief Pomocnicza metoda formatująca raport tekstowy w formacie HTML.
     */
    QString formatMortgageReport(const MortgageResult& base, const MortgageResult& overpay, double overpayment);
};
#endif // ASSETMATE_H
