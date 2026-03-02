#include "assetmate.h"
#include "ui_assetmate.h"

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
