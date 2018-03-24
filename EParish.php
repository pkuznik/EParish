<?php
/*
 * Plugin Name: eParish
 * Plugin URI: https://github.com/pkuznik/EParish/
 * Description: Strona twojej wirtualnej parafii
 * Version: 1.7
 * Author: Piotr Kuźnik <piotr.damian.kuznik@gmail.com>
 * Author URI: https://github.com/pkuznik/
 * Text Domain: intencje mszalne, ogłoszenia, plan kolęd
 * Compatibility: WordPress 4.9.4
 * GitHub Plugin URI: https://github.com/pkuznik/EParish/
 * GitHub Branch: master
 * License: GPLv3
 */

/*
	Restrict User Access for WordPress
	Copyright (C) 2017-2018 Piotr Kuźnik <piotr.damian.kuznik@gmail.com>

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
final class EParish {
	
	/**
	 * @var \EParish
	 */
	private static $instance;
	
	/**
	 * @var array[IIntencjaPlugin]
	 */
	private $plugins = [];
	
	/**
	 * IntencjeMszalne2 constructor.
	 */
	private function __construct() {
		$this->loadPlugin();
	}
	
	private function __clone() {}
	
	/**
	 * getInstance
	 *
	 * @return \EParish
	 */
	public static function getInstance() {
		if (!self::$instance) {
			self::$instance = new EParish();
		}
		
		return self::$instance;
	}
	
	/**
	 * @throws \Exception
	 */
	private function loadPlugin() {
		if (!file_exists(PATH_EPARISH.'/class/plugins')) {
			throw new Exception('Dir Plugins is not exists!');
		}
		require_once PATH_EPARISH.'/interface/IEParishPlugin.inc';

		require_once PATH_EPARISH.'/class/AbsIntentionPlugin.inc';
		require_once PATH_EPARISH.'/class/AbsCarolPlugin.inc';
		require_once PATH_EPARISH.'/class/AbsAdvertisementPlugin.inc';
		require_once PATH_EPARISH.'/class/EParishMenuPage.inc';

		foreach (new DirectoryIterator(PATH_EPARISH.'/class/plugins') as $file) {
			if (!$file->isFile()) continue;
			if ($file->getExtension() !== 'inc') continue;
			
			require_once $file->getPathname();
			$plugin = str_replace('.inc', '', $file->getFilename());
			
			/** @var \IEParishPlugin $objPlugin */
			$objPlugin = new $plugin();
			
			if (!($objPlugin instanceof IEParishPlugin)) {
				unset($objPlugin);
				continue;
			}
			
			$objPlugin->plug();
			$this->plugins[] = $objPlugin;
		}
	}
	
	/**
	 *
	 * @param bool $num
	 *
	 * @return array|bool|mixed
	 */
	public static function getRomanNumbers($num = FALSE) {
		$numbers = [
			1  => 'I',
			2  => 'II',
			3  => 'III',
			4  => 'IV',
			5  => 'V',
			6  => 'VI',
			7  => 'VII',
			8  => 'VIII',
			9  => 'IX',
			10 => 'X',
			11 => 'XI',
			12 => 'XII',
		];
		
		if ($num === FALSE) {
			return $numbers;
		}
		
		return (isset($numbers[ $num ])) ? $numbers[ $num ] : FALSE;
	}
	
	/**
	 *
	 * @param bool $num
	 *
	 * @return array|bool|mixed
	 */
	public static function getWeekName($num = false) {
		$week  = [
			0 => 'Niedziela',
			1 => 'Poniedziałek',
			2 => 'Wtorek',
			3 => 'Środa',
			4 => 'Czwartek',
			5 => 'Piątek',
			6 => 'Sobota',
		];
		if ($num === FALSE) {
			return $week;
		}
		
		return (isset($week[$num])) ? $week[$num] : false;
	}

    /**
     * @return string
     */
	public static function getIntentionPostType() {
		require_once PATH_EPARISH.'/class/AbsIntentionPlugin.inc';
		
		return AbsIntentionPlugin::POST_TYPE;
	}
	
	/**
	 * @return string
	 */
	public static function getCarolPostType() {
		require_once PATH_EPARISH.'/class/AbsCarolPlugin.inc';
		
		return AbsCarolPlugin::POST_TYPE;
	}
	
	/**
	 * @return string
	 */
	public static function getAdvertisementPostType() {
		require_once PATH_EPARISH.'/class/AbsAdvertisementPlugin.inc';
		
		return AbsAdvertisementPlugin::POST_TYPE;
	}
}

if (!defined('ABSPATH')) {
	header('Status: 403 Forbidden');
	header('HTTP/1.1 403 Forbidden');
	exit;
}
define('PATH_EPARISH', plugin_dir_path( __FILE__ ));
add_action( 'plugins_loaded', array('EParish', 'getInstance' ) );