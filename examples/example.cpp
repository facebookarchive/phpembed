// PHP EMBED example program
// Copyright (c) 2007 Andrew Bosworth, Facebook, inc
// All rights reserved

#include "php_stl.h"

// prototype
void print_php_array(php_array &a, int depth = 0);

// define our own output functions with this exact signature
void print_null(const char *str) {}
void print_mine(const char *str){
  printf("My Output: %s", str);
}

int main(int argc, char **argv){

  // Instantiate the object, set type_warnings to true
  php_stl p(true);

  // Load a php file
  if(SUCCESS != p.load("example.php")){
    printf("load failed\n");
    return 1;
  }

  // redefine the output functions to do what we want
  p.set_message_function(print_null);
  p.set_output_function(print_mine);

  // an example of calling into a function with C arguments
  long tre = 3;
  printf("%s\n", p.call_c_string("trivial_func", "sld", "answer: ", tre, 4.5));
  printf("\n");

  // this test function returns a string, but we call_long so it converts it
  // to a long value. Since I have instantiated this php object with the
  // type_warnings parameter to true, this will output an error. If you do NOT
  // want an error message, just omit the argument or set it to false. You can
  // redirect this error output by using set_error_function
  long hit = p.call_long("test");
  printf("long value: %ld\n", hit);
  printf("\n");

  // redirect error output to null
  p.set_error_function(print_null);

  // convert a php string array to a vector of longs
  vector<long> blah = p.call_long_vector("blah");
  for(vector<long>::iterator it = blah.begin(); it != blah.end(); it++){
    printf("%ld\n", *it);
  }
  printf("\n");

  // get a nested array back from php
  php_array a = p.call_php_array("foo_complex_array");

  // use our function below with iterators and type checking to print it in C
  print_php_array(a);
  printf("\n");

  // now try printing it with PHP, should use our output function from above
  p.call_void("print_r", "a", &a);
}


void print_php_array(php_array &a, int depth){
  php_iterator it(a);

  for(int i = 0; i < depth; i++)
    printf(" ");
  printf("%d elements\n", it.size());

  while(!it.done()){

    for(int i = 0; i < depth; i++)
      printf(" ");

    switch(it.get_key_type()){
    case IS_LONG:
      printf("long %ld => ", it.get_key_long());
      break;
    case IS_STRING:
      printf("string %s => ", it.get_key_c_string());
      break;
    default:
      printf("??? %s => ", it.get_key_c_string());
      break;
    }

    switch(it.get_data_type()){
    case IS_LONG:
      printf("long %ld\n", it.get_data_long());
      break;
    case IS_STRING:
      printf("string %s\n", it.get_data_c_string());
      break;
    case IS_DOUBLE:
      printf("double %f\n", it.get_data_double());
      break;
    case IS_BOOL:
      printf("bool %s\n", it.get_data_c_string());
      break;
    case IS_ARRAY:
      printf("Array\n");
      {
        php_array t = it.get_data_array();
        print_php_array(t, depth + 1);
      }
      break;
    default:
      printf("??? %s\n", it.get_data_c_string());
      break;
    }

    it++;
  }
}
