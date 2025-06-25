#pragma once
#include "Component.h"

namespace reflect {
class REFLECT_API ProgressBar : public Component {
private:
  float minValue;
  float value;
  float maxValue;

public:
  // COMPONENT_DECL(ProgressBar);
  ProgressBar();
  void setValue(float value);
  void setMaximumValue(float value);
  void setMininumValue(float value);

  float getValue();
  float getMaximumValue();
  float getMinimumValue();
  void onPaint(PaintEvent pe) override;
};
} // namespace reflect