<?php

/**
 * Description of AdvertisementRegisterPostType
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
class AdvertisementRegisterPostType extends AbsAdvertisementPlugin implements IEParishPlugin {
 
	/**
	 * plug
	 *
	 */
	public function plug() {
		add_action('init', [$this, 'register']);
	}
	
	/**
	 * register
	 *
	 */
	public function register() {
		register_post_type(AdvertisementRegisterPostType::POST_TYPE, [
			'labels'               => [
				'name'          => __('Ogłoszenia parafialne'),
				'singular_name' => __('Ogłoszenie'),
			],
			'public'               => TRUE,
			'has_archive'          => TRUE,
			'rewrite'              => ['slug' => 'ogloszenie', 'with_front' => FALSE],
			'exclude_from_search'  => FALSE,
			'menu_position'        => 8 ,
			'supports'             => FALSE,
			'register_meta_box_cb' => [$this,'printFormPost'],
		]);
	}
	
	/**
	 * printFormPost
	 *
	 */
	public function printFormPost(){
		add_meta_box('box-'.AdvertisementRegisterPostType::POST_TYPE, 'Ogłoszenie Parafialne', function ($obj) {
			if (!is_admin()) {
				return;
			}
			
			require_once PATH_EPARISH.'/class/HTMLInput.inc';
			wp_nonce_field(-1, AdvertisementRegisterPostType::NONCE_KEY);
			echo '<div>';
			
			echo HTMLInput::create($obj, 'adv_date', 'Data', 'date',true);
			$css = HTMLInput::$styleForLabel;
			HTMLInput::$styleForLabel .= 'width:200px';
			echo HTMLInput::create($obj, 'adv_max_elements', 'Maksymalna ilość ogłoszeń', 'number',true, '', $max);
			
			HTMLInput::$styleForLabel = $css;
			$max = intval($max);
			
			echo '</div>';
			echo '<div>&nbsp;</div>';
			HTMLInput::$styleForLabel .='height:64px;';
			for ($i = 1; $i <= $max; $i++) {
				echo '<div>';
				echo HTMLInput::createTextArea($obj, 'adv_'.$i, ''.$i.'.',false, 'height:64px;width:calc(100% - 90px);');
				echo '</div>';
			
			}
		}, AdvertisementRegisterPostType::POST_TYPE, 'normal', 'high', NULL);
	}
}