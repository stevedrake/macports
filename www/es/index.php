	<?php
		$DOCUMENT_ROOT = $_SERVER['DOCUMENT_ROOT'];
		include_once("$DOCUMENT_ROOT/es/includes/common.inc");
		include_once("$DOCUMENT_ROOT/es/includes/functions.inc");
		print_header('Home de DarwinPorts', 'iso-8859-1');
	?>

		<div id="content">
			<h2 class="hdr">Introducci�n a DarwinPorts</h2>

			<p>El objetivo principal del proyecto DarwinPorts es proveer una forma
			sencilla de instalar varios productos de c�digo abierto en un sistema
			Darwin, Mac OS X, FreeBSD o Linux.</p>

			<p>En la actualidad hay unos cuantos cientos de <a href="/es/ports/">portes</a>
			completados y usables, mientras que m�s son agregados regularmente. Usted
			puede conocer sobre los portes recientemente a�adidos al susbcribirse a la
			lista de correo <a href="http://www.opendarwin.org/mailman/listinfo/cvs-darwinports-all">cvs-darwinports-all</a>.</p>

			<p>Para m�s informaci�n sobre la obtenci�n e instalaci�n de DarwinPorts, por
			favor refi�rase la secci�n <a href="/es/getdp/">Obtenci�n de DarwinPorts</a>
			de esta p�gina. Tambi�n aseg�rese de revisar la <a href="/docs/">documentaci�n</a>
			y si tiene preguntas o sufre de alg�n problema, puede <a href="/es/help/">buscar ayuda</a>.</p>

			<p>Reportes de "bugs", peticiones de funcionalidad y nuevos portes deber�an ser
			introducidos en <a href="http://www.opendarwin.org/bugzilla/">Bugzilla</a>.</p>

		<div id="news">
			<h2 class="hdr">Noticias del Proyecto</h2>
			
		<?php
			print_headlines();
        ?>
       </div>
      </div>
     </div>

<?php
	print_footer();
?>
