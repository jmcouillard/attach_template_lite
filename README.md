# Attached template lite

Drupal 7 mdoule which allows a custom template to be attached to a single node. In the lite version, no settings per node are available.

Github home : https://github.com/jmcouillard/attach_template_lite

### Select a template

Templates can be selected in the node form

### Create a custom template

In you active theme directory, create a "attached_templates" directory. For each template, you need to create to files :

 - mytemplate.inc
 - mytemplate.tpl.php

The file named *mytemplate.inc* must include a funcion named `mytemplate()`.

### Examples

Examples can be found in `attached_template_lite/templates` folder. Use those as a starting point for your own templates.