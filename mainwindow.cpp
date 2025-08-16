#include "mainwindow.h"
#include <QCoreApplication>
#include <QDir>
#include <QSplitter>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    canvas = new Canvas();

    QStringList args = QCoreApplication::arguments();
    if(args.length() > 1){
        filePath = QDir(args[1]).path();
        parser = new SvgParser(filePath, canvas);
    }

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(canvas);
    canvas->setMinimumSize(1000, 1000);
    scrollArea->setStyleSheet(R"(
        background-color: #FFFFFF;
        border: 0px solid;
    )");
    sideBar = new QWidget(this);


    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(splitter);

    splitter->addWidget(sideBar);
    splitter->addWidget(scrollArea);
    splitter->setSizes({20,80});

    QVBoxLayout *vlo = new QVBoxLayout(sideBar);
    btnOpen = new QPushButton("Open");
    vlo->addWidget(btnOpen);
    vlo->addStretch(1);

    connect(btnOpen, &QPushButton::clicked, this, &MainWindow::openFile);
}

MainWindow::~MainWindow() {}

void MainWindow::openFile(){
    filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open Document"),
        "",
        tr("*.svg")
    );
    if(filePath != "")
        parser = new SvgParser(filePath, canvas);
}
