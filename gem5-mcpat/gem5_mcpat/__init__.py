#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import absolute_import

__author__     = "Lluís Vilanova"
__copyright__  = "Copyright 2013-2015, Lluís Vilanova"
__license__    = "GPL version 3 or later"

__maintainer__ = "Lluís Vilanova"
__email__      = "vilanova@ac.upc.edu"


def _jinja_check():
    def _error():
        import sys
        print("This program requires jinja2 version 2.7 or later")
        sys.exit(1)

    try:
        import jinja2
    except ImportError:
        _error()
    version = tuple(int(i) for i in jinja2.__version__.split("."))
    if version < (2, 7):
        _error()
_jinja_check()


import collections
import copy
import json
from jinja2 import Environment, FileSystemLoader, StrictUndefined
import jinja2
import pydoc
import re
import sys


##################################################
# Current stats

class Stats (collections.MutableMapping):
    def __init__(self, data={}):
        assert isinstance(data, dict)
        self._data = copy.deepcopy(data)
        self._names = {}
        self.update_names()

    # mutable mapping

    def __delitem__(self, key):
        del self._data[key]
        self.update_names()

    def __getitem__(self, key):
        return self._data[key]

    def __iter__(self):
        return iter(self._data)

    def __len__(self):
        return len(self._data)

    def __setitem__(self, key, val):
        self._data[key] = val
        self.update_names()

    # updating

    def _do_update(self, path, data):
        def get_name(path):
            return ".".join(path)

        name = get_name(path)
        assert name not in self._names
        self._names[name] = data

        if isinstance(data, dict):
            for key, val in data.items():
                self._do_update(path+[key], val)

        elif isinstance(data, list):
            if len(data) == 0:
                pass
            elif len(data) == 1:
                del self._names[name]
                self._do_update(path, data[0])
            else:
                digits = len("%d" % len(data))
                fmt = "%%s%%0%dd" % digits
                for idx, ival in enumerate(data):
                    ipath = list(path)
                    ipath[-1] = fmt % (ipath[-1], idx)
                    self._do_update(ipath, ival)

    def update_names(self):
        # TODO: should ensure names match the 'path' variable (if exists)
        self._names = {}
        self._do_update([], self._data)

    # complex content addition

    def _merge(self, path, data):
        if isinstance(data, dict):
            assert isinstance(path, dict)
            for key, val in data.items():
                if key not in path:
                    path[key] = val
                else:
                    if isinstance(val, dict):
                        p = path[key]
                        if isinstance(p, list):
                            if len(p) != 1:
                                raise ValueError
                            p = p[0]
                        self._merge(p, val)
                    elif isinstance(val, list):
                        raise NotImplementedError
                    else:
                        raise NotImplementedError
        else:
            assert isinstance(data, list)
            assert isinstance(path, list)
            raise NotImplementedError

    def add_dict(self, data, update=True):
        assert isinstance(data, dict)
        self._merge(self._data, data)
        if update:
            self.update_names()

    # value addition

    def _add_value(self, stats, path_prev, path_next, data):
        assert len(path_next) > 0

        key = path_next[0]

        m = re.match("(?P<name>[^0-9]+)(?P<num>[0-9]+)?$", key)
        if m is None:
            key_name = key
            key_idx = None
        else:
            key_name = m.groupdict()["name"]
            key_idx = m.groupdict()["num"]
        if key_idx is not None:
            key_idx = int(key_idx)

        if len(path_next) == 1:
            if key in stats:
                if isinstance(stats[key], list):
                    # already exists, try to merge
                    if len(stats[key]) == 1 and not isinstance(data, list):
                        stats[key][0] = data
                    else:
                        stats[key] = data
                else:
                    stats[key] = data
            elif key_idx is None:
                stats[key] = data
            else:
                name = ".".join(path_prev + [path_next[0]])
                if name in self._names:
                    raise NotImplementedError
                else:
                    assert key_idx == 0, key_idx
                    stats[key] = data

        else:
            if key in stats:
                s = stats[key]
                if isinstance(s, list):
                    assert len(s) == 1
                    s = s[0]
                self._add_value(s,
                                path_prev+[key], path_next[1:],
                                data)
            elif key_name in stats:
                if key_idx >= len(stats[key_name]):
                    raise IndexError(key_idx)
                self._add_value(stats[key_name][key_idx],
                                path_prev+[key], path_next[1:],
                                data)
            elif key_idx is None:
                stats[key] = {}
                self._add_value(stats[key],
                                path_prev+[key], path_next[1:],
                                data)
            else:
                name = ".".join(path_prev + [path_next[0]])
                if name in self._names:
                    self._add_value(stats[key_name][key_idx],
                                    path_prev+[key], path_next[1:],
                                    data)
                else:
                    # do not convert into list
                    stats[key] = {}
                    self._add_value(stats[key],
                                    path_prev+[key], path_next[1:],
                                    data)

    def add_value(self, path, data, update=True):
        assert isinstance(path, list)
        # assert (isinstance(data, list) and isinstance(data[0], dict)) or \
        #     (not isinstance(data, list) and not isinstance(data, dict))
        self._add_value(self._data, [], path, data)
        if update:
            self.update_names()

    # value finding

    def get_regex(self, regex):
        for name, val in self._names.items():
            if regex.match(name):
                yield name, val

    # miscellaneous

    def copy(self):
        return Stats(self._data)

    def to_dict(self):
        return copy.deepcopy(self._data)

    def to_list(self):
        return [(key, val) for key, val in sorted(self._names.items())
                if not isinstance(val, list) and not isinstance(val, dict)]


