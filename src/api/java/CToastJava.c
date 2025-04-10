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
