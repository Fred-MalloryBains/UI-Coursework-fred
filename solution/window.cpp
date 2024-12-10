// COMP2811 Coursework 2 sample solution: main window

#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include "window.hpp"
#include "stats.hpp"
#include "water.hpp"

#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QToolBox>
#include <QProgressBar>
#include <QFrame>

static const int MIN_WIDTH = 620;

WaterWindow::WaterWindow() : QMainWindow(), statsDialog(nullptr)
{
  createTest();
  createFileSelectors();
  createButtons();
  createToolBar();
  createStatusBar();
  addFileMenu();
  addHelpMenu();

  createPageBar();

  createPOPs();

  setMinimumWidth(MIN_WIDTH);
  setWindowTitle("Water Tool");
}

void WaterWindow::createTest()
{
  table = new QTableView();
  table->setModel(&model);

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  table->setFont(tableFont);

  setCentralWidget(table);
}

void WaterWindow::createPOPs()
{
  pop = new QLabel("hello world");
  setCentralWidget(pop);
}

void WaterWindow::createFlourinated()
{

  //  Create a container widget for this page
  QWidget *flourinatedWidget = new QWidget();
  QVBoxLayout *layout = new QVBoxLayout();

  // create compliance widgets and allign them
  QLabel *pfaLabel = new QLabel("Select a pollutant to view data ");
  QLabel *locationLabel = new QLabel("Select a location to view the compliance indicator");

  QFrame *complianceBar = new QFrame();
  QHBoxLayout *complianceLayout = new QHBoxLayout();

  complianceLayout->addWidget(complianceLabel);
  complianceLayout->addWidget(complianceBar);
  complianceLayout->setAlignment(Qt::AlignHCenter);
  layout->addLayout(complianceLayout);

  complianceBar->setFrameShape(QFrame::HLine);
  complianceBar->setFixedHeight(10);

  layout->addWidget(pfaLabel);
  layout->addWidget(locationLabel);

  layout->addWidget(complianceBar);

  // data
  FlourineChart *fchart = new FlourineChart();
  if (model.hasData())
  {
    fchart->loadDataset(model.getData());
  }

  // Create file selectors specific to this window

  updateFileSelector(pollutant, fchart->getDeterminands());
  updateFileSelector(location, fchart->getLocations(pollutant->currentText().toStdString()));

  // Create a chart and chart view
  fchart->loadDataset(model.getData());
  QChart *chart = new QChart();
  fchart->initChart(chart);
  std::cout << "chart created" << std::endl;
  QChartView *chartView = new QChartView(chart);
  layout->addWidget(chartView);

  // Connect selectors to a slot for updating the chart
  connect(pollutant, &QComboBox::currentTextChanged, this, [=]()
          { fchart->updateChart(chart, pollutant->currentText().toStdString());
            updateFileSelector(location, fchart->getLocations(pollutant->currentText().toStdString()));
            fchart->updateCompliance(pfaLabel, locationLabel, complianceBar, pollutant->currentText().toStdString(), 
                              location->currentText().toStdString()); });
  connect(location, &QComboBox::currentTextChanged, this, [=]()
          { fchart->updateCompliance(pfaLabel, locationLabel, complianceBar, pollutant->currentText().toStdString(),
                                     location->currentText().toStdString()); });
  chartView->setChart(chart);
  // Set the layout and widget as the central widget
  flourinatedWidget->setLayout(layout);
  setCentralWidget(flourinatedWidget);
}

void WaterWindow::createPageBar()
{
  QToolBar *pageBar = new QToolBar();
  pageBar->addWidget(overviewButton);
  pageBar->addWidget(popsButton);
  pageBar->addWidget(litterButton);
  pageBar->addWidget(flourinatedButton);
  pageBar->addWidget(complianceButton);

  addToolBar(Qt::TopToolBarArea, pageBar);
}

// change this
void WaterWindow::createFileSelectors()
{
  QStringList pollutantOptions;
  pollutantOptions << "pollutants";
  pollutant = new QComboBox();
  pollutant->addItems(pollutantOptions);

  QStringList locationOptions;
  locationOptions << "locations";
  location = new QComboBox();
  location->addItems(locationOptions);
}

void WaterWindow::updateFileSelector(QComboBox *selector, QStringList options)
{
  selector->clear();
  selector->addItems(options);
}

