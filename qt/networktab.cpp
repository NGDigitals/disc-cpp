#include "networktab.h"
#include "ui_networktab.h"
#include "newnodedialog.h"

#include "constant.h"

#include <QtGui>
#include <QtCore>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>

const int BTN_WIDTH = 70;
const int BTN_HEIGHT = 40;

NetworkTab::NetworkTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkTab)
{
    ui->setupUi(this);

    createTable();
    createButtons();
    createLayouts();

//    process = new QProcess();
//    process->start(constant::PROGRAM);

    connect ( addBtn, SIGNAL( clicked() ), this, SLOT( addNewNode() ) );
    connect ( process, SIGNAL( readyReadStandardOutput() ), this, SLOT( readStdOut() ) );
    connect ( process, SIGNAL( readyReadStandardError() ), this, SLOT( readStdOut() ) );
}

void NetworkTab::createTable(){
    tableWidget = new QTableWidget(this);
    QStringList tableHeader;
    tableHeader << QString::fromUtf8("Name") << QString::fromUtf8("Address") << QString::fromUtf8("Number of Blocks") <<
                   QString::fromUtf8("Last Block Time") << QString::fromUtf8("Ping Time") << QString::fromUtf8("Date Modified");
    tableWidget->setColumnCount(tableHeader.length());
    tableWidget->setHorizontalHeaderLabels(tableHeader);
    //tableWidget->horizontalHeader()-> ->setVisible(false);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //tableWidget->setShowGrid(true);
    tableWidget->setStyleSheet("QTableView {background-color: #666666;}\n"
                               "QHeaderView::section {border:1px solid #FBFBFB;height:32px;font-weight:bold;font:13pt;}");
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setRowCount(10);
    //ui->tableWidget->setColumnCount(numColumns);
    for (auto r=0; r<10; r++)
         for (auto c=0; c<tableWidget->rowCount(); c++)
              tableWidget->setItem( r, c, new QTableWidgetItem("Testing"));
}

void NetworkTab::createButtons(){
    addBtn = new QPushButton(QString::fromUtf8("New"));
    //addButton->setIcon(QIcon(":/icons/ic_plus_thick_green.png"));
    //addButton->setIconSize(QSize(36, 36));
    addBtn->setFixedSize(QSize(BTN_WIDTH, BTN_HEIGHT));
    //addButton->setCursor(Qt::OpenHandCursor);
    addBtn->setStyleSheet("border:3px solid #FBFBFB;border-radius:6px;");

//    uploadButton = new QPushButton();
//    uploadButton->setIcon(QIcon(":/icons/ic_arrow_up_thick_blue.png"));
//    uploadButton->setIconSize(QSize(36, 36));
//    uploadButton->setMaximumSize(QSize(46, 46));

//    downloadButton = new QPushButton();
//    downloadButton->setIcon(QIcon(":/icons/ic_arrow_down_thick_blue.png"));
//    downloadButton->setIconSize(QSize(36, 36));
//    downloadButton->setMaximumSize(QSize(46, 46));

    syncBtn = new QPushButton(QString::fromUtf8("Sync"));
    //syncButton->setIcon(QIcon(":/icons/ic_sync_light.png"));
    //syncButton->setIconSize(QSize(200, 100));
    syncBtn->setFixedSize(QSize(BTN_WIDTH, BTN_HEIGHT));
    syncBtn->setStyleSheet("border:3px solid #FBFBFB;border-radius: 6px;");
    //syncButton->setMaximumSize(QSize(230, addButton->size().height()));
}

void NetworkTab::createLayouts(){
    //QHBoxLayout *btnLeftLayout = new QHBoxLayout();
    //btnLeftLayout->addWidget(addButton);
    //btnLeftLayout->setAlignment(addButton, Qt::AlignLeft);
//    QHBoxLayout *btnMiddleLayout = new QHBoxLayout();
//    btnMiddleLayout->addWidget(uploadButton);
//    btnMiddleLayout->setAlignment(uploadButton, Qt::AlignCenter);
//    btnMiddleLayout->addWidget(downloadButton);
//    btnMiddleLayout->setAlignment(downloadButton, Qt::AlignCenter);
//    QHBoxLayout *btnRightLayout = new QHBoxLayout();
//    btnRightLayout->addWidget(syncButton);
//    btnRightLayout->setAlignment(syncButton, Qt::AlignRight);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    //btnLayout->addLayout(btnLeftLayout);
    //btnLayout->addLayout(btnMiddleLayout);
    //btnLayout->addLayout(btnRightLayout);
    //btnLayout->insertWidget(1, addButton, 0, Qt::AlignLeft);
    //btnLayout->insertWidget(2, syncButton, 0, Qt::AlignRight);
    btnLayout->addWidget(addBtn);
    btnLayout->addSpacerItem(new QSpacerItem(100, 1, QSizePolicy::Expanding));
    btnLayout->addWidget(syncBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tableWidget);
    mainLayout->addLayout(btnLayout);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setLayout(mainLayout);
}

void NetworkTab::readStdOut(){
    QByteArray stdout = process->readAllStandardOutput();
    QByteArray stderr = process->readAllStandardError();
    if(stdout.size()){
        printf(stdout);
    }
    if(stderr.size()){
        printf(stdout);
    }
}

void NetworkTab::addNewNode(){
    NewNodeDialog *nodeDialog = new NewNodeDialog(this);
    nodeDialog->exec();
}

NetworkTab::~NetworkTab()
{
    delete ui;
}
