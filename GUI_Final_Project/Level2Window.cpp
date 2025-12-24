#include "Level2Window.h"
#include "ui_Level2Window.h"
#include "LauncherWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include<iostream>
#include <fstream>
#include <sstream>
#include <QProcess>
#include <QDateTime>
#include <QFile>
// Social Network & Analysis headers
#include "SocialNetwork.h"
#include "Mutual.h"
#include "Suggest.h"
#include "Influencer.h"
#include "ActiveUser.h"
#include "SearchWord.h"
#include "SearchTopic.h"
#include "GraphVisualizer.h"
#include "comp_decomp.h"
#include "XMLParser.h"
#include"XMLTrreeBuilder.h"
#include "XMLTokenizer.h"



Level2Window::Level2Window(LauncherWindow *launcher, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Level2Window),
    launcherWindow(launcher)
{
    ui->setupUi(this);

    // ---- Connections ----//
    connect(ui->backButton, &QPushButton::clicked,
            this, &Level2Window::onBackClicked);

    connect(ui->browseButton, &QPushButton::clicked,
            this, &Level2Window::onBrowseXML);

    connect(ui->drawGraphButton, &QPushButton::clicked,
            this, &Level2Window::onDrawGraph);

    connect(ui->mostActiveButton, &QPushButton::clicked,
            this, &Level2Window::onMostActive);

    connect(ui->mostInfluencerButton, &QPushButton::clicked,
            this, &Level2Window::onMostInfluencer);

    connect(ui->mutualButton, &QPushButton::clicked,
            this, &Level2Window::onFindMutual);

    connect(ui->suggestButton, &QPushButton::clicked,
            this, &Level2Window::onSuggestUsers);

    connect(ui->searchButton, &QPushButton::clicked,
            this, &Level2Window::onSearch);
}

Level2Window::~Level2Window()
{
    delete ui;
}

// ================= Slots =================
bool Level2Window::loadNetwork(const string& inputFile)
{
    string xmlContent;
    try {
        xmlContent = readFile(inputFile);
    } catch (...) {
        cerr << "Error: Cannot open " << inputFile << endl;
        return false;
    }

    XMLParser parser;
    XMLNode* root = nullptr;

    try {
        root = parser.parse(xmlContent);
    } catch (const exception& e) {
        cerr << "XML Parsing Error: " << e.what() << endl;
        return false;
    }

    // ✅ Build the network EXACTLY like CLI
    g_network.parseUsersFromXML(root);
    g_network.linkUsers();

    delete root;
    return true;
}
void Level2Window::onBackClicked()
{
    this->hide();
    launcherWindow->show();
}

void Level2Window::onBrowseXML()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open XML File",
        "",
        "XML Files (*.xml)"
    );

    if (!filePath.isEmpty()) {
        ui->xmlPathLineEdit->setText(filePath);
        ui->statusbar->showMessage("XML file loaded", 3000);
    }
}

void Level2Window::onDrawGraph()
{
    ui->resultsTextEdit->clear();

    QString xmlPath = ui->xmlPathLineEdit->text();
    if (xmlPath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select an XML file first.");
        return;
    }

    QString timeStamp = QString::number(QDateTime::currentMSecsSinceEpoch());
    QString dotFile = "network_" + timeStamp + ".dot";
    QString pngFile = "network_" + timeStamp + ".png";

    // Generate DOT
    generateDOTFile(xmlPath.toStdString(), dotFile.toStdString());

    //  FULL PATH to dot.exe (CHANGE IF NEEDED)
    QString dotPath = "C:/Program Files/Graphviz/bin/dot.exe";

    if (!QFile::exists(dotPath)) {
        QMessageBox::critical(this, "Graphviz Error",
                              "dot.exe not found at:\n" + dotPath);
        return;
    }

    QProcess process;
    process.start(dotPath, QStringList() << "-Tpng" << dotFile << "-o" << pngFile);
    process.waitForFinished();

    if (!QFile::exists(pngFile)) {
        QMessageBox::critical(this, "Graph Error",
                              "Graph image was not created.");
        return;
    }

    QPixmap pixmap;
    pixmap.load(pngFile);
    ui->graphLabel->setPixmap(pixmap);
    ui->graphLabel->repaint();

    ui->statusbar->showMessage("Graph created successfully", 3000);
}



