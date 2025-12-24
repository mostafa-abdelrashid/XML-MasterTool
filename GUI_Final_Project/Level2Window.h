#ifndef LEVEL2WINDOW_H
#define LEVEL2WINDOW_H

#include <QMainWindow>
class LauncherWindow;

QT_BEGIN_NAMESPACE
namespace Ui {
class Level2Window;
}
QT_END_NAMESPACE

class Level2Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Level2Window(LauncherWindow *launcher, QWidget *parent = nullptr);
    ~Level2Window();

private slots:
    void onBackClicked();
    void onBrowseXML();
    void onDrawGraph();
    void onMostActive();
    void onMostInfluencer();
    void onFindMutual();
    void onSuggestUsers();
    void onSearch();
    bool loadNetwork(const std::string& inputFile);
private:
    Ui::Level2Window *ui;
     LauncherWindow *launcherWindow;
};

#endif // LEVEL2WINDOW_H
