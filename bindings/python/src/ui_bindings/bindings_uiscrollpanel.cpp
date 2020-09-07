#include "bindings_uiscrollpanel.h"

PyTypeObject& UIScrollPanelObject_GetType()
{
	return UIScrollPanelObject_Type;
}

PyObject* UIScrollPanelObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UIScrollPanelObject* self = (UIScrollPanelObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<UIScrollPanel>((UIScrollPanel*)native_ptr);
	else
		self->handle = MakeRef<UIScrollPanel>(Frame(70, 80, 180, 240));

	self->_super.handle = self->handle;
    return (PyObject*)self;
}

int UIScrollPanelObject_Init(UIScrollPanelObject* type, PyObject* args, PyObject* kwds)
{
	UIViewObject_GetType().tp_init((PyObject*)type, args, kwds);
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

PyObject* UIScrollPanelObject_AddChild(UIScrollPanelObject* self, PyObject* args)
{
	UIViewObject* view;
    if (!PyArg_ParseTuple(args, "O", &view))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be an instance of UIView");
		return NULL;
	}

    self->handle->AddChild(view->handle);
	return Py_None;
}

PyObject* UIScrollPanelObject_RemoveChild(UIScrollPanelObject* self, PyObject* args)
{
	UIViewObject* view;
    if (!PyArg_ParseTuple(args, "O", &view))
    {
		PyErr_SetString(PyExc_Exception, "Parameter must be an instance of UIView");
		return NULL;
	}

    self->handle->RemoveChild(view->handle);
	return Py_None;
}

PyObject* UIScrollPanelObject_Clear(UIScrollPanelObject* self, PyObject* args)
{
	self->handle->Clear();
	return Py_None;
}

PyObject* UIScrollPanelObject_ScrollToTop(UIScrollPanelObject* self, PyObject* args)
{
	self->handle->ScrollToTop();
	return Py_None;
}

PyObject* UIScrollPanelObject_ScrollToBottom(UIScrollPanelObject* self, PyObject* args)
{
	self->handle->ScrollToTop();
	return Py_None;
}

PyObject* UIScrollPanelObject_ScrollContent(UIScrollPanelObject* self, PyObject* args)
{
	float scroll_distance = 2.0f;
    if (!PyArg_Parse(args, "f", &scroll_distance))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return NULL;
    }

	self->handle->ScrollContent(scroll_distance);
	return Py_None;
}

//================================= //
//============ Getters ============ //
//================================= //

PyObject* UIScrollPanelObject_GetScrollbarColor(UIScrollPanelObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(), 
		"IIIfK", 
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->ScrollbarColor
	);
}

PyObject* UIScrollPanelObject_GetContentView(UIScrollPanelObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&UIViewObject_GetType(), 
		"K", 
		(unsigned long long)self->handle->ContentView.get()
	);
}

//================================= //
//============ Setters ============ //
//================================= //

int UIScrollPanelObject_SetScrollbarColor(UIScrollPanelObject* self, PyObject* value, void* closure)
{
	ColorObject* color;
    if (!PyArg_Parse(value, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->ScrollbarColor = *color->handle.get();
	return 0;
}

