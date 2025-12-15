/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pushButtonRead;
    QCheckBox *checkBoxEncryptWrite;
    QPlainTextEdit *plainTextEditKeyWrite;
    QPushButton *pushButtonWrite;
    QPlainTextEdit *plainTextEditKeyRead;
    QCheckBox *checkBoxDecryptRead;
    QLabel *labelStatus;
    QLabel *labelStatusMessage;
    QComboBox *comboBoxCom;
    QPushButton *pushButtonCom;
    QLineEdit *lineEditWrite;
    QPlainTextEdit *plainTextEditRead;
    QPushButton *pushButtonConnect;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(652, 505);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        pushButtonRead = new QPushButton(centralwidget);
        pushButtonRead->setObjectName("pushButtonRead");
        pushButtonRead->setGeometry(QRect(460, 300, 101, 29));
        checkBoxEncryptWrite = new QCheckBox(centralwidget);
        checkBoxEncryptWrite->setObjectName("checkBoxEncryptWrite");
        checkBoxEncryptWrite->setGeometry(QRect(40, 200, 92, 25));
        plainTextEditKeyWrite = new QPlainTextEdit(centralwidget);
        plainTextEditKeyWrite->setObjectName("plainTextEditKeyWrite");
        plainTextEditKeyWrite->setGeometry(QRect(120, 200, 361, 31));
        pushButtonWrite = new QPushButton(centralwidget);
        pushButtonWrite->setObjectName("pushButtonWrite");
        pushButtonWrite->setGeometry(QRect(450, 150, 101, 29));
        plainTextEditKeyRead = new QPlainTextEdit(centralwidget);
        plainTextEditKeyRead->setObjectName("plainTextEditKeyRead");
        plainTextEditKeyRead->setGeometry(QRect(140, 380, 361, 31));
        checkBoxDecryptRead = new QCheckBox(centralwidget);
        checkBoxDecryptRead->setObjectName("checkBoxDecryptRead");
        checkBoxDecryptRead->setGeometry(QRect(40, 380, 92, 25));
        labelStatus = new QLabel(centralwidget);
        labelStatus->setObjectName("labelStatus");
        labelStatus->setGeometry(QRect(50, 100, 63, 20));
        labelStatusMessage = new QLabel(centralwidget);
        labelStatusMessage->setObjectName("labelStatusMessage");
        labelStatusMessage->setGeometry(QRect(100, 100, 621, 20));
        comboBoxCom = new QComboBox(centralwidget);
        comboBoxCom->setObjectName("comboBoxCom");
        comboBoxCom->setGeometry(QRect(50, 40, 121, 28));
        pushButtonCom = new QPushButton(centralwidget);
        pushButtonCom->setObjectName("pushButtonCom");
        pushButtonCom->setGeometry(QRect(190, 40, 101, 29));
        lineEditWrite = new QLineEdit(centralwidget);
        lineEditWrite->setObjectName("lineEditWrite");
        lineEditWrite->setGeometry(QRect(50, 150, 381, 28));
        lineEditWrite->setMaxLength(32);
        plainTextEditRead = new QPlainTextEdit(centralwidget);
        plainTextEditRead->setObjectName("plainTextEditRead");
        plainTextEditRead->setGeometry(QRect(50, 270, 381, 85));
        plainTextEditRead->setReadOnly(true);
        pushButtonConnect = new QPushButton(centralwidget);
        pushButtonConnect->setObjectName("pushButtonConnect");
        pushButtonConnect->setGeometry(QRect(310, 40, 90, 29));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 652, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButtonRead->setText(QCoreApplication::translate("MainWindow", "Read Card", nullptr));
        checkBoxEncryptWrite->setText(QCoreApplication::translate("MainWindow", "Encrypt", nullptr));
        pushButtonWrite->setText(QCoreApplication::translate("MainWindow", "Write Card", nullptr));
        checkBoxDecryptRead->setText(QCoreApplication::translate("MainWindow", "Decrypt", nullptr));
        labelStatus->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        labelStatusMessage->setText(QString());
        pushButtonCom->setText(QCoreApplication::translate("MainWindow", "Refresh Ports", nullptr));
        pushButtonConnect->setText(QCoreApplication::translate("MainWindow", "Open port", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
