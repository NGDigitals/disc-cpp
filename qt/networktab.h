#ifndef NETWORKTAB_H
#define NETWORKTAB_H

#include <QWidget>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class NetworkTab;
}
class QProcess;
class QPushButton;
QT_END_NAMESPACE

class NetworkTab : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkTab(QWidget *parent = nullptr);
    ~NetworkTab();

private slots:
    void addNewNode();
    void readStdOut();

private:
    //const QString program = QString::fromUtf8("/Users/abrahamoyelaran/Projects/sportsx/biscore/blockchain/blockchain");

    QDataStream in;
    QProcess *process;

    Ui::NetworkTab *ui;

    QPushButton *addBtn;
    QPushButton *syncBtn;

    QTableWidget *tableWidget;

    void createTable();
    void createButtons();
    void createLayouts();
};

#endif // NETWORKTAB_H
