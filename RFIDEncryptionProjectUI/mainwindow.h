#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPortInfo>
#include <QComboBox>
#include <reader.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void populateSerialPorts();
    void readCardInfo();
    void writeCardInfo();
    void toggleConnection(bool checked);
    void showError(const std::string &msg);

private:
    Ui::MainWindow *ui;
    std::unique_ptr<RFID::RFIDReader> reader;
};

#endif
