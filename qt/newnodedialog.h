#ifndef NEWNODEDIALOG_H
#define NEWNODEDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class NewNodeDialog;
}
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class NewNodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewNodeDialog(QWidget *parent = nullptr);
    ~NewNodeDialog();

//protected:
//    void closeEvent(QCloseEvent *event) override;

private slots:
    void authorize();
//    void close();

private:
    Ui::NewNodeDialog *ui;

    QLineEdit *nameEdit;
    QLineEdit *addressEdit;
    QLineEdit *tokenEdit;

    QPushButton *continueBtn;
    QPushButton *cancelBtn;
};

#endif // NEWNODEDIALOG_H
