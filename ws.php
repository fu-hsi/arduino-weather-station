<?php

define('DEBUG', false);

// Lokalna strefa czasowa
date_default_timezone_set('Europe/Warsaw');

// Wybór źródła danych
if (constant('DEBUG')) {
  $data = [
    'Humidity' => 45.5,
    'Temperature' => 23.1
  ];
  
  file_put_contents ('headers.txt', var_export($_SERVER, true), FILE_APPEND);
} else {
  $data = &$_POST;
}

// Uwierzytelnienie
if (constant('DEBUG') || (!empty($_SERVER['HTTP_X_APIKEY']) && $_SERVER['HTTP_X_APIKEY'] == '4Jrt39dj')) {

  // Dozwolona lista przesłanych pól
  $whiteList = [
    'Temperature',
    'Humidity'
  ];

  // Ustaw wartości domyślne
  $whiteList = array_fill_keys($whiteList, NULL);

  // Dodaj lokalny znacznik czasu
  $storeData['DateTime'] = date('Y-m-d H:i:s');

  // Przygotuj tablicę do zapisu
  $storeData = array_merge($storeData, $whiteList, array_intersect_key($data, $whiteList));

  // Zapisz jako CSV
  $fp = fopen('weather.txt', 'a');
  fputcsv($fp, $storeData);
  fclose($fp);

  if (constant('DEBUG')) {
    print_r($storeData);
  }
}
?>