<?php

/**
 * Description of IntentionTemplateSimplePost
 *
 *
 * Created by PhpStorm.
 * @package          Wordpress
 * @createDate       2017-11-11
 * @version          0.0.1
 * @author           Piotr Kuźnik <piotr.damian.kuznik@gmail.com>
 * @licence          License: GPLv3
	Restrict User Access for WordPress
	Copyright (C) 2017 Piotr Kuźnik <piotr.damian.kuznik@gmail.com>
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
class EParishTemplateSimplePost implements IEParishPlugin {
	
	/**
	 * @var string
	 */
	private $templatesPath;

    /**
     * IntentionTemplateSimplePost constructor.
     */
	public function __construct() {
		$this->templatesPath = PATH_EPARISH.'/templates/';
	}
	
	/**
	 * plug
	 *
	 */
	public function plug() {
		add_filter('single_template', [$this, 'register'], 12);
	}
	
	/**
	 * register
	 *
	 * @return null|string
	 */
	public function register() {
		$template = NULL;
		global $wp_query;
		/** @var WP_Post $post */
		if (!is_null($wp_query)) {
			$post = $wp_query->get_queried_object();
			if ($post->post_type == AbsIntentionPlugin::POST_TYPE) {
				$template = $this->templatesPath.'showSimpleItemIntention.php';
			}
			if ($post->post_type == AbsCarolPlugin::POST_TYPE) {
				$template = $this->templatesPath.'showSimpleItemCarol.php';
			}
			if ($post->post_type == AbsAdvertisementPlugin::POST_TYPE) {
				$template = $this->templatesPath.'showSimpleItemAdvertisement.php';
			}
		}
		
		return $template;
	}
}