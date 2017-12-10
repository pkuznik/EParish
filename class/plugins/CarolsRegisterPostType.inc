<?php

/**
 * Description of ParserFileDOC
 *
 *
 * Created by PhpStorm.
 * @package          Wordpress
 * @createDate       2017-11-11
 * @version          0.0.1
 * @author           Piotr Kuźnik <piotr.damian.kuznik@gmail.com>
 * @licence          License: GPLv3
 * Restrict User Access for WordPress
 * Copyright (C) 2017 Piotr Kuźnik <piotr.damian.kuznik@gmail.com>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
class CarolsRegisterPostType extends AbsCarolPlugin implements IEParishPlugin {
	
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
		register_post_type(CarolsRegisterPostType::POST_TYPE, [
			'labels'               => [
				'name'          => __('Plan kolęd'),
				'singular_name' => __('Kolędę'),
			],
			'public'               => TRUE,
			'has_archive'          => TRUE,
			'rewrite'              => ['slug' => 'koleda', 'with_front' => FALSE],
			'exclude_from_search'  => FALSE,
			'menu_position'        => 6,
			'supports'             => FALSE,
			'register_meta_box_cb' => [$this,'printFormPost'],
		]);
	}
	
	/**
	 * printFormPost
	 *
	 */
	public function printFormPost(){
		add_meta_box('box-'.CarolsRegisterPostType::POST_TYPE, 'Plan kolęd', function ($obj) {
			if (!is_admin()) {
				return;
			}
			
			require_once PATH_EPARISH.'/class/HTMLInput.inc';
			wp_nonce_field(-1, CarolsRegisterPostType::NONCE_KEY);
			
			echo '<div>';
			
			echo HTMLInput::create($obj, 'crl_date', 'Data', 'date',true);
			echo HTMLInput::create($obj, 'crl_hour', 'Godzina', 'time', true);
			echo HTMLInput::create($obj, 'crl_dscrpt', 'Ulica', 'text', true, 'width:300px');
			
			echo '</div>';
			
		}, CarolsRegisterPostType::POST_TYPE, 'normal', 'high', NULL);
	}
}