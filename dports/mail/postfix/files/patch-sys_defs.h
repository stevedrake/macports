--- src/util/sys_defs.h.orig	2013-09-30 06:51:55.000000000 +1000
+++ src/util/sys_defs.h	2014-01-17 15:24:37.000000000 +1100
@@ -228,9 +228,9 @@
 #define HAS_DB
 #define HAS_SA_LEN
 #define NATIVE_DB_TYPE	"hash"
-#define ALIAS_DB_MAP	DEF_DB_TYPE ":/etc/aliases"
+#define ALIAS_DB_MAP	DEF_DB_TYPE ":__PREFIX/etc/postfix/aliases"
 #define GETTIMEOFDAY(t) gettimeofday(t,(struct timezone *) 0)
-#define ROOT_PATH	"/bin:/usr/bin:/sbin:/usr/sbin"
+#define ROOT_PATH	"__PREFIX/bin:/bin:/usr/bin:/sbin:/usr/sbin"
 #define USE_STATFS
 #define STATFS_IN_SYS_MOUNT_H
 #define HAS_POSIX_REGEXP
@@ -245,11 +245,11 @@
 # define HAVE_GETIFADDRS
 #endif
 #define HAS_FUTIMES			/* XXX Guessing */
-#define NATIVE_SENDMAIL_PATH "/usr/sbin/sendmail"
-#define NATIVE_MAILQ_PATH "/usr/bin/mailq"
-#define NATIVE_NEWALIAS_PATH "/usr/bin/newaliases"
-#define NATIVE_COMMAND_DIR "/usr/sbin"
-#define NATIVE_DAEMON_DIR "/usr/libexec/postfix"
+#define NATIVE_SENDMAIL_PATH "__PREFIX/sbin/sendmail"
+#define NATIVE_MAILQ_PATH "__PREFIX/bin/mailq"
+#define NATIVE_NEWALIAS_PATH "__PREFIX/bin/newaliases"
+#define NATIVE_COMMAND_DIR "__PREFIX/sbin"
+#define NATIVE_DAEMON_DIR "__PREFIX/libexec/postfix"
 #define SOCKADDR_SIZE	socklen_t
 #define SOCKOPT_SIZE	socklen_t
 #ifndef NO_KQUEUE
