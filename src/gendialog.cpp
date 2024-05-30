#include "gendialog.h"

GenDialog::GenDialog(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QValidator *validator = new QIntValidator(this);

    m_start = new QLineEdit(this);
    m_start->setPlaceholderText("ОТ");
    m_start->setMinimumSize(400, 50);
    m_start->setValidator(validator);
    mainLayout->addWidget(m_start);

    m_end = new QLineEdit(this);
    m_end->setPlaceholderText("ДО");
    m_end->setMinimumSize(400, 50);
    m_end->setValidator(validator);
    mainLayout->addWidget(m_end);

    m_quantity = new QLineEdit(this);
    m_quantity->setPlaceholderText("КОЛЛИЧЕСТВО ВЕРШИН");
    m_quantity->setMinimumSize(400, 50);
    m_quantity->setValidator(validator);
    mainLayout->addWidget(m_quantity);

    QHBoxLayout *childrenLayout = new QHBoxLayout;

    m_childrenLine = new QLineEdit(this);
    m_childrenLine->setPlaceholderText("КОЛЛИЧЕСТВО ДЕТЕЙ");
    m_childrenLine->setMinimumSize(60, 50);
    m_childrenLine->setValidator(validator);
    childrenLayout->addWidget(m_childrenLine);

    m_childrenSlider = new QSlider(Qt::Horizontal, this);
    m_childrenSlider->setMinimumWidth(350);
    m_childrenSlider->setMinimum(1);
    m_childrenSlider->setMaximum(100);
    m_childrenSlider->setValue(5);
    connect(m_childrenSlider, &QAbstractSlider::valueChanged, [this](int val){ m_childrenLine->setText(QString::number(val)); });
    connect(m_childrenLine, &QLineEdit::textEdited, [this](QString val){ m_childrenSlider->setValue(val.toInt()); qDebug() << m_childrenSlider->value();});
    childrenLayout->addWidget(m_childrenSlider);

    mainLayout->addLayout(childrenLayout);

    QRadioButton *radioButton1 = new QRadioButton("ТЕКСТОВЫЙ ВЫВОД", this);
    QRadioButton *radioButton2 = new QRadioButton("ГРАФИЧЕСКИЙ ВЫВОД (НЕ РЕКОМЕНДУЕТСЯ ИСПОЛЬЗОВАТЬ ЕСЛИ КОЛЛИЧЕСТВО ВЕРШИН БОЛЬШЕ 100.000)", this);
    QRadioButton *radioButton3 = new QRadioButton("БЕЗ ВЫВОДА", this);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(radioButton1);
    buttonGroup->addButton(radioButton2);
    buttonGroup->addButton(radioButton3);

    mainLayout->addWidget(radioButton1);
    mainLayout->addWidget(radioButton2);
    mainLayout->addWidget(radioButton3);

    radioButton2->setChecked(true);

    m_saveToFileCheckbox = new QCheckBox("Сохранить в файл", this);
    mainLayout->addWidget(m_saveToFileCheckbox);

    connect(m_saveToFileCheckbox, &QCheckBox::stateChanged, this, &GenDialog::toggleDirectorySelection);

    QHBoxLayout *directoryLayout = new QHBoxLayout;

    m_directoryLine = new QLineEdit(this);
    m_directoryLine->setPlaceholderText("ПУТЬ");
    m_directoryLine->setMinimumSize(300, 50);
    m_directoryLine->setEnabled(false);
    directoryLayout->addWidget(m_directoryLine);

    m_directoryButton = new QPushButton("Выбрать директорию", this);
    m_directoryButton->setMinimumSize(95, 50);
    m_directoryButton->setEnabled(false);
    directoryLayout->addWidget(m_directoryButton);

    connect(m_directoryButton, &QPushButton::clicked, this, &GenDialog::selectDirectory);

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    m_okButton = new QPushButton("OK", this);
    m_okButton->setMinimumHeight(50);
    m_cancelButton = new QPushButton("Отмена", this);
    m_cancelButton->setMinimumHeight(50);
    connect(m_okButton, &QPushButton::clicked, this, &GenDialog::accept);
    connect(m_cancelButton, &QPushButton::clicked, this, &GenDialog::reject);
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);

    mainLayout->addLayout(directoryLayout);

    mainLayout->addLayout(buttonLayout);
}

QString GenDialog::getStart()
{
    return m_start->text();
}

QString GenDialog::getEnd()
{
    return m_end->text();
}

QString GenDialog::getQuality()
{
    return m_quantity->text();
}

int GenDialog::getChildren()
{
    if(!m_childrenLine->text().isEmpty()){
        return m_childrenLine->text().toInt();
    }else{
        return m_childrenSlider->value();
    }
}

QString GenDialog::getSelectedOption()
{
    foreach (QRadioButton *button, findChildren<QRadioButton *>()) {
        if (button->isChecked()) {
            return button->text();
        }
    }
    return QString();
}

bool GenDialog::getCheckbox(){
    return m_saveToFileCheckbox->isChecked() ? true : false;
}

QString GenDialog::getPathDic(){
    return m_directoryLine->text();
}

void GenDialog::toggleDirectorySelection(int state)
{
    m_directoryButton->setEnabled(state == Qt::Checked);
    m_directoryLine->setEnabled(state == Qt::Checked);
}

void GenDialog::selectDirectory()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Выберите директорию для сохранения файла"), QDir::homePath());

    if (!directory.isEmpty()) {
        m_directoryLine->setText(directory);
    }
}
