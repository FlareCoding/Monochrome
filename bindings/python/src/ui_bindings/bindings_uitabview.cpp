#include "bindings_uitabview.h"

PyTypeObject& UITabViewObject_GetType()
{
	return UITabViewObject_Type;
}

PyObject* UITabViewObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UITabViewObject* self = (UITabViewObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<UITabView>((UITabView*)native_ptr);
	else
		self->handle = MakeRef<UITabView>();

	self->_super.handle = self->handle;
    return (PyObject*)self;
}

int UITabViewObject_Init(UITabViewObject* type, PyObject* args, PyObject* kwds)
{
	UIViewObject_GetType().tp_init((PyObject*)type, args, kwds);
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

PyObject* UITabViewObject_AddTab(UITabViewObject* self, PyObject* args)
{
	char* tab_name;
    if (!PyArg_ParseTuple(args, "s", &tab_name))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a string");
		return NULL;
	}

	Ref<UIView> tab = self->handle->AddTab(tab_name);
	return PyObject_CallFunction(
		(PyObject*)&UIViewObject_GetType(),
		"K",
		(unsigned long long)tab.get()
	);
}

PyObject* UITabViewObject_RemoveTabByIndex(UITabViewObject* self, PyObject* args)
{
	Py_ssize_t index;
    if (!PyArg_ParseTuple(args, "n", &index))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a size_t");
		return NULL;
	}

	self->handle->RemoveTab(index);
	return Py_None;
}

PyObject* UITabViewObject_RemoveTab(UITabViewObject* self, PyObject* args)
{
	char* tab_name;
    if (!PyArg_ParseTuple(args, "s", &tab_name))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a string");
		return NULL;
	}

	self->handle->RemoveTab(tab_name);
	return Py_None;
}

PyObject* UITabViewObject_GetTabByIndex(UITabViewObject* self, PyObject* args)
{
	Py_ssize_t index;
    if (!PyArg_ParseTuple(args, "n", &index))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a size_t");
		return NULL;
	}

	Ref<UIView> view = self->handle->GetTab(index);
	if (view)
	{
		return PyObject_CallFunction(
			(PyObject*)&UIViewObject_GetType(),
			"K",
			(unsigned long long)view.get()
		);
	}
	else
		return Py_None;
}

PyObject* UITabViewObject_GetTab(UITabViewObject* self, PyObject* args)
{
	char* tab;
    if (!PyArg_ParseTuple(args, "s", &tab))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a string");
		return NULL;
	}

	Ref<UIView> view = self->handle->GetTab(tab);
	if (view)
	{
		return PyObject_CallFunction(
			(PyObject*)&UIViewObject_GetType(),
			"K",
			(unsigned long long)view.get()
		);
	}
	else
		return Py_None;
}

PyObject* UITabViewObject_OpenTabByIndex(UITabViewObject* self, PyObject* args)
{
	Py_ssize_t index;
    if (!PyArg_ParseTuple(args, "n", &index))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a size_t");
		return NULL;
	}

	self->handle->OpenTab(index);
	return Py_None;
}

PyObject* UITabViewObject_OpenTab(UITabViewObject* self, PyObject* args)
{
	char* tab;
    if (!PyArg_ParseTuple(args, "s", &tab))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a string");
		return NULL;
	}	

	self->handle->OpenTab(tab);
	return Py_None;
}

PyObject* UITabViewObject_GetOpenedTab(UITabViewObject* self, PyObject* args)
{
	return Py_BuildValue("s", self->handle->GetOpenedTab().c_str());
}

PyObject* UITabViewObject_GetTabCount(UITabViewObject* self, PyObject* args)
{
	return Py_BuildValue("n", self->handle->GetTabCount());
}

PyObject* UITabViewObject_StyleTabButtons(UITabViewObject* self, PyObject* args)
{
	PyObject* callback;

    if (!PyArg_ParseTuple(args, "O:StyleTabButtons", &callback))
        return NULL;

    if (!PyCallable_Check(callback))
    {
        PyErr_SetString(PyExc_TypeError, "Parameter must be callable");
        return NULL;
    }

    Py_XINCREF(callback);
    Py_INCREF(Py_None);

    self->handle->StyleTabButtons([callback, self](Ref<UIButton>& tab) {
		UIButtonObject* btnobj = (UIButtonObject*)PyObject_CallFunction(
			(PyObject*)&UIButtonObject_GetType(), 
			"K", 
			(unsigned long long)tab.get()
		);

		PyObject* argslist = Py_BuildValue("(O)", btnobj);
		PyObject* result = PyEval_CallObject(callback, argslist);
		Py_DECREF(argslist);
		
		if (!result)
		{
			PyErr_SetString(PyExc_Exception, "Error calling event handler");
			PyErr_Occurred();
		}

		Py_XDECREF(result);
	});

    return Py_None;
}

//================================= //
//============ Getters ============ //
//================================= //

PyObject* UITabViewObject_GetTabAreaHeight(UITabViewObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->TabAreaHeight);
}

PyObject* UITabViewObject_GetTabWidth(UITabViewObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->TabWidth);
}

PyObject* UITabViewObject_GetUnderlineTabs(UITabViewObject* self, void* closure)
{
	return self->handle->UnderlineTabs ? Py_True : Py_False;
}

PyObject* UITabViewObject_GetTabUnderlineColor(UITabViewObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(), 
		"IIIfK", 
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->TabUnderlineColor
	);
}

PyObject* UITabViewObject_GetSelectedTabColor(UITabViewObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(), 
		"IIIfK", 
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->SelectedTabColor
	);
}

//================================= //
//============ Setters ============ //
//================================= //

int UITabViewObject_SetTabAreaHeight(UITabViewObject* self, PyObject* value, void* closure)
{
	float val;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->TabAreaHeight = val;
	return 0;
}

int UITabViewObject_SetTabWidth(UITabViewObject* self, PyObject* value, void* closure)
{
	float val;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->TabWidth = val;
	return 0;
}

int UITabViewObject_SetUnderlineTabs(UITabViewObject* self, PyObject* value, void* closure)
{
	if (!PyBool_Check(value))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be a boolean");
		return -1;
	}

	bool state = PyObject_IsTrue(value);
	self->handle->UnderlineTabs = state;
	return 0;
}

int UITabViewObject_SetTabUnderlineColor(UITabViewObject* self, PyObject* value, void* closure)
{
	ColorObject* color;
    if (!PyArg_Parse(value, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->TabUnderlineColor = *color->handle.get();
	return 0;
}

int UITabViewObject_SetSelectedTabColor(UITabViewObject* self, PyObject* value, void* closure)
{
	ColorObject* color;
    if (!PyArg_Parse(value, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->SelectedTabColor = *color->handle.get();
	return 0;
}

