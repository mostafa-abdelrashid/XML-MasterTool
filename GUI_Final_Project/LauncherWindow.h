#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class LauncherWindow;
}
QT_END_NAMESPACE

class LauncherWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LauncherWindow(QWidget *parent = nullptr);
    ~LauncherWindow();

private slots:
    void openLevel1();
    void openLevel2();

private:
    Ui::LauncherWindow *ui;
};

#endif // LAUNCHERWINDOW_H
