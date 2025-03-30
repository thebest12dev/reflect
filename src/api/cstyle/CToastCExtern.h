// pragma once if supported
#if defined(__cplusplus) && (defined(_MSVC_VER) || defined(__GNUC__) ||        \
                             defined(__clang__) || defined(__MINGW__))
#pragma once
#endif

#ifndef CTOASTC_H
#define CTOASTC_H
#ifdef __cplusplus
#include "../core/ui/Components.h"
#include <cstdint>
#else
#include <stdbool.h>
#include <stdint.h>
#endif //

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
  uint8_t id;
} CToastComponent;
typedef const char *CToastString;
CToastComponent CToast_getComponentById(CToastString id);
bool CToast_setColor(CToastComponent component, uint8_t r, uint8_t g,
                     uint8_t b);
CToastString CToast_getText(CToastComponent component);
bool CToast_addComponent(CToastComponent parent, CToastComponent child);
bool CToast_setVisibleCommand(CToastComponent comp, uint8_t cmd);
bool CToast_setVisible(CToastComponent comp, bool flag);
bool CToast_setFont(CToastComponent comp, CToastString font);
bool CToast_setFontSize(CToastComponent comp, uint8_t fontSize);
#ifdef __cplusplus
}
#endif
#endif
