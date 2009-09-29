// PHP EMBED tutorial program
// Copyright (c) 2007 Andrew Bosworth, Facebook, inc
// Modified by Dmitry Zenovich <dzenovich@gmail.com>
// All rights reserved

#include "php_stl.h"

// define our own output functions with this exact signature
void print_null(const char *str) {}
void print_mine(const char *str){ printf("%s", str); }

int main(int argc, char **argv){

  // Instantiate the object without type warnings
  php_stl p;

  // redefine the output functions to do what we want
  p.set_message_function(print_null);
  p.set_output_function(print_mine);

  long memused = p.call_long("memory_get_usage"); 
  hash_set<string> ex; 
  ex = p.call_string_hash_set("get_loaded_extensions");

  printf("memused = %ldb\n", memused);

  char *one = "test1"; 
  int comp = 4; 
  long match = p.call_long("strncmp", "ssi", one, "test2", comp);

  printf("match = %ld (should be 0)\n", match);

  php_array a; 
  php_array b(a); 

  a.add("l", 5); 
  a.add_assoc("slsd", "one", 1, "two", 2.5); 
  a.add_index("lsll", 6, "six", 128, 129); 

  a.remove(6); 
  a.remove("two"); 

  p.call_void("print_r", "a", &a); 

  php_iterator it(a); 

  int count = 0; 
  while(!it.done()){ 
    count++; 
    it++; 
  } 

  it.go_to_end(); 

  if(!it.done()){ 
    switch(it.get_key_type()){ 
    case IS_LONG: 
      printf("long %ld => ", it.get_key_long()); 
      break; 
    case IS_STRING:
      {
        char *str = it.get_key_c_string();
        printf("string %s => ", str);
        free(str); 
        break;
      }
    default: 
      {
        char *str = it.get_key_c_string();
        printf("??? %s => ", it.get_key_c_string()); 
        free(str);
        break; 
      } 
    }
    switch(it.get_data_type()){ 
    case IS_LONG: 
      printf("long %ld\n", it.get_data_long()); 
      break; 
    case IS_STRING:
      { 
        char *str = it.get_data_c_string();
        printf("string %s\n", str);
        free(str); 
        break; 
      }
    case IS_DOUBLE: 
      printf("double %f\n", it.get_data_double()); 
      break; 
    case IS_BOOL:
      {
        char *str = it.get_data_c_string();
        printf("bool %s\n", str); 
        free(str);
        break; 
      }
    case IS_ARRAY: 
      printf("Array\n"); 
      {
        php_array suba = it.get_data_array(); 
        php_iterator subit(suba); 
        // now iterate on the sub array! 
      }
      break; 
    default:
      {
        char *str = it.get_data_c_string();
        printf("??? %s\n", str);
        free(str); 
        break;
      }
    }
  } 

  return 0;
}
