<?php

// +---------------------------------------------------------------------------+
// | reCAPTCHA Plugin for Geeklog - The Ultimate Weblog                        |
// +---------------------------------------------------------------------------+
// | geeklog/plugins/recaptcha/language/french_france_utf-8.php                |
// +---------------------------------------------------------------------------|
// | Copyright (C) 2014-2016 mystral-kk - geeklog AT mystral-kk DOT net        |
// |                                                                           |
// | Based on the CAPTCHA Plugin by Ben                                        |
// |                                                - ben AT geeklog DOT fr    |
// | Based on the original CAPTCHA Plugin by Mark R. Evans                     |
// |                                                - mark AT glfusion DOT org |
// | Constructed with the Universal Plugin                                     |
// +---------------------------------------------------------------------------|
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
// | along with this program; if not, write to the Free Software               |
// | Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA|
// |                                                                           |
// +---------------------------------------------------------------------------|

if (stripos($_SERVER['PHP_SELF'], basename(__FILE__)) !== false) {
	die('This file cannot be used on its own!');
}

$LANG_RECAPTCHA = array(
	'plugin'      => 'reCAPTCHA',
	'admin'       => 'reCAPTCHA',
	'entry_error' => 'An invalid reCAPTCHA string was entered in %1s - IP Address: %2s - Error Codes: %3s',	// %1s = $type, %2s = $ip, %3s = $errorCode
));

// Localization of the Admin Configuration UI
$LANG_configsections['recaptcha'] = array(
	'label' => 'reCAPTCHA',
	'title' => 'reCAPTCHA Configuration'
);

$LANG_confignames['recaptcha'] = array(
	'public_key'          => 'reCAPTCHA API Public Key',
	'private_key'         => 'reCAPTCHA API Private Key',
	'logging'             => 'Log invalid reCAPTCHA attempts',
	'anonymous_only'      => 'Anonymous Only',
	'remoteusers'         => 'Force CAPTCHA for all Remote Users',
	'enable_comment'      => 'Enable Comment Support',
	'enable_contact'      => 'Enable Contact Support',
	'enable_emailstory'   => 'Enable Email Story Support',
	'enable_forum'        => 'Enable Forum Support',
	'enable_registration' => 'Enable Registration Support',
	'enable_mediagallery' => 'Enable Media Gallery (Postcards) Support',
	'enable_rating'       => 'Enable Rating Plugin Support',
	'enable_story'        => 'Enable Story Support',
	'enable_calendar'     => 'Enable Calendar Plugin Support',
	'enable_links'        => 'Enable Links Plugin Support',
);

$LANG_configsubgroups['recaptcha'] = array(
    'sg_main' => 'Main Settings'
);

$LANG_tab['recaptcha'] = array(
    'tab_main' => 'General Captcha Settings'
);

$LANG_fs['recaptcha'] = array(
	'fs_system'      => 'System',
	'fs_integration' => 'Integration',
);

// Note: entries 0 is the same as in $LANG_configselects['Core']
$LANG_configselects['recaptcha'] = array(
	0 => array('Yes' => 1, 'No' => 0),
);
