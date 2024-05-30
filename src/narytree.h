#ifndef NARYTREE_H
#define NARYTREE_H

#include <QList>
#include <QMap>
#include <QVariant>
#include <QDebug>
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QtMath>
#include <QFile>
#include <QTextEdit>
#include <QElapsedTimer>
#include <QStack>
#include <QQueue>
#include <queue>

// Класс узла дерева
class Node {
public:
    Node(int value, int id);
    ~Node();

    int getId();
    int value;
    int id;
    std::vector<Node*> children;
};

// Класс N-арного дерева
class NAryTree
{
public:
    // Конструктор дерева
    NAryTree();

    // Деструктор дерева
    ~NAryTree();

    // Метод для итеративной вставки узла в дерево
    void iterativInsert(int value, int children);

    // Метод для случайной вставки значений в дерево
    void randomInsertion(int start, int end, int amount, int children);

    // Метод для записи узла в файл
    void writeNodeToFile(Node *node, QTextStream &out);

    // Метод для записи дерева в файл
    void writeTreeToFile(const QString &filename);

    // Метод для чтения дерева из файла
    void readTreeFromFile(const QString &filename);

    // Метод для вывода дерева в текстовый виджет
    void printTree(QTextEdit *textEdit);

    // Метод для получения корневого узла дерева
    Node *getRoot();

    // Метод для чтения узла из строки файла
    void readNodeFromFile(const QString &line, QMap<int, Node *> &nodeMap);

    // Метод для чтения дерева из текстовой строки
    void readTreeFromText(const QString &text);

    // Метод для чтения узла из текстовой строки
    void readNodeFromText(const QString &line, QMap<int, Node *> &nodeMap);

    // Метод для получения количества узлов на каждом уровне дерева
    std::map<int, int> getNodesPerLevel() const;

private:
    // Вспомогательный метод для итеративной вставки узла в дерево
    Node* m_iterativInsertNode(int value, Node* node, int children);

    // Вспомогательный метод для рекурсивного вывода дерева
    void m_printTreeRecursive(Node *node, const QString &prefix, bool isLast, QString &treeOutput);

    // Вспомогательный метод для удаления дерева
    void m_deleteTree(Node *node);

private:
    Node* root;

    int nodes_amount;
};

#endif // NARYTREE_H
