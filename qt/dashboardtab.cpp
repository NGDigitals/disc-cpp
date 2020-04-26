#include "dashboardtab.h"
#include "ui_dashboardtab.h"

DashboardTab::DashboardTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DashboardTab)
{
    ui->setupUi(this);
}

DashboardTab::~DashboardTab()
{
    delete ui;
}
