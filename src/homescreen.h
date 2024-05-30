#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class HomeScreen : public QWidget
{
    Q_OBJECT
public:
    HomeScreen(QWidget *parent = nullptr);
signals:
    void clickedGenerator();
    void clickedEnter();
    void clickedSearch();
private slots:
    void generator();
    void enter();
    void searchPatch();
private:
    QPushButton *m_buttonGenerator;
    QPushButton *m_buttonEnter;
    QPushButton *m_buttonFile;
};

#endif // HOMESCREEN_H
