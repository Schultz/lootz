<?php

// $Id: wowhead.inc,v 1.1.2.1 2009/05/22 20:25:29 professorbikeybike Exp $
// Modded for WowHead 10/04/2009 by Andy68man

/**
 * @file
 *   Provide WOW Lootz hooks for the Wowhead.com (http://www.wowhead.com) WOW database
 */

define('WOWHEAD_BASE_URL', 'http://www.wowhead.com');
define('WOWHEAD_SEARCH_NAME', '?search=');
define('WOWHEAD_SEARCH_ID', '?item=');

/**
 * Implementation of lootz_hook_item_db_info()
 */
function lootz_wowhead_item_db_info() {
  return array('wowhead' => t('<a href="!url">WoW Head</a>'));
}

/**
 * Implementation of lootz_hook_filter_description().
 */
function lootz_wowhead_filter_description() {
  return t('Converts [item] tags to links/tooltips of the configured item database (<a href="!url">WoW Head</a> is the default).', array('!url' => WOWHEAD_BASE_URL));
}

/**
 * Implementation of lootz_hook_filter_tips().
 */
function lootz_wowhead_filter_tips() {
  return t('You can include [item][/item] tags to automatically link the items to <a href="!url">WoWHead</a>. Either use the <em>Item ID</em> ([item]1234[/item]) or the <em>exact name</em> ([item]Earthwarden[/item])', array('!url' => WOWHEAD_BASE_URL));
}

/**
 * Implementation of lootz_hook_id_lookup().
 */
function lootz_wowhead_id_lookup($name) {
  $name = ucwords($name);
  $url = WOWHEAD_BASE_URL . '/' . WOWHEAD_SEARCH_NAME . drupal_urlencode($name);
  $page = drupal_http_request($url);

  if ($page->code == '200') {
    // Check to see if it's a search page.
    if (preg_match('/<h1>Search results for/', $page->data, $m)) {
      // Find the id of an exact match, or a match with escaped quotes.
      $escaped = preg_quote(addslashes($name));
      $pattern = "/\[([0-9]+)\]=\{name_enus\:'(" . $name . "|" . $escaped . ")'/i";
      if (preg_match($pattern, $page->data, $m)) {
        return check_plain($m[1]);
      }
    }
    else {
      // You're on the right page already!
      if (preg_match('/typeId: ([0-9]+)/', $page->data, $m)) {
        return check_plain($m[1]);
      }
    }
  }
  else if ($page->code == '302') {
	// WoW Head issues a 302 redirect when only 1 item is found. So check the 302.
	// WoW Head issues only relative location header and drupal http request requires a schema hence we just
	// use the $page->redirect_url to find out which item its redirecting to.
	if (preg_match('/item=([0-9]+)/', $page->redirect_url, $m)) {
		return check_plain($m[1]);
	}
  }
  return FALSE;
}

/**
 * Implementation of lootz_hook_name_lookup().
 */
function lootz_wowhead_name_lookup($id) {
  // Grab search results.
  $url = WOWHEAD_BASE_URL . '/' . WOWHEAD_SEARCH_ID . $id;
  $page = drupal_http_request($url);

  if($page->code == '200') {
    // Search for the name which is next to the id 4677, name: 'Veteran Cloak'.
    if (preg_match("/" .$id . ", name: '(.*)'/", $page->data, $m)) {
      return check_plain($m[1]);
    }
  }
  return $id;
}

/**
 * Implementation of lootz_hook_quality_lookup().
 */
function lootz_wowhead_quality_lookup($id, $name) {
  // Pattern: <b class=q4>Earthwarden</b>
  $url = WOWHEAD_BASE_URL . '/' . WOWHEAD_SEARCH_ID . $id;
  if ($url!="http://www.wowhead.com/?item=") {
	$page = drupal_http_request($url);
    $quality = FALSE;
    if($page->code == '200') {
      if (preg_match('/<b class="?(q[0-9]{1})"?>/', $page->data, $m)) {
        switch ($m[1]) {
          case 'q1' :
            // q1: common
            $quality = 'common';
            break;
          case 'q2' :
            // q2: uncommon
            $quality = 'uncommon';
            break;
          case 'q3' :
            // q3: rare
            $quality = 'rare';
            break;
          case 'q4' :
            // q4: epic
            $quality = 'epic';
            break;
          case 'q5' :
            // q5: legendary
            $quality = 'legendary';
            break;
          case 'q6' :
            // q6: artifact?
            $quality = 'artifact';
            break;
          case 'q7' :
            // q7: bind to account
            $quality = 'bind_to_account';
            break;
        }
      }
    }
    return $quality;
  }
  else {
    return FALSE;
  }
}

/**
 * Implementation of lootz_hook_link_item().
 */
function lootz_wowhead_link_item($name, $id) {
  $url = WOWHEAD_BASE_URL . '/' . WOWHEAD_SEARCH_ID . $id;
  return theme('lootz_link_item', $url, $name, $id);
}

/**
 * Implementation of lootz_hook_init().
 */
function lootz_wowhead_init() {
  // Include the external js.
  drupal_set_html_head('<script src="http://www.wowhead.com/widgets/power.js"></script>'."\n");
}