--- ./7font-search.sh.orig	2007-07-30 18:40:56.000000000 +0900
+++ ./7font-search.sh	2007-08-07 06:16:51.000000000 +0900
@@ -101,37 +101,8 @@
 
 # -----------------------------------
 
-mksymlink $CMAP <<EOF
-### ���̾  �����ǥ��쥯�ȥ�/�ե�����
-Acrobat4    /usr{,/local,/sfw}{,/lib}/Acrobat4/Resource/CMap
-Acrobat5    /usr{,/local,/sfw}{,/lib}/Acrobat5/Resource/Font
-Acrobat7    /usr{,/local,/sfw}{,/lib}{,/Adobe}/Acrobat7.0/Resource/CMap
-ghostscript /usr{,/local,/sfw}/{share,lib}/ghostscript{,/*}/Resource/CMap
-openwinja   /usr/openwin/lib/locale/ja/X11/Resource/CMap
-openwinko   /usr/openwin/lib/locale/ko/X11/Resource/CMap
-openwinzh   /usr/openwin/lib/locale/zh/X11/Resource/CMap
-openwintw   /usr/openwin/lib/locale/zh_TW.BIG5/X11/Resource/CMap
-EOF
-
-mksymlink $TRUETYPE <<EOF
-### ���̾  �����ǥ��쥯�ȥ�/�ե�����
-x11       /usr/X11R6/lib/X11/fonts/truetype
-X11       /usr/X11R6/lib/X11/fonts/TrueType
-X11TTF    /usr/X11R6/lib/X11/fonts/TTF
-openwin   /usr/openwin/lib/locale/ja/X11/fonts/TT
-truetype  /usr/share/fonts/truetype
-TTF	  /usr/share/fonts/TTF
-ja        /usr/share/fonts/ja/TrueType
-japanese  /usr/share/fonts/japanese/TrueType
-QtPalmtop /opt/QtPalmtop/lib/X11/fonts/TrueType
-EOF
-
-mksymlink $OPENTYPE <<EOF
-### ���̾  �����ǥ��쥯�ȥ�/�ե�����
-# for test (contains white space)
-hoge "/tmp/hoge hoge"
-QtPalmtop /opt/QtPalmtop/lib/X11/fonts/opentype
-EOF
+$MKDIR $CMAP/ghostscript
+$CP ./cmap-gs854/* $CMAP/ghostscript
 
 if test -d /System/Library/Fonts; then # Mac OS X
 LC_ALL=ja_JP.UTF-8
@@ -145,17 +116,6 @@
 EOF
 fi
 
-if test -d /cygdrive; then # cygwin
-mksymlink $CMAP <<EOF
-Acrobat5 "`cygpath --mixed "$PROGRAMFILES"`/Adobe/Acrobat 5.0/Resource/Cmap"
-Acrobat6 "`cygpath --mixed "$PROGRAMFILES"`/Adobe/Acrobat 6.0/Resource/CMap"
-Acrobat7 "`cygpath --mixed "$PROGRAMFILES"`/Adobe/Acrobat 7.0/Resource/CMap"
-EOF
-mksymlink $TRUETYPE <<EOF
-windows  "`cygpath --mixed -W`/Fonts"
-EOF
-fi
-
 (IFS=";"; NUM=1; for f in $EXTRA_CMAP; do
     echo "extra$NUM $f"
     NUM=`expr $NUM + 1`
