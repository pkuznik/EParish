<article id="post-<?php the_ID(); ?>" <?php post_class(); ?>>

	<?php

	$time  = strtotime(get_post_meta(get_the_ID(), "crl_date", TRUE));
	$week  = EParish::getWeekName();
	$month = EParish::getRomanNumbers();
	?>
    <header class="entry-header" style="width: 100%">
        <h1>
            Ogłoszenia parafialne <?  echo get_post_meta(get_the_ID(), "adv_date", TRUE); ?>
        </h1>
        <table cellspacing="0" cellpadding="0" style="border: none; padding: 0;margin: 0;">
            <tbody style="border: none;">
            <?php

            $max = intval(get_post_meta(get_the_ID(), "adv_max_elements", TRUE));

            for($i=1; $i<=$max; $i++) {
                echo '<tr><td style="width: 50px;">'.$i.'.</td>';
                echo '<td>'. get_post_meta(get_the_ID(), "adv_".$i, TRUE).'</td></tr>';
            }

            ?>
            </tbody>
        </table>
    </header>
</article><!-- #post-## -->
