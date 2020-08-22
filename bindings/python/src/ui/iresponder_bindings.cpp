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
            MouseButtonClickedEventObject* evt = (MouseButtonClickedEventObject*)PyObject_CallObject((PyObject*)&MouseButtonClickedEventObject_GetType(), NULL);
            evt->native_event.button = reinterpret_cast<MouseButtonClickedEvent&>(e).button;
            evt->native_event.location = reinterpret_cast<MouseButtonClickedEvent&>(e).location;

            PyObject* argslist = Py_BuildValue("(O)", evt);
            PyObject* result = PyEval_CallObject(callback, argslist);
            Py_DECREF(argslist);
            
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

PyObject* IResponder_AddMousePressedEventHandler(IResponderObject* self, PyObject* args)
{
    PyObject* callback;

    if (!PyArg_ParseTuple(args, "O:MousePressedEventHandler", &callback))
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
        self->handle->AddEventHandler<EventType::MouseButtonPressed>([callback, self](Event& e, UIView* sender) -> bool {
            MouseButtonPressedEventObject* evt = (MouseButtonPressedEventObject*)PyObject_CallObject((PyObject*)&MouseButtonPressedEventObject_GetType(), NULL);
            evt->native_event.button = reinterpret_cast<MouseButtonPressedEvent&>(e).button;
            evt->native_event.location = reinterpret_cast<MouseButtonPressedEvent&>(e).location;

            PyObject* argslist = Py_BuildValue("(O)", evt);
            PyObject* result = PyEval_CallObject(callback, argslist);
            Py_DECREF(argslist);

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

PyObject* IResponder_AddMouseHoverOnEventHandler(IResponderObject* self, PyObject* args)
{
    PyObject* callback;

    if (!PyArg_ParseTuple(args, "O:MouseHoverOnEventHandler", &callback))
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
        self->handle->AddEventHandler<EventType::MouseHoverOn>([callback, self](Event& e, UIView* sender) -> bool {
            MouseHoverOnEventObject* evt = (MouseHoverOnEventObject*)PyObject_CallObject((PyObject*)&MouseHoverOnEventObject_GetType(), NULL);
            evt->native_event.button = reinterpret_cast<MouseHoverOnEvent&>(e).button;
            evt->native_event.location = reinterpret_cast<MouseHoverOnEvent&>(e).location;
            evt->native_event.distance = reinterpret_cast<MouseHoverOnEvent&>(e).distance;

            PyObject* argslist = Py_BuildValue("(O)", evt);
            PyObject* result = PyEval_CallObject(callback, argslist);
            Py_DECREF(argslist);

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

PyObject* IResponder_AddMouseHoverOffEventHandler(IResponderObject* self, PyObject* args)
{
    PyObject* callback;

    if (!PyArg_ParseTuple(args, "O:MouseHoverOffEventHandler", &callback))
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
        self->handle->AddEventHandler<EventType::MouseHoverOff>([callback, self](Event& e, UIView* sender) -> bool {
            MouseHoverOffEventObject* evt = (MouseHoverOffEventObject*)PyObject_CallObject((PyObject*)&MouseHoverOffEventObject_GetType(), NULL);
            evt->native_event.button = reinterpret_cast<MouseHoverOffEvent&>(e).button;
            evt->native_event.location = reinterpret_cast<MouseHoverOffEvent&>(e).location;
            evt->native_event.distance = reinterpret_cast<MouseHoverOffEvent&>(e).distance;

            PyObject* argslist = Py_BuildValue("(O)", evt);
            PyObject* result = PyEval_CallObject(callback, argslist);
            Py_DECREF(argslist);

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

PyObject* IResponder_AddMouseMovedEventHandler(IResponderObject* self, PyObject* args)
{
    PyObject* callback;

    if (!PyArg_ParseTuple(args, "O:MouseMovedEventHandler", &callback))
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
        self->handle->AddEventHandler<EventType::MouseMoved>([callback, self](Event& e, UIView* sender) -> bool {
            MouseMovedEventObject* evt = (MouseMovedEventObject*)PyObject_CallObject((PyObject*)&MouseMovedEventObject_GetType(), NULL);
            evt->native_event.button = reinterpret_cast<MouseMovedEvent&>(e).button;
            evt->native_event.location = reinterpret_cast<MouseMovedEvent&>(e).location;
            evt->native_event.distance = reinterpret_cast<MouseMovedEvent&>(e).distance;

            PyObject* argslist = Py_BuildValue("(O)", evt);
            PyObject* result = PyEval_CallObject(callback, argslist);
            Py_DECREF(argslist);

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

PyObject* IResponder_AddMouseScrolledEventHandler(IResponderObject* self, PyObject* args)
{
    PyObject* callback;

    if (!PyArg_ParseTuple(args, "O:MouseScrolledEventHandler", &callback))
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
        self->handle->AddEventHandler<EventType::MouseScrolled>([callback, self](Event& e, UIView* sender) -> bool {
            MouseScrolledEventObject* evt = (MouseScrolledEventObject*)PyObject_CallObject((PyObject*)&MouseScrolledEventObject_GetType(), NULL);
            evt->native_event.location = reinterpret_cast<MouseScrolledEvent&>(e).location;
            evt->native_event.ScrollDistance = reinterpret_cast<MouseScrolledEvent&>(e).ScrollDistance;

            PyObject* argslist = Py_BuildValue("(O)", evt);
            PyObject* result = PyEval_CallObject(callback, argslist);
            Py_DECREF(argslist);

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

PyObject* IResponder_AddKeyPressedEventHandler(IResponderObject* self, PyObject* args)
{
    PyObject* callback;

    if (!PyArg_ParseTuple(args, "O:KeyPressedEventHandler", &callback))
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
        self->handle->AddEventHandler<EventType::KeyPressed>([callback, self](Event& e, UIView* sender) -> bool {
            KeyPressedEventObject* evt = (KeyPressedEventObject*)PyObject_CallObject((PyObject*)&KeyPressedEventObject_GetType(), NULL);
            evt->native_event.keycode = reinterpret_cast<KeyPressedEvent&>(e).keycode;
            evt->native_event.repeated = reinterpret_cast<KeyPressedEvent&>(e).repeated;
            evt->native_event.capital = reinterpret_cast<KeyPressedEvent&>(e).capital;
            evt->native_event.capslock_on = reinterpret_cast<KeyPressedEvent&>(e).capslock_on;

            PyObject* argslist = Py_BuildValue("(O)", evt);
            PyObject* result = PyEval_CallObject(callback, argslist);
            Py_DECREF(argslist);

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

PyObject* IResponder_AddKeyReleasedEventHandler(IResponderObject* self, PyObject* args)
{
    PyObject* callback;

    if (!PyArg_ParseTuple(args, "O:KeyReleasedEventHandler", &callback))
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
        self->handle->AddEventHandler<EventType::KeyReleased>([callback, self](Event& e, UIView* sender) -> bool {
            KeyReleasedEventObject* evt = (KeyReleasedEventObject*)PyObject_CallObject((PyObject*)&KeyReleasedEventObject_GetType(), NULL);
            evt->native_event.keycode = reinterpret_cast<KeyReleasedEvent&>(e).keycode;

            PyObject* argslist = Py_BuildValue("(O)", evt);
            PyObject* result = PyEval_CallObject(callback, argslist);
            Py_DECREF(argslist);

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
    { "AddMouseClickedEventHandler",    (PyCFunction)IResponder_AddMouseClickedEventHandler,    METH_VARARGS, "Adds an event handler for MouseClickedEvent." },
    { "AddMousePressedEventHandler",    (PyCFunction)IResponder_AddMousePressedEventHandler,    METH_VARARGS, "Adds an event handler for MousePressedEvent." },
    { "AddMouseHoverOnEventHandler",    (PyCFunction)IResponder_AddMouseHoverOnEventHandler,    METH_VARARGS, "Adds an event handler for MouseHoverOnEvent." },
    { "AddMouseHoverOffEventHandler",   (PyCFunction)IResponder_AddMouseHoverOffEventHandler,   METH_VARARGS, "Adds an event handler for MouseHoverOffEvent." },
    { "AddMouseMovedEventHandler",      (PyCFunction)IResponder_AddMouseMovedEventHandler,      METH_VARARGS, "Adds an event handler for MouseMovedEvent." },
    { "AddMouseScrolledEventHandler",   (PyCFunction)IResponder_AddMouseScrolledEventHandler,   METH_VARARGS, "Adds an event handler for MouseScrolledEvent." },
    { "AddKeyPressedEventHandler",      (PyCFunction)IResponder_AddKeyPressedEventHandler,      METH_VARARGS, "Adds an event handler for KeyPressedEvent." },
    { "AddKeyReleasedEventHandler",     (PyCFunction)IResponder_AddKeyReleasedEventHandler,     METH_VARARGS, "Adds an event handler for KeyReleasedEvent." },
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
