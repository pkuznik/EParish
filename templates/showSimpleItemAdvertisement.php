<?php
/**
 * Created by PhpStorm.
 * @package          wp_parafia
 * @createdate       2017-04-24 07:33
 * @lastmodification 2017-04-24 07:33
 * @version          0.0.1
 * @author           Piotr Kuźnik <piotr.damian.kuznik@gmail.com>
 */

get_header(); ?>

<div class="site-content-contain">
    <div id="content" class="site-content" style="padding: 0; ">
        <div class="wrap">
            <main id="main" class="site-main" role="main">
                <?php
                // Start the loop.
                while ( have_posts() ) : the_post();

                    // Include the single post content template.
                    //get_template_part( 'template-parts/content', 'single' );
                    include __DIR__.'/content/singleAdvertisement.php';

                    // End of the loop.
                endwhile;

                ?>
            </main><!-- .site-main -->
        </div><!-- .wrap -->
        <div class="wrap" style="padding-top: 50px !important; padding-bottom: 20px;">
            <div style="float: left;"><?php previous_post_link('&laquo; %link', 'Poprzednie'); ?> </div>
            <div style="float: right"><?php next_post_link('%link &raquo;', 'Następne'); ?></div>
        </div>
    </div>
</div>
<?php get_footer(); ?>
