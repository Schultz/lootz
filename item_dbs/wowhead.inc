<?php
/**
 * @file
 *   Provide WOW Lootz hooks for the Wowhead.com (http://www.wowhead.com) WOW database
 */

define('WOWHEAD_BASE_URL', 'http://www.wowhead.com/');
define('WOWHEAD_SEARCH_URL', '?item=%s&xml');

/**
 * Implementation of lootz_hook_item_db_info()
 */
function lootz_wowhead_item_db_info() {
  return array('wowhead' => t('<a href="!url">Wowhead</a>', array('!url' => WOWHEAD_BASE_URL)));
}

/**
 * Implementation of lootz_hook_filter_description()
 */
function lootz_wowhead_filter_description() {
  return t('Converts [item] tags to links/tooltips of the configured item database (<a href="!url">Wowhead</a> is the default).', array('!url' => WOWHEAD_BASE_URL));
}

/**
 * Implementation of lootz_hook_filter_tips()
 */
function lootz_wowhead_filter_tips() {
  return t('You can include [item][/item] tags to automatically link the items to <a href="!url">Wowhead</a>. Either use the <em>Item ID</em> ([item]1234[/item]) or the <em>exact name</em> ([item]Earthwarden[/item])', array('!url' => WOWHEAD_BASE_URL));
}

/**
 * Implementation of lootz_hook_id_lookup()
 */
function lootz_wowhead_id_lookup($name) {
  $url = WOWHEAD_BASE_URL . sprintf(WOWHEAD_SEARCH_URL, drupal_urlencode($name));
  $page = drupal_http_request($url);
  if ($page->code == '200') {
    // find id via this redirect tag: <script>self.location="item.aspx?id=29171";</script>
    if (preg_match('/self\.location="'. preg_quote(WOWDB_SEARCH_ID) . '([0-9]+)";/', $page->data, $m)) {
      return check_plain($m[1]);
    }
  }
  return false;
}

/**
 * Implementation of lootz_hook_name_lookup()
 */
function lootz_wowhead_name_lookup($id) {
  // grab search results
  $page = drupal_http_request(WOWHEAD_BASE_URL . sprintf(WOWHEAD_SEARCH_URL,$id));
  if ($page->code == '200') {
    // search for title tag of the form:
    // <h1 id="ctl00_MainContent_PageHeader" title="World of Warcraft Halberd of Desolation">Halberd of Desolation</h1>
    if (preg_match('/<h1 id="ctl00_MainContent_PageHeader" title=".*">(.*)<\/h1>/i', $page->data, $m)) {
      return check_plain($m[1]);
    }
  }
  return $id;
}

/**
 * Implementation of lootz_hook_quality_lookup()
 */
function lootz_wowhead_quality_lookup($id, $name) {
  // pattern: <b class=r4>Earthwarden</b>
  $page = drupal_http_request(WOWHEAD_BASE_URL . sprintf(WOWHEAD_SEARCH_URL,$id));
  if ($page->code == '200') {
    if (preg_match('/<b class="?(r[0-9]{1})"?>/', $page->data, $m)) {
      switch ($m[1]) {
      case 'r1' :
	// r3: common
	$quality = 'common';
	break;
      case 'r2' :
	// r3: uncommon
	$quality = 'uncommon';
	break;
      case 'r3' :
	// r3: rare
	$quality = 'rare';
	break;
      case 'r4' :
	// r4: epic
	$quality = 'epic';
	break;
      case 'r5' :
	// r5: legendary
	$quality = 'legendary';
	break;
      case 'r6' :
	// r6: artifact?
	$quality = 'artifact';
	break;
      default:
	$quality = false;
      }
    }
  }
  return $quality;
}

/**
 * Implementation of lootz_hook_link_item()
 */
function lootz_wowhead_link_item($name, $id) {
  $url = WOWHEAD_BASE_URL . sprintf(WOWHEAD_SEARCH_URL,$id);
  return theme('lootz_link_item', $url, $name, $id);
}

/**
 * Implementation of lootz_hook_init()
 */
function lootz_wowhead_init() {
  // include the external js
  //drupal_set_html_head('<script src="http://www.wowdb.com/js/extooltips.js?8" type="text/javascript" ></script>'."\n");
}
