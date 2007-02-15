<?

// test.php - file for exploring php embed
// Copyright (c) 2007 Andrew Bosworth, Facebook, inc
// All rights reserved
//
// This file is loaded by the PHP interpreter compiled into example

// we set a larger memory limit just in case
ini_set('memory_limit', '100M');

function trivial_func($prefix, $l, $d){
  return $prefix . $l * $d;
}

function foo_complex_array(){
  return (array(array(array("foo" => 1), 2 => "2"), array("x" => 3.14), 4 => false));
}

function test(){
  return '35';
}

function blah(){
  return array('10', '20', '30');
}

?>
