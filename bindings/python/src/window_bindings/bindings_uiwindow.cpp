#include "bindings_uiwindow.h"
#include "../ui_bindings/bindings_uibutton.h"

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

PyObject* UIWindowObject_GetNativeHandle(UIWindowObject* self, PyObject* args)
{
	return Py_BuildValue("K", (unsigned long long)self->handle->GetNativeHandle());
}

PyObject* UIWindowObject_GetWidth(UIWindowObject* self, PyObject* args)
{
	return Py_BuildValue("I", self->handle->GetWidth());
}

PyObject* UIWindowObject_GetHeight(UIWindowObject* self, PyObject* args)
{
	return Py_BuildValue("I", self->handle->GetHeight());
}

PyObject* UIWindowObject_GetTitle(UIWindowObject* self, PyObject* args)
{
	return Py_BuildValue("s", self->handle->GetTitle());
}

PyObject* UIWindowObject_GetDpi(UIWindowObject* self, PyObject* args)
{
	return Py_BuildValue("I", self->handle->GetDpi());
}

PyObject* UIWindowObject_SetSize(UIWindowObject* self, PyObject* args)
{
	PointObject* obj;
    if (!PyArg_Parse(args, "O", &obj))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return NULL;
	}

	uint32_t width = obj->handle->x;
	uint32_t height = obj->handle->y;
	self->handle->SetSize(width, height);
	return Py_None;
}

PyObject* UIWindowObject_SetPos(UIWindowObject* self, PyObject* args)
{
	PointObject* obj;
    if (!PyArg_Parse(args, "O", &obj))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return NULL;
	}

	uint32_t x = obj->handle->x;
	uint32_t y = obj->handle->y;
	self->handle->SetPos(x, y);
	return Py_None;
}

PyObject* UIWindowObject_SetTitle(UIWindowObject* self, PyObject* args)
{
	char* text = (char*)"";
    if (!PyArg_Parse(args, "s", &text))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a string");
        return NULL;
    }

	self->handle->SetTitle(text);
	return Py_None;
}

PyObject* UIWindowObject_AddView(UIWindowObject* self, PyObject* args)
{
	UIViewObject* view;
    if (!PyArg_ParseTuple(args, "O", &view))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be an instance of UIView");
		return NULL;
	}

    self->handle->AddView(view->handle);
    return Py_None;
}

PyObject* UIWindowObject_RemoveView(UIWindowObject* self, PyObject* args)
{
	UIViewObject* view;
    if (!PyArg_ParseTuple(args, "O", &view))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be an instance of UIView");
		return NULL;
	}

    self->handle->RemoveView(view->handle);
    return Py_None;
}

PyObject* UIWindowObject_Update(UIWindowObject* self, PyObject* args)
{
	self->handle->Update();
	return Py_None;
}

PyObject* UIWindowObject_ForceUpdate(UIWindowObject* self, PyObject* args)
{
	self->handle->ForceUpdate();
	return Py_None;
}

PyObject* UIWindowObject_StartWindowLoop(UIWindowObject* self, PyObject* args)
{
	self->handle->StartWindowLoop();
    return Py_None;
}

PyObject* UIWindowObject_IsOpened(UIWindowObject* self, PyObject* args)
{
	return (self->handle->IsOpened()) ? Py_True : Py_False;
}

PyObject* UIWindowObject_GetBackgroundColor(UIWindowObject* self, PyObject* args)
{
	Color color = self->handle->GetBackgroundColor();
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(), 
		"IIIf", 
		color.r, color.g, color.b, color.alpha
	);
}

