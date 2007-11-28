<?php
    /* -*- coding: utf-8; mode: php; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- vim:fenc=utf-8:filetype=php:et:sw=4:ts=4:sts=4 */
    /* Web client to the PortIndex2MySQL script located in MacPorts base/portmgr/jobs svn directory. */
    /* $Id$ */
    /* Copyright (c) 2004, OpenDarwin. */
    /* Copyright (c) 2004-2007, The MacPorts Project. */
    $MPWEB = $_SERVER['DOCUMENT_ROOT'] . dirname($_SERVER['SCRIPT_NAME']);
    include_once("$MPWEB/includes/common.inc");
    print_header('The MacPorts Project -- Available Ports', 'utf-8');
    $portsdb_connect = mysql_pconnect($portsdb_host, $portsdb_user, $portsdb_passwd) or die("Can't connect to the MacPorts database!");
    $by = isset($_GET['by']) ? $_GET['by'] : '';
    $substr = isset($_GET['substr']) ? $_GET['substr'] : '';
?>


<div id="content">

    <h2 class="hdr">MacPorts Portfiles</h2>

<?php
    $sql = "SELECT UNIX_TIMESTAMP(activity_time) FROM $portsdb_name.log ORDER BY UNIX_TIMESTAMP(activity_time) DESC";
    $result = mysql_query($sql);
    if ($result && $row = mysql_fetch_row($result)) {
        $date = date('Y-m-d', $row[0]);
        $time = date('H:i', $row[0]);
    }
?>
    <p>The MacPorts Project currently distributes <?php print ports_count(); ?> ports, distributed across <?php print categories_count(); ?>
    different categories and available below for viewing. This form allows you to search the MacPorts software index, last
    updated on <?php echo $date; ?> at <?php echo $time; ?>.</p>

    <br />

    <form action="<?php echo $_SERVER['PHP_SELF']; ?>">
        <p>
            <label>Search by:</label>
            <select name="by">
                <option value="name"<?php if ($by == "name") { echo " selected=\"selected\""; } ?>>Software Title</option>
                <option value="desc"<?php if ($by == "desc") { echo " selected=\"selected\""; } ?>>Description</option>
                <option value="cat"<?php if ($by == "cat") { echo " selected=\"selected\""; } ?>>Category</option>
                <option value="maintainer"<?php if ($by == "maintainer") { echo " selected=\"selected\""; } ?>>Maintainer</option>
            </select>
            <input type="text" name="substr" size="40" />
        <input type="submit" value="Search" />
        </p>
    </form>



    <h3>Port Categories</h3>

    <p>View the complete <a href="<?php echo $_SERVER['PHP_SELF']; ?>?by=all">ports list (<?php print ports_count(); ?> ports)
    </a></p>

<?php
    if (!$by || (!$substr && $by != "all")) {
        $query = "SELECT DISTINCT category FROM $portsdb_name.categories ORDER BY category";
        $result = mysql_query($query);
        if ($result) {
            while ($row = mysql_fetch_assoc($result)) {
?>
                <div class="port">

                    <a href="<?php echo $_SERVER['PHP_SELF']; ?>?by=cat&amp;substr=<?php echo urlencode($row['category']); ?>">
                    <?php echo htmlspecialchars($row['category']); ?></a>

                </div>
<?php
            }
        }
    }
?>

