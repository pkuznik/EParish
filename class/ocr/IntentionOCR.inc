<?php

require_once PATH_EPARISH.'/class/bean/IntentionBean.inc';
require_once PATH_EPARISH.'/interface/IOCR.inc';
require_once PATH_EPARISH.'/class/parser/ParserFileByExtensions.inc';

/**
 * Description of IntentionOCR
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
class IntentionOCR implements IOCR {
	
	/**
	 * @var \IntentionBean
	 */
	private $bean     = FALSE;
	
	private $date     = NULL;
	private $hour     = NULL;
	private $intencje = [];
	private $patron   = NULL;
	
	private $multi_intencja = FALSE;

    /**
     * @param string $filename
     * @throws Exception
     */
	public function run($filename) {
		$data = ParserFileByExtensions::getData($filename);
		
		foreach ($data as $line) {
			$this->ocrSimpleLine(trim($line));
		}
		
		if ($this->bean) {
			for ($i = 0; $i < count($this->intencje); $i++) {
				$this->bean->set('intencja_'.($i + 1), $this->intencje[ $i ]);
			}
			$this->bean->save();
		}
	}

    /**
     * @param string $lineString
     * @throws Exception
     */
	private function ocrSimpleLine($lineString) {
		if (preg_match('/^[a-zA-ZłŁśŚąĄ]* [0-9]{2}\.[IVX0-9]{1,4}\.[0-9]{4}r\./', $lineString, $matches)) {
			$this->date   = str_replace(['r.', '.'], ['', '-'], explode(' ', $matches[0])[1]);
			$this->patron = trim(str_replace($matches[0], '', $lineString));
		} elseif (preg_match('/^[0-9]{1,2}:[0-9]{2}-/', $lineString, $matches)) {
			if ($this->bean) {
				for ($i = 0; $i < count($this->intencje); $i++) {
					$this->bean->set('intencja_'.($i + 1), $this->intencje[ $i ]);
				}
				$this->intencje       = [];
				$this->multi_intencja = FALSE;
				$this->bean->save();
				$this->bean = false;
			}
			
			$this->hour = trim(str_replace('-', '', $matches[0]));
			$this->bean = IntentionBean::getByChangeID(false, $this->date, $this->hour);
			$this->bean->set('patron', $this->patron);
			echo "\n Zapisuje: ".$this->date." ".$this->hour;
			
			$tmp_intencja = trim(str_replace($matches[0], '', $lineString));
			if (preg_match('/^I./', $tmp_intencja, $matches)) {
				$this->multi_intencja = TRUE;
				
				$tmp_intencja = str_replace($matches[0], '', $tmp_intencja);
			}
			$this->intencje[] = $tmp_intencja;
		} elseif ($this->multi_intencja || preg_match('/^[IVX]{1,4}\./', $lineString)) {
			$this->intencje[] = trim(preg_replace('/^[IVX]{1,3}\./', '', $lineString));
		} elseif (preg_match('/^(Chrzest|Roczki|Po Mszy św.|Po Mszy|Chrzty:|Roczek:)/', $lineString)) {
			$il                    = count($this->intencje) - 1;
			$this->intencje[ $il ] .= '</br>'.$lineString;
		} elseif(empty($lineString)) {
			//nic do zrobienia
		} else {
			echo "\nNie znaleziono schematu dla następującej lini:";
			echo "<b>$lineString</b>\n";
		}
	}
}