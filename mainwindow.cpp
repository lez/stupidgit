#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setUnifiedTitleAndToolBarOnMac(true);
    ui->setupUi(this);
    repo = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionOpen_Repository_triggered()
{
    QString directory = QFileDialog::getExistingDirectory(this, "Open Repository", ".", QFileDialog::ShowDirsOnly);
    if (!directory.length()) {
        return;
    }

    if (repo) {
        delete repo;
    }

    // Open repository
    repo = new GitRepository(this);

    if (!repo->setDirectory(directory)) {
        QMessageBox::critical(this, "Error opening directory", "The given directory is not a git repository");
        delete repo;
        repo = 0;
        return;
    }

    // Run git status
    connect(repo, SIGNAL(refreshed()), this, SLOT(onRepoRefreshed()));
    repo->refresh();
}

void MainWindow::onRepoRefreshed()
{
    QString text;
    text.append("Repository refreshed.\nUnstaged changes:");

    for (QMapIterator<QString,GitFileStatus> i(repo->getUnstagedChanges()); i.hasNext(); ) {
        i.next();
        text.append(i.key());
        text.append(": ");
        text.append((int)i.value());
        text.append("\n");
    }

    text.append("\nStaged changes:\n");
    for (QMapIterator<QString,GitFileStatus> i(repo->getStagedChanges()); i.hasNext(); ) {
        i.next();
        text.append(i.key());
        text.append(": ");
        text.append((int)i.value());
        text.append("\n");
    }

    ui->textBrowser->setText(text);
}
