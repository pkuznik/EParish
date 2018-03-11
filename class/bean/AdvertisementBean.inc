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
class AdvertisementBean {
	
	const MAX_ELEMENTS = 'adv_max_elements';
	
	/**
	 * @var array
	 */
	private static $keys = [
		'date',
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
	 * @var int
	 */
	private $maxElements = 0;
	
	/**
	 * AdvertisementBean constructor.
	 *
	 * @param integer|false $ID
	 * @param string        $title
	 * @param int           $maxElements
	 */
	private function __construct($ID, $title, $maxElements = 0) {
		$this->ID          = $ID;
		$this->title       = $title;
		$this->maxElements = $maxElements;
	}
	
	/**
	 * @param string $date
	 *
	 * @return \AdvertisementBean
	 */
	public static function getByDate($date) {
		$datetime = date('Ymd', strtotime("$date"));
		
		$obj = new AdvertisementBean(FALSE, $datetime);
		
		$post = get_page_by_title($datetime, OBJECT, AbsAdvertisementPlugin::POST_TYPE);
		if (is_a($post, 'WP_Post') && $post->post_type == AbsAdvertisementPlugin::POST_TYPE) {
			$maxElements = intval(get_post_meta($post->ID, self::MAX_ELEMENTS, TRUE));
			$obj         = new AdvertisementBean($post->ID, $datetime, $maxElements);
			
			for ($i = 1; $i <= $maxElements; $i++) {
				$obj->addElement(get_post_meta($post->ID, 'adv_'.$i, TRUE));
			}
			
		}
		$obj->set('date', date('Y-m-d', strtotime("$date")));
		
		unset($post);
		
		return $obj;
	}
	
	/**
	 * @param string $dscrpt
	 */
	public function addElement($dscrpt) {
		$this->maxElements++;
		$this->data[$this->maxElements ] = preg_replace('/^'.$this->maxElements.'\./', '', $dscrpt);
	}
	
	/**
	 * @param string $dscrpt
	 */
	public function addToLastElementToNewLine($dscrpt) {
		$this->data[$this->maxElements ] .= '</br>'.$dscrpt;
	}
	
	/**
	 *
	 * @param string $k {
	 * @type string  $date
	 * }
	 * @param string $v
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
		if (!isset($this->data['date'])) {
			throw new Exception('Invalid date!');
		}
		$title = str_replace(['-', ':', ' '], '', $this->data['date']);
		$meta  = [];
		
		foreach ($this->data as $k => $v) {
			$meta[ 'adv_'.$k ] = $v;
		}
		
		$meta[self::MAX_ELEMENTS] = $this->maxElements;
		
		$this->ID = wp_insert_post([
			'ID'          => (($this->ID) ? $this->ID : 0),
			'post_title'  => $title,
			'post_name'   => $title,
			'post_status' => 'publish',
			'meta_input'  => $meta,
			'post_type'   => AbsAdvertisementPlugin::POST_TYPE,
		]);
		$this->ID = (is_a($this->ID, 'WP_Error')) ? FALSE : $this->ID;
		
		return $this->ID;
	}
}