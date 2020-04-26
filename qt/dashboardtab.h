#ifndef DASHBOARDTAB_H
#define DASHBOARDTAB_H

#include <QWidget>

namespace Ui {
class DashboardTab;
}

class DashboardTab : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardTab(QWidget *parent = nullptr);
    ~DashboardTab();

private:
    Ui::DashboardTab *ui;
};

#endif // DASHBOARDTAB_H
