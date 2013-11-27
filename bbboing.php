<?php
/**
Plugin Name: bbboing
Depends: oik base plugin
Plugin URI: http://www.oik-plugins.com/oik-plugins/bbboing
Description: obfuscate text but leave it readable, using oik
Version: 1.5
Author: bobbingwide
Author URI: http://www.bobbingwide.com
License: GPL2

    Copyright 2012,2013 Bobbing Wide (email : herb@bobbingwide.com )

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2,
    as published by the Free Software Foundation.

    You may NOT assume that you can use any other version of the GPL.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    The license for this software can likely be found here:
    http://www.gnu.org/licenses/gpl-2.0.html

*/

/**
 * Implement the "init" action for bbboing 
 * 
 * Even though "oik" may not yet be loaded, let other plugins know that we've been loaded.
 */
function bbboing_init() {
  do_action( "bbboing_loaded" );
}  

/**
 * Implement the "oik_loaded" action for bbboing
 * 
 * Now it's safe to use oik APIs to register the bbboing shortcode
 */
function bbboing_oik_loaded() { 
  bw_add_shortcode( 'bbboing', 'bbboing_sc', oik_path( "bbboing.inc", "bbboing" ), false );
}

/**
 * Dependency checking for bbboing
 *
 * bbboing is now dependent upon oik version 2.1-alpha or higher and uses the new oik-activation code
 * 
 * bbboing version 1.2 was dependent upon oik version 1.11
 * 
 */ 
function bbboing_activation() {
  static $plugin_basename = null;
  if ( !$plugin_basename ) {
    $plugin_basename = plugin_basename(__FILE__);
    add_action( "after_plugin_row_${plugin_basename}", __FUNCTION__ );   
    require_once( "admin/oik-activation.php" );
  }  
  $depends = "oik:2.1-alpha";
  bw_backtrace();
  oik_plugin_lazy_activation( __FILE__, $depends, "oik_plugin_plugin_inactive" );
}

/**
 * Initialisation when bbboing plugin file loaded
 */
function bbboing_plugin_loaded() {
  add_action( "init", "bbboing_init" );
  add_action( "oik_loaded", "bbboing_oik_loaded" );
  add_action( "admin_notices", "bbboing_activation" );
}

bbboing_plugin_loaded();









