# $Id$
# 
# Copyright (c) 2012 The MacPorts Project
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of The MacPorts Project nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# 
# This PortGroup sets up default behaviors for projects hosted at github.
# 
# Usage:
# 
#   PortGroup               github 1.0
#   github.setup            author project version [tag_prefix]

options github.author github.project github.version github.tag_prefix github.homepage github.master_sites

default github.homepage {https://github.com/${github.author}/${github.project}}
default github.master_sites {${github.homepage}/tarball/[join ${github.tag_prefix} ""]${github.version}}

proc github.setup {gh_author gh_project gh_version {gh_tag_prefix ""}} {
    global github.author github.project github.version github.tag_prefix github.homepage github.master_sites
    
    github.author           ${gh_author}
    github.project          ${gh_project}
    github.version          ${gh_version}
    github.tag_prefix       ${gh_tag_prefix}
    
    name                    ${github.project}
    version                 ${github.version}
    homepage                ${github.homepage}
    git.url                 ${github.homepage}.git
    git.branch              ${github.tag_prefix}${github.version}
    master_sites            ${github.master_sites}
    distname                ${github.project}-${github.version}
    
    post-extract {
        if {"standard" == ${fetch.type} && ${master_sites} == ${github.master_sites} && [llength ${distfiles}] > 0} {
            move [glob ${workpath}/*] ${worksrcpath}
        }
    }
    
    livecheck.type          regex
    livecheck.url           ${github.homepage}/tags
    livecheck.regex         tarball/[join ${github.tag_prefix} ""](\[^"\]+)
}
