#include "treewindow.h"

TreeWindow::TreeWindow(QWidget *parent)
    : QWidget{parent}
{
    m_tree = new NAryTree;
    m_searchStructure = new NAryTree;
    m_scene = new QGraphicsScene(this);
    m_view = new ScalableGraphicsView(this);

    m_reGenButton = new QPushButton(this);
    m_reGenButton->setText("ПЕРЕГЕНЕРИРОВАТЬ");
    m_reGenButton->setMinimumSize(400, 50);
    connect(m_reGenButton, &QPushButton::clicked, this, &TreeWindow::reGen);

    m_searchButton = new QPushButton(this);
    m_searchButton->setText("ПОСЧИТАТЬ КОЛЛИЧЕСТВО ВЕРШИН");
    m_searchButton->setMinimumSize(400, 50);
    connect(m_searchButton, &QPushButton::clicked, this, &TreeWindow::searchValueLevel);

    m_exitButton = new QPushButton(this);
    m_exitButton->setText("ВЕРНУТЬСЯ НА ГЛАВНЫЙ ЭКРАН");
    m_exitButton->setMinimumSize(400, 50);
    connect(m_exitButton, &QPushButton::clicked, this, &TreeWindow::exit);

    m_textToTree = new QPushButton(this);
    m_textToTree->setText("ВВЕСТИ ДЕРЕВО");
    m_textToTree->setMinimumSize(400, 50);
    connect(m_textToTree, &QPushButton::clicked, this, &TreeWindow::textToTree);

    m_textEdit = new QTextEdit(this);

    m_treeOutput = new QTextEdit(this);

    m_dynamicUpdateCheckBox = new QCheckBox("Динамическое обновление", this);
    connect(m_textEdit, &QTextEdit::textChanged, this, &TreeWindow::textEditChanged);
}


