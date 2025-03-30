#include "../../core/CToastAPI.h"

#include "CToastC.h"

using namespace CinnamonToast;

CTOAST_API CToastComponent CToast_getComponentById(CToastString id) {

  return {(uint8_t)ExternalAPI::getComponentById(id)};
};
CTOAST_API bool CToast_setColor(CToastComponent component, uint8_t r, uint8_t g,
                                uint8_t b) {
  ExternalAPI::setComponentColor(component.id, r, g, b);
  return true;
};
CTOAST_API CToastString CToast_getText(CToastComponent component) {
  return ExternalAPI::getComponentText(component.id);
}
CTOAST_API bool CToast_addComponent(CToastComponent parent,
                                    CToastComponent child) {
  ExternalAPI::addComponent(parent.id, child.id);
  return true;
};
CTOAST_API bool CToast_setVisibleCommand(CToastComponent comp, uint8_t cmd) {
  ExternalAPI::setComponentVisibleCommand(comp.id, cmd);
  return true;
};
CTOAST_API bool CToast_setVisible(CToastComponent comp, bool flag) {
  ExternalAPI::setComponentVisible(comp.id, flag);
  return true;
}
CTOAST_API bool CToast_setFont(CToastComponent comp, CToastString font) {
  ExternalAPI::setComponentFont(comp.id, font);
  return true;
}
CTOAST_API bool CToast_setFontSize(CToastComponent comp, uint8_t fontSize) {
  ExternalAPI::setComponentFontSize(comp.id, fontSize);
  return true;
};