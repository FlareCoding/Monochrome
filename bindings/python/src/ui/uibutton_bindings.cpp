#include "uibutton_bindings.h"

// ====================== INIT ====================== //

PyObject* UIButtonObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UIButtonObject* self = (UIButtonObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    self->_base.handle = MakeRef<UIButton>(Frame(70, 80, 200, 50));
    return (PyObject*)self;
}

int UIButtonObject_Init(UIButtonObject* self, PyObject* args, PyObject* kwds)
{
    // super.init
    UIViewObject_GetType().tp_init((PyObject*)self, args, kwds);

    // Label object initialization
    UILabelObject* obj = (UILabelObject*)PyObject_CallObject((PyObject*)&UILabelObject_GetType(), NULL);
    obj->_base.handle = std::dynamic_pointer_cast<UIButton>(self->_base.handle)->Label;
    self->Label = obj;

    return 0;
}

// ====================== GETTERS/SETTERS ====================== //

PyObject* UIButtonObject_GetLabel(UIButtonObject* self, void* closure)
{
    Py_INCREF(self->Label);
    return (PyObject*)self->Label;
}

static PyGetSetDef UIButtonObjectGettersSetters[] = {
    { "Label", (getter)UIButtonObject_GetLabel, (setter)NULL, "Label", NULL},
    { NULL, NULL, 0, NULL }
};

// ====================== METHODS ====================== //

static PyMethodDef UIButtonObjectMethods[] = {
    { NULL, NULL, 0, NULL }
};

PyTypeObject UIButtonObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "UIButton",                 /*tp_name*/
    sizeof(UIButtonObject),     /*tp_basicsize*/
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
    UIButtonObjectMethods,      /*tp_methods*/
    0,                          /*tp_members*/
    UIButtonObjectGettersSetters,/*tp_getset*/
    &UIViewObject_GetType(),    /*tp_base*/
    0,                          /*tp_dict*/
    0,                          /*tp_descr_get*/
    0,                          /*tp_descr_set*/
    0,                          /*tp_dictoffset*/
    (initproc)UIButtonObject_Init,/*tp_init*/
    0,                          /*tp_alloc*/
    UIButtonObject_New,   /*tp_new*/
    0,                          /*tp_free*/
    0,                          /*tp_is_gc*/
};

PyTypeObject& UIButtonObject_GetType()
{
    return UIButtonObject_Type;
}
