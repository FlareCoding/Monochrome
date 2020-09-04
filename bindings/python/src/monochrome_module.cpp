#include "window_bindings/bindings_uiwindow.h"

static PyTypeObject MonochromeObject = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Monochrome",               /*tp_name*/
    0,                          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
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
    Py_TPFLAGS_DEFAULT,         /*tp_flags*/
    0,                          /*tp_doc*/
    0,                          /*tp_traverse*/
    0,                          /*tp_clear*/
    0,                          /*tp_richcompare*/
    0,                          /*tp_weaklistoffset*/
    0,                          /*tp_iter*/
    0,                          /*tp_iternext*/
    0,                          /*tp_methods*/
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

#define MODULE_REGISTER_CLASS(mod, name, ptype) if (PyType_Ready(ptype) < 0) { \
                                            Py_XDECREF(mod); \
                                            return -1; } \
                                         PyModule_AddObject(mod, name, (PyObject*)ptype);

static int MonochromeModuleExec(PyObject* mod)
{
    if (PyType_Ready(&MonochromeObject) < 0)
    {
        Py_XDECREF(mod);
        return -1;
    }

    // Window
    MODULE_REGISTER_CLASS(mod, "UIWindow", &UIWindowObject_GetType());

    // Events
    MODULE_REGISTER_CLASS(mod, "MouseButtonPressedEvent", &MouseButtonPressedEventObject_GetType());
    MODULE_REGISTER_CLASS(mod, "MouseButtonReleasedEvent", &MouseButtonReleasedEventObject_GetType());
    MODULE_REGISTER_CLASS(mod, "MouseMovedEvent", &MouseMovedEventObject_GetType());
    MODULE_REGISTER_CLASS(mod, "MouseHoverOnEvent", &MouseHoverOnEventObject_GetType());
    MODULE_REGISTER_CLASS(mod, "MouseHoverOffEvent", &MouseHoverOffEventObject_GetType());
    MODULE_REGISTER_CLASS(mod, "KeyPressedEvent", &KeyPressedEventObject_GetType());
    MODULE_REGISTER_CLASS(mod, "KeyReleasedEvent", &KeyReleasedEventObject_GetType());
    MODULE_REGISTER_CLASS(mod, "FocusChangedEvent", &FocusChangedEventObject_GetType());
    MODULE_REGISTER_CLASS(mod, "WindowResizedEvent", &WindowResizedEventObject_GetType());
    MODULE_REGISTER_CLASS(mod, "WindowClosedEvent", &WindowClosedEventObject_GetType());
    MODULE_REGISTER_CLASS(mod, "WindowGainedFocusEvent", &WindowGainedFocusEventObject_GetType());
    MODULE_REGISTER_CLASS(mod, "WindowLostFocusEvent", &WindowLostFocusEventObject_GetType());
    MODULE_REGISTER_CLASS(mod, "WindowUpdatedEvent", &WindowUpdatedEventObject_GetType());

    // UI
    MODULE_REGISTER_CLASS(mod, "IResponder", &IResponderObject_GetType());
    MODULE_REGISTER_CLASS(mod, "Frame", &FrameObject_GetType());
    MODULE_REGISTER_CLASS(mod, "Layer", &LayerObject_GetType());
    MODULE_REGISTER_CLASS(mod, "UIView", &UIViewObject_GetType());

    return 0;
}

static PyMethodDef MonochromeModuleMethods[] = {
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef_Slot MonochromeModuleSlots[] = {
    {Py_mod_exec, (void*)MonochromeModuleExec},
    {0, NULL},
};

static struct PyModuleDef MonochromeModule = {
	PyModuleDef_HEAD_INIT,
	"Monochrome",
	"Monochrome python bindings module",
	0,
	MonochromeModuleMethods,
	MonochromeModuleSlots
};

PyMODINIT_FUNC PyInit_Monochrome()
{
	return PyModuleDef_Init(&MonochromeModule);
}