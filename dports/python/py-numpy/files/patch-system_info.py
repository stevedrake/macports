--- numpy/distutils/system_info.py.orig 2006-02-15 21:50:00.000000000 -0800
+++ numpy/distutils/system_info.py      2006-02-20 13:10:31.000000000 -0800
@@ -129,14 +129,14 @@
     default_x11_lib_dirs = []
     default_x11_include_dirs = []
 else:
-    default_lib_dirs = ['/usr/local/lib', '/opt/lib', '/usr/lib',
+    default_lib_dirs = ['DPORT_PREFIX/lib', '/usr/local/lib', '/opt/lib', '/usr/lib',
                         '/opt/local/lib', '/sw/lib']
-    default_include_dirs = ['/usr/local/include',
+    default_include_dirs = ['DPORT_PREFIX/include', '/usr/local/include',
                             '/opt/include', '/usr/include',
                             '/opt/local/include', '/sw/include']
     default_src_dirs = ['.','/usr/local/src', '/opt/src','/sw/src']
-    default_x11_lib_dirs = ['/usr/X11R6/lib','/usr/X11/lib','/usr/lib']
-    default_x11_include_dirs = ['/usr/X11R6/include','/usr/X11/include',
+    default_x11_lib_dirs = ['DPORT_PREFIX/lib', '/usr/X11R6/lib','/usr/X11/lib','/usr/lib']
+    default_x11_include_dirs = ['DPORT_PREFIX/include/X11', '/usr/X11R6/include','/usr/X11/include',
                                 '/usr/include']
 
 if os.path.join(sys.prefix, 'lib') not in default_lib_dirs:
