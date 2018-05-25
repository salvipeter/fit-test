#pragma once

#include <QDockWidget>

class QSpinBox;
class QDoubleSpinBox;
class MyWindow;

class OptionsWindow : public QDockWidget  {
  Q_OBJECT

public:
  explicit OptionsWindow(MyWindow *parent);
  size_t resolution() const;
  double meanRange() const;
  double deviationRange() const;
  double tolerance() const;

private:
  MyWindow *parent;
  QSpinBox *sb_res;
  QDoubleSpinBox *sb_mean, *sb_deviation, *sb_tolerance;
};
