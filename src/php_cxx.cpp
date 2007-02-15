// PHPEmbed implementation
// Copyright (c) 2007 Andrew Bosworth, Facebook, inc
// All rights reserved

#include "php_cxx.h"
#include <stdio.h>

php::php(bool _type_warnings)
{
  type_warnings = _type_warnings;

  char *argv[2] = {"", NULL};

  // set up the callbacks
  php_embed_module.sapi_error = error_wrap;
  php_embed_module.log_message = message_wrap;
  php_embed_module.ub_write = output_wrap;

  if (SUCCESS != php_embed_init(1, argv PTSRMLS_CC))
  {
    internal_error("PHP ERROR: failed initializing php_embed\n");
    status = FAIL;
    return;
  }

  // CLIENTS: you may want to modify these ini arguments in your code

  // as an embedded module, we don't want any PHP timeout!
  php_set_ini_entry("max_execution_time", "0", PHP_INI_STAGE_ACTIVATE);

  // we don't have get or post or cookie data in an embedded context
  php_set_ini_entry("variables_order", "S", PHP_INI_STAGE_ACTIVATE);
}

void php::set_message_function(void (*_message_function)(const char *)){
  p.message_function = _message_function;
}

void php::set_error_function(void (*_error_function)(const char *)){
  p.error_function = _error_function;
}

void php::set_output_function(void (*_output_function)(const char *)){
  p.output_function = _output_function;
}

void php::message_wrap(char * str){
  if(p.message_function != NULL)
    p.message_function(str);
  else 
    fprintf(stderr, "PHP MESSAGE: %s\n", str);
}

void php::internal_error(const char *str){
  if(p.error_function != NULL){
    p.error_function(str);
  } else {
    fprintf(stderr, "PHP ERROR: %s", str);
  }
}

void php::error_wrap(int error, const char * fmt, ...){

  char **buffer;

  va_list ap;
  va_start(ap, fmt);

  int ret = vasprintf(buffer, fmt, ap);
  // give up if we failed to allocate a proper buffer
  if(ret < 0)
    return;

  va_end(ap);

  internal_error(*buffer);

  free(*buffer);
}

int php::output_wrap(const char *str, unsigned int strlen){
  if(strlen <= 0)
    return SUCCESS;

  if(p.output_function != NULL)
    p.output_function(str);
  else
    printf("PHP OUTPUT: %s", str);

  return SUCCESS;
}

php::~php()
{
  php_embed_shutdown(TSRMLS_C);
}

// void is the easiest, we just call the function and free the return
void php::call_void(char *fn, char *argspec, ...)
{
  zval *rv;

  va_list ap;
  va_start(ap, argspec);
  rv = call(fn, argspec, ap);
  va_end(ap);

  if(rv)
    zval_ptr_dtor(&rv);

  return;
}

long php::call_long(char *fn, char *argspec, ...)
{
  zval *rv;
  long rrv = 0;

  va_list ap;
  va_start(ap, argspec);
  rv = call(fn, argspec, ap);
  va_end(ap);

  if(rv)
  {
    if(Z_TYPE_P(rv) != IS_LONG)
    {
      if(type_warnings)
        internal_error("TYPE MISMATCH: expected long return type from PHP!\n");

      // copy rv into new memory for the in place conversion
      zval *cp;
      MAKE_STD_ZVAL(cp);
      *cp = *rv;
      zval_copy_ctor(cp);
      INIT_PZVAL(cp);

      // destroy the original now that we have a copy
      zval_ptr_dtor(&rv);

      // rename rv to our copy
      rv = cp;
      convert_to_long_ex(&rv);

    } 

    rrv = Z_LVAL_P(rv);

    zval_ptr_dtor(&rv);
  }

  return rrv;
}

bool php::call_bool(char *fn, char *argspec, ...)
{
  zval *rv;
  bool rrv = false;

  va_list ap;
  va_start(ap, argspec);
  rv = call(fn, argspec, ap);
  va_end(ap);

  if(rv)
  {
    if(Z_TYPE_P(rv) != IS_BOOL)
    {
      if(type_warnings)
        internal_error("TYPE MISMATCH: expected bool return type from PHP!\n");

      // copy rv into new memory for the in place conversion
      zval *cp;
      MAKE_STD_ZVAL(cp);
      *cp = *rv;
      zval_copy_ctor(cp);
      INIT_PZVAL(cp);

      // destroy the original now that we have a copy
      zval_ptr_dtor(&rv);

      // rename rv to our copy
      rv = cp;
      convert_to_boolean_ex(&rv);
    }

    rrv = (bool)Z_LVAL_P(rv);
    zval_ptr_dtor(&rv);
  }

  return rrv;
}

