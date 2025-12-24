#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LauncherWindow.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <sstream>
#include <QDir>
#include <fstream>
#include <iomanip>

// ===== Your REAL backend headers =====
#include "XMLParser.h"
#include "XMLNode.h"
#include "XMLToJSONConverter.h"
#include "error_check.h"
#include "errorFixing.h"
#include "prettify.h"
#include "minfiy.h"
#include "comp_decomp.h"

// ====================================
MainWindow::MainWindow(LauncherWindow *launcher, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
  launcherWindow(launcher)
{
    ui->setupUi(this);

    // ===== Connect buttons to slots =====
    connect(ui->backButton, &QPushButton::clicked,this, &MainWindow::onBackClicked);
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveFile);

    connect(ui->verifyButton, &QPushButton::clicked, this, &MainWindow::verifyXML);
    connect(ui->fixButton, &QPushButton::clicked,this, &MainWindow::fixErrorsSlot);

    connect(ui->formatButton, &QPushButton::clicked, this, &MainWindow::formatXML);
    connect(ui->jsonButton, &QPushButton::clicked, this, &MainWindow::convertToJSON);
    connect(ui->minifyButton, &QPushButton::clicked, this, &MainWindow::minifyXML);
    connect(ui->compressButton, &QPushButton::clicked, this, &MainWindow::compressXML);
    connect(ui->decompressButton, &QPushButton::clicked, this, &MainWindow::decompressXML);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// =================================================
//                  FILE OPERATIONS
// =================================================

void MainWindow::onBackClicked()
{
    this->hide();              // hide Level 1
    launcherWindow->show();    // show launcher again
}
void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "Open XML File", "", "XML Files (*.xml);;All Files (*)"
    );

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open file");
        return;
    }

    QTextStream in(&file);
    ui->xmlInput->setPlainText(in.readAll());
    file.close();
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "Save Output", "", "All Files (*)"
    );

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot save file");
        return;
    }

    QTextStream out(&file);
    out << ui->xmlOutput->toPlainText();
    file.close();
}

// =================================================
//           Helper functions implementation
// =================================================

std::string MainWindow::readFileContent(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }
    QTextStream in(&file);
    return in.readAll().toStdString();
}

void MainWindow::writeFileContent(const QString &filePath, const std::string &content)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    QTextStream out(&file);
    out << QString::fromStdString(content);
}

// =================================================
//                  XML OPERATIONS
// =================================================

// -------- Verify XML --------
void MainWindow::verifyXML()
{
    // 1. Get XML from UI
    QString xmlQt = ui->xmlInput->toPlainText();
    std::string xml = xmlQt.toStdString();

    // 2. Write to temporary file
    QString tempPath = QDir::temp().filePath("input.xml");
    std::ofstream out(tempPath.toStdString());
    out << xml;
    out.close();

    // 3. Call NEW backend (file path!)
    ErrorInfo errors = countErrorSummary(tempPath.toStdString());

    // 4. Display results
    std::stringstream result;
    result << "XML Verification Results:\n";
    result << "Total errors found: " << errors.count << "\n\n";

    for (int i = 0; i < errors.count; i++) {
        result << "Error " << (i + 1) << ":\n";
        result << "  Line: " << errors.lines[i] << "\n";
        result << "  Type: " << errors.types[i] << "\n";
        result << "  Tag: " << errors.tags[i] << "\n";
        result << "  Description: " << errors.descriptions[i] << "\n\n";
    }

    if (errors.count == 0) {
        result << "XML is valid!";
    }

    ui->xmlOutput->setPlainText(QString::fromStdString(result.str()));
}

// -------- Fix Errors --------
void MainWindow::fixErrorsSlot()
{
    // 1. Get XML from UI
    QString xmlQt = ui->xmlInput->toPlainText();
    std::string xml = xmlQt.toStdString();

    // 2. Write XML to temp file
    QString tempPath = QDir::temp().filePath("input.xml");
    std::ofstream out(tempPath.toStdString());
    out << xml;
    out.close();

    // 3. Analyze using NEW backend
    ErrorInfo errors = countErrorSummary(tempPath.toStdString());

    // 4. Fix XML using SAME content string
    std::string fixedXML = fixErrors(xml, errors);

    // 5. Show output
    ui->xmlOutput->setPlainText(QString::fromStdString(fixedXML));
}


