<?php
require_once PATH_EPARISH.'/class/bean/AdvertisementBean.inc';
require_once PATH_EPARISH.'/interface/IOCR.inc';
require_once PATH_EPARISH.'/class/parser/ParserFileByExtensions.inc';

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
class AdvertisementOCR implements IOCR {
	
	/**
	 * @var \AdvertisementBean
	 */
	private $bean;
	
	/**
	 * @param string $filename
	 *
	 * @throws \Exception
	 */
	public function run($filename) {
		$data = ParserFileByExtensions::getData($filename);
		
		foreach ($data as $line) {
			$this->ocrSimpleLine(trim($line));
		}
		
		$this->bean->save();
	}
	
	/**
	 * @param string $line
	 */
	private function ocrSimpleLine($line) {
		if (preg_match('/^Ogłoszenia Parafialne/', $line) && preg_match('/[0-9]{1,2}\.[0-9]{1,2}\.[0-9]{4}r\./', $line, $matches)) {
			$date = str_replace('r.', '', $matches[0]);
			$this->bean = AdvertisementBean::getByDate($date);
		} elseif (preg_match('/^[0-9]{1,2}\./', $line)) {
			$this->bean->addElement($line);
		} elseif (!empty($line)) {
			$this->bean->addToLastElementToNewLine($line);
		}
	}
}