<?php

/**
 * Description of AbsParserFile
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
abstract class AbsParserFile {
	
	/**
	 * @param string $archiveFile
	 * @param string $dataFile
	 *
	 * @return string
	 */
	protected function readZippedXML($archiveFile, $dataFile) {
		$zip = new ZipArchive;
		
		if (TRUE === $zip->open($archiveFile)) {
			if (($index = $zip->locateName($dataFile)) !== FALSE) {
				$data = $zip->getFromIndex($index);
				$zip->close();
				$xml = DOMDocument::loadXML($data, LIBXML_NOENT | LIBXML_XINCLUDE | LIBXML_NOERROR | LIBXML_NOWARNING);
				
				$content = $xml->saveXML();
				$content = str_replace('</w:r></w:p></w:tc><w:tc>', " ", $content);
				$content = str_replace('</w:r></w:p>', PHP_EOL, $content);
				$content = str_replace('<text:p', PHP_EOL.'<text:p', $content);
				
				
				return strip_tags($content);
			}
			$zip->close();
		}
		
		return "";
	}
}