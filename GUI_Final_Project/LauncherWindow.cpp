#include "LauncherWindow.h"
#include "ui_launcherwindow.h"

#include "mainwindow.h"     // Level 1
#include "level2window.h"   // Level 2

LauncherWindow::LauncherWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LauncherWindow)
{
    ui->setupUi(this);

    // Open Level 1 window
    connect(ui->level1Button, &QPushButton::clicked,
            this, &LauncherWindow::openLevel1);

    // Open Level 2 window
    connect(ui->level2Button, &QPushButton::clicked,
            this, &LauncherWindow::openLevel2);
}

LauncherWindow::~LauncherWindow()
{
    delete ui;
}

void LauncherWindow::openLevel1()
{
    MainWindow *level1 = new MainWindow(this);
    level1->show();

    this->close();   // close launcher
}

void LauncherWindow::openLevel2()
{
    Level2Window *level2 = new Level2Window(this);
    level2->show();

    this->close();   // close launcher
}
