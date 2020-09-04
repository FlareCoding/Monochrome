import re

def to_snake_case(string):
    return re.sub(r'(?<!^)(?=[A-Z])', '_', string).lower()

def get_unique_items_from_list(l1, l2):
    return list((set(l1) | set(l2)) - (set(l1) & set(l2)))

class bindings_generator:
    def __init__(self, ct_name, bindings_name):
        self.bindings_name = bindings_name
        self.ct_name = ct_name
        self.ct_data = {}
        self.ct_data['type_info'] = {}
        self.ct_data['methods'] = []
        self.ct_data['getters'] = []
        self.ct_data['setters'] = []
        self.reading_mode = 'global'
        self.header_code = ''
        self.source_code = ''
    
    def generate_bindings(self):
        self.__read_ct_file(self.ct_name)
        self.__generate_header_code()
        self.__generate_source_code()

        with open('bindings_{}.h'.format(self.bindings_name).lower(), 'w') as h:
            h.write(self.header_code)

        with open('bindings_{}.cpp'.format(self.bindings_name).lower(), 'w') as cpp:
            cpp.write(self.source_code)

    def __read_ct_file(self, filename):
        with open(filename, 'r') as file:
            lines = file.readlines()
            for line in lines:
                self.__parse_line(line)

    def __parse_line(self, line):   
        if len(line.split()) == 0:
            return

        line = line.replace('\n', '')

        if self.reading_mode != 'global':
            if line.split()[0] == '@class_methods_end':
                self.reading_mode = 'global'
                return
            elif line.split()[0] == '@class_getters_end':
                self.reading_mode = 'global'
                return
            elif line.split()[0] == '@class_setters_end':
                self.reading_mode = 'global'
                return

            if self.reading_mode == 'methods':
                entry = '{0}_{1}'.format(self.ct_data['type_info']['object_name'], line)
                self.ct_data['methods'].append(entry)
            elif self.reading_mode == 'getters':
                entry = '{0}_Get{1}'.format(self.ct_data['type_info']['object_name'], line)
                self.ct_data['getters'].append(entry)
            elif self.reading_mode == 'setters':
                entry = '{0}_Set{1}'.format(self.ct_data['type_info']['object_name'], line)
                self.ct_data['setters'].append(entry)
            
        else:
            entry = line.split()

            if entry[0] == '@class':
                self.ct_data['type_info']['object_name'] = '{}Object'.format(entry[1])
                return
            elif entry[0] == '@subclass_of':
                subclass_name = entry[1]
                if subclass_name != 'null':
                    self.ct_data['type_info']['tp_base'] = '&{}Object_Type'.format(subclass_name)
                return
            elif entry[0] == '@class_methods_start':
                self.reading_mode = 'methods'
                return
            elif entry[0] == '@class_getters_start':
                self.reading_mode = 'getters'
                return
            elif entry[0] == '@class_setters_start':
                self.reading_mode = 'setters'
                return

    def __generate_header_code(self):
        self.header_code += self.__generate_includes_code() + '\n'
        self.header_code += self.__generate_object_structure() + '\n'

        # newproc
        self.header_code += 'PyObject* {}_New(PyTypeObject* type, PyObject* args, PyObject* kwds);\n'.format(self.ct_data['type_info']['object_name'])
        
        # initproc
        self.header_code += 'int {0}_Init({0}* type, PyObject* args, PyObject* kwds);\n\n'.format(self.ct_data['type_info']['object_name'])

        if len(self.ct_data['methods']) > 0:
            self.header_code += '//============ Methods ============ //\n'
            self.header_code += self.__generate_class_method_declarations() + '\n'
            self.header_code += self.__generate_py_method_def() + '\n'

        if len(self.ct_data['getters']) > 0:
            self.header_code += '//============ Getters ============ //\n'
            self.header_code += self.__generate_class_getter_declarations() + '\n'

        if len(self.ct_data['setters']) > 0:
            self.header_code += '//============ Setters ============ //\n'
            self.header_code += self.__generate_class_setter_declarations() + '\n'

        if (len(self.ct_data['getters']) > 0 or len(self.ct_data['setters']) > 0):
            self.header_code += self.__generate_py_getset_def() + '\n'

        self.header_code += self.__generate_py_type_object_struct() + '\n'
        self.header_code += 'PyTypeObject& {}_GetType();'.format(self.ct_data['type_info']['object_name'])

    def __generate_source_code(self):
        self.source_code += '#include "bindings_{}.h"\n\n'.format(self.bindings_name).lower()
        
        self.source_code += 'PyTypeObject& {}_GetType()\n{{\n'.format(self.ct_data['type_info']['object_name'])
        self.source_code += '\treturn {}_Type;\n}}\n\n'.format(self.ct_data['type_info']['object_name'])
        
        self.source_code += self.__generate_newproc_definition() + '\n'
        self.source_code += self.__generate_initproc_definition() + '\n'
        self.source_code += '//================================= //\n'
        self.source_code += '//============ Methods ============ //\n'
        self.source_code += '//================================= //\n\n'

        self.source_code += self.__generate_class_method_definitions()

        self.source_code += '//================================= //\n'
        self.source_code += '//============ Getters ============ //\n'
        self.source_code += '//================================= //\n\n'

        self.source_code += self.__generate_class_getters_definitions()

        self.source_code += '//================================= //\n'
        self.source_code += '//============ Setters ============ //\n'
        self.source_code += '//================================= //\n\n'

        self.source_code += self.__generate_class_setters_definitions()

