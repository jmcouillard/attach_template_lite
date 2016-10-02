<?php

/**
 * Example page for attached template lite module.
 */
function example_page_projects() {

  $nodes = array();
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'project');
  $query->fieldOrderBy("field_weight", "value", "ASC");
  $query->propertyCondition('promote', NODE_PROMOTED);
  $query->propertyCondition("status", NODE_PUBLISHED);

  // Execute
  $result = $query->execute();
  if (isset($result['node'])) {
    $nids = array_keys($result['node']);
    $nodes = entity_load('node', $nids);
  }

  $nodeViews = array();
  foreach ($nodes as $nid => &$node) {
    $nodeViews[] = node_view($node, "teaser");
  }

  $output = array(
    "team" => $nodeViews
  );

  return $output;
}