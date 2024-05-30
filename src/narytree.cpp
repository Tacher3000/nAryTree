#include "narytree.h"

// Реализация методов класса Node

Node::Node(int value, int id) : value(value), id(id){}

int Node::getId()
{
    return id;
}


// Реализация методов класса BinaryTree

NAryTree::NAryTree() : root(nullptr), nodes_amount(0) {}




void NAryTree::iterativInsert(int value, int children) {
    // Если дерево пустое, создаем корень с заданным значением
    if (!root) {
        root = new Node(value, nodes_amount++);
        return;
    }
    // Иначе вызываем вспомогательную функцию для вставки значения
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

// Вставляет случайные значения в дерево
void NAryTree::randomInsertion(int start, int end, int amount, int children) {
    for (int i = 0; i < amount; ++i) {
        // Генерируем случайное значение для вставки
        int value = QRandomGenerator::global()->bounded(start, end + 1);
        // Вызываем функцию для вставки значения в дерево
        iterativInsert(value, children);
    }
}

std::map<int, int> NAryTree::getNodesPerLevel() const {
    std::map<int, int> levelCount; // Словарь для хранения количества узлов на каждом уровне
    if (!root) return levelCount; // Если дерево пустое, возвращаем пустой словарь

    std::queue<std::pair<Node*, int>> queue; // Очередь для обхода дерева, хранящая узлы и их уровни
    queue.push({root, 0}); // Начинаем с корня на уровне 0

    while (!queue.empty()) {
        auto [currentNode, level] = queue.front();
        queue.pop();
        levelCount[level]++; // Увеличиваем счетчик узлов на текущем уровне

        // Добавляем всех дочерних узлов в очередь с увеличенным уровнем
        for (Node* child : currentNode->children) {
            queue.push({child, level + 1});
        }
    }

    return levelCount; // Возвращаем словарь с количеством узлов на каждом уровне
}


void NAryTree::writeNodeToFile(Node* node, QTextStream& out) {
    if (node != nullptr) {
        out << node->value << " " << node->id << " " << node->children.size() << " ";
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

void NAryTree::readTreeFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "readTreeFromFile: Невозможно открыть файл для чтения" << file.errorString();
        return;
    }

    QTextStream in(&file);

    // Очистим текущее дерево, если необходимо
    delete root;
    root = nullptr;
    nodes_amount = 0;

    QMap<int, Node*> nodeMap;  // Для хранения узлов по их идентификаторам

    while (!in.atEnd()) {
        QString line = in.readLine();
        readNodeFromFile(line, nodeMap);
    }

    // Установим корневой узел
    root = nodeMap.value(0);

    file.close();

    if (file.error() != QFile::NoError) {
        qDebug() << "readTreeFromFile: Ошибка при чтении файла" << file.errorString();
    }
}

void NAryTree::readNodeFromFile(const QString& line, QMap<int, Node*>& nodeMap) {
    QStringList parts = line.split(" ");
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
            childNode = new Node(0, childId);  // Временно создаем узел с значением 0
            nodeMap[childId] = childNode;
        }
        node->children.push_back(childNode);
    }

    if (id >= nodes_amount) {
        nodes_amount = id + 1;
    }
}

void NAryTree::readTreeFromText(const QString& text) {
    try {

        // Очистим текущее дерево, если необходимо
        delete root;
        root = nullptr;
        nodes_amount = 0;

        QMap<int, Node*> nodeMap;  // Для хранения узлов по их идентификаторам

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
                childNode = new Node(0, childId);  // Временно создаем узел с значением 0
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