double php::call_double(char *fn, char *argspec, ...)
{
  zval *rv;
  double rrv = 0;

  va_list ap;
  va_start(ap, argspec);
  rv = call(fn, argspec, ap);
  va_end(ap);

  if(rv)
  {
    if(Z_TYPE_P(rv) != IS_DOUBLE)
    {
      if(type_warnings)
        internal_error("TYPE MISMATCH: expected double return type from PHP!\n");

      // copy rv into new memory for the in place conversion
      zval *cp;
      MAKE_STD_ZVAL(cp);
      *cp = *rv;
      zval_copy_ctor(cp);
      INIT_PZVAL(cp);

      // destroy the original now that we have a copy
      zval_ptr_dtor(&rv);

      // rename rv to our copy
      rv = cp;
      convert_to_double_ex(&rv);
    }

    rrv = Z_DVAL_P(rv);
    zval_ptr_dtor(&rv);
  }

  return rrv;
}

char *php::call_c_string(char *fn, char *argspec, ...)
{
  zval *rv;
  char *rrv = NULL;

  va_list ap;
  va_start(ap, argspec);
  rv = call(fn, argspec, ap);
  va_end(ap);

  if(rv)
  {
    if(Z_TYPE_P(rv) != IS_STRING)
    {
      if(type_warnings)
        internal_error("TYPE MISMATCH: expected string return type from PHP!\n");

      // copy rv into new memory for the in place conversion
      zval *cp;
      MAKE_STD_ZVAL(cp);
      *cp = *rv;
      zval_copy_ctor(cp);
      INIT_PZVAL(cp);

      // destroy the original now that we have a copy
      zval_ptr_dtor(&rv);

      // rename rv to our copy
      rv = cp;
      convert_to_string_ex(&rv);
    }

    rrv = estrndup(Z_STRVAL_P(rv), Z_STRLEN_P(rv));
    zval_ptr_dtor(&rv);
  }

  return rrv;
}

php_array php::call_php_array(char *fn, char *argspec, ...)
{
  zval *rv;

  va_list ap;
  va_start(ap, argspec);
  rv = call(fn, argspec, ap);
  va_end(ap);

  if(rv)
  {
    if(Z_TYPE_P(rv) != IS_ARRAY)
    {
      if(type_warnings)
        internal_error("TYPE MISMATCH: expected array return type from PHP!\n");

      // copy rv into new memory for the in place conversion
      zval *cp;
      MAKE_STD_ZVAL(cp);
      *cp = *rv;
      zval_copy_ctor(cp);
      INIT_PZVAL(cp);

      // destroy the original now that we have a copy
      zval_ptr_dtor(&rv);

      // rename rv to our copy
      rv = cp;
      convert_to_array_ex(&rv);
    }

    // create the new php array object
    php_array rrv(rv);
    zval_ptr_dtor(&rv);
    return rrv;
  }

  return php_array();
}

long *php::call_long_arr(size_t *size, char *fn, char *argspec, ...)
{
  zval *rv;
  long *rrv = NULL;

  va_list ap;
  va_start(ap, argspec);
  rv = call(fn, argspec, ap);
  va_end(ap);

  if(rv)
  {
    if(Z_TYPE_P(rv) != IS_ARRAY)
      internal_error("TYPE MISMATCH: expected array return type from PHP!\n");
    else
    {
      HashTable *ht = Z_ARRVAL_P(rv);
      zval **data;
      rrv = (long *) malloc(zend_hash_num_elements(ht) * sizeof(long));

      // in case we have to copy data and conver tit
      zval *cp;
      bool copy = false;

      zend_hash_internal_pointer_reset(ht);

      int i = 0;
      while (zend_hash_get_current_data(ht, (void **)&data) == SUCCESS)
      {
        if(Z_TYPE_PP(data) != IS_LONG)
        {
          if(type_warnings)
            internal_error("TYPE MISMATCH: expected long array entry!\n");

          // copy data into new memory for the in place conversion
          copy = true;
          MAKE_STD_ZVAL(cp);
          *cp = **data;
          zval_copy_ctor(cp);
          INIT_PZVAL(cp);
          convert_to_long_ex(&cp);
        }

        if(copy){
          rrv[i] = Z_LVAL_P(cp);
          zval_ptr_dtor(&cp);
          copy = false;
        } else {
          rrv[i] = Z_LVAL_PP(data);
        }

        zend_hash_move_forward(ht);
        i++;
      }

      *size = i;
    }

    zval_ptr_dtor(&rv);
  }

  return rrv;
}

