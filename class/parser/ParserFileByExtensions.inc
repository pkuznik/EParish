<?php
require_once PATH_EPARISH.'/class/parser/AbsParserFile.inc';
require_once PATH_EPARISH.'/interface/IParserFile.inc';

/**
 * Description of ParserFileByExtensions
 *
 *
 * Created by PhpStorm.
 * @package          eParish
 * @createDate       2017-11-13
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
class ParserFileByExtensions {
	
	/**
	 * getData
	 *
	 * @param string $filename
	 *
	 * @return array
	 */
	static function getData($filename) {
		$exts = explode('.', $filename);
		$ext = $exts[count($exts) -1];
		
		$fileClass = 'ParserFile'.strtoupper($ext).'.inc';
		$className = 'ParserFile'.strtoupper($ext);
		$path = __DIR__.'/file/';
		if (file_exists($path.$fileClass)) {
			
			
			require_once $path.$fileClass;
			
			if (class_exists($className)) {
				$parser = new $className();
				
				if ($parser instanceof IParserFile) {
					$text = $parser->getText($filename);
					
					return explode(PHP_EOL, $text);
				}
			}
			
		}
		
		
		return [];
	}
	
	/**
	 * getSupportedExtensions
	 *
	 * @return array
	 */
	static function getSupportedExtensions() {
		$ext = [];
		foreach (new DirectoryIterator( __DIR__.'/file') as $file) {
			if (!$file->isFile() || $file->isDot()) continue;
			$name = str_replace(['ParserFile', '.inc'], '', $file->getFilename());
			
			$ext[] = strtolower($name);
		}
		
		return $ext;
	}
}