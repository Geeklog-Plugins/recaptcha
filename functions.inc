<?php

// +---------------------------------------------------------------------------+
// | reCAPTCHA Plugin for Geeklog - The Ultimate Weblog                        |
// +---------------------------------------------------------------------------+
// | geeklog/plugins/recaptcha/functions.inc                                   |
// +---------------------------------------------------------------------------+
// | Copyright (C) 2014 mystral-kk - geeklog AT mystral-kk DOT net             |
// |                                                                           |
// | Based on the CAPTCHA Plugin by Ben                                        |
// |                                                   - ben AT geeklog DOT fr |
// | Based on the original CAPTCHA Plugin by Mark R. Evans                     |
// |                                                - mark AT glfusion DOT org | 
// | Constructed with the Universal Plugin                                     |
// +---------------------------------------------------------------------------+
// |                                                                           |
// | This program is free software; you can redistribute it and/or             |
// | modify it under the terms of the GNU General Public License               |
// | as published by the Free Software Foundation; either version 2            |
// | of the License, or (at your option) any later version.                    |
// |                                                                           |
// | This program is distributed in the hope that it will be useful,           |
// | but WITHOUT ANY WARRANTY; without even the implied warranty of            |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             |
// | GNU General Public License for more details.                              |
// |                                                                           |
// | You should have received a copy of the GNU General Public License         |
// | along with this program; if not, write to the Free Software Foundation,   |
// | Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           |
// |                                                                           |
// +---------------------------------------------------------------------------+

if (stripos($_SERVER['PHP_SELF'], 'functions.inc') !== false) {
	die('This file cannot be used on its own.');
}

// Language file Include
$langfile = $_CONF['path'] . 'plugins/recaptcha/language/' . $_CONF['language'] . '.php';
clearstatcache();

if (file_exists($langfile)) {
	include_once $langfile;
} else {
	include_once $_CONF['path'] . 'plugins/recaptcha/language/english.php';
}

// Loads the plugin configuration
global $_RECAPTCHA_CONF;

require_once $_CONF['path'] . 'plugins/recaptcha/config.php';
RECAPTCHA_loadConfig();
require_once $_CONF['path'] . 'plugins/recaptcha/vendor/recaptchalib.php';

// +---------------------------------------------------------------------------+
// | Geeklog Plugin API Implementations                                        |
// +---------------------------------------------------------------------------+

/**
* Returns a URL to icon
*/
function plugin_geticon_recaptcha() {
	global $_CONF;

	return $_CONF['site_admin_url'] . '/plugins/recaptcha/images/recaptcha.png';
}

/**
* Puts an option for the plugin in the command and control block on moderation.php
*
* @return   array   Array containing (plugin name, admin url, url of plugin icon)
*/
function plugin_cclabel_recaptcha() {
	global $_CONF;

	if (SEC_hasRights('recaptcha.edit')) {
		$pi_name = RECAPTCHA_str('admin');

		if (!RECAPTCHA_isEnabled()) {
			$pi_name = '<span style="color: red;">' . $pi_name . '!</span>';
		}

		$admin_url = $_CONF['site_admin_url'] . '/plugins/recaptcha/index.php';
		$icon_url  = plugin_geticon_recaptcha();

		return array($pi_name, $admin_url, $icon_url);
	} else {
		return null;
	}
}

/**
* Returns the administrative option for this plugin
*
* @return   array   Array containing (plugin name, plugin admin url, # of items
* in plugin or '')
*/
function plugin_getadminoption_recaptcha() {
	global $_CONF, $_TABLES;

	if (SEC_hasRights('recaptcha.edit')) {
		$pi_name = RECAPTCHA_str('admin');

		if (!RECAPTCHA_isEnabled()) {
			$pi_name = '<span style="color: red;">' . $pi_name . '!</span>';
		}

		$admin_url = $_CONF['site_admin_url'] . '/plugins/recaptcha/index.php';
		$num_items = '';
		return array($pi_name, $admin_url, $num_items);
	} else {
		return null;
	}
}

/**
* Returns the version for this plugin
*
* @return  string   plugin version no
*/
function plugin_chkVersion_recaptcha() {
	global $_RECAPTCHA_CONF;

	return $_RECAPTCHA_CONF['pi_version'];
}

