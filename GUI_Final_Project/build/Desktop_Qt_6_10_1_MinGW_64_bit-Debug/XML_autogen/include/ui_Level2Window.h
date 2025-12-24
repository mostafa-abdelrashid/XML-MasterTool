/********************************************************************************
** Form generated from reading UI file 'Level2Window.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEVEL2WINDOW_H
#define UI_LEVEL2WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Level2Window
{
public:
    QWidget *centralwidget;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QPushButton *backButton;
    QGroupBox *inputGroupBox;
    QHBoxLayout *hboxLayout1;
    QLineEdit *xmlPathLineEdit;
    QPushButton *browseButton;
    QGroupBox *graphGroupBox;
    QVBoxLayout *vboxLayout1;
    QPushButton *drawGraphButton;
    QPushButton *mostActiveButton;
    QPushButton *mostInfluencerButton;
    QGroupBox *mutualGroupBox;
    QHBoxLayout *hboxLayout2;
    QLineEdit *mutualIdsLineEdit;
    QPushButton *mutualButton;
    QGroupBox *suggestGroupBox;
    QHBoxLayout *hboxLayout3;
    QSpinBox *suggestUserIdSpinBox;
    QPushButton *suggestButton;
    QGroupBox *searchGroupBox;
    QVBoxLayout *vboxLayout2;
    QLineEdit *searchLineEdit;
    QHBoxLayout *hboxLayout4;
    QRadioButton *searchWordRadio;
    QRadioButton *searchTopicRadio;
    QPushButton *searchButton;
    QTabWidget *tabWidget;
    QWidget *widget;
    QVBoxLayout *vboxLayout3;
    QTextEdit *resultsTextEdit;
    QWidget *widget1;
    QVBoxLayout *vboxLayout4;
    QLabel *graphLabel;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Level2Window)
    {
        if (Level2Window->objectName().isEmpty())
            Level2Window->setObjectName("Level2Window");
        Level2Window->resize(1100, 700);
        centralwidget = new QWidget(Level2Window);
        centralwidget->setObjectName("centralwidget");
        hboxLayout = new QHBoxLayout(centralwidget);
        hboxLayout->setObjectName("hboxLayout");
        vboxLayout = new QVBoxLayout();
        vboxLayout->setObjectName("vboxLayout");
        backButton = new QPushButton(centralwidget);
        backButton->setObjectName("backButton");

        vboxLayout->addWidget(backButton);

        inputGroupBox = new QGroupBox(centralwidget);
        inputGroupBox->setObjectName("inputGroupBox");
        hboxLayout1 = new QHBoxLayout(inputGroupBox);
        hboxLayout1->setObjectName("hboxLayout1");
        xmlPathLineEdit = new QLineEdit(inputGroupBox);
        xmlPathLineEdit->setObjectName("xmlPathLineEdit");
        xmlPathLineEdit->setReadOnly(true);

        hboxLayout1->addWidget(xmlPathLineEdit);

        browseButton = new QPushButton(inputGroupBox);
        browseButton->setObjectName("browseButton");

        hboxLayout1->addWidget(browseButton);


        vboxLayout->addWidget(inputGroupBox);

        graphGroupBox = new QGroupBox(centralwidget);
        graphGroupBox->setObjectName("graphGroupBox");
        vboxLayout1 = new QVBoxLayout(graphGroupBox);
        vboxLayout1->setObjectName("vboxLayout1");
        drawGraphButton = new QPushButton(graphGroupBox);
        drawGraphButton->setObjectName("drawGraphButton");

        vboxLayout1->addWidget(drawGraphButton);

        mostActiveButton = new QPushButton(graphGroupBox);
        mostActiveButton->setObjectName("mostActiveButton");

        vboxLayout1->addWidget(mostActiveButton);

        mostInfluencerButton = new QPushButton(graphGroupBox);
        mostInfluencerButton->setObjectName("mostInfluencerButton");

        vboxLayout1->addWidget(mostInfluencerButton);


        vboxLayout->addWidget(graphGroupBox);

        mutualGroupBox = new QGroupBox(centralwidget);
        mutualGroupBox->setObjectName("mutualGroupBox");
        hboxLayout2 = new QHBoxLayout(mutualGroupBox);
        hboxLayout2->setObjectName("hboxLayout2");
        mutualIdsLineEdit = new QLineEdit(mutualGroupBox);
        mutualIdsLineEdit->setObjectName("mutualIdsLineEdit");

        hboxLayout2->addWidget(mutualIdsLineEdit);

        mutualButton = new QPushButton(mutualGroupBox);
        mutualButton->setObjectName("mutualButton");

        hboxLayout2->addWidget(mutualButton);


        vboxLayout->addWidget(mutualGroupBox);

        suggestGroupBox = new QGroupBox(centralwidget);
        suggestGroupBox->setObjectName("suggestGroupBox");
        hboxLayout3 = new QHBoxLayout(suggestGroupBox);
        hboxLayout3->setObjectName("hboxLayout3");
        suggestUserIdSpinBox = new QSpinBox(suggestGroupBox);
        suggestUserIdSpinBox->setObjectName("suggestUserIdSpinBox");

        hboxLayout3->addWidget(suggestUserIdSpinBox);

        suggestButton = new QPushButton(suggestGroupBox);
        suggestButton->setObjectName("suggestButton");

        hboxLayout3->addWidget(suggestButton);


        vboxLayout->addWidget(suggestGroupBox);

        searchGroupBox = new QGroupBox(centralwidget);
        searchGroupBox->setObjectName("searchGroupBox");
        vboxLayout2 = new QVBoxLayout(searchGroupBox);
        vboxLayout2->setObjectName("vboxLayout2");
        searchLineEdit = new QLineEdit(searchGroupBox);
        searchLineEdit->setObjectName("searchLineEdit");

        vboxLayout2->addWidget(searchLineEdit);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setObjectName("hboxLayout4");
        searchWordRadio = new QRadioButton(searchGroupBox);
        searchWordRadio->setObjectName("searchWordRadio");
        searchWordRadio->setChecked(true);

        hboxLayout4->addWidget(searchWordRadio);

        searchTopicRadio = new QRadioButton(searchGroupBox);
        searchTopicRadio->setObjectName("searchTopicRadio");

        hboxLayout4->addWidget(searchTopicRadio);


        vboxLayout2->addLayout(hboxLayout4);

        searchButton = new QPushButton(searchGroupBox);
        searchButton->setObjectName("searchButton");

        vboxLayout2->addWidget(searchButton);


        vboxLayout->addWidget(searchGroupBox);


        hboxLayout->addLayout(vboxLayout);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        widget = new QWidget();
        widget->setObjectName("widget");
        vboxLayout3 = new QVBoxLayout(widget);
        vboxLayout3->setObjectName("vboxLayout3");
        resultsTextEdit = new QTextEdit(widget);
        resultsTextEdit->setObjectName("resultsTextEdit");

        vboxLayout3->addWidget(resultsTextEdit);

        tabWidget->addTab(widget, QString());
        widget1 = new QWidget();
        widget1->setObjectName("widget1");
        vboxLayout4 = new QVBoxLayout(widget1);
        vboxLayout4->setObjectName("vboxLayout4");
        graphLabel = new QLabel(widget1);
        graphLabel->setObjectName("graphLabel");
        graphLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxLayout4->addWidget(graphLabel);

        tabWidget->addTab(widget1, QString());

        hboxLayout->addWidget(tabWidget);

        Level2Window->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(Level2Window);
        statusbar->setObjectName("statusbar");
        Level2Window->setStatusBar(statusbar);

        retranslateUi(Level2Window);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Level2Window);
    } // setupUi

    void retranslateUi(QMainWindow *Level2Window)
    {
        Level2Window->setWindowTitle(QCoreApplication::translate("Level2Window", "XML Editor - Level 2 (Network Analysis)", nullptr));
        backButton->setText(QCoreApplication::translate("Level2Window", "<- Back", nullptr));
        inputGroupBox->setTitle(QCoreApplication::translate("Level2Window", "Input XML", nullptr));
        browseButton->setText(QCoreApplication::translate("Level2Window", "Browse", nullptr));
        graphGroupBox->setTitle(QCoreApplication::translate("Level2Window", "Graph Operations", nullptr));
        drawGraphButton->setText(QCoreApplication::translate("Level2Window", "Draw Graph", nullptr));
        mostActiveButton->setText(QCoreApplication::translate("Level2Window", "Most Active User", nullptr));
        mostInfluencerButton->setText(QCoreApplication::translate("Level2Window", "Most Influencer", nullptr));
        mutualGroupBox->setTitle(QCoreApplication::translate("Level2Window", "Mutual Followers", nullptr));
        mutualIdsLineEdit->setPlaceholderText(QCoreApplication::translate("Level2Window", "IDs: 1,2,3", nullptr));
        mutualButton->setText(QCoreApplication::translate("Level2Window", "Find", nullptr));
        suggestGroupBox->setTitle(QCoreApplication::translate("Level2Window", "Suggest Users", nullptr));
        suggestButton->setText(QCoreApplication::translate("Level2Window", "Suggest", nullptr));
        searchGroupBox->setTitle(QCoreApplication::translate("Level2Window", "Post Search", nullptr));
        searchWordRadio->setText(QCoreApplication::translate("Level2Window", "By Word", nullptr));
        searchTopicRadio->setText(QCoreApplication::translate("Level2Window", "By Topic", nullptr));
        searchButton->setText(QCoreApplication::translate("Level2Window", "Search", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(widget), QCoreApplication::translate("Level2Window", "Results", nullptr));
        graphLabel->setText(QCoreApplication::translate("Level2Window", "No graph yet", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(widget1), QCoreApplication::translate("Level2Window", "Graph", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Level2Window: public Ui_Level2Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEVEL2WINDOW_H
