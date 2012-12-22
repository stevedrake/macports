<?php
    /* -*- coding: utf-8; mode: php; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- vim:set fenc=utf-8 filetype=php et sw=4 ts=4 sts=4: */
    /* $Id$ */
    /* Copyright (c) 2007, The MacPorts Project. */
    include_once("includes/common.inc");
    print_header('The MacPorts Project -- Contact Us', 'utf-8');
?>


<div id="content">

    <h2 class="hdr">Getting in Touch With Us</h2>

    <p>There are a number of ways in which you can get in contact with people involved with The MacPorts Project, depending
    on who you need to contact and the type of support and/or feedback you're looking for:</p>

    <ul>
        <li><a href="#Lists">Public Mailing Lists</a></li>
        <li><a href="#PortMgr">Administrative Contact</a></li>
        <li><a href="#Tracker">Bug Reports &amp; Contributions</a></li>
        <li><a href="#IRC">IRC</a></li>
        <li><a href="#Individuals">Individuals</a></li>
    </ul>


    <h3>Getting Help</h3>

    <p>If you're looking for help to troubleshoot a particular failure installing or using MacPorts, providing us with as
    much information about your host platform as you can gather will help us help you in turn to look into the problem.
    The following simple steps comprise our recommended procedure to obtain support:</p>

    <ol>
        <li>First and foremost, check the MacPorts <a href="<?php print $guide_url; ?>">Documentation</a>, man pages, <a
        href="<?php print $trac_url . 'wiki/FAQ'; ?>">FAQ</a> and <a href="<?php print $trac_url . 'wiki/ProblemHotlist';
        ?>">problems hotlist</a>.</li>
        <li>Second, check the archives of the appropriate mailing list you intend to post to (below) to see if your question
        has already been asked and dealt with.</li>
        <li>Finally, if you still haven't found the answer to your problem, send e-mail to the appropriate mailing list
        with the following information:
            <ul>
                <li>output generated by the &#8220;<kbd>port</kbd>&#8221; command's <kbd>-v</kbd> (verbose) or <kbd>-d</kbd>
                (debug) flags;</li>
                <li>platform details such as operating system version (e.g. 10.4.10), hardware architecture (e.g. Intel
                or PowerPC), and the version of Xcode installed -- the &#8220;<kbd>sw_vers</kbd>&#8221; and &#8220;<kbd>uname -a</kbd>&#8221; commands are of
                great help in this regard;</li>
                <li>any third party software that may exist in places such as <kbd>/usr/local</kbd> and/or <kbd>/sw</kbd>.
                </li>
            </ul>
        </li>
    </ol>


    <h3 class="subhdr" id="Lists">Public Mailing Lists</h3>

    <p>The MacPorts Project hosts a number of specialized mailing lists you can freely subscribe to:</p>

    <ul>
        <li><a href="http://lists.macosforge.org/mailman/listinfo/macports-users/">MacPorts Users</a>
        (<a href="http://lists.macosforge.org/pipermail/macports-users/">archives</a>):
            <p>General discussion of MacPorts installation and usage. A moderate volume list.</p>
        </li>
        <li><a href="http://lists.macosforge.org/mailman/listinfo/macports-dev/">MacPorts Developers</a>
        (<a href="http://lists.macosforge.org/pipermail/macports-dev/">archives</a>):
            <p>Where project members and contributors discuss the MacPorts &#8220;base&#8221; system itself and future
            development plans, and related &#8220;Portfile&#8221; writing best practices. A low volume list.</p>
        </li>
        <li><a href="http://lists.macosforge.org/mailman/listinfo/macports-changes/">MacPorts Changes</a>
        (<a href="http://lists.macosforge.org/pipermail/macports-changes/">archives</a>):
            <p>Read-only, low to moderate volume list of changes to our <a href="<?php print $svn_url; ?>">SVN repository</a>
            for both &#8220;base&#8221; code and Portfiles, among others.</p>
        </li>
        <li><a href="http://lists.macosforge.org/mailman/listinfo/macports-tickets/">MacPorts Tickets</a>
        (<a href="http://lists.macosforge.org/pipermail/macports-tickets/">archives</a>):
            <p>Read-only, moderate volume list of ticket activity on our <a href="<?php print $trac_url; ?>">Trac bug tracker
            </a>.</p>
        </li>
        <li><a href="http://lists.macosforge.org/mailman/listinfo/macports-announce/">MacPorts Announce</a>
        (<a href="http://lists.macosforge.org/pipermail/macports-announce/">archives</a>):
            <p>Read-only, low volume list for project related major announcements.</p>
        </li>
    </ul>

    <p>Note that due to spam control policies you must subscribe to our non read-only lists in order to post to any of them.
    Members are expected to abide by the very simple <a href="http://tools.ietf.org/html/rfc1855">Netiquette guidelines</a>
    that are common to most open forums when posting; of particular relevance is sticking to plain text messages, our language
    of choice (English), and reducing the number and size of attachments in any way possible (e.g, by using paste bins such
    as <a href="http://paste.lisp.org/new/macports">lisppaste</a> or <a href="http://pastie.caboo.se">pastie</a> and
    passing along the paste URL rather than full error messages).</p>


    <h3 class="subhdr" id="PortMgr">Administrative Contact</h3>

    <p>The private and read-only &#8220;<?php print obfuscate_email("macports-mgr@lists.macosforge.org"); ?>&#8221; mailing
    list is where you should send mail to in case you need to get in touch with the The MacPorts Project's <a href="<?php print
    $guide_url . '#project.portmgr'; ?>">management team</a> (A.K.A. &#8220;PortMgr&#8221;), in case you have any administrative
    request or if you wish to discuss anything you might feel is of private nature (like the interaction between MacPorts and
    NDA'd software).</p>

    <p>This is also where you should turn to if you are a developer and/or a contributor interested in joning The MacPorts
    Project with full write access to our SVN repository and Wiki pages, either to work on MacPorts itself or as a ports
    maintainer. Please read the <a href="<?php print $guide_url . '#project.membership'; ?>">documentation available</a>
    on joining for more information.</p>


    <h3 class="subhdr" id="Tracker">Bug Tracker</h3>

    <p>We use the popular <a href="http://trac.edgewall.org/">Trac</a> web-based tool for our <a href="<?php print $trac_url .
    'roadmap'; ?>">bug tracking</a> and <a href="<?php print $trac_url . 'wiki'; ?>">Wiki</a> needs, thus buying ourselves
    seamless read-only integration with our SVN repository through its <a href="<?php print $trac_url . 'browser'; ?>">
    source browser</a> and the project <a href="<?php print $trac_url . 'timeline'; ?>">timeline</a> (where ticket activity
    can also be viewed). Note that in order to interact with Trac for anything other than read only operations, you need
    to <a href="<?php print $trac_url . 'auth/register/'; ?>">register</a> with Mac OS Forge for a Wordpress/Trac combined
    account.</p>

    <p>If you think you've found a bug either in one of our <a href="ports.php">available ports</a> or in MacPorts itself, or
    on the other hand if you'd like to make a contribution of any kind to the project, feel free to <a href="<?php print $trac_url
    . 'newticket'; ?>">open a ticket</a> to help us look into the problem and/or submission. Please keep in mind that we
    usually get a fairly high number of duplicate reports for common problems and therefore appreciate any help we can get in
    the process of streamlining our ticket duties. <a href="<?php print $trac_url . 'search'; ?>">Searching</a> the Trac database
    &amp; mailing list archives (above), and reading our <a href="<?php print $trac_url . 'wiki/FAQ'; ?>">FAQ</a> &amp;
    <a href="<?php print $trac_url . 'wiki/ProblemHotlist'; ?>">problems hotlist</a> to see if your report has already been filed
    is recommended, as well as reading the <a href="<?php print $guide_url . '#project.tickets'; ?>"> ticketing guidelines</a>
    that will help you create a better report.</p>

    <p>Viewing existing tickets through the facilities offered by predefined and custom <a href="<?php print $trac_url .
    'report'; ?>">ticket reports</a> that allow for detailed queries is also available.</p>


    <h3 class="subhdr" id="IRC">IRC</h3>

    <p>For a more real-time discussion of any MacPorts related topic, the <a href="irc://chat.freenode.net/#MacPorts">
    #MacPorts</a> channel on the <a href="http://freenode.net/">Freenode network</a> is where some of us usually hang out,
    MacPorts developers and community members alike. Everyone is free and welcomed to join us, even if it is for a random
    fun conversation or a productive troubleshooting session, but please keep in mind that no one is guaranteed to be
    around at any particular moment and that channel members are not obligated to answer your questions. If you fail to
    get any traction at time, do not take it personally and simply direct your questions to the mailing lists instead.</p>

    <p>The language of choice for the IRC channel is also English, for obvious reasons of universality, so sticking to it
    is appreciated.</p>


    <h3 class="subhdr" id="Individuals">Individuals</h3>

    <p>To find out who the people behind MacPorts are and what they are up to, visit the <a href="<?php print $trac_url . 
    'wiki/MacPortsDevelopers'; ?>">team members page</a> on our Wiki.</p>

</div>


<?php
    print_footer();
?>
