#include "window/uiwindow_bindings.h"
#include "ui/uilabel_bindings.h"
#include "ui/uibutton_bindings.h"

static PyMethodDef MonochromeObjectMethods[] = {
    {NULL, NULL}
};

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
    MonochromeObjectMethods,    /*tp_methods*/
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

#define MODULE_REGISTER_CLASS(mod, name, type) if (PyType_Ready(&type) < 0) { \
                                            Py_XDECREF(mod); \
                                            return -1; } \
                                         PyModule_AddObject(mod, ##name, (PyObject*)&type);

static int MonochromeModuleExec(PyObject* mod)
{
    if (PyType_Ready(&MonochromeObject) < 0)
    {
        Py_XDECREF(mod);
        return -1;
    }

    // UIWindow Class
    UIWindowObject_GetType().tp_base = &PyUnicode_Type;
    MODULE_REGISTER_CLASS(mod, "UIWindow", UIWindowObject_GetType());

    // Event Classes
    MouseButtonPressedEventObject_GetType().tp_base = &PyUnicode_Type;
    MODULE_REGISTER_CLASS(mod, "MouseButtonPressedEvent", MouseButtonPressedEventObject_GetType());

    MouseButtonClickedEventObject_GetType().tp_base = &PyUnicode_Type;
    MODULE_REGISTER_CLASS(mod, "MouseButtonClickedEvent", MouseButtonClickedEventObject_GetType());

    MouseHoverOnEventObject_GetType().tp_base = &PyUnicode_Type;
    MODULE_REGISTER_CLASS(mod, "MouseHoverOnEvent", MouseHoverOnEventObject_GetType());

    MouseHoverOffEventObject_GetType().tp_base = &PyUnicode_Type;
    MODULE_REGISTER_CLASS(mod, "MouseHoverOffEvent", MouseHoverOffEventObject_GetType());

    MouseMovedEventObject_GetType().tp_base = &PyUnicode_Type;
    MODULE_REGISTER_CLASS(mod, "MouseMovedEvent", MouseMovedEventObject_GetType());

    MouseScrolledEventObject_GetType().tp_base = &PyUnicode_Type;
    MODULE_REGISTER_CLASS(mod, "MouseScrolledEvent", MouseScrolledEventObject_GetType());

    KeyPressedEventObject_GetType().tp_base = &PyUnicode_Type;
    MODULE_REGISTER_CLASS(mod, "KeyPressedEvent", KeyPressedEventObject_GetType());

    KeyReleasedEventObject_GetType().tp_base = &PyUnicode_Type;
    MODULE_REGISTER_CLASS(mod, "KeyReleasedEvent", KeyReleasedEventObject_GetType());

    // IResponder Class
    IResponderObject_GetType().tp_base = &PyUnicode_Type;
    MODULE_REGISTER_CLASS(mod, "IResponder", IResponderObject_GetType());

    // Layer Class
    LayerObject_GetType().tp_base = &PyUnicode_Type;
    MODULE_REGISTER_CLASS(mod, "Layer", LayerObject_GetType());

    // UIView Class
    MODULE_REGISTER_CLASS(mod, "UIView", UIViewObject_GetType());

    // UILabel Class
    MODULE_REGISTER_CLASS(mod, "UILabel", UILabelObject_GetType());

    // UIButton Class
    MODULE_REGISTER_CLASS(mod, "UIButton", UIButtonObject_GetType());

    return 0;
}

static PyMethodDef MonochromeModuleMethods[] = {
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef_Slot MonochromeModuleSlots[] = {
    {Py_mod_exec, MonochromeModuleExec},
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

