#!/bin/sh -e

        export PREFIX="%p"
        . ./environment-helper.sh

#darwinports
	export UNSERMAKE="no"

        ./build-helper.sh install %N %v %r make -j1 install DESTDIR=%d

        mkdir -p %i/share/doc/installed-packages
        touch %i/share/doc/installed-packages/%N
        touch %i/share/doc/installed-packages/%N-base

        # FIXME: this works around an initialization error in kstars, this should
        # get fixed for real!
        install -d -m 755 %i/share/config
        touch %i/share/config/kstarsrc

        # they changed the library name, but not the compat version??
        pushd %i/lib
        ln -s libkdeeduui.3.dylib libkdeeduui.1.dylib
        popd
