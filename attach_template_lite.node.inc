<?php

/**
 * Implements hook_node_view().
 */
function attach_template_lite_node_view($node, $view_mode, $langcode) {

  $view_modes = explode(' ', variable_get('attach_template_lite_view_modes', 'full'));

  // Only override full view-mode
  if (in_array($view_mode, $view_modes)) {

    // Render attach_template_lite template if select in node
    if (isset($node->attached_template) && !empty($node->attached_template)) {
      $template_name = $node->attached_template;
      $template_location = $node->attached_template_location;

      if ($template_location == "theme") {
        $theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
        //        require_once $theme_path . '/attached_templates/' . $template[0] . ".inc";
        include $theme_path . '/attached_templates/' . $template_name . ".inc";
      } else {
        include "templates/" . $template_name . ".inc";
      }

      if (module_exists("ctools")) {
        ctools_class_add("template-" . $template_name);
      }

      $node->content['attach_template_lite'] = array(
        "#theme" => "attach_template_lite_" . $template_name,
        "#data" => $template_name()
      );

    }
  }
}

/**
 * Implements hook_field_extra_fields().
 */
function attach_template_lite_field_extra_fields() {

  // Define types variables
  $allowed_types = variable_get('attach_template_lite_type', array());

  foreach (entity_get_info() as $entity_type => $entity_info) {
    foreach (array_keys($entity_info['bundles']) as $bundle) {

      // Do not show if not specified in config page
      if (isset($allowed_types[$bundle]) && $allowed_types[$bundle] === 0) {
        continue;
      };

      // This is a compatibilty fix for Display Suite and Metatag compatibilty
      // https://drupal.org/node/1284852
      if (module_exists('ds')) {
        $extra[$entity_type][$bundle]['display']['metatags'] = array(
          'label' => t('Meta tags'),
          'description' => t('Rendered meta tags.'),
          'weight' => 12,
        );
      }

      // Add render of template
      $extra['node'][$bundle]['display']['attach_template_lite'] = array(
        'label' => t('Attached template'),
        'description' => t('Render attached template.'),
        'weight' => 10,
      );
    }
  }

  return $extra;
}

/**
 * Implements hook_node_submit().
 */
function attach_template_lite_node_submit($node, $form, &$form_state) {

  if (isset($form_state['values'])) {

    $values = $form_state['values'];

    // If a template was selected
    if (isset($values['attached_template'])) {

      // Save selected template
      $node->attached_template = $values['attached_template'];

    }
  }
}

/**
 * Implements hook_node_insert().
 */
function attach_template_lite_node_insert($node) {

  // Save selected template
  if (isset($node->attached_template)) {
    if (!empty($node->attached_template)) {
      $template = explode("|", $node->attached_template);
      db_insert('attach_template_lite')->fields(array(
        'template' => $template[0],
        'location' => $template[1],
        'nid' => $node->nid,
      ))->execute();
    }
  }
}

/**
 * Implements hook_node_update().
 */
function attach_template_lite_node_update($node) {
  if (isset($node->attached_template)) {
    if (empty($node->attached_template)) {
      // Database query
      db_delete('attach_template_lite')->condition('nid', $node->nid)->execute();

    } else {

      // Compatible with widget
      $template = explode("|", $node->attached_template);
      if (!isset($template[1])) {
        $template[1] = $node->attached_template_location;
      }

      // Database query
      db_merge('attach_template_lite')->fields(array(
        'template' => $template[0],
        'location' => $template[1],
        'nid' => $node->nid,
      ))->condition('nid', $node->nid)->execute();
    }
  }
}

/**
 * Implements hook_node_load().
 */
function attach_template_lite_node_load($nodes, $types) {

  // Load selected template into $node object
  $result = db_query('SELECT * FROM {attach_template_lite} WHERE nid IN(:nids)', array(':nids' => array_keys($nodes)))->fetchAllAssoc('nid');
  foreach ($nodes as &$node) {
    if (isset($result[$node->nid]->template)) {
      $node->attached_template = $result[$node->nid]->template;
      $node->attached_template_location = $result[$node->nid]->location;
    } else {
      $node->attached_template = FALSE;
      $node->attached_template_location = FALSE;
    }
  }
}

/**
 * Implements hook_form_node_form_alter().
 */
function attach_template_lite_form_node_form_alter(&$form, &$form_state) {

  // Check user permission
  if (!user_access('administer attached template')) {
    return;
  }

  // Define types variables
  $type = $form['#node']->type;
  $allowed_types = variable_get('attach_template_lite_type', array());

  // Do not show if not specified in config page
  if (isset($allowed_types[$type]) && $allowed_types[$type] === 0) {
    return;
  };

  // Define position in form
  $weight = isset($form['field_footer_extra_content']) ? $form['field_footer_extra_content']["#weight"] - 1 : 50;

  // Define current language
  $lang = _attach_template_lite_get_language($form['#node']);

  // Define current template
  $current_template = (isset($form['#node']->attached_template) && !empty($form['#node']->attached_template)) ? $form['#node']->attached_template . "|" . $form['#node']->attached_template_location : 0;

  // If there is no template attached and user is not allowed to attach one, exit
  if (($current_template === 0) && !user_access('administer attached template')) {
    return;
  }

  //  Create a fieldset to make it clean
  $form['template'] = array(
    '#type' => 'fieldset',
    '#title' => t('Template settings'),
    '#description' => t('You may edit your page template here.'),
    '#weight' => $weight,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#multilingual' => TRUE,
  );

  // Add template selector
  $form['template'][$lang]['attached_template'] = array(
    '#type' => user_access('administer attached template') ? 'select' : 'hidden',
    '#title' => t('Public page template'),
    '#empty_option' => "None",
    '#disabled' => !user_access('administer attached template'),
    '#default_value' => $current_template,
    '#options' => _attach_template_lite_get_template_list(),
  );

}
