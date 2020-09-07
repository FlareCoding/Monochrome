#include "bindings_uicombobox.h"

PyTypeObject& UIComboboxObject_GetType()
{
	return UIComboboxObject_Type;
}

PyObject* UIComboboxObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UIComboboxObject* self = (UIComboboxObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<UICombobox>((UICombobox*)native_ptr);
	else
		self->handle = MakeRef<UICombobox>();

	self->_super.handle = self->handle;
    return (PyObject*)self;
}

int UIComboboxObject_Init(UIComboboxObject* type, PyObject* args, PyObject* kwds)
{
	UIViewObject_GetType().tp_init((PyObject*)type, args, kwds);
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

PyObject* UIComboboxObject_AddItem(UIComboboxObject* self, PyObject* args)
{
	char* item;
    if (!PyArg_ParseTuple(args, "s", &item))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a string");
		return NULL;
	}

	self->handle->AddItem(item);
	return Py_None;
}

PyObject* UIComboboxObject_InsertItem(UIComboboxObject* self, PyObject* args)
{
	char* item;
	Py_ssize_t index;
    if (!PyArg_ParseTuple(args, "sn", &item, &index))
    {
		PyErr_SetString(PyExc_Exception, "Parameters: (string, size_t)");
		return NULL;
	}

	self->handle->InsertItem(item, index);
	return Py_None;
}

PyObject* UIComboboxObject_AddItems(UIComboboxObject* self, PyObject* args)
{
	PyObject* pList;
	if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &pList)) {
		PyErr_SetString(PyExc_TypeError, "Parameter must be a list.");
		return NULL;
	}

	std::vector<std::string> items;
	Py_ssize_t n = PyList_Size(pList);
	for (Py_ssize_t i = 0; i < n; i++) {
		PyObject* pItem = PyList_GetItem(pList, i);
		if (!PyUnicode_Check(pItem)) {
			PyErr_SetString(PyExc_TypeError, "List items must contain strings.");
			return NULL;
		}

		PyObject* str = PyUnicode_AsEncodedString(pItem, "utf-8", "~E~");
		items.push_back(std::string(PyBytes_AS_STRING(str)));
	}

	self->handle->AddItems(items);
	return Py_None;
}

PyObject* UIComboboxObject_SetItems(UIComboboxObject* self, PyObject* args)
{
	PyObject* pList;
	if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &pList)) {
		PyErr_SetString(PyExc_TypeError, "Parameter must be a list.");
		return NULL;
	}

	std::vector<std::string> items;
	Py_ssize_t n = PyList_Size(pList);
	for (Py_ssize_t i = 0; i < n; i++) {
		PyObject* pItem = PyList_GetItem(pList, i);
		if (!PyUnicode_Check(pItem)) {
			PyErr_SetString(PyExc_TypeError, "List items must contain strings.");
			return NULL;
		}

		PyObject* str = PyUnicode_AsEncodedString(pItem, "utf-8", "~E~");
		items.push_back(std::string(PyBytes_AS_STRING(str)));
	}

	self->handle->SetItems(items);
	return Py_None;
}

PyObject* UIComboboxObject_IndexOf(UIComboboxObject* self, PyObject* args)
{
	char* item;
    if (!PyArg_ParseTuple(args, "s", &item))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a string");
		return NULL;
	}

	return Py_BuildValue("n", self->handle->IndexOf(item));
}

PyObject* UIComboboxObject_GetItem(UIComboboxObject* self, PyObject* args)
{
	Py_ssize_t index;
    if (!PyArg_ParseTuple(args, "n", &index))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a size_t");
		return NULL;
	}

	return Py_BuildValue("s", self->handle->GetItem(index).c_str());
}

PyObject* UIComboboxObject_RemoveItemByIndex(UIComboboxObject* self, PyObject* args)
{
	Py_ssize_t index;
    if (!PyArg_ParseTuple(args, "n", &index))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a size_t");
		return NULL;
	}

	self->handle->RemoveItem(index);
	return Py_None;
}

PyObject* UIComboboxObject_RemoveItem(UIComboboxObject* self, PyObject* args)
{
	char* item;
    if (!PyArg_ParseTuple(args, "s", &item))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a string");
		return NULL;
	}

	self->handle->RemoveItem(item);
	return Py_None;
}

