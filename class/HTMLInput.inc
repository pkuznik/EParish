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
class HTMLInput {
	
	/**
	 * @var string
	 */
	static $styleForLabel = 'width: 70px;display: inline-block;text-align: right;margin-left:10px;';
	
	/**
	 * @param        $obj
	 * @param string $name
	 * @param string $label
	 * @param string $type
	 * @param bool   $required
	 * @param string $styleForInput
	 *
	 * @return string
	 */
	public static function create($obj, $name, $label, $type = 'text', $required = FALSE, $styleForInput = '', &$value = '') {
		if ($type == 'date') {
			$time = time();
			wp_register_script('jquery'.$time, plugins_url('/EParish/js/jquery-3.2.1.min.js'));
			wp_enqueue_script('jquery'.$time);
			
			wp_register_script('jqueryUI'.$time, plugins_url('/EParish/js/jquery-ui.min.js'));
			wp_enqueue_script('jqueryUI'.$time);
			
			wp_register_style('jqueryUI-style'.$time, plugins_url('/EParish/style/jquery-ui.css'));
			wp_enqueue_style('jqueryUI-style'.$time);
		}
		
		$requiredSymbol = '<b style="color: red;font-size: 1.2em;">*</b>';
		
		$jsDate = '<script>
                    jQuery(function () {
                        jQuery("#'.$name.'").datepicker({
                            dateFormat: "yy-mm-dd"
                        });
                    });
				</script>';
		
		$value = get_post_meta($obj->ID, $name, TRUE);
		
		return '<label id="'.$name.'l" for="'.$name.'" style="'.static::$styleForLabel.'">'.(($required) ? $requiredSymbol : '').$label.'</label>
				<input id="'.$name.'i" type="'.$type.'" name="'.$name.'" '.(($required) ? 'required' : '').'
					style="'.$styleForInput.'"
				    value="'.$value.'"/>'.(($type == 'date') ? $jsDate : '');
	}
	
	/**
	 * @param        $obj
	 * @param string $name
	 * @param string $label
	 * @param bool   $required
	 * @param string $styleForInput
	 *
	 * @return string
	 */
	public static function createTextArea($obj, $name, $label, $required = FALSE, $styleForInput = '') {
		$requiredSymbol = '<b style="color: red;font-size: 1.2em;">*</b>';
		
		return '<label id="'.$name.'l"  for="'.$name.'" style="'.static::$styleForLabel.'">'.(($required) ? $requiredSymbol : '').$label.'</label>
				<textarea id="'.$name.'i" name="'.$name.'" '.(($required) ? 'required' : '').'
					style="'.$styleForInput.'">'.get_post_meta($obj->ID, $name, TRUE).'</textarea>';
	}
}