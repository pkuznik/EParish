<?php
/**
 * Description of IntencjaTemplatesForPage
 *
 *
 * Created by PhpStorm.
 * @package          eParish
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
class EParishTemplatesForPage implements IEParishPlugin {
	
	/**
	 * @var array
	 */
	private $templates = [];
	
	/**
	 * @var string
	 */
	private $templatesPath;

    /**
     * IntentionTemplatesForPage constructor.
     */
	public function __construct() {
		$this->templatesPath = PATH_EPARISH.'/templates/';
	}
	
	/**
	 * plug
	 *
	 */
	public function plug() {
		if (version_compare(floatval(get_bloginfo('version')), '4.7', '<')) {
			// 4.6 and older
			add_filter('page_attributes_dropdown_pages_args', [$this, 'registerProjectTemplates']);
		} else {
			// Add a filter to the wp 4.7 version attributes metabox
			add_filter('theme_page_templates', [$this, 'addNewTemplate']);
		}
		
		add_filter('wp_insert_post_data', [$this, 'registerProjectTemplates']);
		add_filter('template_include', [$this, 'viewProjectTemplate']);
		
		$this->templates = [
			'showItemsOnWeeks.php' => 'Strona z intencjami (od dziś + 2 tyg)',
			'showItemsOnYear.php' => 'Strona z planem kolęd (od dziś + tyle ile wprowadzono)',
			'showIteamOnlyNew.php' => 'Strona z ostatnimi ogłoszeniami',
		//	'archive.php'    => 'Archiwum intencji',
		];
	}
	
	/**
	 *
	 * @param array $posts_templates
	 *
	 * @return array
	 */
	public function addNewTemplate($posts_templates) {
		$posts_templates = array_merge($posts_templates, $this->templates);
		return $posts_templates;
	}
	
	/**
	 *
	 * @param $atts
	 *
	 * @return mixed
	 */
	public function registerProjectTemplates($atts) {
		$cache_key = 'page_templates-'.md5(get_theme_root().'/'.get_stylesheet());
		$templates = wp_get_theme()->get_page_templates();
		if (empty($templates)) {
			$templates = [];
		}
		
		wp_cache_delete($cache_key, 'themes');
		
		$templates = array_merge($templates, $this->templates);
		
		wp_cache_add($cache_key, $templates, 'themes', 1800);
		
		return $atts;
	}
	
	/**
	 *
	 * @param string $template
	 *
	 * @return string
	 */
	public function viewProjectTemplate($template) {
		global $post;
		
		if (!$post) {
			return $template;
		}
		
		if (!isset($this->templates[ get_post_meta($post->ID, '_wp_page_template', TRUE) ])) {
			return $template;
		}
		
		$file = $this->templatesPath.get_post_meta($post->ID, '_wp_page_template', TRUE);
		
		if (file_exists($file)) {
			return $file;
		} else {
			echo $file;
		}
		
		return $template;
	}
}