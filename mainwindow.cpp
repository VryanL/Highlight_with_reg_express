#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Search");
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QHBoxLayout *topButtons = new QHBoxLayout();
    QPushButton *loadB = new QPushButton("&Load");
    QPushButton *processB = new QPushButton("&Process");
    topButtons->addWidget(loadB,0,Qt::AlignLeft);
    topButtons->addWidget(processB,0,Qt::AlignRight);

    textEdit = new QTextEdit();
    mainLayout->addLayout(topButtons);
    mainLayout->addWidget(textEdit);

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    setMinimumSize(400,300);

    connect(loadB, SIGNAL(clicked(bool)), this, SLOT(openFile()));
    connect(processB, SIGNAL(clicked(bool)), this, SLOT(highlightMatches()));

}

MainWindow::~MainWindow()
{}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            textEdit->setText(in.readAll());
            file.close();
        }
    }
}

void MainWindow::highlightMatches()
{
    QRegularExpression emailAdrReg("[\\w.%+-]*@[\\w.%+-]*");
    QRegularExpression phoneNumRe("\\(\\d{3}\\)\\d{7}|\\(\\d{3}\\)\\d{3}-\\d{4}|\\d{10}|\\d{3}-\\d{7}|\\d{3}-\\d{3}-\\d{4}");

    QTextCursor cursor(textEdit->document());

    QTextCharFormat bold;
    bold.setFontWeight(QFont::Bold);

    while (!cursor.isNull() && !cursor.atEnd()) {
        cursor = textEdit->document()->find(emailAdrReg, cursor);
        if (!cursor.isNull()) {
            cursor.mergeCharFormat(bold);
        }
    }

    cursor = QTextCursor(textEdit->document());
    while (!cursor.isNull() && !cursor.atEnd()) {
        cursor = textEdit->document()->find(phoneNumRe, cursor);
        if (!cursor.isNull()) {
            cursor.mergeCharFormat(bold);
        }
    }

}
