#include "newnodedialog.h"
#include "ui_newnodedialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>

const int BTN_WIDTH = 120;
const int BTN_HEIGHT = 40;
const int LINE_EDIT_HEIGHT = 30;

NewNodeDialog::NewNodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewNodeDialog)
{
    ui->setupUi(this);

    setWindowTitle("Authorize Node");

    QLabel *nameLabel = new QLabel(QString::fromUtf8("Name"));
    nameEdit = new QLineEdit();
    nameEdit->setMinimumHeight(LINE_EDIT_HEIGHT);
    nameEdit->setFocus();

    QLabel *addressLabel = new QLabel(QString::fromUtf8("Address"));
    addressEdit = new QLineEdit();
    addressEdit->setMinimumHeight(LINE_EDIT_HEIGHT);
    QLabel *tokenLabel = new QLabel(QString::fromUtf8("Token"));
    tokenEdit = new QLineEdit();
    tokenEdit->setMinimumHeight(LINE_EDIT_HEIGHT);

    continueBtn = new QPushButton(QString::fromUtf8("Continue"));
    continueBtn->setFixedSize(QSize(BTN_WIDTH, BTN_HEIGHT));
    continueBtn->setStyleSheet("border:1px solid #FBFBFB;border-radius:6px;");
    cancelBtn = new QPushButton(QString::fromUtf8("Cancel"));
    cancelBtn->setFixedSize(QSize(BTN_WIDTH, BTN_HEIGHT));
    cancelBtn->setStyleSheet("border:1px solid #FBFBFB;border-radius:6px;");

    QGridLayout *inputLayout = new QGridLayout();
    inputLayout->addWidget(nameLabel, 0, 0, 1, 1, Qt::AlignRight);
    inputLayout->addWidget(nameEdit, 0, 1, 1, 1);
    inputLayout->addWidget(addressLabel, 1, 0, 1, 1, Qt::AlignRight);
    inputLayout->addWidget(addressEdit, 1, 1, 1, 1);
    inputLayout->addWidget(tokenLabel, 2, 0, 1, 1, Qt::AlignRight);
    inputLayout->addWidget(tokenEdit, 2, 1, 1, 1);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    topLayout->addLayout(inputLayout);
    topLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(continueBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch(1);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);
    setFixedSize(400, 220);

    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    connect ( cancelBtn, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect ( continueBtn, SIGNAL( clicked() ), this, SLOT( authorize() ) );
}

void NewNodeDialog::authorize(){

}

//void NewNodeDialog::closeEvent(QCloseEvent *event){
//    printf("closing...");
//}

NewNodeDialog::~NewNodeDialog()
{
    delete ui;
}
