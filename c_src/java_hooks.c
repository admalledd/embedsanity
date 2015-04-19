#include <jni.h>
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
    //Note: classpath can later be redefined to be a .jar container when we have multiple classes finaly
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

    // load the class (on java path)
    //Note that JAVA_BOOTSTRAP_FILE is actually the java-resolved name of the class we are looking for (so not the .class file or a path to said file)
    //This means setting up the java.class.path to where we are storing our java boot file.
    jclass cls = (*env)->FindClass(env, JAVA_BOOTSTRAP_FILE);
    if (cls == NULL)
    {
        printf("java: Can't find class %s\n", JAVA_BOOTSTRAP_FILE);
        return -1;
     }

    // init the class constructor and load the entry point
    jmethodID mid = (*env)->GetStaticMethodID(env, cls, "boot_start", "([Ljava/lang/String;)V");
    if (mid == NULL)
    {
        printf("java: Can't find boot_start method in class %s\n", JAVA_BOOTSTRAP_FILE);
        return -1;
     }

    jobjectArray args;
    args = (*env)->NewObjectArray(env, 1, (*env)->FindClass(env, "java/lang/String"), (*env)->NewStringUTF(env, "Hello World from C!"));

    /* call the main method with the required arguments */
    (*env)->CallStaticVoidMethod(env, cls, mid, args);



    return 1;

}
