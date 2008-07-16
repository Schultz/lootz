$Id: README.txt,v 1.1.2.2 2008/05/02 22:30:18 professorbikeybike Exp $

The Lootz Item Filter
---------------------

This is, more than anything, a filter engine for integrating 3rd party
database lookups. In it's current form, the filter allows for text of
the form

[item]1234[/item]

-or-

[item]Earthwarden[/item] (for example)

to be converted to WoW DB links. The javascript for mouseover popups
is embedded, and results are cached for performance.

The module is designed as a filter engine, so if somebody wants to use
Wowhead or Thottbot instead of WoW DB, they simply need to copy the
wowdb.inc file, and modify the hooks to work for the desired item
database.

This module is still under heavy development. More soon!


The Lootz Item Filter module is developed and maintained by Professor
Bikey Bike (aka, Jonathan Hedstrom). It will soon be in use on

http://professorbikeybike.com/wow

jhedstrom <at> gmail.com

http://drupal.org/user/35926

