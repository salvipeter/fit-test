#include "MyWindow.h"

#include <memory>

#include <QtWidgets>

#include "MyViewer.h"
#include "options-window.hh"

MyWindow::MyWindow(QApplication *parent) :
  QMainWindow(), parent(parent), last_directory(".")
{
  setMinimumSize(1024, 768);
  setWindowTitle(tr("Fit Testing Framework"));
  setStatusBar(new QStatusBar);
  progress = new QProgressBar;
  progress->setMinimum(0); progress->setMaximum(100);
  progress->hide();
  statusBar()->addPermanentWidget(progress);

  viewer = new MyViewer(this);
  connect(viewer, SIGNAL(startComputation(QString)), this, SLOT(startComputation(QString)));
  connect(viewer, SIGNAL(midComputation(int)), this, SLOT(midComputation(int)));
  connect(viewer, SIGNAL(endComputation()), this, SLOT(endComputation()));
  setCentralWidget(viewer);

  options_window = new OptionsWindow(this);
  addDockWidget(Qt::LeftDockWidgetArea, options_window);

  /////////////////////////
  // Setup actions/menus //
  /////////////////////////

  auto openAction = new QAction(tr("&Open"), this);
  openAction->setShortcut(tr("Ctrl+O"));
  openAction->setStatusTip(tr("Load a model from a file"));
  connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

  auto quitAction = new QAction(tr("&Quit"), this);
  quitAction->setShortcut(tr("Ctrl+Q"));
  quitAction->setStatusTip(tr("Quit the program"));
  connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

  auto fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openAction);
  fileMenu->addAction(quitAction);
}

MyWindow::~MyWindow() {
}

void MyWindow::open() {
  auto filename =
    QFileDialog::getOpenFileName(this, tr("Open File"), last_directory,
                                 tr("Mesh (*.obj *.ply *.stl);;"
                                    "BSpline surface (*.bsp);;"
                                    "All files (*.*)"));
  if(filename.isEmpty())
    return;
  last_directory = QFileInfo(filename).absolutePath();

  bool ok;
  if (filename.endsWith(".bsp"))
    ok = viewer->openBSpline(filename.toUtf8().data());
  else
    ok = viewer->openMesh(filename.toUtf8().data());

  if (!ok)
    QMessageBox::warning(this, tr("Cannot open file"),
                         tr("Could not open file: ") + filename + ".");
}

void
MyWindow::updateRanges() {
  viewer->setRanges(options_window->meanRange(), options_window->deviationRange());
}

void
MyWindow::fit() {
  viewer->fit();
}

void MyWindow::startComputation(QString message) {
  statusBar()->showMessage(message);
  progress->setValue(0);
  progress->show();
  parent->processEvents(QEventLoop::ExcludeUserInputEvents);
}

void MyWindow::midComputation(int percent) {
  progress->setValue(percent);
  parent->processEvents(QEventLoop::ExcludeUserInputEvents);
}

void MyWindow::endComputation() {
  progress->hide();
  statusBar()->clearMessage();
}
