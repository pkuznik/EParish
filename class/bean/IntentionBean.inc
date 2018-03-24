<?php

/**
 * Description of IntentionBean
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
final class IntentionBean {
	/**
	 * @var array
	 */
	private $data = [];
	
	/**
	 * @var string
	 */
	private $title;
	
	/**
	 * @var bool|integer
	 */
	private $ID = FALSE;
	
	/**
	 * @var array
	 */
	private static $keys = [
		'date', 'hour', 'patron',
		'intencja_1',
		'intencja_2',
		'intencja_3',
		'intencja_4',
		'intencja_5',
	];
	
	/**
	 * ChangeLog constructor.
	 *
	 */
	private function __construct($ID, $title) {
		$this->ID = $ID;
		$this->title = $title;
	}
	
	/**
	 * getByChangeID
	 *
	 * @param bool $datetime
	 * @param null $date
	 * @param null $hour
	 *
	 * @return \IntentionBean
	 */
	public static function getByChangeID($datetime = false, $date = null, $hour = null) {
		if (!$datetime) {
			$datetime = date('YmdHis',strtotime("$date $hour"));
		}
		$obj = new IntentionBean(FALSE, $datetime);
		
		$post = get_page_by_title($datetime, OBJECT, AbsIntentionPlugin::POST_TYPE);
		if (is_a($post, 'WP_Post') && $post->post_type == AbsIntentionPlugin::POST_TYPE) {
			$obj = new IntentionBean($post->ID, $datetime);
			
			foreach (self::$keys as $key) {
				$obj->set($key, get_post_meta($post->ID, 'im_'.$key, TRUE));
			}
		} else {
			$obj->set('date', date('Y-m-d',strtotime("$date")));
			$obj->set('hour', date('H:i',strtotime("$hour")));
		}
		unset($post);
		
		return $obj;
	}
	
	
	/**
	 *
	 * @param string $k {
	 *
	 * @type string  $date
	 * @type string  $hour
	 * @type string  $patron
	 * @type string  $intencja_1
	 * @type string  $intencja_2
	 * @type string  $intencja_3
	 * @type string  $intencja_4
	 * @type string  $intencja_5
	 *
	 * }
	 * @param mixed  $v
	 */
	public function set($k, $v) {
		if (in_array($k, self::$keys)) {
			$this->data[ $k ] = $v;
		}
	}
	
	
	/**
	 *
	 * @param string $k {
	 *
	 * @type string  $date
	 * @type string  $hour
	 * @type string  $patron
	 * @type string  $intencja_1
	 * @type string  $intencja_2
	 * @type string  $intencja_3
	 * @type string  $intencja_4
	 * @type string  $intencja_5
	 *
	 * }
	 *
	 * @return string|null
	 */
	public function get($k) {
		if (isset($this->data[ $k ])) {
			return $this->data[ $k ];
		}
		
		return NULL;
	}
	
	/**
	 * save
	 *
	 * @return bool|int
	 * @throws \Exception
	 */
	public function save() {
		if (!isset($this->data['date']) || !isset($this->data['hour'])) {
			throw new Exception('Invalid date or hour!');
		}
		$title   = date('YmdHis', strtotime($this->data['date'].' '.$this->data['hour']));
		$meta    = [];
		
		foreach ($this->data as $k => $v) {
			$meta[ 'im_'.$k ] = $v;
		}

		$this->ID = wp_insert_post([
			'ID'           => (($this->ID) ? $this->ID : 0),
			'post_title'   => $title,
			'post_name'    => $title,
			'post_status'  => 'publish',
			'meta_input'   => $meta,
			'post_type'    => AbsIntentionPlugin::POST_TYPE,
		]);
		$this->ID = (is_a($this->ID, 'WP_Error')) ? FALSE : $this->ID;
		
		return $this->ID;
	}
}