<?php
require_once PATH_EPARISH.'/class/parser/ParserFileByExtensions.inc';
require_once PATH_EPARISH.'/class/ocr/AdvertisementOCR.inc';

/**
 * Description of AdvertisementlFileParser
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
class AdvertisementlFileParser extends EParishMenuPage implements IEParishPlugin {
	
	/**
	 * plug
	 *
	 */
	public function plug() {
		add_action('admin_menu', [$this, 'addMenu']);
	}
	
	public function addMenu() {
		$this->addHeadMenu();
		
		add_submenu_page($this->headMenu, 'Załaduj ogłoszenia z pliku', 'Załaduj ogłoszenia z pliku', 'manage_options', 'read-advertisement-from-file', [$this, 'createPageLoadDataFromFile']);
	}
	
	private function createPageHTML() {
		?>
        <div class="wrap">
            <h1 class="wp-heading-inline">Ogłoszenia parafialne</h1>

            <hr class="wp-header-end">


            <p>Załaduj ogłoszeia z pliku (Akceptowane rozszerzenia: *.<?php echo implode(', *.', ParserFileByExtensions::getSupportedExtensions()); ?>)</p>
            <form id="featured_upload" method="post" action="#" enctype="multipart/form-data">
                <input type="file" name="advertisement_upload" id="advertisement_upload" multiple="false"/>
                <input type="hidden" name="post_id" id="post_id" value="0"/>
				<?php wp_nonce_field('advertisement_upload', 'advertisement_upload_nonce'); ?>
                <input id="submit_carol_upload" name="Załaduj" type="submit" value="Upload"/>
            </form>

            <div id="ajax-response"></div>
            <br class="clear">
        </div>
		<?php
    }
	
	public function createPageLoadDataFromFile() {
		
		if (isset($_POST['advertisement_upload_nonce'], $_POST['post_id']) && wp_verify_nonce($_POST['advertisement_upload_nonce'], 'advertisement_upload') && $_POST['post_id'] == 0) {
			// The nonce was valid and the user has the capabilities, it is safe to continue.
			
			// These files need to be included as dependencies when on the front end.
			require_once(ABSPATH.'wp-admin/includes/image.php');
			require_once(ABSPATH.'wp-admin/includes/file.php');
			require_once(ABSPATH.'wp-admin/includes/media.php');
			
			// Let WordPress handle the upload.
			// Remember, 'intencje_upload' is the name of our file input in our form above.
			$attachment_id = media_handle_upload('advertisement_upload', 0);
			
			echo "<pre>";
			if (is_wp_error($attachment_id)) {
				echo "\nBłąd podczas przesłania!";
			} else {
				echo "\nPlik został przesłany!";
				
				$url = get_attached_file($attachment_id);
				
				echo "\nPobieram dane z pliku.. \n$url\n";
    
				$ocr = new AdvertisementOCR();
				$ocr->run($url);
			}
			echo "\nDane zostały zapisane do bazy!";
			echo "</pre>";
		}
		
		
		$this->createPageHTML();
	}
}