PyObject* UIWindowObject_SetBackgroundColor(UIWindowObject* self, PyObject* args)
{
	ColorObject* color;
    if (!PyArg_ParseTuple(args, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return NULL;
	}

	Color native_color = *color->handle.get();
	self->handle->SetBackgroundColor(native_color);
	return Py_None;
}

PyObject* UIWindowObject_GetCloseButtonRef(UIWindowObject* self, PyObject* args)
{
	auto ref = self->handle->GetCloseButtonRef();
	if (ref)
	{
		return PyObject_CallFunction(
			(PyObject*)&UIButtonObject_GetType(),
			"K",
			(unsigned long long)ref.get()
		);
	}
	else
		return Py_None;
}

PyObject* UIWindowObject_GetMaximizeButtonRef(UIWindowObject* self, PyObject* args)
{
	auto ref = self->handle->GetMaximizeButtonRef();
	if (ref)
	{
		return PyObject_CallFunction(
			(PyObject*)&UIButtonObject_GetType(),
			"K",
			(unsigned long long)ref.get()
		);
	}
	else
		return Py_None;
}

PyObject* UIWindowObject_GetMinimizeButtonRef(UIWindowObject* self, PyObject* args)
{
	auto ref = self->handle->GetMinimizeButtonRef();
	if (ref)
	{
		return PyObject_CallFunction(
			(PyObject*)&UIButtonObject_GetType(),
			"K",
			(unsigned long long)ref.get()
		);
	}
	else
		return Py_None;
}

PyObject* UIWindowObject_GetWindowTitleLabelRef(UIWindowObject* self, PyObject* args)
{
	auto ref = self->handle->GetWindowTitleLabelRef();
	if (ref)
	{
		return PyObject_CallFunction(
			(PyObject*)&UILabelObject_GetType(),
			"K",
			(unsigned long long)ref.get()
		);
	}
	else
		return Py_None;
}

PyObject* UIWindowObject_SetModernWindowButtonsColor(UIWindowObject* self, PyObject* args)
{
	ColorObject* color;
    if (!PyArg_ParseTuple(args, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return NULL;
	}

	Color native_color = *color->handle.get();
	self->handle->SetModernWindowButtonsColor(native_color);
	return Py_None;
}

PyObject* UIWindowObject_FocusView(UIWindowObject* self, PyObject* args)
{
	UIViewObject* view;
    if (!PyArg_ParseTuple(args, "O", &view))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be an instance of UIView");
		return NULL;
	}

    self->handle->FocusView(view->handle);
    return Py_None;
}

PyObject* UIWindowObject_GetFocusedView(UIWindowObject* self, PyObject* args)
{
	UIView* view_ptr = self->handle->GetFocusedView();
	if (view_ptr)
	{
		return PyObject_CallFunction(
			(PyObject*)&UIViewObject_GetType(),
			"K",
			(unsigned long long)view_ptr
		);
	}
	else
		return Py_None;
}

PyObject* UIWindowObject_IsViewFocused(UIWindowObject* self, PyObject* args)
{
	UIViewObject* view;
    if (!PyArg_ParseTuple(args, "O", &view))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be an instance of UIView");
		return NULL;
	}

    bool focused = self->handle->IsViewFocused(view->handle);
    return focused ? Py_True : Py_False;
}

PyObject* UIWindowObject_GetMouseCursorPos(UIWindowObject* self, PyObject* args)
{
	Position pos = self->handle->GetMouseCursorPos();
	return PyObject_CallFunction(
		(PyObject*)&PointObject_GetType(), 
		"ff", 
		pos.x, pos.y
	);
}

PyObject* UIWindowObject_GetAbsoluteMouseCursorPos(UIWindowObject* self, PyObject* args)
{
	Position pos = self->handle->GetMouseCursorPos();
	return PyObject_CallFunction(
		(PyObject*)&PointObject_GetType(), 
		"ff", 
		pos.x, pos.y
	);
}

PyObject* UIWindowObject_RemoveAllViews(UIWindowObject* self, PyObject* args)
{
	self->handle->RemoveAllViews();
	return Py_None;
}

//================================= //
//============ Getters ============ //
//================================= //

//================================= //
//============ Setters ============ //
//================================= //

