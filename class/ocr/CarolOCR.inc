<?php
require_once PATH_EPARISH.'/class/bean/CarolBean.inc';
require_once PATH_EPARISH.'/interface/IOCR.inc';
require_once PATH_EPARISH.'/class/parser/ParserFileByExtensions.inc';

/**
 * Description of CarolOCR
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
class CarolOCR implements IOCR {
	
	private $date;
	
	/**
	 * @param string $filename
	 *
	 * @throws \Exception
	 */
	public function run($filename) {
		$data = ParserFileByExtensions::getData($filename);
		define('CAROLS_NOT_CHANGE_TITLE', true);
		foreach ($data as $line) {
			
			$this->ocrSimpleLine(trim($line));
		}
	}
	
	/**
	 * @param string $lineString
	 * @throws Exception
	 */
	private function ocrSimpleLine($lineString) {
		$dscrpt = null;
		$hour = null;
		if (preg_match('/^[0-9]{1,2}\.[0-9XVI]{1,4}\.[0-9]{4} [A-Za-zĄĆĘŁŃÓŚŻŹąćęłńóśżź0-9\s]*: g\.[0-9]{1,2}:[0-9]{2}:/', $lineString, $matches) || preg_match('/^g\.[0-9]{1,2}:[0-9]{2}:/', $lineString, $matches)) {
			$dscrpt = trim(str_replace($matches[0], '', $lineString));
		}
		
		if (preg_match('/^[0-9]{1,2}\.[0-9XVI]{1,4}\.[0-9]{4}/', $lineString, $matches)) {
			
			$date_tmp = explode('.',trim($matches[0]));
			
			$date = $date_tmp[0];
			$date .= '-'.$this->roma2arabic($date_tmp[1]);
			$date .= '-'.$date_tmp[2];
			
			$this->date = $date;
		}
		if (preg_match('/g\.[0-9]{1,2}:[0-9]{2}:/', $lineString, $matches)) {
			$hour = str_replace('g.', '', trim(trim($matches[0],':')));
		}
		
		if ($this->date && !is_null($dscrpt)) {
			$bean = CarolBean::getByChangeID($this->date, $hour, $dscrpt);
			$bean->set('dscrpt', $dscrpt);
			
			$bean->save();
			echo "\nZapisuje kolędę o treści: Data({$this->date}) Godzina($hour) Opis($dscrpt)";
		}
	}
	
	/**
	 * @param string $roma
	 *
	 * @return string
	 */
	private function roma2arabic($roma) {
		switch ((string)$roma) {
			case 'I':
				return '01';
			case 'II':
				return '02';
			case 'III':
				return '03';
			case 'IV':
				return '04';
			case 'V':
				return '05';
			case 'VI':
				return '06';
			case 'VII':
				return '07';
			case 'VIII':
				return '08';
			case 'IX':
				return '09';
			case 'X':
				return '10';
			case 'XI':
				return '11';
			case 'XII':
				return '12';
		}
	}
}