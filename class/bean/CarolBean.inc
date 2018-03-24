<?php

/**
 * Description of CarolBean
 *
 *
 * Created by PhpStorm.
 * @package          eParish
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
class CarolBean {
	
	/**
	 * @var array
	 */
	private static $keys = [
		'date',
		'hour',
		'dscrpt',
	];
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
	 * ChangeLog constructor.
	 *
	 */
	private function __construct($ID, $title) {
		$this->ID    = $ID;
		$this->title = $title;
	}
	
	/**
	 * @param string $date
	 * @param string $hour
	 * @param string $dscrpt
	 *
	 * @return \CarolBean
	 */
	public static function getByChangeID($date, $hour, $dscrpt) {
		$datetime = date('YmdHis', strtotime("$date $hour"));
		
		$title = $datetime.str_replace(' ', '', $dscrpt);
		
		$obj = new CarolBean(FALSE, $title);
		
		$post = get_page_by_title($datetime, OBJECT, AbsCarolPlugin::POST_TYPE);
		if (is_a($post, 'WP_Post') && $post->post_type == AbsCarolPlugin::POST_TYPE) {
			$obj = new CarolBean($post->ID, $title);
			
			foreach (self::$keys as $key) {
				$obj->set($key, get_post_meta($post->ID, 'crl_'.$key, TRUE));
			}
		} else {
			$obj->set('date', date('Y-m-d', strtotime("$date")));
			$obj->set('hour', date('H:i', strtotime("$hour")));
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
	 * @type string  $dscrpt
	 *
	 * }
	 * @param string  $v
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
	 * @type string  $dscrpt
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
		$title = str_replace(['-', ':', ' '], '', $this->data['date'].$this->data['hour'].$this->data['dscrpt']);
		$meta  = [];
		
		foreach ($this->data as $k => $v) {
			$meta[ 'crl_'.$k ] = $v;
		}
		
		$this->ID = wp_insert_post([
			'ID'          => (($this->ID) ? $this->ID : 0),
			'post_title'  => $title,
			'post_name'   => $title,
			'post_status' => 'publish',
			'meta_input'  => $meta,
			'post_type'   => AbsCarolPlugin::POST_TYPE,
		]);
		$this->ID = (is_a($this->ID, 'WP_Error')) ? FALSE : $this->ID;
		
		return $this->ID;
	}
	
}