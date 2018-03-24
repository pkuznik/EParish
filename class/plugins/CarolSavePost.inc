<?php


/**
 * Description of CarolSavePost
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
class CarolSavePost extends AbsCarolPlugin implements IEParishPlugin {
	
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
		error_log('POST');
		error_log(print_r($_POST,true));
		if (!isset($_POST[ CarolSavePost::NONCE_KEY ]) || FALSE === wp_verify_nonce($_POST[ CarolSavePost::NONCE_KEY ], -1)) {
			error_log('NONCE');
			error_log((wp_verify_nonce($_POST[ CarolSavePost::NONCE_KEY ], -1)) ? 't' : 'f');
			return $post_id;
		}
		
		if (!current_user_can("edit_post", $post_id)) {
			error_log('CAN');
			return $post_id;
		}
		
		if (defined("DOING_AUTOSAVE") && DOING_AUTOSAVE) {
			return $post_id;
		}
		
		$slug = CarolSavePost::POST_TYPE;
		if ($slug != $_POST['post_type']) {
			return $post_id;
		}
		
		$keys = [
			'crl_date',
			'crl_hour',
			'crl_dscrpt',
		];
		
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
		
		if ($data['post_type'] == CarolSavePost::POST_TYPE) {
			if (isset($_POST['crl_date']) && isset($_POST['crl_hour'])) {
				$time = $_POST['crl_date'].' '.$_POST['crl_hour'];
			} else {
				$time = date('Y-m-d H:i:s');
			}
			
			if (!defined('CAROLS_NOT_CHANGE_TITLE')) {
				
				$title = date('YmdHis', strtotime($time)).str_replace(' ', '', $_POST['crl_dscrpt']);
				$data['post_name']  = $title;
				$data['post_title'] = $title;
			}
		}
		
		return $data;
	}
}