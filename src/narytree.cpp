#include "narytree.h"

// Реализация методов класса Node

Node::Node(int value, int id) : value(value), id(id){}

int Node::getId()
{
    return id;
}
Node::~Node() {
    for (Node* child : children) {
        delete child;
    }
}


// Реализация методов класса BinaryTree

NAryTree::NAryTree() : root(nullptr), nodes_amount(0) {}



NAryTree::~NAryTree() {
    m_deleteTree(root);
}

void NAryTree::m_deleteTree(Node* node) {
    if (node) {
        for (Node* child : node->children) {
            m_deleteTree(child);
        }
        delete node;
    }
}

void NAryTree::iterativInsert(int value, int children) {
    if (!root) {
        root = new Node(value, nodes_amount++);
        return;
    }
    m_iterativInsertNode(value, root, children);
}

Node* NAryTree::m_iterativInsertNode(int value, Node* node, int children) {
    while (true) {
        if (node->children.size() < children) {
            node->children.push_back(new Node(value, nodes_amount++));
            return node;
        } else {
            int randomIndex = QRandomGenerator::global()->bounded(0, static_cast<int>(node->children.size()));
            node = node->children[randomIndex];
        }
    }
}

void NAryTree::randomInsertion(int start, int end, int amount, int children) {
    for (int i = 0; i < amount; ++i) {
        int value = QRandomGenerator::global()->bounded(start, end + 1);
        iterativInsert(value, children);
    }
}

std::map<int, int> NAryTree::getNodesPerLevel() const {
    std::map<int, int> levelCount;
    if (!root) return levelCount;

    std::queue<std::pair<Node*, int>> queue;
    queue.push({root, 0});

    while (!queue.empty()) {
        auto [currentNode, level] = queue.front();
        queue.pop();
        levelCount[level]++;

        for (Node* child : currentNode->children) {
            queue.push({child, level + 1});
        }
    }

    return levelCount;
}


// void NAryTree::writeNodeToFile(Node* node, QTextStream& out) {
//     if (node != nullptr) {
//         out << node->value << " " << node->id << " " << node->children.size() << " ";
//         for (Node* child : node->children) {
//             out << child->id << " ";
//         }
//         out << "\n";
//         for (Node* child : node->children) {
//             writeNodeToFile(child, out);
//         }
//     }
// }

// void NAryTree::writeTreeToFile(const QString& filename) {
//     QElapsedTimer timer;
//     timer.start();

//     QFile file(filename);
//     if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//         qDebug() << "writeTreeToFile: Невозможно открыть файл для записи" << file.errorString();
//         return;
//     }

//     QTextStream out(&file);
//     writeNodeToFile(root, out);

//     file.close();

//     if (file.error() != QFile::NoError) {
//         qDebug() << "writeTreeToFile: Ошибка при записи файла" << file.errorString();
//     }

//     qDebug() << "writeTreeToFile" << timer.elapsed() << "milliseconds";
// }

// void NAryTree::readTreeFromFile(const QString& filename) {
//     QFile file(filename);
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         qDebug() << "readTreeFromFile: Невозможно открыть файл для чтения" << file.errorString();
//         return;
//     }

//     QTextStream in(&file);

//     delete root;
//     root = nullptr;
//     nodes_amount = 0;

//     QMap<int, Node*> nodeMap;

//     while (!in.atEnd()) {
//         QString line = in.readLine();
//         readNodeFromFile(line, nodeMap);
//     }

//     // Установим корневой узел
//     root = nodeMap.value(0);

//     file.close();

//     if (file.error() != QFile::NoError) {
//         qDebug() << "readTreeFromFile: Ошибка при чтении файла" << file.errorString();
//     }
// }

// void NAryTree::readNodeFromFile(const QString& line, QMap<int, Node*>& nodeMap) {
//     QStringList parts = line.split(" ");
//     if (parts.size() < 3) return;

//     int value = parts[0].toInt();
//     int id = parts[1].toInt();
//     int childrenCount = parts[2].toInt();

//     Node* node = nodeMap.value(id, nullptr);
//     if (!node) {
//         node = new Node(value, id);
//         nodeMap[id] = node;
//     } else {
//         node->value = value;
//     }

//     for (int i = 0; i < childrenCount; ++i) {
//         int childId = parts[3 + i].toInt();
//         Node* childNode = nodeMap.value(childId, nullptr);
//         if (!childNode) {
//             childNode = new Node(0, childId);
//             nodeMap[childId] = childNode;
//         }
//         node->children.push_back(childNode);
//     }

//     if (id >= nodes_amount) {
//         nodes_amount = id + 1;
//     }
// }

void NAryTree::writeNodeToFile(Node* node, QTextStream& out) {
    if (node != nullptr) {
        out << node->value << " " << node->id << " ";
        for (Node* child : node->children) {
            out << child->id << " ";
        }
        out << "\n";
        for (Node* child : node->children) {
            writeNodeToFile(child, out);
        }
    }
}


