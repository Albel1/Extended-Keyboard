#pragma once
#include <QMainWindow>
#include <vector>
#include "./ui_mainwindow.h"
#include "MacroParser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(MacroParser& macroParser,QWidget *parent);
    ~MainWindow();
    void showExlusionOptions(std::wstring activeProcessName);
    static MainWindow createGUI(MacroParser& macroParser);

private slots:
    void on_okButton_clicked();

private:
    void addExcludeOptionToList(std::string excludeOption);
    Ui::MainWindow *ui;
    std::vector<QListWidgetItem*> checkBoxes;
    static QApplication* application;
    MacroParser macroParser;
    std::wstring activeProcessName;
};
