#ifndef ASSETMATE_H
#define ASSETMATE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class AssetMate;
}
QT_END_NAMESPACE

class AssetMate : public QMainWindow
{
    Q_OBJECT

public:
    AssetMate(QWidget *parent = nullptr);
    ~AssetMate();

private slots:
    void on_calcSavingsButton_clicked();
    void on_calcMortgageButton_clicked();

private:
    Ui::AssetMate *ui;
};
#endif // ASSETMATE_H
