// PHP EMBED test program
// Copyright (c) 2007 Andrew Bosworth, Facebook, inc
// Modified by Dmitry Zenovich <dzenovich@gmail.com>
// All rights reserved

#include "php_stl.h"

int main(int argc, char **argv){

  int failcount = 0;

  printf("loading test.php...");
  php_stl p;
  if(SUCCESS != p.load("test.php")){
    printf("failed! aborting...\n");
    return 1;
  } else {
    printf("passed.\n");
  }

  // 
  // CODE TO TEST PASSING C ARGUMENTS TO PHP
  //

  {
    long l = 2;
    double d = 3.14;
    char *c = "four";
    bool b = true;
    int i = 5;
    unsigned int u = 6;
    char *s = "a.\0.\0.a";
    unsigned int sLen = sizeof("a.\0.\0.a") - 1;

    printf("testing passing arguments to php...");
    if(!p.call_bool("bar", "ldsbiiS", l, d, c, b, i, u, s, sLen)){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }
  }


  //
  // CODE TO TEST RETURNING PHP VALUES TO C
  // 

  {
    printf("testing call_void...");
    p.call_void("foo_void");
    if(p.status != SUCCESS){
      printf("failed!\n");
      failcount++;
    } // success is printed by php

    printf("testing call_long...");
    long l = p.call_long("foo_long");
    if(p.status != SUCCESS || l != 3){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_bool...");
    bool b = p.call_bool("foo_bool");
    if(p.status != SUCCESS || !b){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_double...");
    double d = p.call_double("foo_double");
    if(p.status != SUCCESS || d != 3.14){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_c_string...");
    char *c = p.call_c_string("foo_string");
    if(p.status != SUCCESS || strcmp(c, "hello world\n") != 0){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }
    if(c){
      free(c);
    }

    printf("testing call_c_string_ex...");
    unsigned int len;
    char *ce = p.call_c_string_ex("foo_string_ex", &len);
    if(p.status != SUCCESS || len != sizeof("hello \0world\n")-1 ||
       memcmp(ce, "hello \0world\n\0", len+1) != 0){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }
    if(ce){
      free(ce);
    }

    size_t sla, sba, sda, sca, sia, sua;

    printf("testing call_long_arr...");
    long *la = p.call_long_arr(&sla, "foo_vector_long");
    if(p.status != SUCCESS || sla != 4 ||
       la[0] != 2 || la[1] != 3 || la[2] != 4 || la[3] != 3){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_bool_arr...");
    bool *ba = p.call_bool_arr(&sba, "foo_vector_bool");
    if(p.status != SUCCESS || sba != 3 || !ba[0] || ba[1] || !ba[2]){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_double_arr...");
    double *da = p.call_double_arr(&sda, "foo_vector_double");
    if(p.status != SUCCESS || sda != 4 ||
       da[0] != 2.71 || da[1] != 3.14 || da[2] != 4.0 || da[3] != 3.14){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_c_string_arr...");
    char **ca = p.call_c_string_arr(&sca, "foo_vector_string");
    if(p.status != SUCCESS || sca != 4 || strcmp(ca[0], "two") != 0 ||
       strcmp(ca[1], "three") != 0 || strcmp(ca[2], "four") != 0 ||
       strcmp(ca[3], "three") != 0){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_int_arr...");
    int *ia = p.call_int_arr(&sia, "foo_vector_long");
    if(p.status != SUCCESS || sia != 4 || 
       ia[0] != 2 || ia[1] != 3 || ia[2] != 4 || ia[3] != 3){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_uint_arr...");
    unsigned int *ua = p.call_uint_arr(&sua, "foo_vector_long");
    if(p.status != SUCCESS || sua != 4 ||
       ua[0] != 2 || ua[1] != 3 || ua[2] != 4 || ua[3] != 3){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string...");
    string s = p.call_string("foo_string");
    if(p.status != SUCCESS || s != "hello world\n"){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_vector...");
    vector<string> sv = p.call_string_vector("foo_vector_string");
    if(p.status != SUCCESS || sv.size() != 4 || sv[0] != "two" || 
       sv[1] != "three" || sv[2] != string("four\0\1", 6) || sv[3] != "three"){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_double_vector...");
    vector<double> dv = p.call_double_vector("foo_vector_double");
    if(p.status != SUCCESS || dv.size() != 4 ||
       dv[0] != 2.71 || dv[1] != 3.14 || dv[2] != 4.0 || dv[3] != 3.14){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_long_vector...");
    vector<long> lv = p.call_long_vector("foo_vector_long");
    if(p.status != SUCCESS || lv.size() != 4 ||
       lv[0] != 2 || lv[1] != 3 || lv[2] != 4 || lv[3] != 3){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_bool_vector...");
    vector<bool> bv = p.call_bool_vector("foo_vector_bool");
    if(p.status != SUCCESS || bv.size() != 3 || !bv[0] || bv[1] || !bv[2]){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_int_vector...");
    vector<int> iv = p.call_int_vector("foo_vector_long");
    if(p.status != SUCCESS || iv.size() != 4 ||
       lv[0] != 2 || lv[1] != 3 || lv[2] != 4 || lv[3] != 3){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_uint_vector...");
    vector<unsigned int> uv = p.call_uint_vector("foo_vector_long");
    if(p.status != SUCCESS || uv.size() != 4 ||
       lv[0] != 2 || lv[1] != 3 || lv[2] != 4 || lv[3] != 3){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_hash_set...");
    hash_set<string> shs = p.call_string_hash_set("foo_vector_string");
    if(p.status != SUCCESS || shs.size() != 3 || shs.find("two") == shs.end() ||
       shs.find("three") == shs.end() ||
       shs.find(string("four\0\1", 6)) == shs.end()){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_string_map...");
    map<string, string> ssm = p.call_string_string_map("foo_assoc_string");
    if(p.status != SUCCESS || ssm.size() != 3 || ssm["two"] != "dos" ||
       ssm["three"] != "tres" ||
       ssm[string("four\0\1", 6)] != string("quatro\0\1", 8)){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_double_map...");
    map<string, double> sdm = p.call_string_double_map("foo_assoc_double");
    if(p.status != SUCCESS || sdm.size() != 4 || sdm["two"] != 2.71 ||
       sdm["three"] != 3.14 || sdm[string("four\0\1", 6)] != 4.0 ||
       sdm["five"] != 3.14){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_long_map...");
    map<string, long> slm = p.call_string_long_map("foo_assoc_long");
    if(p.status != SUCCESS || slm.size() != 4 || slm["two"] != 2 ||
       slm["three"] != 3 || slm[string("four\0\1", 6)] != 4 ||
       slm["five"] != 3){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_bool_map...");
    map<string, bool> sbm = p.call_string_bool_map("foo_assoc_bool");
    if(p.status != SUCCESS || sbm.size() != 3 || !sbm["two"] || 
       sbm["three"] || !sbm[string("four\0\1", 6)]){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_int_map...");
    map<string, int> sim = p.call_string_int_map("foo_assoc_long");
    if(p.status != SUCCESS || sim.size() != 4 || sim["two"] != 2 ||
       sim["three"] != 3 || sim[string("four\0\1", 6)] != 4 ||
       sim["five"] != 3){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_uint_map...");
    map<string, unsigned int> sum = p.call_string_uint_map("foo_assoc_long");
    if(p.status != SUCCESS || sum.size() != 4 || sum["two"] != 2 ||
       sum["three"] != 3 || sum[string("four\0\1", 6)] != 4 ||
       sum["five"] != 3){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_long_string_map...");
    map<long, string> lsm = p.call_long_string_map("foo_index_string");
    if(p.status != SUCCESS || lsm.size() != 4 || lsm[2] != "two" ||
       lsm[3] != "three" || lsm[4] != string("four\0\1", 6) ||
       lsm[5] != "three"){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_long_double_map...");
    map<long, double> ldm = p.call_long_double_map("foo_index_double");
    if(p.status != SUCCESS || ldm.size() != 4 || ldm[2] != 2.71 ||
       ldm[3] != 3.14 || ldm[4] != 4.0 || ldm[5] != 3.14){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_long_long_map...");
    map<long, long> llm = p.call_long_long_map("foo_index_long");
    if(p.status != SUCCESS || llm.size() != 4 || llm[2] != 2 || 
       llm[3] != 3 || llm[4] != 4 || llm[6] != 6){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_long_bool_map...");
    map<long, bool> lbm = p.call_long_bool_map("foo_index_bool");
    if(p.status != SUCCESS || lbm.size() != 3 || !lbm[2] || lbm[3] || !lbm[4]){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_long_int_map...");
    map<long, int> lim = p.call_long_int_map("foo_index_long");
    if(p.status != SUCCESS || lim.size() != 4 || lim[2] != 2 || 
       lim[3] != 3 || lim[4] != 4 || lim[6] != 6){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_long_uint_map...");
    map<long, unsigned int> lum = p.call_long_uint_map("foo_index_long");
    if(p.status != SUCCESS || lum.size() != 4 || lum[2] != 2 || 
       lum[3] != 3 || lum[4] != 4 || lum[6] != 6){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_string_hash_map...");
    hash_map<string, string> sshm =
       p.call_string_string_hash_map("foo_assoc_string");
    if(p.status != SUCCESS || sshm.size() != 3 || sshm["two"] != "dos" ||
       sshm["three"] != "tres" ||
       sshm[string("four\0\1", 6)] != string("quatro\0\1", 8)){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_double_hash_map...");
    hash_map<string, double> sdhm =
        p.call_string_double_hash_map("foo_assoc_double");
    if(p.status != SUCCESS || sdhm.size() != 4 || sdhm["two"] != 2.71 ||
       sdhm["three"] != 3.14 || sdhm[string("four\0\1", 6)] != 4.0 ||
       sdhm["five"] != 3.14){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_long_hash_map...");
    hash_map<string, long> slhm = p.call_string_long_hash_map("foo_assoc_long");
    if(p.status != SUCCESS || slhm.size() != 4 || slhm["two"] != 2 ||
       slhm["three"] != 3 || slhm[string("four\0\1", 6)] != 4 ||
       slhm["five"] != 3){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_bool_hash_map...");
    hash_map<string, bool> sbhm = p.call_string_bool_hash_map("foo_assoc_bool");
    if(p.status != SUCCESS || sbhm.size() != 3 || !sbhm["two"] || 
       sbhm["three"] || !sbhm[string("four\0\1", 6)]){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_int_hash_map...");
    hash_map<string, int> sihm = p.call_string_int_hash_map("foo_assoc_long");
    if(p.status != SUCCESS || sihm.size() != 4 || sihm["two"] != 2 ||
       sihm["three"] != 3 || sihm[string("four\0\1", 6)] != 4 ||
       sihm["five"] != 3){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing call_string_uint_hash_map...");
    hash_map<string, unsigned int> suhm =
        p.call_string_uint_hash_map("foo_assoc_long");
    if(p.status != SUCCESS || suhm.size() != 4 || suhm["two"] != 2 ||
       suhm["three"] != 3 || suhm[string("four\0\1", 6)] != 4 ||
       suhm["five"] != 3){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    // don't forget to free this memory!
    free(la);
    free(ba);
    free(da);
    for(unsigned int i=0; i<sca; i++) {
      free(ca[i]);
    }
    free(ca);
    free(ia);
    free(ua);
  }


  //
  // CODE TO TEST CREATION OF PHP ARRAYS IN C
  //

  {
    // test associative array creation
    printf("testing php_array add_assoc...");
    php_array a;
    a.add_assoc("slsSSl", "a", 1l, "bee", "tw\0\1", 4, "cee\0\1", 5, 3l);
    if(!p.call_bool("verify_assoc", "a", &a)){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    // test indexed array creation and mixed arrays
    printf("testing php_array add_index...");
    php_array b;
    b.add_index("lslSld", 5, "one", 2, "abc", sizeof("abc") - 1, 10, 2.71);
    if(!p.call_bool("verify_index", "a", &b)){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    // test mixed array creation
    printf("testing php_array mixed...");
    b.add_assoc("ss", "what", "yo");
    if(!p.call_bool("verify_mixed", "a", &b)){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    // test adding an arbitrary enumerated value
    printf("testing php_array enumerated...");
    b.add("d", 2.1413);
    if(!p.call_bool("verify_enum", "a", &b)){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }
    
    // set up deep copy test
    php_array d(b);

    // test removing elements both by association and index
    // test nested arrays
    printf("testing php_array nested...");
    php_array c;
    c.add("aa", &a, &b);
    if(!p.call_bool("verify_nested", "a", &c)){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    printf("testing php_array remove...");
    a.remove("a");
    a.remove("cee\0\1", sizeof("cee\0\1") - 1);
    b.remove(5);
    if(!p.call_bool("verify_remove", "aa", &a, &b)){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    // test deep copy
    printf("testing php_array deep copy...");
    if(!p.call_bool("verify_enum", "a", &d)){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }

    // 
    // CODE TO TEST PHP ARRAY RETURNS AND ITERATORS
    //

    printf("testing call_php_array and php_iterator...");
    bool pass = true;
    php_array *e = p.call_php_array("foo_complex_array");
    php_iterator it(*e);
    if(!it.done() && it.get_data_type() == IS_ARRAY){

      php_array f = it.get_data_array();
      php_iterator subit(f);
      if(!subit.done() && subit.get_data_type() == IS_ARRAY){

        php_array g = subit.get_data_array();
        php_iterator subsubit(g);
        if(!subsubit.done() && subsubit.get_data_type() == IS_LONG){
          if(subsubit.get_data_long() != 1)
            pass = false;

          subsubit++;
          if(!subsubit.done())
            pass = false;

        } else {
          pass = false;
        }

        // this also tests type conversion...
        subit++;
        if(!subit.done() && subit.get_data_type() == IS_STRING){
          if(subit.get_data_long() != 2)
            pass = false;
        } else {
          pass = false;
        }

        subit++;
        if(!subit.done())
          pass = false;

        it++;
        if(!it.done() && it.get_data_type() == IS_ARRAY){
          php_array h = it.get_data_array();
          php_iterator subit2(h);

          if(!subit2.done() && subit2.get_data_type() == IS_DOUBLE
             && subit2.get_key_type() == IS_STRING){

            char *key = subit2.get_key_c_string();
            if(subit2.get_data_double() != 3.14
              || strcmp(key, "x") != 0){
              pass = false;
            }
            if(key){
              free(key);
            }
          } else {
            pass = false;
          }
        } else {
          pass = false;
        }

        it++;
        if(!it.done() && it.get_data_type() == IS_BOOL 
           && it.get_key_type() == IS_LONG){
          if(it.get_data_bool() || it.get_key_long() != 4)
            pass = false;
        } else {
          pass = false;
        }

        it++;
        if(!it.done() && it.get_data_type() == IS_STRING 
           && it.get_key_type() == IS_STRING){
          unsigned int len;
          char *str = it.get_key_c_string(&len);
          if (len != sizeof("a\0.\0.a") - 1 ||
              memcmp(str, "a\0.\0.a\0", len + 1) != 0){
            pass=false;
          }
          if (str) {
            free(str);
          }
          str = it.get_data_c_string(&len);
          if (len != sizeof("b\0.\0.b\1") - 1 ||
              memcmp(str, "b\0.\0.b\1\0", len + 1) != 0) {
            pass=false;
          }
          if (str) {
            free(str);
          }
        } else {
          pass = false;
        }

        it++;
        if(!it.done())
          pass = false;

      } else {
        pass = false;
      }
    } else {
      pass = false;
    }

    delete e;

    if(!pass){
      printf("failed!\n");
      failcount++;
    } else {
      printf("passed.\n");
    }
  }

  if(failcount == 0)
    printf("ALL TESTS PASSED!\n");
  else
    printf("%d TESTS FAILED\n", failcount);
}
