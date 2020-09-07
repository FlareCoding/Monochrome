#include "bindings_uislider.h"

PyTypeObject& UISliderObject_GetType()
{
	return UISliderObject_Type;
}

PyObject* UISliderObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    UISliderObject* self = (UISliderObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

	unsigned long long native_ptr = 0;
    if (!PyArg_ParseTuple(args, "|K", &native_ptr))
    {
        PyErr_SetString(PyExc_Exception, "Invalid parameter.");
        return NULL;
    }

	if (native_ptr)
		self->handle = Ref<UISlider>((UISlider*)native_ptr);
	else
		self->handle = MakeRef<UISlider>(Frame(70, 80, 160, 20));

	self->_super.handle = self->handle;
    return (PyObject*)self;
}

int UISliderObject_Init(UISliderObject* type, PyObject* args, PyObject* kwds)
{
	UIViewObject_GetType().tp_init((PyObject*)type, args, kwds);
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

PyObject* UISliderObject_AddValueChangedEventHandler(UISliderObject* self, PyObject* args)
{
	PyObject* callback;

    if (!PyArg_ParseTuple(args, "O:ValueChangedEventHandler", &callback))
        return NULL;

    if (!PyCallable_Check(callback))
    {
        PyErr_SetString(PyExc_TypeError, "Parameter must be callable");
        return NULL;
    }

    Py_XINCREF(callback);
    Py_INCREF(Py_None);

    self->handle->AddValueChangedEventHandler([callback, self](float value, UISlider* sender) {
		UIViewObject* pysender = (UIViewObject*)PyObject_CallFunction(
			(PyObject*)&UISliderObject_GetType(), 
			"K", 
			(unsigned long long)sender
		);

		PyObject* argslist = Py_BuildValue("(fO)", value, pysender);
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

PyObject* UISliderObject_GetSliderKnobShape(UISliderObject* self, void* closure)
{
	return Py_BuildValue("I", (uint32_t)self->handle->SliderKnobShape);
}

PyObject* UISliderObject_GetSliderBarHeight(UISliderObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->SliderBarHeight);
}

PyObject* UISliderObject_GetSliderKnobColor(UISliderObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(),
		"IIIfK",
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->SliderKnobColor
	);
}

PyObject* UISliderObject_GetTickmarksColor(UISliderObject* self, void* closure)
{
	return PyObject_CallFunction(
		(PyObject*)&ColorObject_GetType(),
		"IIIfK",
		255, 255, 255, 1.0f,
		(unsigned long long)&self->handle->TickmarksColor
	);
}

PyObject* UISliderObject_GetMaxValue(UISliderObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->MaxValue);
}

PyObject* UISliderObject_GetMinValue(UISliderObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->MinValue);
}

PyObject* UISliderObject_GetValue(UISliderObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->Value);
}

PyObject* UISliderObject_GetIntervals(UISliderObject* self, void* closure)
{
	return Py_BuildValue("f", self->handle->Intervals);
}

PyObject* UISliderObject_GetVisibleTickmarks(UISliderObject* self, void* closure)
{
	return self->handle->VisibleTickmarks ? Py_True : Py_False;
}

//================================= //
//============ Setters ============ //
//================================= //

int UISliderObject_SetSliderKnobShape(UISliderObject* self, PyObject* value, void* closure)
{
	uint32_t val = 0;
    if (!PyArg_Parse(value, "I", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a uint32_t");
        return -1;
    }

	self->handle->SliderKnobShape = (Shape)val;
	return 0;
}

int UISliderObject_SetSliderBarHeight(UISliderObject* self, PyObject* value, void* closure)
{
	float val = 0;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->SliderBarHeight = val;
	return 0;
}

int UISliderObject_SetSliderKnobColor(UISliderObject* self, PyObject* value, void* closure)
{
	ColorObject* color;
    if (!PyArg_Parse(value, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->SliderKnobColor = *color->handle.get();
	return 0;
}

int UISliderObject_SetTickmarksColor(UISliderObject* self, PyObject* value, void* closure)
{
	ColorObject* color;
    if (!PyArg_Parse(value, "O", &color))
    {
		PyErr_SetString(PyExc_Exception, "Invalid parameter.");
		return -1;
	}

	self->handle->TickmarksColor = *color->handle.get();
	return 0;
}

int UISliderObject_SetMaxValue(UISliderObject* self, PyObject* value, void* closure)
{
	float val = 0;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->MaxValue = val;
	return 0;
}

int UISliderObject_SetMinValue(UISliderObject* self, PyObject* value, void* closure)
{
	float val = 0;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->MinValue = val;
	return 0;
}

int UISliderObject_SetValue(UISliderObject* self, PyObject* value, void* closure)
{
	float val = 0;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->Value = val;
	return 0;
}

int UISliderObject_SetIntervals(UISliderObject* self, PyObject* value, void* closure)
{
	float val = 0;
    if (!PyArg_Parse(value, "f", &val))
    {
        PyErr_SetString(PyExc_Exception, "Parameter must be a float");
        return -1;
    }

	self->handle->Intervals = val;
	return 0;
}

int UISliderObject_SetVisibleTickmarks(UISliderObject* self, PyObject* value, void* closure)
{
	if (!PyBool_Check(value))
	{
		PyErr_SetString(PyExc_TypeError, "Value must be a boolean");
		return -1;
	}

	bool visible_tickmarks = PyObject_IsTrue(value);
	self->handle->VisibleTickmarks = visible_tickmarks;
	return 0;
}

