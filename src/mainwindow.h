#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "homescreen.h"
#include "gendialog.h"
#include "treewindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QDialogButtonBox>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void openGenDialog();
    void homeScreen();
    void enterTree();
    void searchTree();
private:
    QStackedWidget *m_stackedWidget;
    HomeScreen *m_homeScreen;
    TreeWindow *m_treeWindow;
};

#endif // MAINWINDOW_H
