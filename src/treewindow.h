#ifndef TREEWINDOW_H
#define TREEWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVariantMap>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QTextEdit>
#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QRadioButton>
#include <QButtonGroup>

#include "narytree.h"
#include "scalablegraphicsview.h"

constexpr qreal NODE_WIDTH = 50;
constexpr qreal NODE_HEIGHT = 50;

class TreeWindow : public QWidget
{
    Q_OBJECT
public:
    enum OutputType {
        TextOutput,
        GraphicOutput,
        BothOutput
    };

    explicit TreeWindow(QWidget *parent = nullptr);
    void randomInsertion(int start, int end, int amount, bool checkBox, QString pathDic, int output, int children);
    void enterTree();
    void fromFile(const QString &filePath, OutputType outputType);
    void textEditChanged();
    void searchValueLevel();
    void changeNodeColor(Node *node, const QColor &color);

    void addLevelGrid(const std::map<int, int> &nodesPerLevel);
signals:
    void clickedExit();
private slots:
    void exit();
    void reGen();
    void textToTree();
private:
     // Вычисляет ширину уровня в дереве
     qreal calculateLevelWidth(Node *node);
     // Добавляет узлы дерева на сцену
     void addNodesToScene(QGraphicsScene& scene, Node* node, qreal x, qreal y, qreal offsetY, qreal levelWidth);
private:

    NAryTree *m_tree;
    QGraphicsScene *m_scene;
    ScalableGraphicsView *m_view;

    QLabel m_quantityLabel;

    QPushButton *m_reGenButton;
    QPushButton *m_searchButton;
    QPushButton *m_exitButton;
    QPushButton *m_textToTree;

    int m_start;
    int m_end;
    int m_quantity;
    int m_output;
    int m_children;

    bool m_checkBox;
    QString m_patchDic;

    QTextEdit *m_textEdit;
    QTextEdit *m_treeOutput;

    QCheckBox *m_dynamicUpdateCheckBox;
    NAryTree *m_searchStructure;
};

#endif // TREEWINDOW_H
