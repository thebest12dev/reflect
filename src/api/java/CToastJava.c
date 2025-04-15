/**
 * Copyright (c) 2025 thebest12lines
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
 * "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "CToastJava.h"
#include "../cstyle/CToastCExtern.h"
#include "CToastJavaUtil.h"
#include <jni.h>

JNIEXPORT jint JNICALL JavaFunction(CToastNative,
                                    getReferenceById)(JNI_PARAM_DECL,
                                                      jstring id) {
  // Convert jstring to const char*
  const char *nativeId = (*env)->GetStringUTFChars(env, id, 0);

  // Get the component reference using the nativeId
  CToastComponent ref = CToast_getComponentById(nativeId);

  // Release the jstring memory
  (*env)->ReleaseStringUTFChars(env, id, nativeId);

  // Return the component id as jint
  return (jint)ref.id;
}

JNIEXPORT jstring JNICALL JavaFunction(CToastNative, getText)(JNI_PARAM_DECL,
                                                              jint ref) {
  CToastComponent comp;
  comp.id = ref;
  return (*env)->NewStringUTF(env, CToast_getText(comp));
}

JNIEXPORT void JNICALL JavaFunction(CToastNative, addComp)(JNI_PARAM_DECL,
                                                           jint parentRef,
                                                           jint compRef) {
  CToastComponent comp1;
  comp1.id = parentRef;
  CToastComponent comp2;
  comp2.id = compRef;
  CToast_addComponent(comp1, comp2);
}
