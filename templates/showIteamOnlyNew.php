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
            <?php
            $original_query = $wp_query;
            $wp_query       = NULL;
            $args           = [
                'post_type'      => EParish::getAdvertisementPostType(),
                'orderby'        => 'ID',
                'order'          => 'DESC',
                'posts_per_page' => 1
            ];
            $wp_query       = new WP_Query($args);
            ?>
            <div class="wrap" style="padding-bottom:0px;">
                <header class="entry-header" style="padding-bottom:0px;">
			        <?php the_title('<h2 class="entry-title">', '</h2>'); ?>
			
			        <?php twentyseventeen_edit_link(get_the_ID()); ?>

                </header><!-- .entry-header -->
            </div>
            <div class="wrap" style="padding-bottom: 0;">

                <header class="entry-header" style="width: 100%; display: block;margin-bottom: 20px;">
                
				<?php
				
				if (have_posts()) :
                    while (have_posts()) :
                        the_post();
				        
				        ?>
                        <h1>Ogłoszenia parafialne <?php  echo date('d.m.Y', strtotime(get_post_meta(get_the_ID(), "adv_date", TRUE))); ?>r.</h1>
	                    <?php twentyseventeen_edit_link(get_the_ID()); ?>
                        <table cellspacing="0" cellpadding="0" style="border: none; padding: 0;margin: 0;">
                            <tbody style="border: none;"><?php
                       
                        $max = intval(get_post_meta(get_the_ID(), "adv_max_elements", TRUE));
                        
                        for($i=1; $i<=$max; $i++) {
	                        echo '<tr style="border: none;"><td style="width: 30px;text-align: right;vertical-align: top;">'.$i.'.</td>';
	                        echo '<td>'. get_post_meta(get_the_ID(), "adv_".$i, TRUE).'</td></tr>';
                        }
                        
                    endwhile;
				endif;
					?>
                </tbody>
                    </table>
                </header>
            </div>
            <div class="wrap" style="padding-bottom: 0px;padding-top: 10px">
                <header class="entry-header" style="width: 100%; display: block;padding: 0;">
                    <div style="float: left;"><?php previous_post_link('&laquo; %link', 'Poprzednie'); ?> </div>
                </header>
            </div>
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