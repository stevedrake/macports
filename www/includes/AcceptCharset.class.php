<?php

# -*- coding: utf-8; mode: php; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- vim:fenc=utf-8:filetype=php:et:sw=4:ts=4:sts=4
# $Id$
# Copyright (c) 2008, The MacPorts Project.

require_once 'AcceptAbstract.class.php';

class AcceptCharset extends AcceptAbstract {
    function AcceptCharset() {
        parent::AcceptAbstract(isset($_SERVER['HTTP_ACCEPT_CHARSET']) ? $_SERVER['HTTP_ACCEPT_CHARSET'] : '*');
    }
}
