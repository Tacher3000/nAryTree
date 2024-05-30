#include "homescreen.h"

HomeScreen::HomeScreen(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    m_buttonGenerator = new QPushButton(this);
    m_buttonGenerator->setText("СГЕНЕРИРОВАТЬ СЛУЧАЙНОЕ ДЕРЕВО");
    m_buttonGenerator->setMinimumSize(400, 50);
    connect(m_buttonGenerator, &QPushButton::clicked, this, &HomeScreen::generator);

    m_buttonEnter = new QPushButton(this);
    m_buttonEnter->setText("ВВЕСТИ ДЕРЕВО");
    m_buttonEnter->setMinimumSize(400, 50);
    connect(m_buttonEnter, &QPushButton::clicked, this, &HomeScreen::enter);

    m_buttonFile = new QPushButton(this);
    m_buttonFile->setText("ЗАГРУЗИТЬ ИЗ ФАЙЛА");
    m_buttonFile->setMinimumSize(400, 50);
    connect(m_buttonFile, &QPushButton::clicked, this, &HomeScreen::searchPatch);

    mainLayout->addWidget(m_buttonGenerator);
    mainLayout->addWidget(m_buttonEnter);
    mainLayout->addWidget(m_buttonFile);
}

void HomeScreen::generator()
{
    emit clickedGenerator();
}

void HomeScreen::enter()
{
    emit clickedEnter();
}

void HomeScreen::searchPatch()
{
    emit clickedSearch();
}
