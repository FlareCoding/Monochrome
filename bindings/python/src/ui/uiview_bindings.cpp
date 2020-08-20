#include "uiview_bindings.h"

// ====================== INIT ====================== //

PyObject* UIViewObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UIViewObject* self = (UIViewObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    self->handle = MakeRef<UIView>(Frame(40, 80, 260, 200));

    return (PyObject*)self;
}

int UIViewObject_Init(UIViewObject* self, PyObject* args, PyObject* kwds)
{
    self->_responder_base.handle = self->handle;

    self->layer = (LayerObject*)PyObject_CallObject((PyObject*)&LayerObject_GetType(), NULL);
    self->layer->handle = Ref<Layer>(&self->handle->layer);
    return 0;
}

// ====================== GETTERS/SETTERS ====================== //

PyObject* UIViewObject_GetLayer(UIViewObject* self, void* closure)
{
    Py_INCREF(self->layer);
    return (PyObject*)self->layer;
}

static PyGetSetDef UIViewObjectGettersSetters[] = {
    { "layer", (getter)UIViewObject_GetLayer, (setter)NULL, "Layer", NULL },
    { NULL, NULL, 0, NULL }
};

// ====================== METHODS ====================== //

static PyMethodDef UIViewObjectMethods[] = {
    { NULL, NULL, 0, NULL }
};

PyTypeObject UIViewObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "UIView",                   /*tp_name*/
    sizeof(UIViewObject),       /*tp_basicsize*/
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
    UIViewObjectMethods,        /*tp_methods*/
    0,                          /*tp_members*/
    UIViewObjectGettersSetters, /*tp_getset*/
    &IResponderObject_GetType(),/*tp_base*/
    0,                          /*tp_dict*/
    0,                          /*tp_descr_get*/
    0,                          /*tp_descr_set*/
    0,                          /*tp_dictoffset*/
    (initproc)UIViewObject_Init,/*tp_init*/
    0,                          /*tp_alloc*/
    UIViewObject_New,           /*tp_new*/
    0,                          /*tp_free*/
    0,                          /*tp_is_gc*/
};

PyTypeObject& UIViewObject_GetType()
{
    return UIViewObject_Type;
}

// ============================================================================= //
// ============================= LAYER OBJECT ================================== //
// ============================================================================= //

PyObject* LayerObject_GetPosition(LayerObject* self, void* closure)
{
    if (!self->handle) {
        PyErr_SetString(PyExc_Exception, "Layer doesn't have a valid native handle");
        return NULL;
    }

    Position pos = self->handle->frame.position;
    return Py_BuildValue("(f, f)", pos.x, pos.y);
}

int LayerObject_SetPosition(LayerObject* self, PyObject* value, void* closure)
{
    if (!self->handle) {
        PyErr_SetString(PyExc_Exception, "Layer doesn't have a valid native handle");
        return -1;
    }

    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute");
        return -1;
    }

    if (!PyTuple_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The attribute value must be a tuple");
        return -1;
    }

    PyObject* xpos = PyTuple_GetItem(value, 0);
    PyObject* ypos = PyTuple_GetItem(value, 1);

    if (!xpos || !ypos) {
        PyErr_SetString(PyExc_Exception, "Invalid tuple item");
        return -1;
    }

    Position pos;
    pos.x = (float)PyFloat_AsDouble(xpos);
    pos.y = (float)PyFloat_AsDouble(ypos);

    self->handle->frame.position = pos;
    return 0;
}

PyObject* LayerObject_GetSize(LayerObject* self, void* closure)
{
    if (!self->handle) {
        PyErr_SetString(PyExc_Exception, "Layer doesn't have a valid native handle");
        return NULL;
    }

    Size size = self->handle->frame.size;
    return Py_BuildValue("(f, f)", size.width, size.height);
}

int LayerObject_SetSize(LayerObject* self, PyObject* value, void* closure)
{
    if (!self->handle) {
        PyErr_SetString(PyExc_Exception, "Layer doesn't have a valid native handle");
        return -1;
    }

    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute");
        return -1;
    }

    if (!PyTuple_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The attribute value must be a tuple");
        return -1;
    }

    PyObject* width = PyTuple_GetItem(value, 0);
    PyObject* height = PyTuple_GetItem(value, 1);

    if (!width || !height) {
        PyErr_SetString(PyExc_Exception, "Invalid tuple item");
        return -1;
    }

    Size size;
    size.width = (float)PyFloat_AsDouble(width);
    size.height = (float)PyFloat_AsDouble(height);

    self->handle->frame.size = size;
    return 0;
}

PyObject* LayerObject_GetColor(LayerObject* self, void* closure)
{
    if (!self->handle) {
        PyErr_SetString(PyExc_Exception, "Layer doesn't have a valid native handle");
        return NULL;
    }

    Color color = self->handle->color;
    return Py_BuildValue("(f, f)", color.r, color.g, color.b, color.alpha);
}

int LayerObject_SetColor(LayerObject* self, PyObject* value, void* closure)
{
    if (!self->handle) {
        PyErr_SetString(PyExc_Exception, "Layer doesn't have a valid native handle");
        return -1;
    }

    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete attribute");
        return -1;
    }

    if (!PyTuple_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "The attribute value must be a tuple");
        return -1;
    }

    PyObject* r = PyTuple_GetItem(value, 0);
    PyObject* g = PyTuple_GetItem(value, 1);
    PyObject* b = PyTuple_GetItem(value, 2);

    if (!r || !g || !b) {
        PyErr_SetString(PyExc_Exception, "Invalid tuple item");
        return -1;
    }

    Color color;
    color.r = (uint32_t)PyFloat_AsDouble(r);
    color.g = (uint32_t)PyFloat_AsDouble(g);
    color.b = (uint32_t)PyFloat_AsDouble(b);

    if (PyTuple_GET_SIZE(value) == 4)
    {
        PyObject* a = PyTuple_GetItem(value, 3);
        color.alpha = (float)PyFloat_AsDouble(a);
    }

    self->handle->color = color;
    return 0;
}

static PyGetSetDef LayerObjectGettersSetters[] = {
    { "position",   (getter)LayerObject_GetPosition,    (setter)LayerObject_SetPosition,    "Position", NULL },
    { "size",       (getter)LayerObject_GetSize,        (setter)LayerObject_SetSize,        "Size",     NULL },
    { "color",      (getter)LayerObject_GetColor,       (setter)LayerObject_SetColor,       "Color",    NULL },
    { NULL, NULL, 0, NULL }
};

PyTypeObject LayerObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Layer",                    /*tp_name*/
    sizeof(LayerObject),        /*tp_basicsize*/
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
    0,                          /*tp_methods*/
    0,                          /*tp_members*/
    LayerObjectGettersSetters,  /*tp_getset*/
    0,                          /*tp_base*/
    0,                          /*tp_dict*/
    0,                          /*tp_descr_get*/
    0,                          /*tp_descr_set*/
    0,                          /*tp_dictoffset*/
    0,                          /*tp_init*/
    0,                          /*tp_alloc*/
    0,                          /*tp_new*/
    0,                          /*tp_free*/
    0,                          /*tp_is_gc*/
};

PyTypeObject& LayerObject_GetType()
{
    return LayerObject_Type;
}