bool *php::call_bool_arr(size_t *size, char *fn, char *argspec, ...)
{
  zval *rv;
  bool *rrv = NULL;

  if(NULL == size){
    internal_error("size must point to a valid size_t object\n");
    return NULL;
  }

  va_list ap;
  va_start(ap, argspec);
  rv = call(fn, argspec, ap);
  va_end(ap);

  if(rv)
  {
    if(Z_TYPE_P(rv) != IS_ARRAY)
      internal_error("TYPE MISMATCH: expected array return type from PHP!\n");
    else
    {
      HashTable *ht = Z_ARRVAL_P(rv);
      zval **data;
      rrv = (bool *) malloc(zend_hash_num_elements(ht) * sizeof(bool));

      // in case we have to copy data and conver tit
      zval *cp;
      bool copy = false;

      zend_hash_internal_pointer_reset(ht);

      int i = 0;
      while (zend_hash_get_current_data(ht, (void **)&data) == SUCCESS)
      {
        if(Z_TYPE_PP(data) != IS_BOOL)
        {
          if(type_warnings)
            internal_error("TYPE MISMATCH: expected bool array entry!\n");

          // copy data into new memory for the in place conversion
          copy = true;
          MAKE_STD_ZVAL(cp);
          *cp = **data;
          zval_copy_ctor(cp);
          INIT_PZVAL(cp);
          convert_to_boolean_ex(&cp);
        }

        if(copy){
          rrv[i] = (bool)Z_LVAL_P(cp);
          zval_ptr_dtor(&cp);
          copy = false;
        } else {
          rrv[i] = (bool)Z_LVAL_PP(data);
        }

        zend_hash_move_forward(ht);
        i++;
      }

      *size = i;
    }

    zval_ptr_dtor(&rv);
  }

  return rrv;
}

double *php::call_double_arr(size_t *size, char *fn, char *argspec, ...)
{
  zval *rv;
  double *rrv = NULL;

  if(NULL == size){
    internal_error("size must point to a valid size_t object\n");
    return NULL;
  }

  va_list ap;
  va_start(ap, argspec);
  rv = call(fn, argspec, ap);
  va_end(ap);

  if(rv)
  {
    if(Z_TYPE_P(rv) != IS_ARRAY)
      internal_error("TYPE MISMATCH: expected array return type from PHP!\n");
    else
    {
      HashTable *ht = Z_ARRVAL_P(rv);
      zval **data;
      rrv = (double *) malloc(zend_hash_num_elements(ht) * sizeof(double));

      // in case we have to copy data and conver tit
      zval *cp;
      bool copy = false;

      zend_hash_internal_pointer_reset(ht);

      int i = 0;
      while (zend_hash_get_current_data(ht, (void **)&data) == SUCCESS)
      {
        if(Z_TYPE_PP(data) != IS_DOUBLE)
        {
          if(type_warnings)
            internal_error("TYPE MISMATCH: expected double array entry!\n");

          // copy data into new memory for the in place conversion
          copy = true;
          MAKE_STD_ZVAL(cp);
          *cp = **data;
          zval_copy_ctor(cp);
          INIT_PZVAL(cp);
          convert_to_double_ex(&cp);
        }

        if(copy){
          rrv[i] = Z_DVAL_P(cp);
          zval_ptr_dtor(&cp);
          copy = false;
        } else {
          rrv[i] = Z_DVAL_PP(data);
        }

        zend_hash_move_forward(ht);
        i++;
      }

      *size = i;
    }

    zval_ptr_dtor(&rv);
  }

  return rrv;
}

