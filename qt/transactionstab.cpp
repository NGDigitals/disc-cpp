#include "transactionstab.h"
#include "ui_transactionstab.h"

TransactionsTab::TransactionsTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransactionsTab)
{
    ui->setupUi(this);
}

TransactionsTab::~TransactionsTab()
{
    delete ui;
}
