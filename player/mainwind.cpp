#include "mainwind.h"
#include "ui_mainwind.h"

MainWind::MainWind(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWind)
{
    ui->setupUi(this);
    //InitUI();
    //InitSignalsAndSlots();
    //InitPlayer();
}

MainWind::~MainWind()
{
    delete ui;
}

