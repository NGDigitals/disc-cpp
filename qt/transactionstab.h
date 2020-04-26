#ifndef TRANSACTIONSTAB_H
#define TRANSACTIONSTAB_H

#include <QWidget>

namespace Ui {
class TransactionsTab;
}

class TransactionsTab : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionsTab(QWidget *parent = nullptr);
    ~TransactionsTab();

private:
    Ui::TransactionsTab *ui;
};

#endif // TRANSACTIONSTAB_H
