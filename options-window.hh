#pragma once

#include <QDockWidget>

class QDoubleSpinBox;
class MyWindow;

class OptionsWindow : public QDockWidget  {
  Q_OBJECT

public:
  explicit OptionsWindow(MyWindow *parent);
  double meanRange() const;
  double deviationRange() const;

private:
  MyWindow *parent;
  QDoubleSpinBox *sb_mean, *sb_deviation;
};
