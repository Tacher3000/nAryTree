#ifndef GENDIALOG_H
#define GENDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QFileDialog>
#include <QSlider>
#include <QIntValidator>

class GenDialog : public QDialog
{
    Q_OBJECT
public:
    GenDialog(QWidget *parent);
    QString getStart();
    QString getEnd();
    QString getQuality();
    int getChildren();
    QString getSelectedOption();
    void toggleDirectorySelection(int state);
    void selectDirectory();
    bool getCheckbox();
    QString getPathDic();
private:
    QLineEdit *m_start;
    QLineEdit *m_end;
    QLineEdit *m_quantity;
    QLineEdit *m_childrenLine;
    QSlider *m_childrenSlider;
    QCheckBox *m_saveToFileCheckbox;
    QLineEdit *m_directoryLine;
    QPushButton *m_directoryButton;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
};

#endif // GENDIALOG_H
