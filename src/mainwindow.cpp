#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    m_stackedWidget = new QStackedWidget(this);

    m_homeScreen = new HomeScreen(this);
    connect(m_homeScreen, &HomeScreen::clickedGenerator, this, &MainWindow::openGenDialog);
    connect(m_homeScreen, &HomeScreen::clickedEnter, this, &MainWindow::enterTree);
    connect(m_homeScreen, &HomeScreen::clickedSearch, this, &MainWindow::searchTree);
    m_stackedWidget->addWidget(m_homeScreen);

    m_treeWindow = new TreeWindow(this);
    connect(m_treeWindow, &TreeWindow::clickedExit, this, &MainWindow::homeScreen);
    m_stackedWidget->addWidget(m_treeWindow);

    setCentralWidget(m_stackedWidget);
}

void MainWindow::openGenDialog()
{
    GenDialog genDialog(this);
    if (genDialog.exec() == QDialog::Accepted){
        QString selectedOption = genDialog.getSelectedOption();

        if (selectedOption == "ТЕКСТОВЫЙ ВЫВОД") {
            m_treeWindow->randomInsertion(genDialog.getStart().toInt(), genDialog.getEnd().toInt(), genDialog.getQuality().toInt(), genDialog.getCheckbox(), genDialog.getPathDic(), 1, genDialog.getChildren());
        } else if (selectedOption == "ГРАФИЧЕСКИЙ ВЫВОД (НЕ РЕКОМЕНДУЕТСЯ ИСПОЛЬЗОВАТЬ ЕСЛИ КОЛЛИЧЕСТВО ВЕРШИН БОЛЬШЕ 100.000)") {
            m_treeWindow->randomInsertion(genDialog.getStart().toInt(), genDialog.getEnd().toInt(), genDialog.getQuality().toInt(), genDialog.getCheckbox(), genDialog.getPathDic(), 2, genDialog.getChildren());
        } else if (selectedOption == "БЕЗ ВЫВОДА") {
            m_treeWindow->randomInsertion(genDialog.getStart().toInt(), genDialog.getEnd().toInt(), genDialog.getQuality().toInt(), genDialog.getCheckbox(), genDialog.getPathDic(), 3, genDialog.getChildren());
        }
        m_stackedWidget->setCurrentIndex(1);

    }
}

void MainWindow::homeScreen()
{
    m_stackedWidget->setCurrentIndex(0);

    delete m_treeWindow;
    m_treeWindow = new TreeWindow(this);
    connect(m_treeWindow, &TreeWindow::clickedExit, this, &MainWindow::homeScreen);
    m_stackedWidget->addWidget(m_treeWindow);
}

void MainWindow::enterTree()
{
    m_treeWindow->enterTree();
    m_stackedWidget->setCurrentIndex(1);
}

void MainWindow::searchTree()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Выберите тип вывода"));
    QVBoxLayout layout(&dialog);

    QCheckBox *textCheckBox = new QCheckBox(tr("Текстовый вывод"), &dialog);
    QCheckBox *graphicCheckBox = new QCheckBox(tr("Графический вывод"), &dialog);
    layout.addWidget(textCheckBox);
    layout.addWidget(graphicCheckBox);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    layout.addWidget(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        bool textOutput = textCheckBox->isChecked();
        bool graphicOutput = graphicCheckBox->isChecked();

        QString filePath = QFileDialog::getOpenFileName(this, tr("Выберите файл"), QDir::homePath(), tr("Текстовые файлы (*.txt)"));

        if (!filePath.isEmpty()) {
            if (textOutput && !graphicOutput) {
                m_treeWindow->fromFile(filePath, TreeWindow::TextOutput);
            } else if (graphicOutput && !textOutput) {
                m_treeWindow->fromFile(filePath, TreeWindow::GraphicOutput);
            } else if (textOutput && graphicOutput) {
                m_treeWindow->fromFile(filePath, TreeWindow::BothOutput);
            }

            m_stackedWidget->setCurrentIndex(1);
        }
    }
}
