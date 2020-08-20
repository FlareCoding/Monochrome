#include "uiwindow_bindings.h"

PyObject* UIWindowObject_StartWindowLoop(UIWindowObject* self, PyObject* Py_UNUSED(ignored))
{
    if (!self->handle)
    {
        PyErr_SetString(PyExc_Exception, "Window was not created.");
        return NULL;
    }

    self->handle->StartWindowLoop();
    return Py_None;
}

PyObject* UIWindowObject_AddView(UIWindowObject* self, PyObject* args)
{
    UIViewObject* view;

    if (!PyArg_ParseTuple(args, "O", &view))
        return NULL;

    self->handle->AddView(view->handle);
    return Py_None;
}

PyObject* UIWindowObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UIWindowObject* self = (UIWindowObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    uint32_t width = 1080, height = 720;
    char* title = "Monochrome Window";
    if (!PyArg_ParseTuple(args, "|iis", &width, &height, &title))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter. (Parameters are: width, height, title)");
        return NULL;
    }

    self->handle = UIWindow::Create(WindowStyle::Modern, width, height, title);
    return (PyObject*)self;
}

static PyMethodDef UIWindowObjectMethods[] = {
    { "StartWindowLoop",    (PyCFunction)UIWindowObject_StartWindowLoop,    METH_NOARGS,    "Starts the window updating loop." },
    { "AddView",            (PyCFunction)UIWindowObject_AddView,            METH_VARARGS,   "Adds a view to the window's view hierarchy." },
    { NULL, NULL, 0, NULL }
};

PyTypeObject UIWindowObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "UIWindow",                 /*tp_name*/
    0,                          /*tp_basicsize*/
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
    UIWindowObjectMethods,      /*tp_methods*/
    0,                          /*tp_members*/
    0,                          /*tp_getset*/
    0, /* see PyInit_xx */      /*tp_base*/
    0,                          /*tp_dict*/
    0,                          /*tp_descr_get*/
    0,                          /*tp_descr_set*/
    0,                          /*tp_dictoffset*/
    0,                          /*tp_init*/
    0,                          /*tp_alloc*/
    UIWindowObject_New,         /*tp_new*/
    0,                          /*tp_free*/
    0,                          /*tp_is_gc*/
};

PyTypeObject& UIWindowObject_GetType()
{
    return UIWindowObject_Type;
}
