<?php


/**
 * Example page for attached template lite module.
 */
function pages_team() {

	$nodes = array();
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node')->entityCondition('bundle', 'team');
	$query->fieldOrderBy("field_weight", "value", "ASC");

	// Execute
	$result = $query->execute();
	if(isset($result['node'])) {
		$nids = array_keys($result['node']);
		$nodes = entity_load('node', $nids);
	}

	$nodeViews = array();
	foreach ($nodes as $nid => &$node) {
		$nodeViews[] = node_view($node, "full");
	}

	$output = array(
		"team"=>$nodeViews
	);

	return $output;
}