/**
* Upgrades the plugin
*
* @return   boolean   true (= success)
*/
function plugin_upgrade_recaptcha() {
	global $_CONF, $_TABLES, $_RECAPTCHA_CONF;

	$sql = "SELECT pi_version "
		 . "FROM {$_TABLES['plugins']} "
		 . "WHERE (pi_name = 'recaptcha') ";
	$result = DB_query($sql);
	$A = DB_fetchArray($result);
	$pi_version = $A['pi_version'];		// Version info stored in database

	// Does upgrading
	while (version_compare($pi_version, $_RECAPTCHA_CONF['pi_version']) < 0) {
		switch ($pi_version) {
			case '1.0.0':
				$pi_version = '1.0.1';
				break;

			default:
				$pi_version = $_RECAPTCHA_CONF['pi_version'];
				break;
		}
	}

	// Updates plugin info
	$sql = "UPDATE {$_TABLES['plugins']} "
		 . "SET pi_version = '{$_RECAPTCHA_CONF['pi_version']}' "
		 . "WHERE (pi_name = 'recaptcha') ";
	$result = DB_query($sql);
	COM_errorLog("reCAPTCHA plugin was successfully updated to version {$_RECAPTCHA_CONF['pi_version']}.");

	return true;
}

/**
* Automatic uninstall function for plugins
*
* @return   array
*/
function plugin_autouninstall_recaptcha() {
	global $_RECAPTCHA_CONF;

	return array (
		'tables'     => array(),
		'groups'     => array('reCAPTCHA Admin'),
		'features'   => $_RECAPTCHA_CONF['FEATURES'],
		'php_blocks' => array(),
		'vars'       => array()
	);
}

/**
* Returns the URL to a given document
*/
function plugin_getdocumentationurl_recaptcha($file) {
	global $_CONF;

	$lang = COM_getLanguageName();
	$path = $_CONF['path_html'] . 'admin/plugins/recaptcha/docs/' . $lang . '/'
		  . $file . '.html';
	clearstatcache();

	if (!file_exists($path)) {
		$lang = 'english';
	}

	$retval = $_CONF['site_admin_url'] . '/plugins/recaptcha/docs/' . $lang . '/'
			. $file . '.html';

	return $retval;
}

/**
* Provides text for a Configuration tooltip
*
* @param    string  $id     Id of config value
* @return   mixed           Text to use regular tooltip, null to use config 
*                           tooltip hack, or empty string when not available
*/
function plugin_getconfigtooltip_recaptcha($id) {
	// Use config tooltip hack where tooltip is read from the config documentation
	return null;
}

/**
* Callback function to add HTML text for reCAPTCHA
*
* @param     string    $type
* @param     object    $template   a reference to a Template class object
* @return    void
*/
function plugin_templatesetvars_recaptcha($type, &$template) {
	global $_RECAPTCHA_CONF;

	if (RECAPTCHA_isEnabled() && RECAPTCHA_requireCaptcha($type)) {
		if ((isset($_SERVER['HTTPS']) &&
					(($_SERVER['HTTPS'] === 'on') || ($_SERVER['HTTPS'] === '1'))) ||
			(isset($_SERVER['HTTP_X_FORWARDED_BY']) &&
					(stripos($_SERVER['HTTP_X_FORWARDED_BY'], 'SSL') !== false)) ||
			(isset($_SERVER['HTTP_X_FORWARDED_HOST']) &&
					(stripos($_SERVER['HTTP_X_FORWARDED_HOST'],'SSL') !== false))) {
			$use_ssl = true;
		} else {
			$use_ssl = false;
		}

		$captcha = '<div class="gl-recaptcha_' . $type . '">' . recaptcha_get_html($_RECAPTCHA_CONF['public_key'], $error, $use_ssl) . '</div>';
	} else {
		$captcha = '';
	}

	$template->set_var('captcha', $captcha);
}

/**
* Callback function to check the code entered by a user
*
* @param     (not used in this function)
* @param     object    $template   a reference to a Template class object
* @return    string    empty = success
*/
function plugin_commentPreSave_recaptcha($uid, $title, $comment, $sid, $pid, $type, $postmode) {
	return plugin_itemPreSave_recaptcha('comment', '');
}

/**
* Callback function to check the code entered by a user
*
* @param     string    $type       item type
* @param     object    $template   a reference to a Template class object
* @return    string    empty = success
*/
function plugin_itemPreSave_recaptcha($type, $content = '') {
	global $_RECAPTCHA_CONF, $LANG_RECAPTCHA, $_SERVER;

	$msg = '';

	if (RECAPTCHA_isEnabled() && RECAPTCHA_requireCaptcha($type)) {
		$challenge = isset($_POST['recaptcha_challenge_field'])
				   ? COM_applyFilter($_POST['recaptcha_challenge_field'])
				   : '';
		$response  = isset($_POST['recaptcha_response_field'])
				   ? COM_applyFilter($_POST['recaptcha_response_field'])
				   : '';
		$extra_params = array();
		$answer = recaptcha_check_answer(
			$_RECAPTCHA_CONF['private_key'], $_SERVER['REMOTE_ADDR'], $challenge,
			$response, $extra_params
		);

		if (!$answer->is_valid) {
			if ($_RECAPTCHA_CONF['logging']) {
				$msg = sprintf(
					RECAPTCHA_str('entry_error', true),
					$type,
					$_SERVER['REMOTE_ADDR']
				);
				$msg = RECAPTCHA_esc($msg);
				reCAPTCHA_errorLog($msg);
			}
		}
	}

	return $msg;
}

