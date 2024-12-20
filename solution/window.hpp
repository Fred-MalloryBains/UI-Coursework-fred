// COMP2811 Coursework 2 sample solution: main window

#pragma once

#include <QMainWindow>
#include "model.hpp"
#include "water.hpp"
#include "fchart.hpp"
#include <QChart>
#include <QLineSeries>

class QString;
class QComboBox;
class QLabel;
class QPushButton;
class QTableView;
class StatsDialog;

class WaterWindow : public QMainWindow
{
  Q_OBJECT

public:
  WaterWindow();

private:
  void createFileSelectors();
  void createButtons();
  void createToolBar();
  void createPageBar();
  void createStatusBar();
  void addFileMenu();
  void addHelpMenu();
  void addPOPMenu();
  void updateFileSelector(QComboBox *selector, QStringList options);

  WaterModel model;         // data model used by table
  QString dataLocation;     // location of CSV data files
  QComboBox *pollutant;     // selector for quake feed significance level
  QComboBox *location;      // selector for quake feed time period
  QPushButton *loadButton;  // button to load a new CSV file
  QPushButton *statsButton; // button to display dataset stats
  QPushButton *overviewButton;
  QPushButton *popsButton;
  QPushButton *litterButton;
  QPushButton *flourinatedButton;
  QPushButton *complianceButton;
  QToolBar *toolBar; // toolbar for file and data controls

  QTableView *table;        // table of quake data
  QLabel *fileInfo;         // status bar info on current file
  StatsDialog *statsDialog; // dialog to display stats

  QLabel *pollutantName;

  QLabel *pop;

  QChart *chart;
  QLineSeries *lineView;

private slots:
  void setDataLocation();
  void openCSV();
  void displayStats();
  void about();
  void createPOPs();
  void createTest();
  void createFlourinated();
};
