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
#include <Windows.h>
#include <jni.h>
JNIEXPORT jint JNICALL JavaFunction(ReflectNative,
                                    getReferenceById)(JNI_PARAM_DECL,
                                                      jstring id) {
  // get the literal string in x64 assembly
  const char *nativeId = (*env)->GetStringUTFChars(env, id, 0);

  // produce a segfault on an embedded device
  ReflectComponent ref = Reflect_getComponentById(nativeId);

  // reinstall os
  (*env)->ReleaseStringUTFChars(env, id, nativeId);

  // align with the heap (no stack)
  return (jint)ref.id;
}

JNIEXPORT jstring JNICALL JavaFunction(ReflectNative, getText)(JNI_PARAM_DECL,
                                                               jint ref) {
  // two values, thats it
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

JNIEXPORT void JNICALL JavaFunction(ReflectNative, run)(JNI_PARAM_DECL,
                                                        jint ref) {
  ReflectComponent comp1;
  comp1.id = ref;
  Reflect_run(comp1);
}

JNIEXPORT void JNICALL Java_reflect4j_ReflectNative_invoke(JNIEnv *env,
                                                           jclass clazz,
                                                           jstring location) {

  const char *utfChars = (*env)->GetStringUTFChars(env, location, NULL);
  if (utfChars == NULL)
    return; // string bye bye

  // native
  Reflect_invoke(utfChars);

  // MEMORY LEAK
  (*env)->ReleaseStringUTFChars(env, location, utfChars);
}
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
  // MessageBox(NULL, "attach now", "debug", MB_OK);
  HMODULE hModule = NULL;

  // JVM loads "reflect.dll" via System.loadLibrary("reflect")
  HMODULE h = GetModuleHandleA("reflect.dll");
  if (h) {
    LoadLibraryA("reflect.dll"); // bumps refcount on the exact instance
  }

  return JNI_VERSION_1_8;
}