/**
* Writes an entry into a log file
*
* @param    string    $logentry
* @param    int       $override
*/
function reCAPTCHA_errorLog($logentry, $override = 0) {
	global $_CONF, $_RECAPTCHA_CONF;

	if (!isset($_RECAPTCHA_CONF['logging'])) {
		$_RECAPTCHA_CONF['logging'] = 0;
	}

	if (($_RECAPTCHA_CONF['logging'] != 1) && ($override == 0)) {
		return;
	}

	if (!empty($logentry)) {
		$timestamp = date('Y-m-d H:i:s');
		$logfile = $_CONF['path_log'] . 'recaptcha.log';
		$entry   = "{$timestamp} - {$logentry} \n";
		
		if (($file = @fopen($logfile, 'a')) !== false) {
			fputs($file, "$timestamp - $logentry \n");
			fclose($file);
		} else {
			COM_errorLog(__FUNCTION__ . ': cannot open "' . $logfile . '"');
		}
	}
}

/**
* Loads config info with config.class.php
*/
function RECAPTCHA_loadConfig() {
	global $_CONF, $_RECAPTCHA_CONF;

	$config = config::get_instance();

	if ($config->group_exists('recaptcha')) {
		$_RECAPTCHA_CONF = array_merge($_RECAPTCHA_CONF, $config->get_config('recaptcha'));
	} else {
		COM_errorLog(__FUNCTION__ . ': cannot load configuration.  Using default setiings instead');
		require_once 'install_defaults.php';
		$_RECAPTCHA_CONF = $_RC_DEFAULT;
	}
}

/**
* Returns a string escaped for HTML output
*/
function RECAPTCHA_esc($str) {
	global $_CONF;

	static $encoding = null;

	if ($encoding === null) {
		$encoding = is_callable('COM_getCharset')
				  ? COM_getCharset()
				  : $_CONF['default_charset'];
	}

	if (version_compare(PHP_VERSION, '5.2.3') >= 0) {
		return htmlspecialchars($str, ENT_QUOTES, $encoding, false);
	} else {
		return str_replace(
			array('&amp;&amp;', '&amp;&lt;', '&amp;&gt;', '&amp;&quot;', '&amp;&#039;'),
			array('&amp;', '&lt:', '&gt;', '&quot;', '&#039;'),
			htmlspecialchars($str, ENT_QUOTES, $encoding)
		);
	}
}

/**
* Returns a lang var
*/
function RECAPTCHA_str($index, $noesc = false) {
	global $LANG_RECAPTCHA;

	if (isset($LANG_RECAPTCHA[$index])) {
		return $noesc ? $LANG_RECAPTCHA[$index] : RECAPTCHA_esc($LANG_RECAPTCHA[$index]);
	} else {
		COM_errorLog(__FUNCTION__ . ': undefined index: "' . $index . '"');
		return '(undefined)';
	}
}

/**
* Returns if the current type of item requires reCAPTCHA
*
* @param    string    $type    item type
* @return   boolean   true = requires reCAPTCHA
*/
function RECAPTCHA_requireCaptcha($type) {
	global $_RECAPTCHA_CONF, $_USER;

	if (!RECAPTCHA_isEnabled()) {
		return false;
	}

	if ((in_array($type, $_RECAPTCHA_CONF['supported_items']) &&
				($_RECAPTCHA_CONF['enable_' . $type] == 0)) ||
			!in_array($type, $_RECAPTCHA_CONF['supported_items'])) {
		return false;
	}

	if (!isset($_USER['uid'])) {
		$_USER['uid'] = 1;
	}

	if (($_RECAPTCHA_CONF['anonymous_only'] && ($_USER['uid'] < 2)) ||
			($_RECAPTCHA_CONF['anonymous_only'] == 0) ||
			(($_RECAPTCHA_CONF['remoteusers'] == 1) && SEC_inGroup('Remote Users'))) {
		return true;
	}

	return false;
}

/**
* Returns if the plugin is ready to work
*
* @return   boolean   true = OK
*/
function RECAPTCHA_isEnabled() {
	global $_RECAPTCHA_CONF;

	return ($_RECAPTCHA_CONF['private_key'] !== '') && ($_RECAPTCHA_CONF['public_key'] !== '');
}
