#include "com_rest2_jni_ActiveDispatcher.h"
#include "jni.h"
#include "TransactionManager.h"

#include "string.h"
#include <boost/lexical_cast.hpp>

using namespace std;
/*
* Class:     com_rest2_jni_ActiveDispatcher
* Method:    createRecords
* Signature: ()I
*/
JNIEXPORT jint JNICALL Java_com_rest2_jni_ActiveDispatcher_createRecords
(JNIEnv *env, jobject obj, jstring jquery) {

	/*jclass cls = env->GetObjectClass(obj);

	jfieldID portField = env->GetFieldID(cls, "m_dbport", "I");
	int port = (int)env->GetIntField(obj, portField);

	jfieldID urlField = env->GetFieldID(cls, "m_dburl", "S");
	jstring jurl = (jstring)env->GetObjectField(obj, urlField);

	char* dburl = (char*)env->GetStringUTFChars(jurl, false);

	//char* tableName = (char*)env->GetStringUTFChars(jtable, false);

	//char* soid = (char*)env->GetStringUTFChars(joid, false);

	TransactionManager *mngr = new TransactionManager(dburl, port);
	//int oid = boost::lexical_cast<int>(soid);
	//char * record = mngr->getRecord(tableName, oid);
	//printf("getAllRecords=%s", record);

	
	char* query = (char*)env->GetStringUTFChars(jquery, false);
	int xx=mngr->createRecords(query);

	delete mngr;

	return xx;*/

	return 1;
}

/*
* Class:     com_rest2_jni_ActiveDispatcher
* Method:    getRecord
* Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_com_rest2_jni_ActiveDispatcher_getRecord
(JNIEnv *env, jobject obj, jstring jtable, jstring joid) {
	//printf("came inside");
	jclass cls = env->GetObjectClass(obj);

	jfieldID portField = env->GetFieldID(cls, "m_dbport", "I");
	int port = (int)env->GetIntField(obj, portField);

	jfieldID urlField = env->GetFieldID(cls, "m_dburl", "Ljava/lang/String;");
	jstring jurl = (jstring)env->GetObjectField(obj, urlField);

	char* dburl = (char*)env->GetStringUTFChars(jurl, false);

	char* tableName = (char*)env->GetStringUTFChars(jtable, false);

	char* soid = (char*)env->GetStringUTFChars(joid, false);
	
	TransactionManager *mngr = new TransactionManager(dburl, port);
	int oid = boost::lexical_cast<int>(soid);
	char * record = mngr->getRecord(tableName,oid);
	printf("getAllRecords=%s", record);

	delete mngr;

	return env->NewStringUTF(record);
	
	//return env->NewStringUTF("getRecord1");
}


/*
* Class:     com_rest2_jni_ActiveDispatcher
* Method:    getAllRecords
* Signature: (Ljava/lang/String;)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_com_rest2_jni_ActiveDispatcher_getAllRecords
(JNIEnv *env, jobject obj, jstring jtable) {

	printf("came inside2");
	jclass cls = env->GetObjectClass(obj);

	jfieldID portField = env->GetFieldID(cls, "m_dbport", "I");
	int port = (int)env->GetIntField(obj, portField);

	jfieldID urlField = env->GetFieldID(cls, "m_dburl", "Ljava/lang/String;");
	jstring jurl = (jstring)env->GetObjectField(obj, urlField);

	char* dburl = (char*)env->GetStringUTFChars(jurl, false);

	char* tableName = (char*)env->GetStringUTFChars(jtable, false);

	TransactionManager *mngr = new TransactionManager(dburl,port);
	char * records=mngr->getAllRecords(tableName,"");
	printf("getAllRecords=%s", records);

	delete mngr;

		return env->NewStringUTF(records);
	
}

