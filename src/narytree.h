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

class Node {
public:
    Node(int value, int id);

    int getId();
    int value;
    int id;
    // QList<Node*> children;
    std::vector<Node*> children;
};

class NAryTree
{
public:
    // Конструктор
    NAryTree();

    // Метод для вставки нового значения в дерево
    // void recursiveInsert(int value);

    void iterativInsert(int value, int children);

    // Метод для случайной вставки значений в дерево
    void randomInsertion(int start, int end, int amount, int children);


    void writeNodeToFile(Node *node, QTextStream &out);

    void writeTreeToFile(const QString &filename);

    // void addNodeToTreeRecursive(Node *currentNode, Node *newNode);

    // void addNodeToTree(Node *node);

    void readTreeFromFile(const QString &filename);

    // void readTreeFromText(const QString &text);

    void printTree(QTextEdit *textEdit);

    Node *getRoot();


    void readNodeFromFile(const QString &line, QMap<int, Node *> &nodeMap);
    void readTreeFromText(const QString &text);
    void readNodeFromText(const QString &line, QMap<int, Node *> &nodeMap);
    std::map<int, int> getNodesPerLevel() const;
    // void m_printTreeRecursive(Node *node, const QString &prefix, bool isLast);
private:
    // Node* m_recursiveInsertNode(int value, Node* node);

    // Node* m_iterativInsertNode(int value, Node* node);
    Node* m_iterativInsertNode(int value, Node* node, int children);

    void m_printTreeRecursive(Node *node, const QString &prefix, bool isLast, QString &treeOutput);

private:
    Node* root;

    // Количество узлов в дереве
    int nodes_amount;
};

#endif // NARYTREE_H