PyObject* UIComboboxObject_Clear(UIComboboxObject* self, PyObject* args)
{
	self->handle->Clear();
	return Py_None;
}

PyObject* UIComboboxObject_SelectItemByIndex(UIComboboxObject* self, PyObject* args)
{
	Py_ssize_t index;
    if (!PyArg_ParseTuple(args, "n", &index))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a size_t");
		return NULL;
	}

	self->handle->SelectItem(index);
	return Py_None;
}

PyObject* UIComboboxObject_SelectItem(UIComboboxObject* self, PyObject* args)
{
	char* item;
    if (!PyArg_ParseTuple(args, "s", &item))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be a string");
		return NULL;
	}

	self->handle->SelectItem(item);
	return Py_None;
}

PyObject* UIComboboxObject_GetItemCount(UIComboboxObject* self, PyObject* args)
{
	return Py_BuildValue("n", self->handle->GetItemCount());
}

PyObject* UIComboboxObject_GetSelectedItem(UIComboboxObject* self, PyObject* args)
{
	return Py_BuildValue("s", self->handle->GetSelectedItem().c_str());
}

PyObject* UIComboboxObject_SetItemBackgroundColor(UIComboboxObject* self, PyObject* args)
{
	ColorObject* color;
    if (!PyArg_ParseTuple(args, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return NULL;
	}

	self->handle->SetItemBackgroundColor(*color->handle.get());
	return Py_None;
}

PyObject* UIComboboxObject_GetItemBackgroundColor(UIComboboxObject* self, PyObject* args)
{
	Color color = self->handle->GetItemBackgroundColor();
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(), 
		"IIIf", 
		color.r, color.g, color.b, color.alpha
	);
}

PyObject* UIComboboxObject_SetItemTextColor(UIComboboxObject* self, PyObject* args)
{
	ColorObject* color;
    if (!PyArg_ParseTuple(args, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return NULL;
	}

	self->handle->SetItemTextColor(*color->handle.get());
	return Py_None;
}

PyObject* UIComboboxObject_GetItemTextColor(UIComboboxObject* self, PyObject* args)
{
	Color color = self->handle->GetItemTextColor();
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(), 
		"IIIf", 
		color.r, color.g, color.b, color.alpha
	);
	return Py_None;
}

PyObject* UIComboboxObject_SetDropdownArrowColor(UIComboboxObject* self, PyObject* args)
{
	ColorObject* color;
    if (!PyArg_ParseTuple(args, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return NULL;
	}

	self->handle->SetDropdownArrowColor(*color->handle.get());
	return Py_None;
}

PyObject* UIComboboxObject_SetSelectedItemColor(UIComboboxObject* self, PyObject* args)
{
	ColorObject* color;
    if (!PyArg_ParseTuple(args, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return NULL;
	}

	self->handle->SetSelectedItemColor(*color->handle.get());
	return Py_None;
}

PyObject* UIComboboxObject_AddItemChangedEventHandler(UIComboboxObject* self, PyObject* args)
{
	PyObject* callback;

    if (!PyArg_ParseTuple(args, "O:ItemChangedEventHandler", &callback))
        return NULL;

    if (!PyCallable_Check(callback))
    {
        PyErr_SetString(PyExc_TypeError, "Parameter must be callable");
        return NULL;
    }

    Py_XINCREF(callback);
    Py_INCREF(Py_None);

    self->handle->AddItemChangedEventHandler([callback, self](size_t index, UICombobox* sender) {
		UIComboboxObject* pysender = (UIComboboxObject*)PyObject_CallFunction(
			(PyObject*)&UIComboboxObject_GetType(), 
			"K", 
			(unsigned long long)sender
		);

		PyObject* argslist = Py_BuildValue("(nO)", index, pysender);
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

PyObject* UIComboboxObject_GetSlotSize(UIComboboxObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->SlotSize);
}

//================================= //
//============ Setters ============ //
//================================= //

int UIComboboxObject_SetSlotSize(UIComboboxObject* self, PyObject* value, void* closure)
{
	float size = 20.0f;
    if (!PyArg_Parse(value, "f", &size))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->SlotSize = size;
	return 0;
}

