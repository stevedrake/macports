#always install the library
win32 {
   dlltarget.path=$$[QT_INSTALL_BINS]
   INSTALLS += dlltarget
}
target.path=$$[QT_INSTALL_LIBS]
INSTALLS += target

#headers
qt_install_headers {
    message(qt_install_headers)
    message(QT_CONFIG is $$QT_CONFIG)
    message(CONFIG is $$CONFIG)
    INSTALL_HEADERS = $$SYNCQT.HEADER_FILES
    equals(TARGET, QtCore) {
       #headers generated by configure
       INSTALL_HEADERS *= $$QT_BUILD_TREE/src/corelib/global/qconfig.h \
                          $$QT_SOURCE_TREE/src/corelib/arch/$$QT_ARCH/arch
    }

    equals(TARGET, phonon) {
        class_headers.path = $$[QT_INSTALL_HEADERS]/$$TARGET
    } else {
        flat_headers.files = $$INSTALL_HEADERS
        flat_headers.path = $$[QT_INSTALL_HEADERS]/Qt
        INSTALLS += flat_headers

        class_headers.path = $$[QT_INSTALL_HEADERS]/$$TARGET
    }
    class_headers.files = $$SYNCQT.HEADER_CLASSES
    INSTALLS += class_headers

    targ_headers.files = $$INSTALL_HEADERS
    targ_headers.path = $$[QT_INSTALL_HEADERS]/$$TARGET
    INSTALLS += targ_headers

    message(INSTALL_HEADERS is $$INSTALL_HEADERS)
    contains(QT_CONFIG,private_tests) {
        private_headers.files = $$SYNCQT.PRIVATE_HEADER_FILES
        private_headers.path = $$[QT_INSTALL_HEADERS]/$$TARGET/private
        INSTALLS += private_headers
    }
}

embedded:equals(TARGET, QtGui) {
    # install fonts for embedded
    INSTALLS += fonts
    fonts.path = $$[QT_INSTALL_LIBS]/fonts
    fonts.files = $$QT_SOURCE_TREE/lib/fonts/*
}
