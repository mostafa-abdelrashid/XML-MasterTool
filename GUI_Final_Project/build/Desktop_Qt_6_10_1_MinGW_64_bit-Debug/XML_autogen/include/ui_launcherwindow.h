/********************************************************************************
** Form generated from reading UI file 'launcherwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAUNCHERWINDOW_H
#define UI_LAUNCHERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LauncherWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QHBoxLayout *buttonLayout;
    QPushButton *level1Button;
    QPushButton *level2Button;

    void setupUi(QWidget *LauncherWindow)
    {
        if (LauncherWindow->objectName().isEmpty())
            LauncherWindow->setObjectName("LauncherWindow");
        LauncherWindow->resize(400, 200);
        verticalLayout = new QVBoxLayout(LauncherWindow);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(LauncherWindow);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        titleLabel->setFont(font);

        verticalLayout->addWidget(titleLabel);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        level1Button = new QPushButton(LauncherWindow);
        level1Button->setObjectName("level1Button");
        level1Button->setMinimumHeight(40);

        buttonLayout->addWidget(level1Button);

        level2Button = new QPushButton(LauncherWindow);
        level2Button->setObjectName("level2Button");
        level2Button->setMinimumHeight(40);

        buttonLayout->addWidget(level2Button);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(LauncherWindow);

        QMetaObject::connectSlotsByName(LauncherWindow);
    } // setupUi

    void retranslateUi(QWidget *LauncherWindow)
    {
        LauncherWindow->setWindowTitle(QCoreApplication::translate("LauncherWindow", "XML Project", nullptr));
        titleLabel->setText(QCoreApplication::translate("LauncherWindow", "Choose Project Level", nullptr));
        level1Button->setText(QCoreApplication::translate("LauncherWindow", "Level 1", nullptr));
        level2Button->setText(QCoreApplication::translate("LauncherWindow", "Level 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LauncherWindow: public Ui_LauncherWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAUNCHERWINDOW_H
