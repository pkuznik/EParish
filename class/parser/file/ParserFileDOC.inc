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
class ParserFileDOC implements IParserFile {
	
	/**
	 * getText
	 *
	 * @param $filename
	 *
	 * @return mixed|string
	 */
	public function getText($filename) {
		$fileHandle = fopen($filename, "r");
		$line       = @fread($fileHandle, filesize($filename));
		$lines      = explode(chr(0x0D), $line);
		$outtext    = "";
		foreach ($lines as $thisline) {
			$pos = strpos($thisline, chr(0x00));
			if (($pos !== FALSE) || (strlen($thisline) == 0)) {
			} else {
				$outtext .= $thisline." ";
			}
		}
		$outtext = preg_replace("/[^a-zA-Z0-9\s\,\.\-\n\r\t@\/\_\(\)]/", "", $outtext);
		
		return $outtext;
	}
}