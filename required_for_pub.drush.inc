<?php

/**
* Implements hook_drush_command().
*/
function required_for_pub_drush_command() {
  $items['required-for-pub-convert'] = array(
    'description' => 'Convert required fields from required to required_for_pub.',
    'aliases' => array('rfp-c'),
    'arguments' => array(
      'content_type' => 'The content type in which to convert required fields. Defaults to "node".',
      'bundle' => 'The bundle in which to convert required fields in. Leave empty to scan all bundles',
    ),
    'examples' => array(
      'drush rfp-c node' => 'Converts all node fields (with confirmation).',
      'drush rfp-c node page' => 'Converts all basic page fields (with confirmation).',
      'drush -y rfp-c node page' => 'Converts all basic page fields (WITHOUT confirmation).',
    )
  );
  return $items;
}

function drush_required_for_pub_convert($type='node', $bundle = NULL) {
  $entities = field_info_instances($type, $bundle);
  if ($bundle) {
    $entities = array($bundle => $entities);
  }
  if ($type) {
    $entities = array($type => $entities);
  }
  foreach ($entities as $entity_name => $bundles) {
    foreach ($bundles as $bundle_name => $fields) {
      foreach ($fields as $field_name => $field) {
        if ($field['required'] && empty($field['required_for_pub'])) {
          $go = drush_confirm(dt('Convert @type:@bundle:@field_name ("@label")?', array(
            '@type' => $entity_name,
            '@bundle' => $bundle_name,
            '@field_name' => $field_name,
            '@label' => $field['label'],
          )));
          if ($go) {
            $field['required'] = (is_bool($field['required'])) ? FALSE : 0;
            $field['required_for_pub'] = 'required_for_pub';
            // Features sets this field erroneously. Remove it if it's there.
            unset($field['settings']['required_for_pub']);
            field_update_instance($field);
          }
        }
      }
    }
  }
}