#============== HEADER ==============#

    def __generate_includes_code(self):
        code = '#pragma once\n'
        code += '#include <Python.h>\n'
        code += '#include <Monochrome.h>\n'
        code += 'using namespace mc;\n'
        return code

    def __generate_object_structure(self):
        code = 'typedef struct _{} {{\n'.format(self.ct_data['type_info']['object_name'])
        if 'tp_base' in self.ct_data['type_info']:
            code += '\t{} _super;\n'.format(self.ct_data['type_info']['tp_base'].replace('&', '').replace('_Type', ''))
        else:
            code += '\tPyObject_HEAD\n'
        
        code += '}} {};\n'.format(self.ct_data['type_info']['object_name'])
        return code

    def __generate_class_method_declarations(self):
        code = ''
        for method in self.ct_data['methods']:
            code += 'PyObject* {0}({1}* self, PyObject* args);\n'.format(method, self.ct_data['type_info']['object_name'])

        return code

    def __generate_py_method_def(self):
        code = 'static PyMethodDef {}Methods[] = {{\n'.format(self.ct_data['type_info']['object_name'])
        for method in self.ct_data['methods']:
            export_name = method.replace('{}_'.format(self.ct_data['type_info']['object_name']), '')
            code += '\t{{ "{0}", (PyCFunction){1}, METH_VARARGS, "{0}" }},\n'.format(to_snake_case(export_name), method)

        code += '\t{ NULL, NULL, 0, NULL }\n'
        code += '};\n'
        return code

    def __generate_class_getter_declarations(self):
        code = ''
        for getter in self.ct_data['getters']:
            code += 'PyObject* {0}({1}* self, void* closure);\n'.format(getter, self.ct_data['type_info']['object_name'])

        return code

    def __generate_class_setter_declarations(self):
        code = ''
        for getter in self.ct_data['setters']:
            code += 'int {0}({1}* self, PyObject* value, void* closure);\n'.format(getter, self.ct_data['type_info']['object_name'])

        return code

    def __generate_py_getset_def(self):
        code = 'static PyGetSetDef {}GettersSetters[] = {{\n'.format(self.ct_data['type_info']['object_name'])

        items = self.ct_data['getters']
        for setter in self.ct_data['setters']:
            if setter.replace('_Set', '_Get') not in items:
                items.append(setter)

        for item in items:
            text = item.replace('_Get', '').replace('_Set', '').replace(self.ct_data['type_info']['object_name'], '')
            getter = '0'
            matchers = [text]
            matching = [s for s in self.ct_data['getters'] if any(xs in s for xs in matchers)]
            if len(matching) > 0:
                getter = matching[0]

            setter = '0'
            matchers = [text]
            matching = [s for s in self.ct_data['setters'] if any(xs in s for xs in matchers)]
            if len(matching) > 0:
                setter = matching[0]    

            code += '\t{{ "{0}", (getter){1}, (setter){2}, "{0}", NULL }},\n'.format(to_snake_case(text), getter, setter)

        code += '\t{ NULL, NULL, 0, NULL }\n'
        code += '};\n'
        return code

    def __generate_py_type_object_struct(self):
        object_name = self.ct_data['type_info']['object_name']

        new_proc = '{}_New'.format(object_name)
        init_proc = '{}_Init'.format(object_name)

        methods = '0'
        if len(self.ct_data['methods']) > 0:
            methods = '{}Methods'.format(object_name)

        getset = '0'
        if (len(self.ct_data['getters']) > 0 or len(self.ct_data['setters']) > 0):
            getset = '{}GettersSetters'.format(object_name)

        base = '0'
        if 'tp_base' in self.ct_data['type_info']:
            base = self.ct_data['type_info']['tp_base']

        code = 'static PyTypeObject {}_Type = {{\n'.format(object_name)
        code += '\tPyVarObject_HEAD_INIT(NULL, 0)\n'
        code += '\t"{}", /*tp_name*/\n'.format(object_name.replace('Object', ''))
        code += '\tsizeof({}), /*tp_basicsize*/\n'.format(object_name)

        code += '\t'
        for _ in range(0, 16):
            code += '0, '
        code += '\n'

        code += '\tPy_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/\n'

        code += '\t'
        for _ in range(0, 7):
            code += '0, '
        code += '\n'

        code += '\t{}, /*tp_methods*/\n'.format(methods)
        code += '\t0, /*tp_members*/\n'
        code += '\t{}, /*tp_getset*/\n'.format(getset)
        code += '\t{}, /*tp_base*/\n'.format(base)

        code += '\t'
        for _ in range(0, 4):
            code += '0, '
        code += '\n'

        code += '\t(initproc){}, /*tp_init*/\n'.format(init_proc)
        code += '\t0, /*tp_alloc*/\n'
        code += '\t{}, /*tp_new*/\n'.format(new_proc)
        code += '\t0, /*tp_free*/\n'
        code += '\t0, /*tp_is_gc*/\n'

        code += '};\n'
        return code

