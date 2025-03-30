// pragma once if supported
#if defined(__cplusplus) && (defined(_MSVC_VER) || defined(__GNUC__) ||        \
                             defined(__clang__) || defined(__MINGW__))
#pragma once
#endif

#ifndef CTOASTC_H
#define CTOASTC_H
#undef CTOAST_API
#ifdef CTOAST_BUILDING
#define CTOAST_API __declspec(dllexport)
#else
#define CTOAST_API __declspec(dllimport)
#endif
#ifdef __cplusplus
#include "../../src/core/ui/Components.h"
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
CTOAST_API CToastComponent CToast_getComponentById(CToastString id);
CTOAST_API bool CToast_setColor(CToastComponent component, uint8_t r, uint8_t g,
                                uint8_t b);
CTOAST_API CToastString CToast_getText(CToastComponent component);
CTOAST_API bool CToast_addComponent(CToastComponent parent,
                                    CToastComponent child);
CTOAST_API bool CToast_setVisibleCommand(CToastComponent comp, uint8_t cmd);
CTOAST_API bool CToast_setVisible(CToastComponent comp, bool flag);
CTOAST_API bool CToast_setFont(CToastComponent comp, CToastString font);
CTOAST_API bool CToast_setFontSize(CToastComponent comp, uint8_t fontSize);
#ifdef __cplusplus
}
#endif
#endif