void TreeWindow::randomInsertion(int start, int end, int quantity, bool checkBox, QString pathDic, int output, int children)
{
    m_textEdit->setVisible(0);
    m_textToTree->setVisible(0);
    m_dynamicUpdateCheckBox->setVisible(0);
    m_quantityLabel.setText("КОЛЛИЧЕСТВО ВЕРШИН: " + QString::number(quantity));

    m_start = start;
    m_end = end;
    m_quantity = quantity;
    m_checkBox = checkBox;
    m_patchDic = pathDic;
    m_output = output;
    m_children = children;
    m_tree->randomInsertion(start, end, quantity, children);

    if(checkBox){
        m_tree->writeTreeToFile(pathDic + "/" + QString::number(quantity) + ".txt");
    }

    QHBoxLayout * mainLayout = new QHBoxLayout(this);

    qreal levelWidth;
    switch (output) {
    case 1:
        m_tree->printTree(m_treeOutput);
        mainLayout->addWidget(m_treeOutput);
        break;
    case 2:
        m_treeOutput->setVisible(0);
        levelWidth = calculateLevelWidth(m_tree->getRoot());

        addNodesToScene(*m_scene, m_tree->getRoot(), 0, 0, 200, levelWidth);

        m_view->setScene(m_scene);
        mainLayout->addWidget(m_view);
        break;
    default:
        m_treeOutput->setVisible(0);
        break;
    }

    QVBoxLayout * buttonLayout = new QVBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(&m_quantityLabel);
    buttonLayout->addWidget(m_reGenButton);
    buttonLayout->addWidget(m_searchButton);
    buttonLayout->addWidget(m_exitButton);

    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

void TreeWindow::enterTree()
{
    m_reGenButton->setVisible(0);
    m_treeOutput->setVisible(0);
    m_textEdit->setReadOnly(false);

    qreal levelWidth = calculateLevelWidth(m_tree->getRoot());

    addNodesToScene(*m_scene, m_tree->getRoot(), 0, 0, 200, levelWidth);

    m_view->setScene(m_scene);

    QVBoxLayout * buttonLayout = new QVBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(m_textEdit);
    buttonLayout->addWidget(m_dynamicUpdateCheckBox);
    buttonLayout->addWidget(m_textToTree);
    buttonLayout->addWidget(m_searchButton);
    buttonLayout->addWidget(m_exitButton);

    QHBoxLayout * mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_view);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

void TreeWindow::fromFile(const QString &filePath, OutputType outputType)
{
    m_reGenButton->setVisible(0);
    m_textToTree->setVisible(0);
    m_treeOutput->setVisible(0);
    m_dynamicUpdateCheckBox->setVisible(0);
    m_textEdit->setReadOnly(true);


    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString fileContent = in.readAll();
        file.close();

        m_textEdit->setText(fileContent);
    }
    else
    {
        m_textEdit->setText("Ошибка открытия файла!");
    }

    m_tree->readTreeFromFile(filePath);

    qreal levelWidth;
    QVBoxLayout * buttonLayout = new QVBoxLayout();
    QHBoxLayout * mainLayout = new QHBoxLayout(this);

    switch (outputType) {
    case TextOutput:
        buttonLayout->addWidget(m_textEdit);
        break;
    case GraphicOutput:
        m_textEdit->setVisible(0);
        levelWidth = calculateLevelWidth(m_tree->getRoot());
        addNodesToScene(*m_scene, m_tree->getRoot(), 0, 0, 200, levelWidth);
        m_view->setScene(m_scene);
        mainLayout->addWidget(m_view);
        break;
    case BothOutput:
        levelWidth = calculateLevelWidth(m_tree->getRoot());
        addNodesToScene(*m_scene, m_tree->getRoot(), 0, 0, 200, levelWidth);
        m_view->setScene(m_scene);
        mainLayout->addWidget(m_view);
        buttonLayout->addWidget(m_textEdit);
        break;
    }

    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(m_searchButton);
    buttonLayout->addWidget(m_exitButton);


    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

void TreeWindow::exit()
{
    emit clickedExit();
}

void TreeWindow::reGen()
{
    m_scene->clear();
    delete m_tree;
    m_tree = new NAryTree;

    randomInsertion(m_start, m_end, m_quantity, m_checkBox, m_patchDic, m_output, m_children);
}

void TreeWindow::textToTree()
{
    delete m_tree;
    m_tree = new NAryTree;
    m_scene->clear();
    m_tree->readTreeFromText(m_textEdit->toPlainText());

    qreal levelWidth = calculateLevelWidth(m_tree->getRoot());
    addNodesToScene(*m_scene, m_tree->getRoot(), 0, 0, 200, levelWidth);

    m_view->setScene(m_scene);
}

void TreeWindow::addNodesToScene(QGraphicsScene &scene, Node *node, qreal x, qreal y, qreal offsetY, qreal levelWidth) {
    if (node == nullptr)
        return;

    qreal numChildren = node->children.size();
    qreal spaceBetweenChildren = levelWidth / numChildren;
    qreal currentX = x - (levelWidth / 2);

    for (size_t i = 0; i < numChildren; ++i) {
        Node *child = node->children[i];
        qreal childX = currentX + (i * spaceBetweenChildren);
        addNodesToScene(scene, child, childX, y + offsetY, offsetY, spaceBetweenChildren);

        // Add line from current node to its child
        scene.addLine(x + NODE_WIDTH / 2, y + NODE_HEIGHT, childX + NODE_WIDTH / 2, y + offsetY);
    }

    // Add current node
    QGraphicsEllipseItem* ellipseItem = scene.addEllipse(x, y, NODE_WIDTH, NODE_HEIGHT);
    ellipseItem->setBrush(Qt::lightGray);
    ellipseItem->setData(0, node->id);

    qreal centerX = x + NODE_WIDTH / 2;
    qreal centerY = y + NODE_HEIGHT / 2;

    QGraphicsTextItem* textItem = scene.addText(QString::number(node->value));
    textItem->setPos(centerX - textItem->boundingRect().width() / 2, centerY - textItem->boundingRect().height() / 2);
    textItem->setFont(QFont("Arial", 10, QFont::Bold));
}

qreal TreeWindow::calculateLevelWidth(Node *node) {
    if (node == nullptr)
        return 0;

    qreal numChildren = node->children.size();
    qreal childrenWidth = 0;
    for (size_t i = 0; i < numChildren; ++i) {
        childrenWidth += calculateLevelWidth(node->children[i]);
    }

    return qMax(NODE_WIDTH * numChildren + 50 * (numChildren - 1), childrenWidth);
}


void TreeWindow::textEditChanged() {
    if (m_dynamicUpdateCheckBox->isChecked()) {
        textToTree();
    }
}


void TreeWindow::searchValueLevel() {

    QString fileName = QFileDialog::getSaveFileName(this, "Save Matrix File");

    if (!fileName.isEmpty()) {

        std::map<int, int> valueNodes = m_tree->getNodesPerLevel();

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            for (const auto& [level, count] : valueNodes) {
                stream << level << ": " << count << "\n";
            }
            file.close();
        }
    }

}
