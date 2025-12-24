#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <string>

class LauncherWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(LauncherWindow *launcher, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
     void onBackClicked();
    void openFile();
    void saveFile();

    void verifyXML();
    void fixErrorsSlot();   // <-- uppercase 'S'
    void formatXML();
    void convertToJSON();
    void minifyXML();
    void compressXML();
    void decompressXML();

private:
    Ui::MainWindow *ui;
    QString currentFile;
    LauncherWindow *launcherWindow;
    std::string readFileContent(const QString &filePath);
    void writeFileContent(const QString &filePath,
                          const std::string &content);
};

#endif // MAINWINDOW_H
