#include "uilabel_bindings.h"

// ====================== INIT ====================== //

PyObject* UILabelObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UILabelObject* self = (UILabelObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    self->_base.handle = MakeRef<UILabel>(Frame(70, 80, 200, 50));
    return (PyObject*)self;
}

int UILabelObject_Init(UILabelObject* self, PyObject* args, PyObject* kwds)
{
    // super.init
    UIViewObject_GetType().tp_init((PyObject*)self, args, kwds);
    return 0;
}

// ====================== GETTERS/SETTERS ====================== //

PyObject* UILabelObject_GetText(UILabelObject* self, void* closure)
{
    return Py_BuildValue("s", std::dynamic_pointer_cast<UILabel>(self->_base.handle)->Text);
}

int UILabelObject_SetText(UILabelObject* self, PyObject* value, void* closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute");
        return -1;
    }
    if (!PyUnicode_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
            "The attribute value must be a string");
        return -1;
    }

    PyObject* str = PyUnicode_AsEncodedString(value, "utf-8", "~E~");

    std::dynamic_pointer_cast<UILabel>(self->_base.handle)->Text = PyBytes_AS_STRING(str);

    Py_XDECREF(str);
    return 0;
}

static PyGetSetDef UILabelObjectGettersSetters[] = {
    { "text", (getter)UILabelObject_GetText, (setter)UILabelObject_SetText, "Text", NULL},
    { NULL, NULL, 0, NULL }
};

// ====================== METHODS ====================== //

static PyMethodDef UILabelObjectMethods[] = {
    { NULL, NULL, 0, NULL }
};

PyTypeObject UILabelObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "UILabel",                  /*tp_name*/
    sizeof(UILabelObject),      /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    0,                          /*tp_dealloc*/
    0,                          /*tp_vectorcall_offset*/
    0,                          /*tp_getattr*/
    0,                          /*tp_setattr*/
    0,                          /*tp_as_async*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash*/
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    0,                          /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    0,                          /*tp_doc*/
    0,                          /*tp_traverse*/
    0,                          /*tp_clear*/
    0,                          /*tp_richcompare*/
    0,                          /*tp_weaklistoffset*/
    0,                          /*tp_iter*/
    0,                          /*tp_iternext*/
    UILabelObjectMethods,       /*tp_methods*/
    0,                          /*tp_members*/
    UILabelObjectGettersSetters,/*tp_getset*/
    &UIViewObject_GetType(),    /*tp_base*/
    0,                          /*tp_dict*/
    0,                          /*tp_descr_get*/
    0,                          /*tp_descr_set*/
    0,                          /*tp_dictoffset*/
    (initproc)UILabelObject_Init,/*tp_init*/
    0,                          /*tp_alloc*/
    UILabelObject_New,          /*tp_new*/
    0,                          /*tp_free*/
    0,                          /*tp_is_gc*/
};

PyTypeObject& UILabelObject_GetType()
{
    return UILabelObject_Type;
}

