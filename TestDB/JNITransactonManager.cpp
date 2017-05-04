#include "JNITransactonManager.h"

#include "jni.h"
#include "string.h"
/*
JNIEXPORT int JNICALL
java_test_jni_ActiveDispatcher_createRecords(JNIEnv* env, jobject jobj, jobject JTransactionManager) {
	jclass cls = env->GetObjectClass(JTransactionManager);
	
	jfieldID portField = env->GetFieldID(cls, "m_dbport", "I");
	int port = (int)env->GetIntField(JTransactionManager, portField);
	
	jfieldID urlField = env->GetFieldID(cls,"m_dburl", "S");
	jstring jurl = (jstring)env->GetObjectField(JTransactionManager, urlField);
	
	char* dburl = (char*)env->GetStringUTFChars(jurl,false);

	//construct 

	return 0;
}

JNIEXPORT char* JNICALL
java_test_jni_ActiveDispatcher_getRecord(JNIEnv* env, jobject jobj, jobject JTransactionManager) {

	jclass cls = env->GetObjectClass(JTransactionManager);

	jfieldID portField = env->GetFieldID(cls, "m_dbport", "I");
	int port = (int)env->GetIntField(JTransactionManager, portField);

	jfieldID urlField = env->GetFieldID(cls, "m_dburl", "S");
	jstring jurl = (jstring)env->GetObjectField(JTransactionManager, urlField);

	char* dburl = (char*)env->GetStringUTFChars(jurl, false);
	return 0;
}

JNIEXPORT char* JNICALL
java_test_jni_ActiveDispatcher_getAllRecords(JNIEnv* env, jobject jobj, jobject JTransactionManager) {

	jclass cls = env->GetObjectClass(JTransactionManager);

	jfieldID portField = env->GetFieldID(cls, "m_dbport", "I");
	int port = (int)env->GetIntField(JTransactionManager, portField);

	jfieldID urlField = env->GetFieldID(cls, "m_dburl", "S");
	jstring jurl = (jstring)env->GetObjectField(JTransactionManager, urlField);

	char* dburl = (char*)env->GetStringUTFChars(jurl, false);

}


JNIEXPORT void JNICALL Java_TestJNIInstanceVariable_modifyInstanceVariable
(JNIEnv *env, jobject thisObj) {
	// Get a reference to this object's class
	jclass thisClass = (*env)->GetObjectClass(env, thisObj);

	// int
	// Get the Field ID of the instance variables "number"
	jfieldID fidNumber = (*env)->GetFieldID(env, thisClass, "number", "I");
	if (NULL == fidNumber) return;

	// Get the int given the Field ID
	jint number = (*env)->GetIntField(env, thisObj, fidNumber);
	printf("In C, the int is %d\n", number);

	// Change the variable
	number = 99;
	(*env)->SetIntField(env, thisObj, fidNumber, number);

	// Get the Field ID of the instance variables "message"
	jfieldID fidMessage = (*env)->GetFieldID(env, thisClass, "message", "Ljava/lang/String;");
	if (NULL == fidMessage) return;

	// String
	// Get the object given the Field ID
	jstring message = (*env)->GetObjectField(env, thisObj, fidMessage);

	// Create a C-string with the JNI String
	const char *cStr = (*env)->GetStringUTFChars(env, message, NULL);
	if (NULL == cStr) return;

	printf("In C, the string is %s\n", cStr);
	(*env)->ReleaseStringUTFChars(env, message, cStr);

	// Create a new C-string and assign to the JNI string
	message = (*env)->NewStringUTF(env, "Hello from C");
	if (NULL == message) return;

	// modify the instance variables
	(*env)->SetObjectField(env, thisObj, fidMessage, message);
}
*/


JNITransactonManager::JNITransactonManager()
{
}


JNITransactonManager::~JNITransactonManager()
{
}
