// -*- mode: c++ -*-
#pragma once

#include <QtWidgets/QMainWindow>

class QApplication;
class QProgressBar;
class MyViewer;
class OptionsWindow;

class MyWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MyWindow(QApplication *parent);
  ~MyWindow();

private slots:
  void open();
  void updateRanges();
  void fit();
  void startComputation(QString message);
  void midComputation(int percent);
  void endComputation();

private:
  QApplication *parent;
  MyViewer *viewer;
  OptionsWindow *options_window;
  QProgressBar *progress;
  QString last_directory;
};