#============== SOURCE ==============#

    def __generate_newproc_definition(self):
        object_name = self.ct_data['type_info']['object_name']
        code = 'PyObject* {}_New(PyTypeObject* type, PyObject* args, PyObject* kwds)\n'.format(object_name)
        code += '{'

        code += """
    {0}* self = ({0}*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;\n""".format(object_name)

        code += '}\n'
        return code

    def __generate_initproc_definition(self):
        object_name = self.ct_data['type_info']['object_name']
        code = 'int {0}_Init({0}* type, PyObject* args, PyObject* kwds)\n'.format(object_name)
        code += '{\n'
        code += '\treturn 0;\n'
        code += '}\n'
        return code

    def __generate_class_method_definitions(self):
        object_name = self.ct_data['type_info']['object_name']
        code = ''
        for method in self.ct_data['methods']:
            code += 'PyObject* {0}({1}* self, PyObject* args)\n'.format(method, object_name)
            code += '{\n'
            code += '\treturn Py_None;\n'
            code += '}\n\n'
        return code

    def __generate_class_getters_definitions(self):
        object_name = self.ct_data['type_info']['object_name']
        code = ''
        for getter in self.ct_data['getters']:
            code += 'PyObject* {0}({1}* self, void* closure)\n'.format(getter, object_name)
            code += '{\n'
            code += '\treturn Py_None;\n'
            code += '}\n\n'
        return code

    def __generate_class_setters_definitions(self):
        object_name = self.ct_data['type_info']['object_name']
        code = ''
        for setter in self.ct_data['setters']:
            code += 'int {0}({1}* self, PyObject* value, void* closure)\n'.format(setter, object_name)
            code += '{\n'
            code += '\treturn 0;\n'
            code += '}\n\n'
        return code