// -------- Format (Prettify) --------
void MainWindow::formatXML()
{
    std::string xml = ui->xmlInput->toPlainText().toStdString();
    
    // Write to temporary file
    QString tempInput = QDir::tempPath() + "/temp_input.xml";
    QString tempOutput = QDir::tempPath() + "/temp_output.xml";
    
    writeFileContent(tempInput, xml);
    
    // Call prettifyXML
    prettifyXML(tempInput.toStdString(), tempOutput.toStdString());
    
    // Read result
    std::string formatted = readFileContent(tempOutput);
    
    ui->xmlOutput->setPlainText(QString::fromStdString(formatted));
    
    // Clean up temp files
    QFile::remove(tempInput);
    QFile::remove(tempOutput);
}

// -------- XML → JSON --------
void MainWindow::convertToJSON()
{
    std::string xml = ui->xmlInput->toPlainText().toStdString();

    try {
        XMLParser parser;
        XMLNode* root = parser.parse(xml);

        XMLToJSONConverter converter;
        converter.setPrettyPrint(true);
        converter.setAutoDetectTypes(true);
        
        std::string json = converter.convert(root);

        ui->xmlOutput->setPlainText(QString::fromStdString(json));
        
        delete root;  // Don't forget to clean up
    }
    catch (const std::exception& e) {
        ui->xmlOutput->setPlainText(QString("Error converting to JSON: ") + e.what());
    }
}

// -------- Minify --------
void MainWindow::minifyXML()
{
    std::string xml = ui->xmlInput->toPlainText().toStdString();
    
    // Write to temporary file
    QString tempInput = QDir::tempPath() + "/temp_input.xml";
    QString tempOutput = QDir::tempPath() + "/temp_output.xml";
    
    writeFileContent(tempInput, xml);
    
    // Call minifyXML
    ::minifyXML(tempInput.toStdString(), tempOutput.toStdString());
    
    // Read result
    std::string minified = readFileContent(tempOutput);
    
    ui->xmlOutput->setPlainText(QString::fromStdString(minified));
    
    // Clean up temp files
    QFile::remove(tempInput);
    QFile::remove(tempOutput);
}

// -------- Compress --------
void MainWindow::compressXML()
{
    std::string xml = ui->xmlInput->toPlainText().toStdString();

    if (xml.empty()) {
        QMessageBox::warning(this, "Error", "No XML content to compress.");
        return;
    }

    // Ask user where to save the compressed file
    QString savePath = QFileDialog::getSaveFileName(
        this,
        "Save Compressed File",
        QDir::homePath() + "/compressed.bin",  // default location + name
        "Binary Files (*.bin);;All Files (*)"
        );

    if (savePath.isEmpty())
        return;  // user cancelled

    // Write XML to a temporary input file
    QString tempInput = QDir::tempPath() + "/temp_input.xml";
    writeFileContent(tempInput, xml);

    // Call your backend compress() function
    compress(tempInput.toStdString(), savePath.toStdString());

    // Get compressed file size
    std::ifstream f(savePath.toStdString(), std::ios::binary | std::ios::ate);
    size_t compressedSize = f.tellg();
    f.close();

    // Display results
    std::stringstream result;
    result << "XML compressed successfully!\n";
    result << "Original size: " << xml.size() << " bytes\n";
    result << "Compressed size: " << compressedSize << " bytes\n";
    result << "Compression ratio: "
           << std::fixed << std::setprecision(2)
           << (100.0 - (compressedSize * 100.0 / xml.size())) << "%\n";
    result << "Saved to: " << savePath.toStdString();

    ui->xmlOutput->setPlainText(QString::fromStdString(result.str()));

    // Clean up temporary file
    QFile::remove(tempInput);
}

// -------- Decompress --------
void MainWindow::decompressXML()
{
    QString inputFile = QFileDialog::getOpenFileName(
        this, "Open Compressed File", "", "Binary Files (*.bin);;All Files (*)"
    );
    
    if (inputFile.isEmpty())
        return;
    
    QString tempOutput = QDir::tempPath() + "/temp_decompressed.xml";
    
    // Use decompress function from comp_decomp.cpp
    decompress(inputFile.toStdString(), tempOutput.toStdString());
    
    // Read decompressed result
    std::string decompressed = readFileContent(tempOutput);
    
    ui->xmlOutput->setPlainText(QString::fromStdString(decompressed));
    
    // Clean up temp file
    QFile::remove(tempOutput);
}