<?php
    if ($by && ($substr || $by == "all")) {
        $fields = "name, path, version, description";
        $query = "1";
        $tables = "$portsdb_name.portfiles p";
        if ($by == "name") {
            $query .= " AND p.name LIKE '%" . mysql_real_escape_string($substr) . "%'";
        }
        if ($by == "library") {
            $query .= " AND p.name='" . mysql_real_escape_string($substr) . "'";
        }
        if ($by == "desc") {
            $query .= " AND p.description LIKE '%" . mysql_real_escape_string($substr) . "%'";
        }
        if ($by == "cat") {
            $tables .= ", $portsdb_name.categories c";
            $query .= " AND c.portfile=p.name AND c.category='" . mysql_real_escape_string($substr) . "'";
        }
        if ($by == "variant") {
            $tables .= ", $portsdb_name.variants v";
            $query .= " AND v.portfile=p.name AND v.variant='" . mysql_real_escape_string($substr) . "'";
        }
        if ($by == "platform") {
            $tables .= ", $portsdb_name.platforms pl";
            $query .= " AND pl.portfile=p.name AND pl.platform ='" . mysql_real_escape_string($substr) . "'";
        }
        if ($by == "maintainer") {
            $tables .= ", $portsdb_name.maintainers m";
            $query .= " AND m.portfile=p.name AND m.maintainer LIKE '%" . mysql_real_escape_string($substr) . "%'";
        }
        $query = "SELECT DISTINCT $fields FROM $tables WHERE $query ORDER BY name";
        $result = mysql_query($query);
        if($result) {
?>
            <p><i><?php echo mysql_num_rows($result) . ' ' . (mysql_num_rows($result) == 1 ? 'Portfile' : 'Portfiles') . 
            ' Selected'; ?></i></p>

            <dl>
<?php
            while ($row = mysql_fetch_assoc($result)) {
?>
                <dt><b><a href="<?php print $trac_url . 'browser/trunk/dports/' . $row['path'] . '/Portfile'; ?>"><?php echo
                htmlspecialchars($row['name']); ?></a></b> <?php echo htmlspecialchars($row['version']); ?></dt>
                <dd>
                    <?php echo htmlspecialchars($row['description']); ?><br />
<?php
/* MAINTAINERS */
                    $nquery = "SELECT maintainer FROM $portsdb_name.maintainers WHERE portfile='" . mysql_real_escape_string($row['name']) .
                    "' ORDER BY is_primary DESC, maintainer";
                    $nresult = mysql_query($nquery);
                    if ($nresult) {
?>
                        <i>Maintained by:</i>
<?php
                        $primary = 1;
                        while ($nrow = mysql_fetch_array($nresult)) {
                            if ($primary) { echo "<b>"; }
                            else { echo " "; }
                            $addr = obfuscate_email($nrow[0]);
                            print $addr;
                            if ($primary) { echo "</b>"; }
                            $primary = 0;
                        }
                    }

/* CATEGORIES */
                    $nquery = "SELECT category FROM $portsdb_name.categories WHERE portfile='" . mysql_real_escape_string($row['name']) .
                    "' ORDER BY is_primary DESC, category";
                    $nresult = mysql_query($nquery);
                    if ($nresult) {
?>
                        <br />
                        <i>Categories:</i>
<?php
                        $primary = 1;
                        while ($nrow = mysql_fetch_assoc($nresult)) {
                            if ($primary) { echo "<b>"; }
?>
                            <a href="<?php echo $_SERVER['PHP_SELF']; ?>?by=cat&amp;substr=<?php echo urlencode($nrow['category']); ?>">
                            <?php echo htmlspecialchars($nrow['category']); ?></a>
<?php
                            if ($primary) { echo "</b>"; }
                            $primary = 0;
                        }
                    }

/* PLATFORMS */
                    $nquery = "SELECT platform FROM $portsdb_name.platforms WHERE portfile='" . mysql_real_escape_string($row['name']) .
                    "' ORDER BY platform";
                    $nresult = mysql_query($nquery);
                    if ($nresult && mysql_num_rows($nresult) > 0) {
?>
                        <br />
                        <i>Platforms:</i>
<?php
                        while ($nrow = mysql_fetch_array($nresult)) {
                            $platform = $nrow[0];
?>
                            <a href="<?php echo $_SERVER['PHP_SELF']; ?>?by=platform&amp;substr=<?php echo urlencode($platform); ?>">
                            <?php echo htmlspecialchars($platform); ?></a>
<?php
                        }
                    }

/* DEPENDENCIES */
                    $nquery = "SELECT library FROM $portsdb_name.dependencies WHERE portfile='" . mysql_real_escape_string($row['name']) .
                    "' ORDER BY library";
                    $nresult = mysql_query($nquery);
                    if ($nresult && mysql_num_rows($nresult) > 0) {
?>
                        <br />
                        <i>Dependencies:</i>
<?php
                        while ($nrow = mysql_fetch_array($nresult)) {
                            // lib:libpng.3:libpng -> libpng
                            // might need adapting to the new port: depspec
                            $library = eregi_replace("^([^:]*:[^:]*:|[^:]*:)", "", $nrow[0]);
?>
                            <a href="<?php echo $_SERVER['PHP_SELF']; ?>?by=library&amp;substr=<?php echo urlencode($library); ?>">
                            <?php echo htmlspecialchars($library); ?></a>
<?php
                        }
                    }

/* VARIANTS */
                    $nquery = "SELECT variant FROM $portsdb_name.variants WHERE portfile='" . mysql_real_escape_string($row['name']) .
                    "' ORDER BY variant";
                    $nresult = mysql_query($nquery);
                    if ($nresult && mysql_num_rows($nresult) > 0) {
?>
                        <br />
                        <i>Variants:</i>
<?php
                        while ($nrow = mysql_fetch_array($nresult)) {
                            $variant = $nrow[0];
?>
                            <a href="<?php echo $_SERVER['PHP_SELF']; ?>?by=variant&amp;substr=<?php echo urlencode($variant); ?>">
                            <?php echo htmlspecialchars($variant); ?></a>
<?php
                        }
                    }

?>
                    <br />
                </dd>
<?php
            }

        } else {
            echo "An Error Occurred. (501)";
        }
?>
        </dl>

<?php
    }
?>

</div>


<?php
    print_footer();
?>
