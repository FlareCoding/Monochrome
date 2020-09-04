#include "bindings_uiwindow.h"

PyTypeObject& UIWindowObject_GetType()
{
    return UIWindowObject_Type;
}

PyObject* UIWindowObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UIWindowObject* self = (UIWindowObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    uint32_t width = 1080, height = 720;
    char* title = (char*)"Monochrome Window";
    if (!PyArg_ParseTuple(args, "|iis", &width, &height, &title))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter. (Parameters are: width, height, title)");
        return NULL;
    }

    self->handle = UIWindow::Create(WindowStyle::Modern, width, height, title);
    return (PyObject*)self;
}

int UIWindowObject_Init(UIWindowObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

PyObject* UIWindowObject_StartWindowLoop(UIWindowObject* self, PyObject* args)
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

//================================= //
//============ Getters ============ //
//================================= //

//================================= //
//============ Setters ============ //
//================================= //
