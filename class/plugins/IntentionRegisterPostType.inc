<?php

/**
 * Description of IntentionRegisterPostType
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
class IntentionRegisterPostType extends AbsIntentionPlugin implements IEParishPlugin {
 
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
		register_post_type(IntentionRegisterPostType::POST_TYPE, [
			'labels'               => [
				'name'          => __('Intencje mszalne'),
				'singular_name' => __('Intencja'),
			],
			'public'               => TRUE,
			'has_archive'          => TRUE,
			'rewrite'              => ['slug' => 'intencja', 'with_front' => FALSE],
			'exclude_from_search'  => FALSE,
			'menu_position'        => 5,
			'supports'             => FALSE,
			'register_meta_box_cb' => [$this,'printFormPost'],
		]);
	}
	
	/**
	 * printFormPost
	 *
	 */
	public function printFormPost(){
		add_meta_box('box-'.IntentionRegisterPostType::POST_TYPE, 'Intencja Mszalna', function ($obj) {
			if (!is_admin()) {
				return;
			}
			
			require_once PATH_EPARISH.'/class/HTMLInput.inc';
			wp_nonce_field(-1, IntentionRegisterPostType::NONCE_KEY);
			echo '<div>';
			
			echo HTMLInput::create($obj, 'im_date', 'Data', 'date',true);
			echo HTMLInput::create($obj, 'im_hour', 'Godzina', 'time', true);
			echo HTMLInput::create($obj, 'im_patron', 'Patron dnia', 'text', false, 'width:300px');
			
			echo '</div>';
			echo '<div>&nbsp;</div>';
			
			for ($i = 1; $i <= 5; $i++) {
				echo '<div>';
				echo HTMLInput::create($obj, 'im_intencja_'.$i, 'Intencja '.$i, 'text',$i == 1, 'width:calc(100% - 90px);');
				echo '</div>';
			
			}
		}, IntentionRegisterPostType::POST_TYPE, 'normal', 'high', NULL);
	}
}