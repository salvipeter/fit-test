#include "options-window.hh"

#include <QtWidgets>

#include "MyWindow.h"

OptionsWindow::OptionsWindow(MyWindow *parent) : parent(parent) {
  setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable); // not closable
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  sb_res = new QSpinBox;
  sb_res->setRange(5, 100);
  sb_res->setValue(30);         // default value in BSplineModel
  connect(sb_res, SIGNAL(valueChanged(int)), parent, SLOT(updateRanges()));

  sb_mean = new QDoubleSpinBox;
  sb_mean->setDecimals(5);
  sb_mean->setRange(0.0, 1000.0);
  sb_mean->setSingleStep(0.01);
  sb_mean->setValue(0.0);
  connect(sb_mean, SIGNAL(valueChanged(double)), parent, SLOT(updateRanges()));

  sb_deviation = new QDoubleSpinBox;
  sb_deviation->setDecimals(5);
  sb_deviation->setRange(0.0, 1000.0);
  sb_deviation->setSingleStep(0.01);
  sb_deviation->setValue(0.0);
  connect(sb_deviation, SIGNAL(valueChanged(double)), parent, SLOT(updateRanges()));

  auto *gb_vis = new QGroupBox("Visualization options");
  auto *form_vis = new QFormLayout;
  form_vis->addRow(tr("Surface resolution"), sb_res);
  form_vis->addRow(tr("Mean map range"), sb_mean);
  form_vis->addRow(tr("Deviation range"), sb_deviation);
  gb_vis->setLayout(form_vis);

  auto *sb_tolerance = new QDoubleSpinBox;
  sb_tolerance->setDecimals(5);
  sb_tolerance->setRange(0.0, 1000.0);
  sb_tolerance->setSingleStep(0.01);
  sb_tolerance->setValue(0.0);

  auto *pb_fit = new QPushButton(tr("Fit"));
  connect(pb_fit, SIGNAL(pressed()), parent, SLOT(fit()));

  auto *gb_fit = new QGroupBox("Fitting options");
  auto *form_fit = new QFormLayout;
  form_fit->addRow(tr("Fitting tolerance"), sb_tolerance);
  form_fit->addRow(pb_fit);
  gb_fit->setLayout(form_fit);

  auto *vb = new QVBoxLayout;
  vb->addWidget(gb_vis);
  vb->addWidget(gb_fit);
  auto *w = new QWidget;
  w->setLayout(vb);
  auto *sa = new QScrollArea;
  sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  sa->setWidget(w);
  setWidget(sa);
}

size_t
OptionsWindow::resolution() const {
  return sb_res->value();
}

double
OptionsWindow::meanRange() const {
  return sb_mean->value();
}

double
OptionsWindow::deviationRange() const {
  return sb_deviation->value();
}
