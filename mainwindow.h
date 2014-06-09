#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QStandardItemModel>
#include <QMainWindow>
#include <map>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int count = 0;
    QString filePath = ".";
    std::map<std::string,int> loginErrorCount;
    QStandardItemModel *log_model;
    unsigned int searchType;
    QString keyWord;
    QVector<QString> infoVector;
    void setTableViewHeader();
    void setShowStaticsGraph(std::map<std::string, int> &tmpMap);
    bool addContent();
    bool isContain(QString str,QString strArr[]);
    bool isFind(QVector<QString> infoVector1,int searchType1,QString keyWord1);
    ~MainWindow();

private slots:
    void on_pushButton_clicked(); //开始查询按钮

    void on_tongji_tabBarClicked(int index);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_actionAbout_triggered();

    void on_actionOpen_triggered();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
