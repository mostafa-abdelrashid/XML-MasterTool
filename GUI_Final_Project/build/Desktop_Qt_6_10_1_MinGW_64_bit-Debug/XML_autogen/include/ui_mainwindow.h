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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QPushButton *backButton;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *openButton;
    QPushButton *saveButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *verifyButton;
    QPushButton *fixButton;
    QPushButton *formatButton;
    QPushButton *jsonButton;
    QPushButton *minifyButton;
    QPushButton *compressButton;
    QPushButton *decompressButton;
    QSplitter *splitter;
    QTextEdit *xmlInput;
    QTextEdit *xmlOutput;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(900, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        backButton = new QPushButton(centralwidget);
        backButton->setObjectName("backButton");

        verticalLayout_2->addWidget(backButton);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        openButton = new QPushButton(centralwidget);
        openButton->setObjectName("openButton");

        horizontalLayout_2->addWidget(openButton);

        saveButton = new QPushButton(centralwidget);
        saveButton->setObjectName("saveButton");

        horizontalLayout_2->addWidget(saveButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        verifyButton = new QPushButton(centralwidget);
        verifyButton->setObjectName("verifyButton");

        horizontalLayout_2->addWidget(verifyButton);

        fixButton = new QPushButton(centralwidget);
        fixButton->setObjectName("fixButton");

        horizontalLayout_2->addWidget(fixButton);

        formatButton = new QPushButton(centralwidget);
        formatButton->setObjectName("formatButton");

        horizontalLayout_2->addWidget(formatButton);

        jsonButton = new QPushButton(centralwidget);
        jsonButton->setObjectName("jsonButton");

        horizontalLayout_2->addWidget(jsonButton);

        minifyButton = new QPushButton(centralwidget);
        minifyButton->setObjectName("minifyButton");

        horizontalLayout_2->addWidget(minifyButton);

        compressButton = new QPushButton(centralwidget);
        compressButton->setObjectName("compressButton");

        horizontalLayout_2->addWidget(compressButton);

        decompressButton = new QPushButton(centralwidget);
        decompressButton->setObjectName("decompressButton");

        horizontalLayout_2->addWidget(decompressButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Orientation::Horizontal);
        xmlInput = new QTextEdit(splitter);
        xmlInput->setObjectName("xmlInput");
        splitter->addWidget(xmlInput);
        xmlOutput = new QTextEdit(splitter);
        xmlOutput->setObjectName("xmlOutput");
        splitter->addWidget(xmlOutput);

        verticalLayout_2->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "XML Editor", nullptr));
        backButton->setText(QCoreApplication::translate("MainWindow", "<- Back", nullptr));
        openButton->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        saveButton->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        verifyButton->setText(QCoreApplication::translate("MainWindow", "Verify", nullptr));
        fixButton->setText(QCoreApplication::translate("MainWindow", "Fix Errors", nullptr));
        formatButton->setText(QCoreApplication::translate("MainWindow", "Format", nullptr));
        jsonButton->setText(QCoreApplication::translate("MainWindow", "To JSON", nullptr));
        minifyButton->setText(QCoreApplication::translate("MainWindow", "Minify", nullptr));
        compressButton->setText(QCoreApplication::translate("MainWindow", "Compress", nullptr));
        decompressButton->setText(QCoreApplication::translate("MainWindow", "Decompress", nullptr));
        xmlInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter or load XML content here...", nullptr));
        xmlOutput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Output or logs appear here...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
