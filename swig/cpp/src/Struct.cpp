/**
 * @file Struct.cpp
 * @author Mislav Novakovic <mislav.novakovic@sartura.hr>
 * @brief Sysrepo class header implementation for C struts.
 *
 * @copyright
 * Copyright 2016 Deutsche Telekom AG.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <memory>
#include <string.h>

#include "Struct.h"
#include "Sysrepo.h"
#include "Internal.h"

extern "C" {
#include "sysrepo.h"
#include "sysrepo/values.h"
#include "sysrepo/trees.h"
}

using namespace std;

// Data
Data::Data(sr_data_t data, sr_type_t type, S_Deleter deleter) {_d = data; _t = type; _deleter = deleter;}
Data::~Data() {}
char *Data::get_binary() {
    if (_t != SR_BINARY_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.binary_val;
}
char *Data::get_bits() {
    if (_t != SR_BITS_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.bits_val;
}
bool Data::get_bool() {
    if (_t != SR_BOOL_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.bool_val;
}
double Data::get_decimal64() {
    if (_t != SR_DECIMAL64_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.decimal64_val;
}
char *Data::get_enum() {
    if (_t != SR_ENUM_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.enum_val;
}
char *Data::get_identityref() {
    if (_t != SR_IDENTITYREF_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.identityref_val;
}
char *Data::get_instanceid() {
    if (_t != SR_INSTANCEID_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.instanceid_val;
}
int8_t Data::get_int8() {
    if (_t != SR_INT8_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.int8_val;
}
int16_t Data::get_int16() {
    if (_t != SR_INT16_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.uint32_val;
}
int32_t Data::get_int32() {
    if (_t != SR_INT32_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.int32_val;
}
int64_t Data::get_int64() {
    if (_t != SR_INT64_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.int64_val;
}
char *Data::get_string() {
    if (_t != SR_STRING_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.string_val;
}
uint8_t Data::get_uint8() {
    if (_t != SR_UINT8_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.uint32_val;
}
uint16_t Data::get_uint16() {
    if (_t != SR_UINT16_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.uint16_val;
}
uint32_t Data::get_uint32() {
    if (_t != SR_UINT32_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.uint32_val;
}
uint64_t Data::get_uint64() {
    if (_t != SR_UINT64_T) throw_exception(SR_ERR_DATA_MISSING);
    return _d.uint64_val;
}

// Val
Val::Val(sr_val_t *val, S_Deleter deleter) {
    if (val == NULL)
        throw_exception(SR_ERR_INVAL_ARG);
    _val = val;
    _deleter = deleter;
}
Val::Val() {
    _val = NULL;
    _deleter = S_Deleter(new Deleter(_val));
}
Val::~Val() {}
Val::Val(const char *value, sr_type_t type) {
    int ret = SR_ERR_OK;
    sr_val_t *val = NULL;
    val = (sr_val_t*) calloc(1, sizeof(sr_val_t));

    if (val == NULL)
        throw_exception(SR_ERR_NOMEM);

    val->type = type;

    if (type == SR_BINARY_T || type == SR_BITS_T || type == SR_ENUM_T || type == SR_IDENTITYREF_T || \
        type == SR_INSTANCEID_T || type == SR_STRING_T) {
        ret = sr_val_set_str_data(val, type, value);
        if (ret != SR_ERR_OK)
            throw_exception(ret);
    } else if (value != NULL && ( type != SR_LIST_T && type != SR_CONTAINER_T && type != SR_CONTAINER_PRESENCE_T &&\
        type != SR_UNKNOWN_T && type != SR_LEAF_EMPTY_T)) {
        free(val);
        throw_exception(SR_ERR_INVAL_ARG);
    }

    _val = val;
    _deleter = S_Deleter(new Deleter(val));
}
Val::Val(bool bool_val, sr_type_t type) {
    sr_val_t *val = NULL;
    val = (sr_val_t*) calloc(1, sizeof(sr_val_t));
    if (val == NULL)
        throw_exception(SR_ERR_NOMEM);
    if (type == SR_BOOL_T) {
	val->data.bool_val = bool_val;
    } else {
        free(val);
        throw_exception(SR_ERR_INVAL_ARG);
    }

    val->type = type;
    _val = val;
    _deleter = S_Deleter(new Deleter(val));
}
Val::Val(double decimal64_val) {
    sr_val_t *val = NULL;
    val = (sr_val_t*) calloc(1, sizeof(sr_val_t));
    if (val == NULL) {
        throw_exception(SR_ERR_NOMEM);
    } else {
	val->data.decimal64_val = decimal64_val;
    }

    val->type = SR_DECIMAL64_T;
    _val = val;
    _deleter = S_Deleter(new Deleter(val));
}
Val::Val(int8_t int8_val, sr_type_t type) {
    sr_val_t *val = NULL;
    val = (sr_val_t*) calloc(1, sizeof(sr_val_t));
    if (val == NULL)
        throw_exception(SR_ERR_NOMEM);
    if (type == SR_INT8_T) {
	val->data.int8_val = int8_val;
    } else {
        free(val);
        throw_exception(SR_ERR_INVAL_ARG);
    }

    val->type = type;
    _val = val;
    _deleter = S_Deleter(new Deleter(val));
}
Val::Val(int16_t int16_val, sr_type_t type) {
    sr_val_t *val = NULL;
    val = (sr_val_t*) calloc(1, sizeof(sr_val_t));
    if (val == NULL)
        throw_exception(SR_ERR_NOMEM);
    if (type == SR_INT16_T) {
	val->data.int16_val = int16_val;
    } else {
        free(val);
        throw_exception(SR_ERR_INVAL_ARG);
    }

    val->type = type;
    _val = val;
    _deleter = S_Deleter(new Deleter(val));
}
Val::Val(int32_t int32_val, sr_type_t type) {
    sr_val_t *val = NULL;
    val = (sr_val_t*) calloc(1, sizeof(sr_val_t));
    if (val == NULL)
        throw_exception(SR_ERR_NOMEM);
    if (type == SR_INT32_T) {
	val->data.int32_val = int32_val;
    } else {
        free(val);
        throw_exception(SR_ERR_INVAL_ARG);
    }

    val->type = type;
    _val = val;
    _deleter = S_Deleter(new Deleter(val));
}
Val::Val(int64_t int64_val, sr_type_t type) {
    sr_val_t *val = NULL;
    val = (sr_val_t*) calloc(1, sizeof(sr_val_t));
    if (val == NULL)
        throw_exception(SR_ERR_NOMEM);
    if (type == SR_UINT64_T) {
        val->data.uint64_val = (uint64_t) int64_val;
    } else if (type == SR_UINT32_T) {
        val->data.uint32_val = (uint32_t) int64_val;
    } else if (type == SR_UINT16_T) {
        val->data.uint16_val = (uint16_t) int64_val;
    } else if (type == SR_UINT8_T) {
        val->data.uint8_val = (uint8_t) int64_val;
    } else if (type == SR_INT64_T) {
        val->data.int64_val = (int64_t) int64_val;
    } else if (type == SR_INT32_T) {
        val->data.int32_val = (int32_t) int64_val;
    } else if (type == SR_INT16_T) {
        val->data.int16_val = (int16_t) int64_val;
    } else if (type == SR_INT8_T) {
        val->data.int8_val = (int8_t) int64_val;
    } else {
	    printf("\nERROR \n\n\n\n");
        free(val);
        throw_exception(SR_ERR_INVAL_ARG);
    }

    val->type = type;
    _val = val;
    _deleter = S_Deleter(new Deleter(val));
}
Val::Val(uint8_t uint8_val, sr_type_t type) {
    sr_val_t *val = NULL;
    val = (sr_val_t*) calloc(1, sizeof(sr_val_t));
    if (val == NULL)
        throw_exception(SR_ERR_NOMEM);
    if (type == SR_UINT8_T) {
	val->data.uint8_val = uint8_val;
    } else {
        free(val);
        throw_exception(SR_ERR_INVAL_ARG);
    }

    val->type = type;
    _val = val;
    _deleter = S_Deleter(new Deleter(val));
}
Val::Val(uint16_t uint16_val, sr_type_t type) {
    sr_val_t *val = NULL;
    val = (sr_val_t*) calloc(1, sizeof(sr_val_t));
    if (val == NULL)
        throw_exception(SR_ERR_NOMEM);
    if (type == SR_UINT16_T) {
	val->data.uint16_val = uint16_val;
    } else {
        free(val);
        throw_exception(SR_ERR_INVAL_ARG);
    }

    val->type = type;
    _val = val;
    _deleter = S_Deleter(new Deleter(val));
}
Val::Val(uint32_t uint32_val, sr_type_t type) {
    sr_val_t *val = NULL;
    val = (sr_val_t*) calloc(1, sizeof(sr_val_t));
    if (val == NULL)
        throw_exception(SR_ERR_NOMEM);
    if (type == SR_UINT32_T) {
	val->data.uint32_val = uint32_val;
    } else {
        free(val);
        throw_exception(SR_ERR_INVAL_ARG);
    }

    val->type = type;
    _val = val;
    _deleter = S_Deleter(new Deleter(val));
}
Val::Val(uint64_t uint64_val, sr_type_t type) {
    sr_val_t *val = NULL;
    val = (sr_val_t*) calloc(1, sizeof(sr_val_t));
    if (val == NULL)
        throw_exception(SR_ERR_NOMEM);
    if (type == SR_UINT64_T) {
	val->data.uint64_val = uint64_val;
    } else {
        free(val);
        throw_exception(SR_ERR_INVAL_ARG);
    }

    val->type = type;
    _val = val;
    _deleter = S_Deleter(new Deleter(val));
}
void Val::set(const char *xpath, const char *value, sr_type_t type) {
    int ret = SR_ERR_OK;
    if (_val == NULL) throw_exception(SR_ERR_OPERATION_FAILED);

    ret = sr_val_set_xpath(_val, xpath);
    if (ret != SR_ERR_OK) throw_exception(ret);

    _val->type = type;

    if (type == SR_BINARY_T || type == SR_BITS_T || type == SR_ENUM_T || type == SR_IDENTITYREF_T || \
        type == SR_INSTANCEID_T || type == SR_STRING_T) {
        ret = sr_val_set_str_data(_val, type, value);
        if (ret != SR_ERR_OK)
            throw_exception(ret);
    } else if (value != NULL && ( type != SR_LIST_T && type != SR_CONTAINER_T && type != SR_CONTAINER_PRESENCE_T &&\
        type != SR_UNKNOWN_T && type != SR_LEAF_EMPTY_T)) {
        throw_exception(SR_ERR_INVAL_ARG);
    }
}
void Val::set(const char *xpath, bool bool_val, sr_type_t type) {
    if (_val == NULL) throw_exception(SR_ERR_OPERATION_FAILED);

    int ret = sr_val_set_xpath(_val, xpath);
    if (ret != SR_ERR_OK) throw_exception(ret);

    if (type == SR_BOOL_T) {
	    _val->data.bool_val = bool_val;
    } else {
        throw_exception(SR_ERR_INVAL_ARG);
    }

    _val->type = type;
}
void Val::set(const char *xpath, double decimal64_val) {
    if (_val == NULL) throw_exception(SR_ERR_OPERATION_FAILED);

    int ret = sr_val_set_xpath(_val, xpath);
    if (ret != SR_ERR_OK) throw_exception(ret);

    _val->data.decimal64_val = decimal64_val;

    _val->type = SR_DECIMAL64_T;
}
void Val::set(const char *xpath, int8_t int8_val, sr_type_t type) {
    if (_val == NULL) throw_exception(SR_ERR_OPERATION_FAILED);

    int ret = sr_val_set_xpath(_val, xpath);
    if (ret != SR_ERR_OK) throw_exception(ret);

    if (type == SR_INT8_T) {
	    _val->data.int8_val = int8_val;
    } else {
        throw_exception(SR_ERR_INVAL_ARG);
    }

    _val->type = type;
}
void Val::set(const char *xpath, int16_t int16_val, sr_type_t type) {
    if (_val == NULL) throw_exception(SR_ERR_OPERATION_FAILED);

    int ret = sr_val_set_xpath(_val, xpath);
    if (ret != SR_ERR_OK) throw_exception(ret);

    if (type == SR_INT16_T) {
	    _val->data.int16_val = int16_val;
    } else {
        throw_exception(SR_ERR_INVAL_ARG);
    }

    _val->type = type;
}
void Val::set(const char *xpath, int32_t int32_val, sr_type_t type) {
    if (_val == NULL) throw_exception(SR_ERR_OPERATION_FAILED);

    int ret = sr_val_set_xpath(_val, xpath);
    if (ret != SR_ERR_OK) throw_exception(ret);

    if (type == SR_INT32_T) {
	    _val->data.int32_val = int32_val;
    } else {
        throw_exception(SR_ERR_INVAL_ARG);
    }

    _val->type = type;
}

void Val::set(const char *xpath, int64_t int64_val, sr_type_t type) {
    if (_val == NULL) throw_exception(SR_ERR_OPERATION_FAILED);

    int ret = sr_val_set_xpath(_val, xpath);
    if (ret != SR_ERR_OK) throw_exception(ret);

    if (type == SR_UINT64_T) {
        _val->data.uint64_val = (uint64_t) int64_val;
    } else if (type == SR_UINT32_T) {
        _val->data.uint32_val = (uint32_t) int64_val;
    } else if (type == SR_UINT16_T) {
        _val->data.uint16_val = (uint16_t) int64_val;
    } else if (type == SR_UINT8_T) {
        _val->data.uint8_val = (uint8_t) int64_val;
    } else if (type == SR_INT64_T) {
        _val->data.int64_val = (int64_t) int64_val;
    } else if (type == SR_INT32_T) {
        _val->data.int32_val = (int32_t) int64_val;
    } else if (type == SR_INT16_T) {
        _val->data.int16_val = (int16_t) int64_val;
    } else if (type == SR_INT8_T) {
        _val->data.int8_val = (int8_t) int64_val;
    } else {
        throw_exception(SR_ERR_INVAL_ARG);
    }

    _val->type = type;
}
void Val::set(const char *xpath, uint8_t uint8_val, sr_type_t type) {
    if (_val == NULL) throw_exception(SR_ERR_OPERATION_FAILED);

    int ret = sr_val_set_xpath(_val, xpath);
    if (ret != SR_ERR_OK) throw_exception(ret);

    if (type == SR_UINT8_T) {
	    _val->data.uint8_val = uint8_val;
    } else {
        throw_exception(SR_ERR_INVAL_ARG);
    }

    _val->type = type;
}
void Val::set(const char *xpath, uint16_t uint16_val, sr_type_t type) {
    if (_val == NULL) throw_exception(SR_ERR_OPERATION_FAILED);

    int ret = sr_val_set_xpath(_val, xpath);
    if (ret != SR_ERR_OK) throw_exception(ret);

    if (type == SR_UINT16_T) {
	    _val->data.uint16_val = uint16_val;
    } else {
        throw_exception(SR_ERR_INVAL_ARG);
    }

    _val->type = type;
}
void Val::set(const char *xpath, uint32_t uint32_val, sr_type_t type) {
    if (_val == NULL) throw_exception(SR_ERR_OPERATION_FAILED);

    int ret = sr_val_set_xpath(_val, xpath);
    if (ret != SR_ERR_OK) throw_exception(ret);

    if (type == SR_UINT32_T) {
	    _val->data.uint32_val = uint32_val;
    } else {
        throw_exception(SR_ERR_INVAL_ARG);
    }

    _val->type = type;
}
void Val::set(const char *xpath, uint64_t uint64_val, sr_type_t type) {
    if (_val == NULL) throw_exception(SR_ERR_OPERATION_FAILED);

    int ret = sr_val_set_xpath(_val, xpath);
    if (ret != SR_ERR_OK) throw_exception(ret);

    if (type == SR_UINT64_T) {
	    _val->data.uint64_val = uint64_val;
    } else {
        throw_exception(SR_ERR_INVAL_ARG);
    }

    _val->type = type;
}
S_String Val::to_string() {
    char *mem = NULL;

    int ret = sr_print_val_mem(&mem, _val);
    if (SR_ERR_OK == ret) {
        if (mem == NULL)
            return NULL;
        S_String string_val = mem;
        free(mem);
        return string_val;
    } else if (SR_ERR_NOT_FOUND == ret) {
        return NULL;
    } else {
        throw_exception(ret);
        return NULL;
    }
}
S_String Val::val_to_string() {
    char *value = sr_val_to_str(_val);
    if (value == NULL) {
        throw_exception(SR_ERR_OPERATION_FAILED);
        return NULL;
    }
    S_String string_val = value;
    free(value);

    return string_val;
}

S_Val Val::dup() {
    sr_val_t *new_val = NULL;
    int ret = sr_dup_val(_val, &new_val);
    if (ret != SR_ERR_OK)
        throw_exception(ret);

    S_Deleter deleter(new Deleter(new_val));
    S_Val val(new Val(new_val, deleter));
    return val;
}

// Vals
Vals::Vals(const sr_val_t *vals, const size_t cnt, S_Deleter deleter) {
    _vals = (sr_val_t *) vals;
    _cnt = (size_t) cnt;

    _deleter = deleter;
}
Vals::Vals(sr_val_t **vals, size_t *cnt, S_Deleter deleter) {
    _vals = *vals;
    _cnt = *cnt;
    _deleter = deleter;
}
Vals::Vals(size_t cnt): Vals() {
    if (cnt) {
        int ret = sr_new_values(cnt, &_vals);
        if (ret != SR_ERR_OK)
            throw_exception(ret);

        _cnt = cnt;
        _deleter = S_Deleter(new Deleter(_vals, _cnt));
    }
}
Vals::Vals(): _cnt(0), _vals(nullptr) {}
Vals::~Vals() {}
S_Val Vals::val(size_t n) {
    if (n >= _cnt)
        throw std::out_of_range("Vals::val: index out of range");
    if (!_vals)
        throw std::logic_error("Vals::val: called on null Vals");

    S_Val val(new Val(&_vals[n], _deleter));
    return val;
}
S_Vals Vals::dup() {
    sr_val_t *new_val = NULL;
    int ret = sr_dup_values(_vals, _cnt, &new_val);
    if (ret != SR_ERR_OK)
        throw_exception(ret);

    S_Vals vals(new Vals(new_val, _cnt));
    return vals;
}

// Vals_Holder
Vals_Holder::Vals_Holder(sr_val_t **vals, size_t *cnt) {
    p_vals = vals;
    p_cnt = cnt;
    _allocate = true;
}
S_Vals Vals_Holder::allocate(size_t n) {
    if (_allocate == false)
        throw_exception(SR_ERR_DATA_EXISTS);
    _allocate = false;

    if (n == 0)
        return NULL;

    *p_cnt = n;
    int ret = sr_new_values(n, p_vals);
    if (ret != SR_ERR_OK)
        throw_exception(ret);
    S_Vals vals(new Vals(p_vals, p_cnt, NULL));
    return vals;
}
Vals_Holder::~Vals_Holder() {}

// Val_iter
Val_Iter::Val_Iter(sr_val_iter_t *iter) {_iter = iter;}
Val_Iter::~Val_Iter() {}

// Change_Iter
Change_Iter::Change_Iter(sr_change_iter_t *iter) {_iter = iter;}
Change_Iter::~Change_Iter() {}

// Error
Error::Error() {_info = NULL;}
Error::Error(const sr_error_info_t *info) {_info = info;}
Error::~Error() {}

// Errors
Errors::Errors() {_info = NULL; _cnt = 0;}
Errors::~Errors() {}
S_Error Errors::error(size_t n) {
    if (n >= _cnt)
        throw std::out_of_range("Errors:error: index out of range");

    S_Error error(new Error(&_info[n]));
    return error;
}

// Schema_Revision
Schema_Revision::Schema_Revision(sr_sch_revision_t rev) {_rev = rev;}
Schema_Revision::~Schema_Revision() {}

// Schema_Submodule
Schema_Submodule::Schema_Submodule(sr_sch_submodule_t sub, S_Deleter deleter) {
    _sub = sub;
    _deleter = deleter;
}
Schema_Submodule::~Schema_Submodule() {}
S_Schema_Revision Schema_Submodule::revision() {
    S_Schema_Revision rev(new Schema_Revision(_sub.revision));
    return rev;
}

// Yang_Schema
Yang_Schema::Yang_Schema(sr_schema_t *sch, S_Deleter deleter) {_sch = sch; _deleter = deleter;}
Yang_Schema::~Yang_Schema() {}
S_Schema_Revision Yang_Schema::revision() {
    S_Schema_Revision rev(new Schema_Revision(_sch->revision));
    return rev;
}
S_Schema_Submodule Yang_Schema::submodule(size_t n) {
    if (n >= _sch->submodule_count)
        throw std::out_of_range("Schema_Submodule::submodule: index out of range");

    S_Schema_Submodule sub(new Schema_Submodule(_sch->submodules[n], _deleter));
    return sub;
}
char *Yang_Schema::enabled_features(size_t n) {
    if (n >= _sch->enabled_feature_cnt)
        throw std::out_of_range("Yang_Schema::enabled_features: index out of range");

   return _sch->enabled_features[n];
}

// Yang_Schemas
Yang_Schemas::Yang_Schemas() {
    _sch = NULL;
    _cnt = 0;
    _deleter = S_Deleter(new Deleter(_sch, _cnt));
}
Yang_Schemas::~Yang_Schemas() {}
S_Yang_Schema Yang_Schemas::schema(size_t n) {
    if (n >= _cnt)
        throw std::out_of_range("Yang_Schema::schema: index out of range");

    S_Yang_Schema rev(new Yang_Schema((sr_schema_t *) &_sch[n], _deleter));
    return rev;
}

// Fd_Change
Fd_Change::Fd_Change(sr_fd_change_t *ch) {_ch = ch;}
Fd_Change::~Fd_Change() {}

// Fd_Changes
Fd_Changes::Fd_Changes(sr_fd_change_t *ch, size_t cnt) {_ch = ch; _cnt = cnt;}
Fd_Changes::~Fd_Changes() {}
S_Fd_Change Fd_Changes::fd_change(size_t n) {
    if (n >= _cnt)
        throw std::out_of_range("Fd_Changes::fd_change: index out of range");

    S_Fd_Change change(new Fd_Change(&_ch[n]));
    return change;
}

Iter_Value::Iter_Value(sr_val_iter_t *iter) {_iter = iter;}
Iter_Value::~Iter_Value() {if (_iter) sr_free_val_iter(_iter);}
void Iter_Value::Set(sr_val_iter_t *iter) {
    if (_iter)
        sr_free_val_iter(_iter);
    _iter = iter;
}

Iter_Change::Iter_Change(sr_change_iter_t *iter) {_iter = iter;}
Iter_Change::~Iter_Change() {if (_iter) sr_free_change_iter(_iter);}

Change::Change() {
    _oper = SR_OP_CREATED;
    _new = NULL;
    _old = NULL;

    _deleter_old = S_Deleter(new Deleter(_old));
    _deleter_new = S_Deleter(new Deleter(_new));
}
S_Val Change::new_val() {
    if (_new == NULL) return NULL;

    S_Val new_val(new Val(_new, _deleter_new));
    return new_val;
}
S_Val Change::old_val() {
    if (_old == NULL) return NULL;

    S_Val old_val(new Val(_old, _deleter_old));
    return old_val;
}
Change::~Change() {
    if (_new)
        sr_free_val(_new);
    if (_old)
        sr_free_val(_old);
}
