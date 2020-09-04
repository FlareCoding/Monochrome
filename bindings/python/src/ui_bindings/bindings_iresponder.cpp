#include "bindings_iresponder.h"
#include "bindings_uiview.h"

PyTypeObject& IResponderObject_GetType()
{
    return IResponderObject_Type;
}

PyObject* IResponderObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    IResponderObject* self = (IResponderObject*)type->tp_alloc(type, 0);
    if (!self)
        return NULL;

    return (PyObject*)self;
}

int IResponderObject_Init(IResponderObject* type, PyObject* args, PyObject* kwds)
{
	return 0;
}

//================================= //
//============ Methods ============ //
//================================= //

template <EventType ET>
static PyObject* AddEventHandler(
	IResponderObject* self,
	PyObject* args,
	std::function<PyObject*(Event&)> evt_getter
)
{
    PyObject* callback;

    if (!PyArg_ParseTuple(args, "O:EventHandler", &callback))
        return NULL;

    if (!PyCallable_Check(callback))
    {
        PyErr_SetString(PyExc_TypeError, "Parameter must be callable");
        return NULL;
    }

    Py_XINCREF(callback);
    Py_INCREF(Py_None);

    if (self->handle)
    {
        self->handle->AddEventHandler<ET>([callback, self, evt_getter](Event& e, UIView* sender) -> bool {
            PyObject* evt = evt_getter(e);

			UIViewObject* pysender = (UIViewObject*)PyObject_CallFunction(
				(PyObject*)&UIViewObject_GetType(), 
				"K", 
				(unsigned long long)sender
			);

			PyObject* argslist = Py_BuildValue("(OO)", evt, pysender);
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

PyObject* IResponderObject_AddMousePressedEventHandler(IResponderObject* self, PyObject* args)
{
	AddEventHandler<EventType::MouseButtonPressed>(self, args, [](Event& e) {
		auto* evt = (MouseButtonPressedEventObject*)PyObject_CallObject((PyObject*)&MouseButtonPressedEventObject_GetType(), NULL);
		evt->handle.button = reinterpret_cast<MouseButtonPressedEvent&>(e).button;
		evt->handle.location = reinterpret_cast<MouseButtonPressedEvent&>(e).location;
		return (PyObject*)evt;
	});

	return Py_None;
}

PyObject* IResponderObject_AddMouseReleasedEventHandler(IResponderObject* self, PyObject* args)
{
	AddEventHandler<EventType::MouseButtonReleased>(self, args, [](Event& e) {
		MouseButtonReleasedEventObject* evt = (MouseButtonReleasedEventObject*)PyObject_CallObject((PyObject*)&MouseButtonReleasedEventObject_GetType(), NULL);
		evt->handle.button = reinterpret_cast<MouseButtonReleasedEvent&>(e).button;
		evt->handle.location = reinterpret_cast<MouseButtonReleasedEvent&>(e).location;
		return (PyObject*)evt;
	});

	return Py_None;
}

PyObject* IResponderObject_AddMouseClickedEventHandler(IResponderObject* self, PyObject* args)
{
	return IResponderObject_AddMouseReleasedEventHandler(self, args);
}

PyObject* IResponderObject_AddMouseMovedEventHandler(IResponderObject* self, PyObject* args)
{
	AddEventHandler<EventType::MouseMoved>(self, args, [](Event& e) {
		MouseMovedEventObject* evt = (MouseMovedEventObject*)PyObject_CallObject((PyObject*)&MouseMovedEventObject_GetType(), NULL);
		evt->handle.button = reinterpret_cast<MouseMovedEvent&>(e).button;
		evt->handle.distance = reinterpret_cast<MouseMovedEvent&>(e).distance;
		return (PyObject*)evt;
	});

	return Py_None;
}

PyObject* IResponderObject_AddHoverOnEventHandler(IResponderObject* self, PyObject* args)
{
	AddEventHandler<EventType::MouseHoverOn>(self, args, [](Event& e) {
		return PyObject_CallObject((PyObject*)&MouseHoverOnEventObject_GetType(), NULL);
	});

	return Py_None;
}

PyObject* IResponderObject_AddHoverOffEventHandler(IResponderObject* self, PyObject* args)
{
	AddEventHandler<EventType::MouseHoverOff>(self, args, [](Event& e) {
		return PyObject_CallObject((PyObject*)&MouseHoverOffEventObject_GetType(), NULL);
	});

	return Py_None;
}

PyObject* IResponderObject_AddKeyPressedEventHandler(IResponderObject* self, PyObject* args)
{
	AddEventHandler<EventType::KeyPressed>(self, args, [](Event& e) {
		KeyPressedEventObject* evt = (KeyPressedEventObject*)PyObject_CallObject((PyObject*)&KeyPressedEventObject_GetType(), NULL);
		evt->handle.keycode = reinterpret_cast<KeyPressedEvent&>(e).keycode;
		evt->handle.repeated = reinterpret_cast<KeyPressedEvent&>(e).repeated;
		evt->handle.capital = reinterpret_cast<KeyPressedEvent&>(e).capital;
		evt->handle.capslock_on = reinterpret_cast<KeyPressedEvent&>(e).capslock_on;
		return (PyObject*)evt;
	});

	return Py_None;
}

PyObject* IResponderObject_AddKeyReleasedEventHandler(IResponderObject* self, PyObject* args)
{
	AddEventHandler<EventType::KeyReleased>(self, args, [](Event& e) {
		KeyReleasedEventObject* evt = (KeyReleasedEventObject*)PyObject_CallObject((PyObject*)&KeyReleasedEventObject_GetType(), NULL);
		evt->handle.keycode = reinterpret_cast<KeyPressedEvent&>(e).keycode;
		return (PyObject*)evt;
	});

	return Py_None;
}

PyObject* IResponderObject_AddFocusChangedEventHandler(IResponderObject* self, PyObject* args)
{
	AddEventHandler<EventType::FocusChanged>(self, args, [](Event& e) {
		FocusChangedEventObject* evt = (FocusChangedEventObject*)PyObject_CallObject((PyObject*)&FocusChangedEventObject_GetType(), NULL);
		evt->handle = reinterpret_cast<FocusChangedEvent&>(e).GainedFocus;
		return (PyObject*)evt;
	});

	return Py_None;
}

PyObject* IResponderObject_AddWindowResizedEventHandler(IResponderObject* self, PyObject* args)
{
	AddEventHandler<EventType::WindowResized>(self, args, [](Event& e) {
		WindowResizedEventObject* evt = (WindowResizedEventObject*)PyObject_CallObject((PyObject*)&WindowResizedEventObject_GetType(), NULL);
		evt->handle.width = reinterpret_cast<WindowResizedEvent&>(e).width;
		evt->handle.height = reinterpret_cast<WindowResizedEvent&>(e).height;
		return (PyObject*)evt;
	});

	return Py_None;
}

PyObject* IResponderObject_AddWindowClosedEventHandler(IResponderObject* self, PyObject* args)
{
	AddEventHandler<EventType::WindowClosed>(self, args, [](Event& e) {
		return PyObject_CallObject((PyObject*)&WindowClosedEventObject_GetType(), NULL);
	});

	return Py_None;
}

PyObject* IResponderObject_AddWindowGainedFocusEventHandler(IResponderObject* self, PyObject* args)
{
	AddEventHandler<EventType::WindowGainedFocus>(self, args, [](Event& e) {
		return PyObject_CallObject((PyObject*)&WindowGainedFocusEventObject_GetType(), NULL);
	});

	return Py_None;
}

PyObject* IResponderObject_AddWindowLostFocusEventHandler(IResponderObject* self, PyObject* args)
{
	AddEventHandler<EventType::WindowLostFocus>(self, args, [](Event& e) {
		return PyObject_CallObject((PyObject*)&WindowLostFocusEventObject_GetType(), NULL);
	});

	return Py_None;
}

PyObject* IResponderObject_AddWindowUpdatedEventHandler(IResponderObject* self, PyObject* args)
{
	AddEventHandler<EventType::WindowUpdated>(self, args, [](Event& e) {
		WindowUpdatedEventObject* evt = (WindowUpdatedEventObject*)PyObject_CallObject((PyObject*)&WindowUpdatedEventObject_GetType(), NULL);
		evt->handle.EventProcessingTime = reinterpret_cast<WindowUpdatedEvent&>(e).EventProcessingTime;
		evt->handle.GraphicsRenderingTime = reinterpret_cast<WindowUpdatedEvent&>(e).GraphicsRenderingTime;
		return (PyObject*)evt;
	});

	return Py_None;
}

//================================= //
//============ Getters ============ //
//================================= //

//================================= //
//============ Setters ============ //
//================================= //

