#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStyle>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QScreen>
#include <QMenuBar>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("BISC-MASTER");

    //createMenus();
    createTabBars();
    createStatusBar();

    setMinimumSize(1200, 650);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter, size(), QGuiApplication::primaryScreen()->geometry()));

//    process = new QProcess;
//    process->start(program);

    //connect(process, SIGNAL(readyReadStandardOutput()),this, SLOT(this->readStandardOuput()));
    //connect(process, SIGNAL(readyReadStandardError()), this, SLOT(this->readStandardOuput()));
}

//void MainWindow::readStandardOutput(){
//    QByteArray stdout = process->readAllStandardOutput();
//    QByteArray stderr = process->readAllStandardError();
//    if(stdout.size()){
//        printf(stdout);
//    }
//    if(stderr.size()){
//        printf(stdout);
//    }
//}

//void MainWindow::createMenus(){
//    fileMenu = menuBar()->addMenu(tr("&File"));
//    settingsMenu = menuBar()->addMenu(tr("&Settings"));
//    helpMenu = menuBar()->addMenu(tr("&Help"));
//    menuBar()->setNativeMenuBar(false);
//}

void MainWindow::createTabBars(){
    centralwidget = new QWidget();

    tabWidget = new QTabWidget(centralwidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setDocumentMode(false);
    tabWidget->setTabPosition(QTabWidget::North);
    tabWidget->setStyleSheet("QTabWidget::pane{alignment:center;}\n"
            "QTabWidget::tab-bar{}\n"
            "QTabBar::tab{font:12pt;font-weight:bold;}\n");

    dashboardTab = new DashboardTab();
    dashboardTab->setObjectName(QString::fromUtf8("dashboardTab"));
    tabWidget->addTab(dashboardTab, QString::fromUtf8("Dashboard"));

    networkTab = new NetworkTab();
    networkTab->setObjectName(QString::fromUtf8("networkTab"));
    tabWidget->addTab(networkTab, QString::fromUtf8("Network"));

    transactionsTab = new TransactionsTab();
    transactionsTab->setObjectName(QString::fromUtf8("transactionsTab"));
    tabWidget->addTab(transactionsTab, QString::fromUtf8("Transactions"));
    tabWidget->setCurrentIndex(1);

    transactionsTab = new TransactionsTab();
    transactionsTab->setObjectName(QString::fromUtf8("settingsTab"));
    tabWidget->addTab(transactionsTab, QString::fromUtf8("Settings"));
    setCentralWidget(tabWidget);
}

void MainWindow::createStatusBar(){
    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    statusBar->showMessage("Ready");
    setStatusBar(statusBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

