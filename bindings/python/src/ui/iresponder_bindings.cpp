#include "iresponder_bindings.h"
#include "uiview_bindings.h"

// ====================== METHODS ====================== //

PyObject* IResponder_AddMouseClickedEventHandler(IResponderObject* self, PyObject* args)
{
    PyObject* callback;

    if (!PyArg_ParseTuple(args, "O:MouseClickedEventHandler", &callback))
        return NULL;

    if (!PyCallable_Check(callback))
    {
        PyErr_SetString(PyExc_TypeError, "parameter must be callable");
        return NULL;
    }

    Py_XINCREF(callback);
    Py_INCREF(Py_None);

    if (self->handle)
    {
        self->handle->AddEventHandler<EventType::MouseButtonClicked>([callback, self](Event& e, UIView* sender) -> bool {
            PyObject* result = PyEval_CallObject(callback, NULL);
            if (!result)
            {
                PyErr_SetString(PyExc_Exception, "Error calling event handler");
                PyErr_Occurred();
            }

            Py_XDECREF(result);
            return EVENT_HANDLED;
        });
    }

    return Py_None;
}

static PyMethodDef IResponderObjectMethods[] = {
    { "AddMouseClickedEventHandler", (PyCFunction)IResponder_AddMouseClickedEventHandler, METH_VARARGS, "Adds an event handler for MouseClickEvent." },
    { NULL, NULL, 0, NULL }
};

PyTypeObject IResponderObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "IResponder",               /*tp_name*/
    sizeof(IResponderObject),   /*tp_basicsize*/
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
    IResponderObjectMethods,    /*tp_methods*/
    0,                          /*tp_members*/
    0,                          /*tp_getset*/
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

PyTypeObject& IResponderObject_GetType()
{
    return IResponderObject_Type;
}
