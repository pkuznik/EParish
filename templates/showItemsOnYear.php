<?php

get_header();

while (have_posts()) : the_post();
	$contentW = 12;
	//	if (get_field('sidebar_aktywny')) :
	//		echo '<div class="layout with-left-sidebar js-layout">
	//				<div class="row">';
	//		get_template_part('content', 'sidebar');
	//		$contentW = 9;
	//
	//	endif;
	?>

    <article id="post-<?php the_ID(); ?>" <?php post_class('twentyseventeen-panel '); ?> >
        <div class="panel-content">
            <div class="wrap" style="padding-bottom:0px;">
                <header class="entry-header" style="padding-bottom:0px;">
					<?php the_title('<h2 class="entry-title">', '</h2>'); ?>
					
					<?php twentyseventeen_edit_link(get_the_ID()); ?>

                </header><!-- .entry-header -->
					<?php
					/* translators: %s: Name of current post */
					the_content();
					
					$timeToday = strtotime(date('Y-m-d'));
					$original_query = $wp_query;
					$wp_query       = NULL;
					$args           = [
						'post_type'      => EParish::getCarolPostType(),
						'orderby'        => 'title',
                        'order'          => 'ASC',
                        'posts_per_page' => -1
					];
					$wp_query       = new WP_Query($args);
					?>

            
            </div><!-- .wrap -->
            <div class="wrap">

                <header class="entry-header" style="width: 100%; display: block;margin-bottom: 20px;">
                    <table cellspacing="0" cellpadding="0" style="border: none; padding: 0;margin: 0;">
                        <tbody style="border: none;">
				<?php
				$week  = EParish::getWeekName();
				$month = EParish::getRomanNumbers();
				
				$lastTime = NULL;
				if (have_posts()) :
                    while (have_posts()) :
                        the_post();
				
                        $time = strtotime(get_post_meta(get_the_ID(), "crl_date", TRUE));
                        
                        echo '<tr><td style="width: 250px;">';
                        if ($time != $lastTime) {
                            echo date('d', $time);
                            echo ' ';
                            echo $month[ intval(date('m', $time)) ];
                            echo ' ';
                            echo date('Y', $time).'r.';
                            echo ' ';
                            echo ($week[ date('w', $time) ]);
                        }
                        echo '</td><td style="width: 100px;">g.&nbsp;';
                        echo get_post_meta(get_the_ID(), "crl_hour", TRUE);
                        echo '</td><td>';
                        echo get_post_meta(get_the_ID(), "crl_dscrpt", TRUE);
                        echo '</td>';
                        if (is_user_logged_in()) {
                            echo '<td  style="width: 100px;padding: 0;text-align: center;">';
	                        twentyseventeen_edit_link(get_the_ID());
                            echo '</td>';
                        }
                        echo '</tr>';
                        $lastTime = $time;
                    
                    endwhile;
				endif;
					?>
                </tbody>
                    </table>
                </header>
            </div>
            <div class="entry-content">
				<?php
				wp_link_pages([
					'before'      => '<div class="page-links">'.__('Pages:', 'twentyseventeen'),
					'after'       => '</div>',
					'link_before' => '<span class="page-number">',
					'link_after'  => '</span>',
				]);
				?>
            </div><!-- .entry-content -->
			
			<?php
			$wp_query = NULL;
			$wp_query = $original_query;
			wp_reset_postdata();
			?>
        </div><!-- .panel-content -->
    </article><!-- #post-## -->
	<?php
endwhile;
?>


<?php get_footer(); ?>