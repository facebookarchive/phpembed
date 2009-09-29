<?

// test.php - file for testing php embed
// Copyright (c) 2007 Andrew Bosworth, Facebook, inc
// Modified by Dmitry Zenovich <dzenovich@gmail.com>
// All rights reserved
//
// This file is loaded by the PHP interpreter compiled into test

// we set a larger memory limit just in case
ini_set('memory_limit', '100M');

function foo_void(){
  echo "passed.\n";
  ob_flush();
  return;
}

function foo_long(){
  return 3;
}

function foo_bool(){
  return true;
}

function foo_double(){
  return 3.14;
}

function foo_string(){
  return "hello world\n";
}

function foo_string_ex(){
  return "hello \0world\n";
}

function foo_vector_long(){
  return array(2, 3, 4, 3);
}

function foo_vector_bool(){
  return array(true, false, true);
}

function foo_vector_double(){
  return array(2.71, 3.14, 4.0, 3.14);
}

function foo_vector_string(){
  return array("two", "three", "four\0\1", "three");
}

function foo_assoc_string(){
  return array("two" => "dos", "three" => "tres", "four\0\1" => "quatro\0\1");
}

function foo_assoc_double(){
  return array("two" => 2.71, "three" => 3.14, "four\0\1" => 4.0, "five" => 3.14);
}

function foo_assoc_long(){
  return array("two" => 2, "three" => 3, "four\0\1" => 4, "five" => 3);
}

function foo_assoc_bool(){
  return array("two" => true, "three" => false, "four\0\1" => true);
}

function foo_index_string(){
  return array(2 => "two", 3 => "three", 4 => "four\0\1", 5 => "three");
}

function foo_index_double(){
  return array(2 => 2.71, 3 => 3.14, 4 => 4.0, 5 => 3.14);
}

function foo_index_long(){
  return array(2 => 2, 3 => 3, 4 => 4, 6 => 6);
}

function foo_index_bool(){
  return array(2 => true, 3 => false, 4 => true);
}

function foo_complex_array(){
  return array(array(array("1" => 1),2 => "2"), array("x" => 3.14), 4 => false, "a\0.\0.a" => "b\0.\0.b\1");
}

function bar($l, $d, $c, $b, $i, $u, $s){
  return($l == 2 && $d == 3.14 && $c == 'four' && $b && $i == 5 && $u == 6 && $s == "a.\0.\0.a");
}

function verify_assoc($a){

  if(!isset($a["a"]) || !isset($a["bee"]) || !isset($a["cee\0\1"]))
    return false;

  $first = $a["a"];
  $second = $a["bee"];
  $third = $a["cee\0\1"];

  return($first == 1 && $second == "tw\0\1" && $third == 3);
}

function verify_index($a){
  return(isset($a[5]) && isset($a[2]) && isset($a[10]) && $a[5] == "one" && $a[2] == "abc" && $a[10] == 2.71);
}

function verify_mixed($a){
  return(isset($a[5]) && isset($a[2]) && isset($a[10]) && isset($a["what"]) &&
         $a[5] == "one" && $a[2] == "abc" && $a[10] == 2.71 && $a["what"] == "yo");
}

function verify_enum($a){
  return(isset($a[5]) && isset($a[2]) && isset($a[10]) && isset($a["what"]) && isset($a[11]) &&
         $a[5] == "one" && $a[2] == "abc" && $a[10] == 2.71 && $a["what"] == "yo" &&
         $a[11] == 2.1413);
}

function verify_nested($a){
  return(verify_assoc($a[0]) && verify_enum($a[1]));
}

function verify_remove($a, $b){
  if(isset($a["a"]) || isset($b[5]) || isset($a["cee\0\1"]))
    return false;

  if(!isset($a["bee"]) || !isset($b[2]) || !isset($b[10]) || !isset($b[11]))
    return false;

  $first = $a["bee"];
  return($first == "tw\0\1" && $b[2] == "abc" && $b[10] == 2.71 && $b[11] == 2.1413);
}
?>
