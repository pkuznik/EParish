<?php
/**
 * Description of EParishCoreUpdate
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
class EParishCoreUpdate implements IEParishPlugin {
	
	public function plug() {
		require PATH_EPARISH.'/update-core/plugin-update-checker.php';
		$className = PucFactory::getLatestClassVersion('PucGitHubChecker');
		$myUpdateChecker = new $className(
			'https://github.com/eDokumenty/seo-summary/',
			'IntencjeMszalne2.php',
			'master'
		);
	}
}