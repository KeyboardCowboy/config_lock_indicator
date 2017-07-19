<?php
/**
 * @file
 * Custom functions for Config Lock Indicator.
 */

/**
 * Load the $conf values from the settings.php file only.
 *
 * Ignore the values set only in the variables table.
 *
 * @return array
 *   The $conf values from settings.php.
 */
function config_lock_indicator_get_settings() {
  static $conf = array();
  $settings_path = DRUPAL_ROOT . '/' . conf_path() . '/settings.php';

  if (empty($conf) && file_exists($settings_path)) {
    require $settings_path;
  }

  return $conf;
}

/**
 * Recurse through the elements to find and mark form fields.
 *
 * @param array $element
 *   An element to traverse, such as a $form array.
 */
function config_lock_indicator_check_locked_form_fields(array &$element, array $settings, array $types_to_check = array('textfield', 'textarea', 'checkbox', 'checkboxes', 'radio', 'radios', 'password')) {
  foreach (element_children($element) as $child) {
    // Recurse down the form tree to check all children first.
    config_lock_indicator_check_locked_form_fields($element[$child], $settings, $types_to_check);

    if (isset($settings[$child])) {
      if (is_array($settings[$child])) {
        config_lock_indicator_check_locked_form_fields($element[$child], $settings[$child], $types_to_check);
      }
      elseif (isset($element[$child]['#type']) && in_array($element[$child]['#type'], $types_to_check)) {
        $element[$child]['#disabled'] = TRUE;
        $element[$child]['#description'] .= '<br /><small style="color: maroon;">' . t("Value is locked by the settings.php file.") . '</small>';
      }
    }
  }
}
