#include "bindings_uibutton.h"

PyTypeObject& UIButtonObject_GetType()
{
	return UIButtonObject_Type;
}

PyObject* UIButtonObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UIButtonObject* self = (UIButtonObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<UIButton>((UIButton*)native_ptr);
	else
		self->handle = MakeRef<UIButton>(Frame(70, 80, 180, 40));

	self->_super.handle = self->handle;
    return (PyObject*)self;
}

int UIButtonObject_Init(UIButtonObject* type, PyObject* args, PyObject* kwds)
{
	UIViewObject_GetType().tp_init((PyObject*)type, args, kwds);
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

//================================= //
//============ Getters ============ //
//================================= //

PyObject* UIButtonObject_GetFilled(UIButtonObject* self, void* closure)
{
	return (self->handle->Filled) ? Py_True : Py_False;
}

PyObject* UIButtonObject_GetStroke(UIButtonObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->Stroke);
}

PyObject* UIButtonObject_GetHoverOnColor(UIButtonObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(), 
		"IIIfK", 
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->HoverOnColor
	);
}

PyObject* UIButtonObject_GetOnMousePressColor(UIButtonObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(),
		"IIIfK",
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->OnMousePressColor
	);
}

PyObject* UIButtonObject_GetLabel(UIButtonObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&UILabelObject_GetType(),
		"K",
		(unsigned long long)self->handle->Label.get()
	);
}

//================================= //
//============ Setters ============ //
//================================= //

int UIButtonObject_SetFilled(UIButtonObject* self, PyObject* value, void* closure)
{
	if (!PyBool_Check(value))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be a boolean");
		return -1;
	}

	bool filled = PyObject_IsTrue(value);
	self->handle->Filled = filled;
	return 0;
}

int UIButtonObject_SetStroke(UIButtonObject* self, PyObject* value, void* closure)
{
	float stroke = 2.0f;
    if (!PyArg_Parse(value, "f", &stroke))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->Stroke = stroke;
	return 0;
}

int UIButtonObject_SetHoverOnColor(UIButtonObject* self, PyObject* value, void* closure)
{
	ColorObject* color;
    if (!PyArg_Parse(value, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->HoverOnColor = *color->handle.get();
	return 0;
}

int UIButtonObject_SetOnMousePressColor(UIButtonObject* self, PyObject* value, void* closure)
{
	ColorObject* color;
    if (!PyArg_Parse(value, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->OnMousePressColor = *color->handle.get();
	return 0;
}