char **php::call_c_string_arr(size_t *size, char *fn, char *argspec, ...)
{
  zval *rv;
  char **rrv = NULL;

  if(NULL == size){
    internal_error("size must point to a valid size_t object\n");
    return NULL;
  }

  va_list ap;
  va_start(ap, argspec);
  rv = call(fn, argspec, ap);
  va_end(ap);

  if(rv)
  {
    if(Z_TYPE_P(rv) != IS_ARRAY)
      internal_error("TYPE MISMATCH: expected array return type from PHP!\n");
    else
    {
      HashTable *ht = Z_ARRVAL_P(rv);
      zval **data;
      rrv = (char **) malloc(zend_hash_num_elements(ht) * sizeof(char *));

      // in case we have to copy data and conver tit
      zval *cp;
      bool copy = false;

      zend_hash_internal_pointer_reset(ht);

      int i = 0;
      while (zend_hash_get_current_data(ht, (void **)&data) == SUCCESS)
      {
        if(Z_TYPE_PP(data) != IS_STRING)
        {
          if(type_warnings)
            internal_error("TYPE MISMATCH: expected string array entry!\n");

          // copy data into new memory for the in place conversion
          copy = true;
          MAKE_STD_ZVAL(cp);
          *cp = **data;
          zval_copy_ctor(cp);
          INIT_PZVAL(cp);
          convert_to_string_ex(&cp);
        }

        if(copy){
          rrv[i] = estrndup(Z_STRVAL_P(cp), Z_STRLEN_P(cp));
          zval_ptr_dtor(&cp);
          copy = false;
        } else {
          rrv[i] = estrndup(Z_STRVAL_PP(data), Z_STRLEN_PP(data));
        }

        zend_hash_move_forward(ht);
        i++;
      }

      *size = i;
    }

    zval_ptr_dtor(&rv);
  }

  return rrv;
}

// NOTE: this just truncates the php long to an int!
int *php::call_int_arr(size_t *size, char *fn, char *argspec, ...)
{
  zval *rv;
  int *rrv = NULL;

  if(NULL == size){
    internal_error("size must point to a valid size_t object\n");
    return NULL;
  }

  va_list ap;
  va_start(ap, argspec);
  rv = call(fn, argspec, ap);
  va_end(ap);

  if(rv)
  {
    if(Z_TYPE_P(rv) != IS_ARRAY)
      internal_error("TYPE MISMATCH: expected array return type from PHP!\n");
    else
    {
      HashTable *ht = Z_ARRVAL_P(rv);
      zval **data;
      rrv = (int *) malloc(zend_hash_num_elements(ht) * sizeof(int));

      // in case we have to copy data and conver tit
      zval *cp;
      bool copy = false;

      zend_hash_internal_pointer_reset(ht);

      int i = 0;
      while (zend_hash_get_current_data(ht, (void **)&data) == SUCCESS)
      {
        if(Z_TYPE_PP(data) != IS_LONG)
        {
          if(type_warnings)
            internal_error("TYPE MISMATCH: expected long array entry!\n");

          // copy data into new memory for the in place conversion
          copy = true;
          MAKE_STD_ZVAL(cp);
          *cp = **data;
          zval_copy_ctor(cp);
          INIT_PZVAL(cp);
          convert_to_long_ex(&cp);
        }

        // typecast each element
        if(copy){
          rrv[i] = (int)Z_LVAL_P(cp);
          zval_ptr_dtor(&cp);
          copy = false;
        } else {
          rrv[i] = (int)Z_LVAL_PP(data);
        }

        zend_hash_move_forward(ht);
        i++;
      }

      *size = i;
    }

    zval_ptr_dtor(&rv);
  }

  return rrv;
}

// NOTE: this just truncates the php long to an int!
unsigned int *php::call_uint_arr(size_t *size, char *fn, char *argspec, ...)
{
  zval *rv;
  unsigned int *rrv = NULL;

  if(NULL == size){
    internal_error("size must point to a valid size_t object\n");
    return NULL;
  }

  va_list ap;
  va_start(ap, argspec);
  rv = call(fn, argspec, ap);
  va_end(ap);

  if(rv)
  {
    if(Z_TYPE_P(rv) != IS_ARRAY)
      internal_error("TYPE MISMATCH: expected array return type from PHP!\n");
    else
    {
      HashTable *ht = Z_ARRVAL_P(rv);
      zval **data;
      rrv = (unsigned int *) malloc(zend_hash_num_elements(ht)
                                    * sizeof(unsigned int));

      // in case we have to copy data and conver tit
      zval *cp;
      bool copy = false;

      zend_hash_internal_pointer_reset(ht);

      int i = 0;
      while (zend_hash_get_current_data(ht, (void **)&data) == SUCCESS)
      {
        if(Z_TYPE_PP(data) != IS_LONG)
        {
          if(type_warnings)
            internal_error("TYPE MISMATCH: expected long array entry!\n");

          // copy data into new memory for the in place conversion
          copy = true;
          MAKE_STD_ZVAL(cp);
          *cp = **data;
          zval_copy_ctor(cp);
          INIT_PZVAL(cp);
          convert_to_long_ex(&cp);
        }

        // typecast each element
        if(copy){
          rrv[i] = (unsigned int)Z_LVAL_P(cp);
          zval_ptr_dtor(&cp);
          copy = false;
        } else {
          rrv[i] = (unsigned int)Z_LVAL_PP(data);
        }

        zend_hash_move_forward(ht);
        i++;
      }

      *size = i;
    }

    zval_ptr_dtor(&rv);
  }

  return rrv;
}

