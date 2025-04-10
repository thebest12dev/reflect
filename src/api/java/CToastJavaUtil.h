#ifndef _CTOAST_JAVA_UTIL_H
#define _CTOAST_JAVA_UTIL_H
#define JavaFunction(clazz, method) Java_ctoast4j_##clazz##_##method
#define JNI_PARAM_DECL JNIEnv *env, jclass clazz
#endif // !_CTOAST_JAVA_UTIL_H
