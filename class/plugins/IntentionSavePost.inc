<?php


/**
 * Description of IntentionSavePost
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
class IntentionSavePost extends AbsIntentionPlugin implements IEParishPlugin {
	
	/**
	 * plug
	 *
	 */
	public function plug() {
		add_filter('wp_insert_post_data', [$this, 'changePostName'], '99', 1);
		add_action("save_post", [$this, 'savePost']);
	}
	
	/**
	 *
	 * @param $post_id
	 *
	 * @return mixed
	 */
	public function savePost($post_id) {
		if (!isset($_POST[ IntentionSavePost::NONCE_KEY ]) || !wp_verify_nonce($_POST[ IntentionSavePost::NONCE_KEY ], -1)) {
			return $post_id;
		}
		
		if (!current_user_can("edit_post", $post_id)) {
			return $post_id;
		}
		
		if (defined("DOING_AUTOSAVE") && DOING_AUTOSAVE) {
			return $post_id;
		}
		
		$slug = IntentionRegisterPostType::POST_TYPE;
		if ($slug != $_POST['post_type']) {
			return $post_id;
		}
		
		$keys = [
			'im_date',
			'im_hour',
			'im_patron',
		];
		for ($i = 1; $i <= 5; $i++) {
			$keys[] = 'im_intencja_'.$i;
		}
		
		foreach ($keys as $key) {
			$value = '';
			if (isset($_POST[ $key ])) {
				$value = $_POST[ $key ];
			}
			update_post_meta($post_id, $key, $value);
		}
		
	}
	
	/**
	 *
	 * @param array $data
	 *
	 * @return mixed
	 */
	public function changePostName($data) {
		if ($data['post_type'] == IntentionSavePost::POST_TYPE) {
			$time = false;
			if (isset($_POST['im_date']) && isset($_POST['im_hour'])) {
				$time = $_POST['im_date'].' '.$_POST['im_hour'];
			} else {
				//$time = date('Y-m-d H:i:s');
			}
			

			if ($time){
				$title = date('YmdHis', strtotime($time));
				
				$data['post_name']  = $title;
				$data['post_title'] = $title;
			}
		}
		
		return $data;
	}
}