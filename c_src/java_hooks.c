#include <jni.h>       /* where everything is defined */
#include <stdbool.h>

#include "globals.h"
#include "util.h"
#include "java_hooks.h"

#define JAVA_BOOTSTRAP_FILE "bootstrap"

int init_java()
{
    JavaVM *jvm;       /* denotes a Java VM */
    JNIEnv *env;       /* pointer to native method interface */
    JavaVMInitArgs vm_args; /* JDK/JRE 6 VM initialization arguments */
    JavaVMOption* options=malloc(sizeof(JavaVMOption)*1);
    options[0].optionString = concat(2,"-Djava.class.path=",G_paths.java_class_home);

    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;
    /* load and initialize a Java VM, return a JNI interface
     * pointer in env */
    JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    free(options[0].optionString);
    free(options);
    /* invoke the Main.test method using the JNI */
    //jclass cls = (*env)->FindClass(env,"bootstrap");

    //jmethodID mid = (*env)->GetStaticMethodID(env,cls, "test", "(I)V");
    // (*env)->CallStaticVoidMethod(env,cls, mid, "String from master code");
    /* We are done. */
    // (*jvm)->DestroyJavaVM(jvm);


    /* locate and load the class file */
    jclass cls;
    if ((cls = (*env)->FindClass(env, JAVA_BOOTSTRAP_FILE)) == 0)
    {
        fprintf(stderr, "java: Can't find class %s\n", JAVA_BOOTSTRAP_FILE);
        return -1;
     }

    /* locate the main method */
    jmethodID mid;
    if ((mid = (*env)->GetStaticMethodID(env, cls, "boot_start", "([Ljava/lang/String;)V")) == 0)
    {
        fprintf(stderr, "java: Can't find boot_start method in class %s\n", JAVA_BOOTSTRAP_FILE);
        return -1;
     }

    /* create a new java string to be passes to the class */
    jstring jstr;
    if ((jstr = (*env)->NewStringUTF(env, "Hello World from C!")) == 0)
    {
        fprintf(stderr, "java: Out of memory\n");
        return -1;
     }

    /* create a new string array with a single element containing the
    string created above */
    jobjectArray args;
    args = (*env)->NewObjectArray(env, 1, (*env)->FindClass(env, "java/lang/String"), jstr);
    if (args == 0)
    {
        fprintf(stderr, "java: Out of memory\n");
        return -1;
    }

    /* call the main method with the required arguments */
    (*env)->CallStaticVoidMethod(env, cls, mid, args);



    return 1;

}