void NAryTree::writeTreeToFile(const QString& filename) {
    QElapsedTimer timer;
    timer.start();

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "writeTreeToFile: Невозможно открыть файл для записи" << file.errorString();
        return;
    }

    QTextStream out(&file);
    writeNodeToFile(root, out);

    file.close();

    if (file.error() != QFile::NoError) {
        qDebug() << "writeTreeToFile: Ошибка при записи файла" << file.errorString();
    }

    qDebug() << "writeTreeToFile" << timer.elapsed() << "milliseconds";
}

void NAryTree::readNodeFromFile(const QString& line, QMap<int, Node*>& nodeMap) {
    QStringList parts = line.split(" ", Qt::SkipEmptyParts);
    if (parts.size() < 2) {
        qDebug() << "readNodeFromFile: Incorrect format, line ignored:" << line;
        return;
    }

    bool valueOk, idOk;
    int value = parts[0].toInt(&valueOk);
    int id = parts[1].toInt(&idOk);

    if (!valueOk || !idOk) {
        qDebug() << "readNodeFromFile: Conversion error, line ignored:" << line;
        return;
    }

    Node* node = nodeMap.value(id, nullptr);
    if (!node) {
        node = new Node(value, id);
        nodeMap[id] = node;
    } else {
        node->value = value;
    }

    for (int i = 2; i < parts.size(); ++i) {
        bool childIdOk;
        int childId = parts[i].toInt(&childIdOk);
        if (!childIdOk) {
            qDebug() << "readNodeFromFile: Child ID conversion error, part ignored:" << parts[i];
            continue;
        }

        Node* childNode = nodeMap.value(childId, nullptr);
        if (!childNode) {
            childNode = new Node(0, childId);
            nodeMap[childId] = childNode;
        }
        node->children.push_back(childNode);
    }

    if (id >= nodes_amount) {
        nodes_amount = id + 1;
    }
}


void NAryTree::readTreeFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "readTreeFromFile: Невозможно открыть файл для чтения" << file.errorString();
        return;
    }

    QTextStream in(&file);

    delete root;
    root = nullptr;
    nodes_amount = 0;

    QMap<int, Node*> nodeMap;

    while (!in.atEnd()) {
        QString line = in.readLine();
        readNodeFromFile(line, nodeMap);
    }

    // Установим корневой узел
    root = nodeMap.value(0, nullptr);
    if (!root) {
        qDebug() << "readTreeFromFile: Корневой узел не найден";
    }

    file.close();

    if (file.error() != QFile::NoError) {
        qDebug() << "readTreeFromFile: Ошибка при чтении файла" << file.errorString();
    }
}



void NAryTree::readTreeFromText(const QString& text) {
    try {
        delete root;
        root = nullptr;
        nodes_amount = 0;

        QMap<int, Node*> nodeMap;

        QStringList lines = text.split('\n', Qt::SkipEmptyParts);

        for (const QString& line : lines) {
            readNodeFromText(line, nodeMap);
        }

        // Установим корневой узел
        root = nodeMap.value(0);
    } catch (...) {
    }
}

void NAryTree::readNodeFromText(const QString& line, QMap<int, Node*>& nodeMap) {
    try {
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        if (parts.size() < 3) return;

        int value = parts[0].toInt();
        int id = parts[1].toInt();
        int childrenCount = parts[2].toInt();

        Node* node = nodeMap.value(id, nullptr);
        if (!node) {
            node = new Node(value, id);
            nodeMap[id] = node;
        } else {
            node->value = value;
        }

        for (int i = 0; i < childrenCount; ++i) {
            int childId = parts[3 + i].toInt();
            Node* childNode = nodeMap.value(childId, nullptr);
            if (!childNode) {
                childNode = new Node(0, childId);
                nodeMap[childId] = childNode;
            }
            node->children.push_back(childNode);
        }

        if (id >= nodes_amount) {
            nodes_amount = id + 1;
        }
    } catch (...) {
    }
}

void NAryTree::printTree(QTextEdit *textEdit) {
    QString treeOutput;
    m_printTreeRecursive(root, "", true, treeOutput);
    textEdit->setText(treeOutput);
}




void NAryTree::m_printTreeRecursive(Node* node, const QString& prefix, bool isLast, QString& treeOutput) {
    if (node != nullptr) {
        treeOutput += prefix;
        treeOutput += (isLast ? "└── " : "├── ");
        treeOutput += QString::number(node->value) + "\n";

        QString childPrefix = prefix + (isLast ? "       " : "│       ");

        int childCount = node->children.size();
        for (int i = 0; i < childCount; ++i) {
            m_printTreeRecursive(node->children[i], childPrefix, i == childCount - 1, treeOutput);
        }
    }
}

Node *NAryTree::getRoot()
{
    return root;
}

