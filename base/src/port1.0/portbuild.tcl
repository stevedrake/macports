# ex:ts=4
# portbuild.tcl
#
# Copyright (c) 2002 Apple Computer, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of Apple Computer, Inc. nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

package provide portbuild 1.0
package require portutil 1.0

register com.apple.build target build_main build_init
register com.apple.build provides build 
register com.apple.build requires main fetch extract checksum patch configure depends_build depends_lib

# define options
options make.cmd make.type make.target.all make.target.install make.env

# defaults
default make.type bsd
default make.cmd make
default make.target.all all

set UI_PREFIX "---> "

proc build_init {args} {
    global make.type make.cmd make.target.all make.worksrcdir

    switch -exact -- ${make.type} {
	bsd {
	    set make.cmd bsdmake
	}
	gnu {
	    set make.cmd gnumake
	}
    }
}

proc build_main {args} {
    global portname portpath workdir prefix make.type make.cmd make.env make.target.all UI_PREFIX worksrcdir

    if [info exists make.worksrcdir] {
	set configpath ${portpath}/${workdir}/${worksrcdir}/${make.worksrcdir}
    } else {
	set configpath ${portpath}/${workdir}/${worksrcdir}
    }

    cd $configpath
    lappend env_vars PREFIX=${prefix}
    if [info exists make.env] {
	lappend env_vars ${make.env}
    }
	
    ui_msg "$UI_PREFIX Building $portname with target ${make.target.all}"
    system "env ${env_vars} ${make.cmd} ${make.target.all}"
    return 0
}
