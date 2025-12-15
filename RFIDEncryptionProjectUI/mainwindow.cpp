#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "reader.h"
#include "encryptor.h"
#include "bytemanip.h"
#include <iostream>
#include <QMessageBox>

using namespace RFID;
using namespace Crypto;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    populateSerialPorts();

    connect(ui->pushButtonCom, &QPushButton::clicked,
            this, &MainWindow::populateSerialPorts);
    connect(ui->pushButtonRead, &QPushButton::clicked,
            this, &MainWindow::readCardInfo);
    connect(ui->pushButtonWrite, &QPushButton::clicked,
            this, &MainWindow::writeCardInfo);

    ui->pushButtonConnect->setCheckable(true);
    connect(ui->pushButtonConnect, &QPushButton::toggled,
            this, &MainWindow::toggleConnection);

    reader.reset();
    ui->pushButtonConnect->setText("Connect");
    ui->labelStatusMessage->setText("Disconnected");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::populateSerialPorts()
{
    ui->comboBoxCom->clear();

    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : serialPortInfos) {
        ui->comboBoxCom->addItem(info.portName());
    }
}

void MainWindow::toggleConnection(bool checked)
{
    if (checked) {
        try {
            std::string port = ui->comboBoxCom->currentText().toStdString();
            reader = std::make_unique<RFIDReader>(port, 115200);
            reader->setFullPower();
            ui->labelStatusMessage->setText("Connected to " + QString::fromStdString(port));
            ui->pushButtonConnect->setText("Disconnect");
        } catch (const std::exception &e) {
            ui->labelStatusMessage->setText(e.what());
            ui->pushButtonConnect->setChecked(false);
        }
    } else {
        reader.reset();
        ui->labelStatusMessage->setText("Disconnected");
        ui->pushButtonConnect->setText("Connect");
    }
}

void MainWindow::readCardInfo(){
    try {
        if (!reader) {
            throw std::runtime_error("No reader connected");
        }
        std::cout << ui->comboBoxCom->currentText().toStdString() << std::endl;
        ui->labelStatusMessage->clear();
        ui->plainTextEditRead->clear();

        reader->setFullPower();
        reader->clearUID();
        std::string uid = reverseBytes(reader->readUID());
        std::cout << "uid: " << uid << std::endl;
        std::string data = reader->readAsciiDataFromCardAddress();
        std::cout << "data: " << data << std::endl;

        if(ui->checkBoxDecryptRead->isChecked() == true){
            std::string iv = ui->plainTextEditKeyRead->toPlainText().toStdString();
            if (iv.size() < 16) iv.append(16 - iv.size(), '0');
            else if (iv.size() > 16) iv = iv.substr(0,16);
            Encryptor encryptor(new AESEncryption(), (unsigned char *)"0123456789ABCDEF0123456789ABCDEF",
                                (unsigned char *) iv.c_str());
            data = encryptor.decrypt(data);
        }
        std::string message = "UID: " + uid + "\nData: " + data;
        ui->plainTextEditRead->setPlainText(message.c_str());
        ui->labelStatusMessage->setText("Read from card successfully");

    } catch (const std::exception& e) {
       showError(e.what());

    }

}

void MainWindow::writeCardInfo(){
    try {
        if (!reader) {
            throw std::runtime_error("No reader connected");
        }
        std::cout << ui->comboBoxCom->currentText().toStdString() << std::endl;
        ui->labelStatusMessage->clear();
        reader->clearUID();
        std::string uid = reader->readUID();
        std::cout << "uid: " << reverseBytes(uid) << std::endl;

        std::string data = ui->lineEditWrite->text().toStdString();
        if(ui->checkBoxEncryptWrite->isChecked() == true){
            std::string iv = ui->plainTextEditKeyWrite->toPlainText().toStdString();
            if (iv.size() < 16) iv.append(16 - iv.size(), '0');
            else if (iv.size() > 16) iv = iv.substr(0,16);
            Encryptor encryptor(new AESEncryption(), (unsigned char *)"0123456789ABCDEF0123456789ABCDEF",
                                (unsigned char *) iv.c_str());
            data = encryptor.encrypt(data);
        }

        reader->writeAsciiDataToCardAddress(data);
        ui->labelStatusMessage->setText("Written to card successfully");
        ui->lineEditWrite->clear();


    } catch (const std::exception& e) {
        showError(e.what());
    }

}

void MainWindow::showError(const std::string &msg)
{
    QMessageBox::critical(this, "Error", QString::fromStdString(msg));
    ui->labelStatusMessage->setText("Error: " + QString::fromStdString(msg));
}

