#include "test_jni_ActiveDispatcher.h"
#include <string.h>
#include "jni.h"

/*
	* Class:     test_jni_ActiveDispatcher
	* Method:    createRecords
	* Signature: ()I
	*/

JNIEXPORT jint JNICALL Java_test_jni_ActiveDispatcher_createRecords
(JNIEnv *, jobject) {

}

	/*
	* Class:     test_jni_ActiveDispatcher
	* Method:    getRecord
	* Signature: ()Ljava/lang/String;
	*/
JNIEXPORT jstring JNICALL Java_test_jni_ActiveDispatcher_getRecord
(JNIEnv *env, jobject obj) {

	//const char *str = env->GetStringUTFChars(env, string, 0);
	//17.     char cap[128];
	//18.     strcpy(cap, str);
	//19.     (*env)->ReleaseStringUTFChars(env, string, str);
	//20.     return (*env)->NewStringUTF(env, strupr(cap));

}
	/*
	* Class:     test_jni_ActiveDispatcher
	* Method:    getAllRecords
	* Signature: ()Ljava/lang/String;
	*/
JNIEXPORT jstring JNICALL Java_test_jni_ActiveDispatcher_getAllRecords
(JNIEnv *, jobject) {
}