def stats_add_json(stats, file_):
    data = json.load(file_)
    stats.add_dict(data)


def stats_set_variables(stats, variables):
    for name, val in variables:
        val = _convert_value(val)
        path = Name.parse(name, escape=True).parts
        stats.add_value(path, val)
    stats.update_names()


##################################################
# Miscellaneous

class NameSyntaxError (Exception):
    def __init__(self, name):
        Exception.__init__(self, "not a well-formed name: " + name)


class Name:
    def __init__(self, parts):
        regex = ""
        for part in parts[1:]:
            if regex == "":
                regex = "(\.%s)?" % part
            else:
                regex = "(\.%s%s)?" % (part, regex)
        regex = "(%s%s)$" % (parts[0], regex)
        self.cre = re.compile(regex)
        self.parts = parts

    def match(self, *args, **kwargs):
        m = self.cre.match(*args, **kwargs)
        if m is None:
            return None
        return m

    def __len__(self):
        return len(self.parts)

    def __getitem__(self, idx):
        return self.parts[idx]

    @staticmethod
    def parse(name, escape=False):
        if escape:
            name = name.replace(".", "\\.")
        path = re.split(r"\\\.", name)
        if path[0] == "" or path[-1] == "":
            raise NameSyntaxError(name)
        return Name(path)


##################################################
# GEM5 configuration

def gem5_config_load(stats, file_):
    stats_add_json(stats, file_)


##################################################
# GEM5 statistics

def _read_lines(file_):
    begin = False
    lines = file_.readlines()
    line_idx = 0
    while line_idx < len(lines):
        line = lines[line_idx]
        comment_idx = line.find("#")
        line = line[:comment_idx]

        line_idx += 1
        if "Begin Simulation Statistics" in line:
            begin = True
            continue
        if not begin:
            continue
        if "End Simulation Statistics" in line:
            return
        if line == "":
            continue

        action = yield line
        if action is not None:
            line_idx += action


def _parse_line(line):
    line = line.split()
    assert len(line) >= 2

    line[0] = [part.split(".")
               for part in line[0].split("::")]

    name = line[0][0]
    field = line[0][1] if len(line[0]) == 2 else None
    assert len(line[0]) <= 2
    values = line[1:]
    if len(values) > 1 and values[0] == "|": # buckets
        field = ["buckets"]
        bucket_values = []
        for value in values:
            if value == "|":
                bucket_values.append([])
            else:
                bucket_values[-1].append(value)
        values = bucket_values

    return name, field, values


def _convert_value(value):
    try:
        return int(value)
    except ValueError:
        pass
    try:
        return float(value)
    except ValueError:
        pass
    return value


def _get_value(value):
    if len(value) == 1:                 # plain
        return _convert_value(value[0])
    elif len(value) == 3:               # distribution
        assert value[1][-1] == "%"
        assert value[2][-1] == "%"
        return {"count": _convert_value(value[0]),
                "percentage": _convert_value(value[1][:-1]),
                "cumulative": _convert_value(value[2][:-1])}
    else:
        assert len(value) > 0
        if len(value[0]) > 1:           # histogram
            result = []
            for bucket in value:
                assert bucket[1][-1] == "%"
                assert bucket[2][-1] == "%"
                result.append({"count": _convert_value(bucket[0]),
                               "percentage": _convert_value(bucket[1][:-1]),
                               "cumulative": _convert_value(bucket[2][:-1])})
            return result

        else:                           # vector
            return [_convert_value(j) for i in value for j in i]


