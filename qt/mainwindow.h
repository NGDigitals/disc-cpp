#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "networktab.h"
#include "dashboardtab.h"
#include "transactionstab.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QProcess;
class QActionGroup;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createMenus();

private slots:
//    void readStandardOutput();

private:
    const QString program = QString::fromUtf8("/Users/abrahamoyelaran/Projects/sportsx/biscore/blockchain/blockchain");

    Ui::MainWindow *ui;

    QProcess *process;

    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QStatusBar *statusBar;

    NetworkTab * networkTab;
    DashboardTab *dashboardTab;
    TransactionsTab *transactionsTab;

    QMenu *fileMenu;
    QMenu *settingsMenu;
    QMenu *helpMenu;

    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *exitAction;

    void createActions();
    void createTabBars();
    void createStatusBar();

};
#endif // MAINWINDOW_H