void Level2Window::onMostActive()
{
    ui->resultsTextEdit->clear();

    QString path = ui->xmlPathLineEdit->text();
    if (path.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select an XML file first.");
        return;
    }

    // Redirect cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    // Run your EXISTING backend logic
    if (!loadNetwork(path.toStdString())) {
        std::cout.rdbuf(oldCout);
        ui->resultsTextEdit->setText("Error loading XML file.");
        return;
    }

    most_activeUser();   // prints using cout

    // Restore cout
    std::cout.rdbuf(oldCout);

    // Show output in UI
    ui->resultsTextEdit->append(QString::fromStdString(buffer.str()));

}


void Level2Window::onMostInfluencer()
{
    ui->resultsTextEdit->clear();

    QString path = ui->xmlPathLineEdit->text();
    if (path.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select an XML file first.");
        return;
    }

    // Redirect cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    // Run your EXISTING backend logic
    if (!loadNetwork(path.toStdString())) {
        std::cout.rdbuf(oldCout);
        ui->resultsTextEdit->setText("Error loading XML file.");
        return;
    }

    most_influencer();   // prints using cout

    // Restore cout
    std::cout.rdbuf(oldCout);

    // Show output in UI
    ui->resultsTextEdit->append(QString::fromStdString(buffer.str()));
}


void Level2Window::onFindMutual()
{
    ui->resultsTextEdit->clear();

    QString path = ui->xmlPathLineEdit->text();
    QString idsText = ui->mutualIdsLineEdit->text();

    std::vector<int> ids;
    for (const QString& s : idsText.split(",", Qt::SkipEmptyParts))
        ids.push_back(s.trimmed().toInt());

    // 🔴 Capture cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    mutual(path.toStdString(), ids);   // prints to cout

    std::cout.rdbuf(oldCout);          // restore cout

    // Show captured output
    ui->resultsTextEdit->append("---- Mutual Followers ----");
    ui->resultsTextEdit->append(QString::fromStdString(buffer.str()));
}

void Level2Window::onSuggestUsers()
{
    ui->resultsTextEdit->clear();

    QString path = ui->xmlPathLineEdit->text();
    if (path.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please choose an XML file");
        return;
    }

    int userId = ui->suggestUserIdSpinBox->value();

    // 🔴 Capture cout
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    suggest(path.toStdString(), userId);  // prints to cout

    std::cout.rdbuf(oldCout);             // restore cout

    ui->resultsTextEdit->append("---- Suggested Users ----");
    ui->resultsTextEdit->append(QString::fromStdString(buffer.str()));
}


void Level2Window::onSearch()
{
    QString keyword = ui->searchLineEdit->text();

    if (ui->searchWordRadio->isChecked()){
        ui->resultsTextEdit->clear();

    QString path1 = ui->xmlPathLineEdit->text();
    QString word = ui->searchLineEdit->text();

    if (path1.isEmpty() || word.isEmpty()) {
        QMessageBox::warning(this, "Error", "Missing input");
        return;
    }

    std::stringstream buffer1;
    std::streambuf* oldCout1 = std::cout.rdbuf(buffer1.rdbuf());

    std::ifstream in1(path1.toStdString());
    std::string content1((std::istreambuf_iterator<char>(in1)),
                   std::istreambuf_iterator<char>());

    auto results1 = searchPostsByWord(content1, word.toStdString());
    printMatches(results1);   // prints to cout

    std::cout.rdbuf(oldCout1); // restore cout

    ui->resultsTextEdit->append("---- Search Results ----");
    ui->resultsTextEdit->append(QString::fromStdString(buffer1.str()));

    }else{

            ui->resultsTextEdit->clear();

            QString path = ui->xmlPathLineEdit->text();
            QString topic = ui->searchLineEdit->text();

            if (path.isEmpty() || topic.isEmpty()) {
                QMessageBox::warning(this, "Error", "Missing input");
                return;
            }

            std::stringstream buffer;
            std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

            // 🔴 original console-based logic
            std::ifstream in(path.toStdString());
            std::string content((std::istreambuf_iterator<char>(in)),
                           std::istreambuf_iterator<char>());

            auto results = searchPostsByTopic(content, topic.toStdString());
            printMatches(results);

            std::cout.rdbuf(oldCout); // restore cout

            ui->resultsTextEdit->append("---- Search Results ----");
            ui->resultsTextEdit->append(QString::fromStdString(buffer.str()));

}
}
