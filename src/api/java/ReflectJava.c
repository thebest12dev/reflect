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
#include "ReflectJava.h"
#include "../cstyle/ReflectCExtern.h"
#include "ReflectJavaUtil.h"
#include <jni.h>

JNIEXPORT jint JNICALL JavaFunction(ReflectNative,
                                    getReferenceById)(JNI_PARAM_DECL,
                                                      jstring id) {
  // Convert jstring to const char*
  const char *nativeId = (*env)->GetStringUTFChars(env, id, 0);

  // Get the component reference using the nativeId
  ReflectComponent ref = Reflect_getComponentById(nativeId);

  // Release the jstring memory
  (*env)->ReleaseStringUTFChars(env, id, nativeId);

  // Return the component id as jint
  return (jint)ref.id;
}

JNIEXPORT jstring JNICALL JavaFunction(ReflectNative, getText)(JNI_PARAM_DECL,
                                                               jint ref) {
  ReflectComponent comp;
  comp.id = (uint8_t)ref;
  return (*env)->NewStringUTF(env, Reflect_getText(comp));
}

JNIEXPORT void JNICALL JavaFunction(ReflectNative, addComp)(JNI_PARAM_DECL,
                                                            jint parentRef,
                                                            jint compRef) {
  ReflectComponent comp1;
  comp1.id = (uint8_t)parentRef;
  ReflectComponent comp2;
  comp2.id = (uint8_t)compRef;
  Reflect_addComponent(comp1, comp2);
}
