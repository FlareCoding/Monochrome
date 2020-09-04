#include "bindings_uiview.h"

PyTypeObject& UIViewObject_GetType()
{
    return UIViewObject_Type;
}

PyObject* UIViewObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UIViewObject* self = (UIViewObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<UIView>((UIView*)native_ptr);
	else
		self->handle = MakeRef<UIView>(Frame(70, 80, 260, 200));
		
    return (PyObject*)self;
}

int UIViewObject_Init(UIViewObject* type, PyObject* args, PyObject* kwds)
{
	type->_super.handle = type->handle;
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

PyObject* UIViewObject_AddSubview(UIViewObject* self, PyObject* args)
{
	UIViewObject* view;
    if (!PyArg_ParseTuple(args, "O", &view))
        return NULL;

    self->handle->AddSubview(view->handle);
    return Py_None;
}

PyObject* UIViewObject_RemoveSubview(UIViewObject* self, PyObject* args)
{
	UIViewObject* view;
    if (!PyArg_ParseTuple(args, "O", &view))
        return NULL;

    self->handle->RemoveSubview(view->handle);
    return Py_None;
}

//================================= //
//============ Getters ============ //
//================================= //

PyObject* UIViewObject_GetVisible(UIViewObject* self, void* closure)
{
	return (self->handle->Visible) ? Py_True : Py_False;
}

PyObject* UIViewObject_GetLayer(UIViewObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&LayerObject_GetType(), 
		"K", 
		(unsigned long long)&self->handle->layer
	);
}

PyObject* UIViewObject_GetZIndex(UIViewObject* self, void* closure)
{
	return Py_BuildValue("i", self->handle->GetZIndex());
}

//================================= //
//============ Setters ============ //
//================================= //

int UIViewObject_SetVisible(UIViewObject* self, PyObject* value, void* closure)
{
	if (!PyBool_Check(value))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be a boolean");
		return -1;
	}

	bool visible = PyObject_IsTrue(value);
	self->handle->Visible = (bool)visible;
	return 0;
}

int UIViewObject_SetZIndex(UIViewObject* self, PyObject* value, void* closure)
{
	int index = 0;
	if (!PyArg_Parse(value, "i", &index))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be an integer");
		return -1;
	}

	self->handle->SetZIndex(index);
	return 0;
}

