#include "mainwindow.h"
#include <iostream>

QApplication* MainWindow::application = nullptr;

MainWindow::MainWindow(MacroParser& macroParser,QWidget *parent=nullptr)
    : QMainWindow(parent),macroParser(macroParser)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->excludeLabel->setAlignment(Qt::AlignCenter);
}
MainWindow MainWindow::createGUI(MacroParser& macroParser){
    int x = 0;
    MainWindow::application = new QApplication(x,{});
    MainWindow::application->setQuitOnLastWindowClosed(false);
    return MainWindow(macroParser);
}
MainWindow::~MainWindow()
{
    delete ui;
    for (QListWidgetItem* checkbox : checkBoxes){
        delete checkbox;
    }
    delete MainWindow::application;
}

void MainWindow::showExlusionOptions(std::wstring activeProcessName){
    this->activeProcessName = activeProcessName;
    std::wstring s = L"Exclude from Process \"" + activeProcessName + L"\"";
    ui->excludeLabel->setText(QString::fromStdWString(s));
    for (auto& pair : macroParser.getExcludesMap()){
        QListWidgetItem* item = new QListWidgetItem(pair.first.c_str() , ui->listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        macroParser.getExcludesMap().at(pair.first)->contains(activeProcessName) ?
                    item->setCheckState(Qt::Checked):item->setCheckState(Qt::Unchecked);
        checkBoxes.emplace_back(item);
    }
    static bool executed = false;
    this->show();
    this->setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    this->raise();
    this->activateWindow();
    if (!executed){
        executed = true;
        application->exec();
    }
}

void MainWindow::addExcludeOptionToList(std::string excludeOption){
    QListWidgetItem* item = new QListWidgetItem(excludeOption.c_str() , ui->listWidget);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
    checkBoxes.emplace_back(item);
}

void MainWindow::on_okButton_clicked(){
    std::unordered_map<std::string,ExcludeStatement> excludeStatements = macroParser.getExcludeStatements();
    for (auto checkbox : checkBoxes){
        if (checkbox->checkState() == Qt::Checked ){
            excludeStatements.at(checkbox->text().toStdString()).excludeProgram(activeProcessName);
        }
        else if (checkbox->checkState() == Qt::Unchecked){
            excludeStatements.at(checkbox->text().toStdString()).unexcludeProgram(activeProcessName);
        }
    }
    for (QListWidgetItem* checkbox : checkBoxes){
        delete checkbox;
    }
    checkBoxes.clear();
    this->hide();
}