// We could go to great lengths to use zend_file_handles...or just do this...
php_ret php::load(const char *filename)
{
  return eval_string("include_once('%s');", filename);
}

// evaulate a string of php in our environment, changes are permanent!
php_ret php::eval_string(const char *fmt, ...)
{
  char *data = NULL;
  va_list ap;

  va_start(ap, fmt);

  zend_first_try {
    vspprintf(&data, 0, fmt, ap);
    status = zend_eval_string(data, NULL, "");
  } zend_catch {
    status = FAIL;
  } zend_end_try();

  if (data)
    efree(data);

  va_end(ap);
  return status;
}

// call an arbitrary php function with the given arguments
zval *php::call(char *fn, char *argspec, ...){
  zval *rv;

  va_list ap;
  va_start(ap, argspec);
  rv = call(fn, argspec, ap);
  va_end(ap);

  return rv;
}

// call an arbitrary php function with the given arguments
zval *php::call(char *fn, char *argspec, va_list ap)
{
  zend_try {
    // convert the function name to a zval
    zval *function_name;
    MAKE_STD_ZVAL(function_name);
    ZVAL_STRING(function_name, fn, 0);

    // parse the parameter list
    zval *params[strlen(argspec)];
    zend_uint count;
    if (parse_args(params, &count, argspec, ap) != SUCCESS)
    {
      error_wrap(0, "parsing args for function %s\n", fn);
      for(unsigned int i = 0; i < count; i++)
        if(params[i]) zval_ptr_dtor(&params[i]);
      efree(function_name);
      status = FAIL;
      return NULL;
    }

    zval *rv;
    MAKE_STD_ZVAL(rv);
    if(call_user_function(EG(function_table), NULL, function_name, rv,
                          count, params TSRMLS_CC) != SUCCESS)
    {
      error_wrap(0, "calling function %s\n", fn);
      for(unsigned int i = 0; i < count; i++)
        if(params[i]) zval_ptr_dtor(&params[i]);
      efree(function_name);
      status = FAIL;
      return NULL;
    }

    for(unsigned int i = 0; i < count; i++)
      if(params[i]) zval_ptr_dtor(&params[i]);
    efree(function_name);
    return rv;
  } zend_catch {
    error_wrap(0, "preparing function %s\n", fn);
    status = FAIL;
    return NULL;
  } zend_end_try() {
    return NULL;
  }
  return NULL;
}


// parse arbitrary C++ argument list into a zend argument list based on
// argspec string.
//
//  params and count are return values.
//  params should be freed by the caller
php_ret php::parse_args(zval **params, zend_uint *count, char *argspec, va_list ap)
{
  int i = 0;
  for(char *trav = argspec; *trav; trav++)
  {
    MAKE_STD_ZVAL(params[i]);
    switch(*trav)
    {
    case 'b':
      {
        // va promotes bools to ints
        int arg = va_arg(ap, int);
        ZVAL_BOOL(params[i], arg);
      }
      break;

    case 'i':
      {
        int arg = va_arg(ap, int);
        ZVAL_LONG(params[i], arg);
      }
      break;

    case 'l':
      {
        long arg = va_arg(ap, long);
        ZVAL_LONG(params[i], arg);
      }
      break;

    case 'd':
      {
        double arg = va_arg(ap, double);
        ZVAL_DOUBLE(params[i], arg);
      }
      break;

    case 's':
      {
        char *arg = va_arg(ap, char *);
        ZVAL_STRING(params[i], arg, 1);
      }
      break;

    case 'a':
      {
        php_array *arg = va_arg(ap, php_array *);
        *params[i] = *arg->data();

        // forge on even if this isn't what we expected
        if(type_warnings && Z_TYPE_P(params[i]) != IS_ARRAY)
          internal_error("TYPE WARNING: Expecting an array argument!\n");

        // copy the contents to new memory for this array
        zval_copy_ctor(params[i]);

        // reset refcount
        INIT_PZVAL(params[i]);
      }
      break;

    default:
      error_wrap(0, "encountered bad argument specifier: %s\n", trav);
      status = FAIL;
      return status;
    }

    i++;
  }

  *count = i;

  return SUCCESS;
}

// set an ini entry (you could also do this with set_ini in a php file)
int php::php_set_ini_entry(char *entry, char *value, int stage)
{
  return zend_alter_ini_entry(entry, strlen(entry)+1, value, strlen(value)+1,
                              PHP_INI_USER, stage);
}
