<?php

/**
 * Form function, called by drupal_get_form()
 * in the admin page.
 */
function attach_template_lite_config_form($form, &$form_state) {

	// Get formatted content types
	$types =   _node_types_build()->types;
	$processedTypes = array();
	foreach ($types as $type) {
		$processedTypes[$type->type] = $type->name;
	}

	// Add content type selection
	$form['attach_template_lite_view_modes'] = array(
		'#type' => 'textfield',
		'#title' => t('Attach templates to the following view modes'),
		'#default_value' => variable_get('attach_template_lite_view_modes', 'full'),
		'#description' => t('Machine name separated by spaces.'),
		'#required' => TRUE,
	);

	// Add content type selection
	$form['attach_template_lite_type'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Allow attach templates to the following content type'),
		'#default_value' => variable_get('attach_template_lite_type', array()),
		'#options' => $processedTypes,
		'#description' => t('Only the selected content type will be protected.'),
		'#required' => FALSE,
	);


	return system_settings_form($form);
}
?>