--- src/native/unix/native/java.c.orig	Mon Feb  9 07:55:21 2004
+++ src/native/unix/native/java.c	Thu Oct 14 14:19:34 2004
@@ -44,6 +44,11 @@
     if (reload==TRUE) main_reload();
     else main_shutdown();
 }
+/* Automaticly restart when the JVM crashes */
+static void java_abort123()
+{
+    exit(123);
+}
 
 char *java_library(arg_data *args, home_data *data) {
     char *libf=NULL;
@@ -118,12 +123,29 @@
 
 #ifdef OS_DARWIN
     /* MacOS/X actually has two libraries, one with the REAL vm, and one for
-       the VM startup. The first one (libappshell.dyld) contains CreateVM */
-    if (replace(appf,1024,"$JAVA_HOME/../Libraries/libappshell.dylib",
+       the VM startup. The first one (libappshell.dyld) contains CreateVM.
+       
+       		Thru JVM 1.3.1: library name is libappshell.dylib
+       		After JVM 1.4.1: library name is libjvm_compat.dylib
+    */
+    
+    /* Try older library first */
+	if (replace(appf,1024,"$JAVA_HOME/../Libraries/libappshell.dylib",
                  "$JAVA_HOME",data->path)!=0) {
         log_error("Cannot replace values in loader library");
         return(false);
     }
+    
+    /* Try newer library next */
+    struct stat sb;
+    if (stat(appf, &sb)) {
+		if (replace(appf,1024,"$JAVA_HOME/../Libraries/libjvm_compat.dylib",
+					 "$JAVA_HOME",data->path)!=0) {
+			log_error("Cannot replace values in loader library");
+			return(false);
+		}
+	}
+
     apph=dso_link(appf);
     if (apph==NULL) {
         log_error("Cannot load required shell library %s",appf);
@@ -146,20 +168,28 @@
     log_debug("JVM library entry point found (0x%08X)",symb);
 
     /* Prepare the VM initialization arguments */
-    arg.version=JNI_VERSION_1_2;
-    arg.ignoreUnrecognized=FALSE;
+    
+    /*
+    	Mac OS X Java will load JVM 1.3.1 instead of 1.4.2 if JNI_VERSION_1_2
+    	is specified. So use JNI_VERSION_1_4 if we can.
+    */
+    #if defined(JNI_VERSION_1_4)
+    	arg.version=JNI_VERSION_1_4;
+    #else
+		arg.version=JNI_VERSION_1_2;
+	#endif
+	arg.ignoreUnrecognized=FALSE;
     arg.nOptions=args->onum;
-    if (arg.nOptions==0) {
-        arg.options=NULL;
-    } else {
-        opt=(JavaVMOption *)malloc(arg.nOptions*sizeof(JavaVMOption));
-        for (x=0; x<args->onum; x++) {
-            opt[x].optionString=strdup(args->opts[x]);
-            jsvc_xlate_to_ascii(opt[x].optionString);
-            opt[x].extraInfo=NULL;
-        }
-        arg.options=opt;
-    }
+    arg.nOptions++; /* Add abort code */
+    opt=(JavaVMOption *)malloc(arg.nOptions*sizeof(JavaVMOption));
+    for (x=0; x<args->onum; x++) {
+        opt[x].optionString=strdup(args->opts[x]);
+        jsvc_xlate_to_ascii(opt[x].optionString);
+        opt[x].extraInfo=NULL;
+    }
+    opt[x].optionString="abort";
+    opt[x].extraInfo=java_abort123;
+    arg.options=opt;
 
     /* Do some debugging */
     if (log_debug_flag==true) {
