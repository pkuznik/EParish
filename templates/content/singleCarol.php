<article id="post-<?php the_ID(); ?>" <?php post_class(); ?>>

	<?php

	$time  = strtotime(get_post_meta(get_the_ID(), "crl_date", TRUE));
	$week  = EParish::getWeekName();
	$month = EParish::getRomanNumbers();
	?>
    <header class="entry-header">
        <h1>
			<?php echo($week[ date('w', $time) ]); ?>
            -
			<?php echo date('d', $time); ?>
			<?php echo $month[ intval(date('m', $time)) ]; ?>
			<?php echo date('Y').'r.' ?>
	        <?php echo 'g. '.get_post_meta(get_the_ID(), "crl_hour", TRUE); ?>
			<?php echo get_post_meta(get_the_ID(), "crl_dscrpt", TRUE); ?>
        </h1>
       
    </header>
</article><!-- #post-## -->
