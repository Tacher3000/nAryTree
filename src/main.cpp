#include <QApplication>
#include "mainwindow.h"

#include <QRandomGenerator>
#include <QDateTime>
#include <QDebug>
#include <QElapsedTimer>
#include <chrono>
#include "narytree.h"


void timeTest();

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow w;
    w.showMaximized();
    // timeTest();

    return app.exec();
}


void timeTest(){
    NAryTree tree;

    const int minNodes = 900000;
    const int maxNodes = 1000000;
    const int step = 10000;
    const int iterations = 5;

    for (int numNodes = minNodes; numNodes <= maxNodes; numNodes += step) {
        long long recursiveTotalTime = 0;
        long long iterativeTotalTime = 0;

        for (int i = 0; i < iterations; ++i) {
            QVector<int> values;
            for (int j = 0; j < numNodes; ++j) {
                values.append(QRandomGenerator::global()->generate());
            }

            auto startRecursive = std::chrono::high_resolution_clock::now();
            for (int value : values) {
                // tree.recursiveInsert(value);
            }
            auto endRecursive = std::chrono::high_resolution_clock::now();
            recursiveTotalTime += std::chrono::duration_cast<std::chrono::milliseconds>(endRecursive - startRecursive).count();

            tree = NAryTree();

            auto startIterative = std::chrono::high_resolution_clock::now();
            for (int value : values) {
                // tree.iterativInsert(value);
            }
            auto endIterative = std::chrono::high_resolution_clock::now();
            iterativeTotalTime += std::chrono::duration_cast<std::chrono::milliseconds>(endIterative - startIterative).count();

            tree = NAryTree();
        }

        qDebug() << "Number of nodes:" << numNodes;
        qDebug() << "Recursive average time (ms):" << recursiveTotalTime / iterations;
        qDebug() << "Iterative average time (ms):" << iterativeTotalTime / iterations;
    }
}
