/*
The MIT License (MIT)

Copyright (c) Latino - Lenguaje de Programacion

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */

#include <jansson.h>

#include <latino.h>

#define LIB_JSON_NAME "json"

static json_t *json_cargar(lat_mv *mv, const char *text) {
  json_t *root;
  json_error_t error;
  root = json_loads(text, 0, &error);
  if (root) {
    return root;
  } else {
    latC_error(mv, "JSON error %d: %s", error.line, error.text);
    return (json_t *)0;
  }
}

static lat_objeto *json_alatino(lat_mv *mv, json_t *element) {
  size_t i;
  size_t size;
  json_t *value = NULL;
  const char *key;
  switch (json_typeof(element)) {
  case JSON_OBJECT: {
    size = json_object_size(element);
    lat_objeto *dic = latC_crear_dic(mv, latH_crear(mv));

    json_object_foreach(element, key, value) {
      // printf("key: %s\n", key);
      latH_asignar(mv, latC_checar_dic(mv, dic), key, json_alatino(mv, value));
    }
    return dic;
  } break;
  case JSON_ARRAY: {
    size = json_array_size(element);
    lat_objeto *lst = latC_crear_lista(mv, latL_crear(mv));
    for (i = 0; i < size; i++) {
      value = json_array_get(element, i);
      latL_agregar(mv, latC_checar_lista(mv, lst),
                   (void *)json_alatino(mv, value));
    }
    return lst;
  } break;
  case JSON_STRING: {
    lat_objeto *str = latC_crear_cadena(mv, json_string_value(element));
    return str;
  } break;
  case JSON_INTEGER: {
    lat_objeto *dec =
        latC_crear_numerico(mv, (double)json_integer_value(element));
    return dec;
  } break;
  case JSON_REAL: {
    lat_objeto *dec = latC_crear_numerico(mv, (double)json_real_value(element));
    return dec;
  } break;
  case JSON_TRUE:
    return latO_verdadero;
    break;
  case JSON_FALSE:
    return latO_falso;
    break;
  case JSON_NULL:
    return latO_nulo;
    break;
  default:
    fprintf(stderr, "unrecognized JSON type %d\n", json_typeof(element));
  }
  return NULL;
}

static json_t *latino_ajson(lat_mv *mv, lat_objeto *element) {
  json_t *value = NULL;
  switch (element->tipo) {
  case T_DIC: {
    value = json_object();
    int i;
    for (i = 0; i < 256; i++) {
      lista *list = latC_checar_dic(mv, element)->buckets[i];
      if (list != NULL) {

        LIST_FOREACH(list, primero, siguiente, cur) {
          if (cur->valor != NULL) {
            json_object_set(
                value, ((hash_val *)cur->valor)->llave,
                latino_ajson(mv,
                             (lat_objeto *)((hash_val *)cur->valor)->valor));
          }
        }
      }
    }
    return value;
  } break;
  case T_LIST: {
    value = json_array();
    lista *list = latC_checar_lista(mv, element);

    LIST_FOREACH(list, primero, siguiente, cur) {
      if (cur->valor != NULL) {
        json_array_append_new(value,
                              latino_ajson(mv, (lat_objeto *)cur->valor));
      }
    }
    return value;
  } break;
  case T_STR: {
    value = json_string(latC_checar_cadena(mv, element));
    return value;
  } break;
  case T_NUMERIC: {
    double d = latC_checar_numerico(mv, element);
    if (fmod(d, 1) == 0) {
      value = json_integer((int)latC_checar_numerico(mv, element));
    } else {
      value = json_real(latC_checar_numerico(mv, element));
    }
    return value;
  } break;
  case T_BOOL: {
    if (latC_checar_logico(mv, element)) {
      value = json_true();
    } else {
      value = json_false();
    }
    return value;
  } break;
  case T_NULL: {
    value = json_null();
    return value;
  } break;
  default: {
    value = json_null();
    return value;
  }
  }
}

static void json_decodificar(lat_mv *mv) {
  lat_objeto *a = latC_desapilar(mv);
  char *str = latC_checar_cadena(mv, a);
  json_t *root = json_cargar(mv, str);
  lat_objeto *tmp = latO_nulo;
  if (root) {
    tmp = json_alatino(mv, root);
    json_decref(root);
  }
  latC_apilar(mv, tmp);
}

static void json_codificar(lat_mv *mv) {
  lat_objeto *a = latC_desapilar(mv);
  lat_objeto *tmp = latO_nulo;
  json_t *j = latino_ajson(mv, a);
  char *s = json_dumps(j, JSON_ALLOW_NUL);
  tmp = latC_crear_cadena(mv, s);
  latC_apilar(mv, tmp);
  free(s);
}

static void json_formato(lat_mv *mv) {
  lat_objeto *a = latC_desapilar(mv);
  int spaces = 4;
  char *str = NULL;
  json_t *root = NULL;
  if (a->tipo != T_STR) {
    root = latino_ajson(mv, a);
  } else {
    str = latC_checar_cadena(mv, a);
    root = json_cargar(mv, str);
  }
  lat_objeto *tmp = latO_nulo;
  if (root) {
    int flags = JSON_INDENT(spaces);
    char *buf = json_dumps(root, flags);
    tmp = latC_crear_cadena(mv, buf);
    json_decref(root);
    free(buf);
  }
  latC_apilar(mv, tmp);
}

static const lat_CReg libjson[] = {{"decodificar", json_decodificar, 1},
                                   {"codificar", json_codificar, 1},
                                   {"formato", json_formato, 1},
                                   {NULL, NULL}};

void latMV_abrir_latjsonlib(lat_mv *mv) {
  latMV_abrir_lib(mv, LIB_JSON_NAME, libjson);
}