void WaterWindow::createButtons()
{
  loadButton = new QPushButton("Load");
  statsButton = new QPushButton("Stats");
  overviewButton = new QPushButton("Overview");
  popsButton = new QPushButton("POPs");
  litterButton = new QPushButton("Litter Indicators");
  flourinatedButton = new QPushButton("Flourinated compounds");
  complianceButton = new QPushButton("Compliance Dashboard");

  connect(loadButton, SIGNAL(clicked()), this, SLOT(openCSV()));
  connect(statsButton, SIGNAL(clicked()), this, SLOT(displayStats()));
  connect(overviewButton, SIGNAL(clicked()), this, SLOT(createTest()));
  connect(popsButton, SIGNAL(clicked()), this, SLOT(createPOPs()));
  connect(litterButton, SIGNAL(clicked()), this, SLOT(createLitter()));
  connect(flourinatedButton, SIGNAL(clicked()), this, SLOT(createFlourinated()));
  connect(complianceButton, SIGNAL(clicked()), this, SLOT(createCompliance()));
}

void WaterWindow::createToolBar()
{
  QToolBar *toolBar = new QToolBar();

  QLabel *pollutantLabel = new QLabel("Pollutant");
  pollutantLabel->setAlignment(Qt::AlignVCenter);
  toolBar->addWidget(pollutantLabel);
  toolBar->addWidget(pollutant);

  QLabel *locationLabel = new QLabel("location");
  locationLabel->setAlignment(Qt::AlignVCenter);
  toolBar->addWidget(locationLabel);
  toolBar->addWidget(location);

  toolBar->addSeparator();

  toolBar->addWidget(loadButton);
  toolBar->addWidget(statsButton);

  addToolBar(Qt::LeftToolBarArea, toolBar);
}

void WaterWindow::createStatusBar()
{
  fileInfo = new QLabel("Current file: <none>");
  QStatusBar *status = statusBar();
  status->addWidget(fileInfo);
}

void WaterWindow::addFileMenu()
{
  QAction *locAction = new QAction("Set Data &Location", this);
  locAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
  connect(locAction, SIGNAL(triggered()), this, SLOT(setDataLocation()));

  QAction *closeAction = new QAction("Quit", this);
  closeAction->setShortcut(QKeySequence::Close);
  connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

  QMenu *fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction(locAction);
  fileMenu->addAction(closeAction);
}

void WaterWindow::addHelpMenu()
{
  QAction *aboutAction = new QAction("&About", this);
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  QAction *aboutQtAction = new QAction("About &Qt", this);
  connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  QMenu *helpMenu = menuBar()->addMenu("&Help");
  helpMenu->addAction(aboutAction);
  helpMenu->addAction(aboutQtAction);
}

void WaterWindow::setDataLocation()
{
  QString directory = QFileDialog::getExistingDirectory(
      this, "Data Location", ".",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (directory.length() > 0)
  {
    dataLocation = directory;
  }
}

void WaterWindow::openCSV()
{
  if (dataLocation == "")
  {
    QMessageBox::critical(this, "Data Location Error",
                          "Data location has not been set!\n\n"
                          "You can specify this via the File menu.");
    return;
  }

  auto filename = QString("Y-2024.csv");

  auto path = dataLocation + "/" + filename;
  std::cout << path.toStdString() << std::endl;
  try
  {
    model.updateFromFile(path);
  }
  catch (const std::exception &error)
  {
    QMessageBox::critical(this, "CSV File Error", error.what());
    return;
  }

  fileInfo->setText(QString("Current file: <kbd>%1</kbd>").arg(filename));
  table->resizeColumnsToContents();

  if (statsDialog != nullptr)
  {
    statsDialog->hide();
  }
}

// this is the function for the table view
// needs to be updated to view from water using stats dialouge

void WaterWindow::displayStats()
{
  if (model.hasData())
  {
    if (statsDialog == nullptr)
    {
      statsDialog = new StatsDialog(this);
    }

    // statsDialog->update(model.meanDepth(), model.meanMagnitude());

    statsDialog->show();
    statsDialog->raise();
    statsDialog->activateWindow();
  }
}

void WaterWindow::about()
{
  QMessageBox::about(this, "About Quake Tool",
                     "Quake Tool displays and analyzes earthquake data loaded from"
                     "a CSV file produced by the USGS Earthquake Hazards Program.\n\n"
                     "(c) 2024 Nick Efford");
}