def _process_line(stats, line, lines_iter):
    name, field, values = _parse_line(line)
    name = Name(name)

    def _set_value(stats, path, value):
        value = _get_value(value)
        if isinstance(value, dict):
            for k, v in value.items():
                stats.add_value(path + [k], v, update=False)
        else:
            stats.add_value(path, value, update=False)

    if field is not None:
        # get all fields
        fields = [field]
        values = [values]
        for line in lines_iter:
            lname, lfield, lvalues = _parse_line(line)
            if lname != name:
                lines_iter.send(-2)
                break

            fields.append(lfield)
            values.append(lvalues)

        for lname, lvalues in zip(fields, values):
            _set_value(stats, name.parts+lname, lvalues)

    else:
        _set_value(stats, name.parts, values)


def gem5_stats_load(stats, file_):
    lines_iter = _read_lines(file_)
    for line in lines_iter:
        try:
            _process_line(stats, line, lines_iter)
        except:
            print("Error while parsing line: %r\n" % line, file=sys.stderr)
            raise
    stats.update_names()


##################################################
# McPAT templates

class AssertError (Exception):
    def __init__(self, message):
        Exception.__init__(self, "failed assert" +\
                           ((": "+message) if message != "" else ""))


def _template_funcs(stats):
    def assert_func(val, message=""):
        """Ensure 'val' is true."""
        if not val:
            raise AssertError(message)
        return ""
    assert_func.__name__ = "assert"

    def find(regex):
        """Return a list of the elements that match the name in 'regex'.

        Argument '--show-text' lists available names.
        """
        return [val for _, val in stats.get_regex(re.compile(regex))]

    def log(x, base=10):
        import math
        return math.log(x, base)

    return {
        "assert": assert_func,
        "find": find,
        "log": log,
    }


def _template_filters(stats):

    def sdefault(arg, *args):
        """Default value when dereferencing a sequence of fields.

        This is similar to jinja's `default` filter, except that the default
        value will be returnes if any element of the given sequence does not
        exist.

        The following codes are equivalent::
            ( ( elem1|default(0) ).elem2|default(0) ).elem3|default(0)
            elem1|sdefault('elem2', 'elem3', 0)

        """
        if len(args) < 1:
            raise AssertError("Need at least a default value")
        default = args[-1]
        elements = args[:-1]
        var = jinja2.filters.do_default(value, default)
        for elem in elements:
            if not isinstance(var, dict) or elem not in var:
                return default
            var = var[elem]
        return var

    def lmatch_has(arg, var):
        """Filter input list to elements that have variable 'var'."""
        return [a for a in arg
                if isinstance(a, dict) and var in a]

    def lmatch_eq(arg, var, regex):
        """Filter input list to elements that have variable 'var' with a value matching
  'regex'.

        """
        return [a for a in lmatch_has(arg, var)
                if re.match(regex, str(a[var]))]

    def lmatch_ne(arg, var, regex):
        """Filter input list to elements that have variable 'var' with a value not
        matching 'regex'.

        """
        return [a for a in lmatch_has(arg, var)
                if not re.match(regex, str(a[var]))]

    return dict(
        sdefault=sdefault,
        lmatch_has=lmatch_has,
        lmatch_eq=lmatch_eq,
        lmatch_ne=lmatch_ne,
    )


def template_load(search_paths, name, stats):
    env = Environment(loader=FileSystemLoader(search_paths),
                      extensions=[
                          "jinja2.ext.with_",
                          "jinja2.ext.do",
                      ],
                      undefined=StrictUndefined,
                      trim_blocks=True, lstrip_blocks=True)
    for var, val in _template_funcs(stats).items():
        assert var not in env.globals
        env.globals[var] = val
    for var, val in _template_filters(stats).items():
        assert var not in env.filters
        env.filters[var] = val
    template = env.get_template(name, globals=stats)
    return template.render()


def template_doc():
    def doc(elems):
        return [pydoc.text.document(fobj)
                for _, fobj in elems.items()]

    def title(name):
        return [pydoc.text.bold(name), "-"*len(name), ""]

    return \
        title("Functions:") +\
        doc(_template_funcs(None)) +\
        title("Filters:") +\
        doc(_template_filters(None))
