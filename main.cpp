#include "assetmate.h"
#include <QApplication>
#include <QFile>

/**
 * @brief Główna funkcja programu.
 * Inicjalizuje środowisko Qt, ładuje oprawę wizualną (CSS) i uruchamia okno główne.
 * * @param argc Liczba argumentów wiersza poleceń.
 * @param argv Tablica argumentów wiersza poleceń.
 * @return int Kod wyjścia programu (0 oznacza sukces).
 */
int main(int argc, char *argv[])
{
    // 1. Inicjalizacja obiektu aplikacji.
    // QApplication zarządza zasobami systemowymi, ustawieniami GUI i pętlą zdarzeń.
    QApplication a(argc, argv);

    // 2. Obsługa arkuszy stylów (Qt Style Sheets - QSS).
    // Ścieżka ":/style.qss" wskazuje na plik osadzony w systemie zasobów Qt (zdefiniowany w src.qrc).
    QFile styleFile(":/style.qss");

    // Próba otwarcia pliku w trybie "Tylko do odczytu".
    if(styleFile.open(QFile::ReadOnly)) {
        // Odczytanie całej zawartości pliku i konwersja na format tekstowy QString.
        // QLatin1String jest używane dla wydajności przy konwersji surowych danych.
        QString styleSheet = QLatin1String(styleFile.readAll());

        // Zastosowanie stylu do całej aplikacji.
        // Wszystkie widżety w projekcie odziedziczą te reguły wizualne.
        a.setStyleSheet(styleSheet);

        // Dobra praktyka: zamknij plik po zakończeniu odczytu.
        styleFile.close();
    }

    // 3. Utworzenie instancji okna głównego.
    // AssetMate jest zdefiniowane w plikach assetmate.h/.cpp.
    AssetMate w;

    // Wyświetlenie okna na ekranie użytkownika.
    w.show();

    // 4. Uruchomienie pętli zdarzeń (Event Loop).
    // Metoda exec() blokuje dalsze wykonywanie main() do czasu zamknięcia aplikacji.
    // Przetwarza ona kliknięcia, ruchy myszy i odświeżanie interfejsu.
    return a.exec();
}
