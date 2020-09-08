#pragma once
#include "bindings_uiview.h"

typedef struct _UIComboboxObject {
	UIViewObject _super;
	Ref<UICombobox> handle = nullptr;
} UIComboboxObject;

PyObject* UIComboboxObject_New(PyTypeObject* type, PyObject* args, PyObject* kwds);
int UIComboboxObject_Init(UIComboboxObject* type, PyObject* args, PyObject* kwds);

//============ Methods ============ //
PyObject* UIComboboxObject_AddItem(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_InsertItem(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_AddItems(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_SetItems(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_IndexOf(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_GetItem(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_RemoveItemByIndex(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_RemoveItem(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_Clear(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_SelectItemByIndex(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_SelectItem(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_GetItemCount(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_GetSelectedItem(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_SetItemBackgroundColor(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_GetItemBackgroundColor(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_SetItemTextColor(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_GetItemTextColor(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_SetDropdownArrowColor(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_SetSelectedItemColor(UIComboboxObject* self, PyObject* args);
PyObject* UIComboboxObject_AddItemChangedEventHandler(UIComboboxObject* self, PyObject* args);

static PyMethodDef UIComboboxObjectMethods[] = {
	{ "add_item", (PyCFunction)UIComboboxObject_AddItem, METH_VARARGS, "add_item" },
	{ "insert_item", (PyCFunction)UIComboboxObject_InsertItem, METH_VARARGS, "insert_item" },
	{ "add_items", (PyCFunction)UIComboboxObject_AddItems, METH_VARARGS, "add_items" },
	{ "set_items", (PyCFunction)UIComboboxObject_SetItems, METH_VARARGS, "set_items" },
	{ "index_of", (PyCFunction)UIComboboxObject_IndexOf, METH_VARARGS, "index_of" },
	{ "get_item", (PyCFunction)UIComboboxObject_GetItem, METH_VARARGS, "get_item" },
	{ "remove_item_by_index", (PyCFunction)UIComboboxObject_RemoveItemByIndex, METH_VARARGS, "remove_item_by_index" },
	{ "remove_item", (PyCFunction)UIComboboxObject_RemoveItem, METH_VARARGS, "remove_item" },
	{ "clear", (PyCFunction)UIComboboxObject_Clear, METH_VARARGS, "clear" },
	{ "select_item_by_index", (PyCFunction)UIComboboxObject_SelectItemByIndex, METH_VARARGS, "select_item_by_index" },
	{ "select_item", (PyCFunction)UIComboboxObject_SelectItem, METH_VARARGS, "select_item" },
	{ "get_item_count", (PyCFunction)UIComboboxObject_GetItemCount, METH_VARARGS, "get_item_count" },
	{ "get_selected_item", (PyCFunction)UIComboboxObject_GetSelectedItem, METH_VARARGS, "get_selected_item" },
	{ "set_item_background_color", (PyCFunction)UIComboboxObject_SetItemBackgroundColor, METH_VARARGS, "set_item_background_color" },
	{ "get_item_background_color", (PyCFunction)UIComboboxObject_GetItemBackgroundColor, METH_VARARGS, "get_item_background_color" },
	{ "set_item_text_color", (PyCFunction)UIComboboxObject_SetItemTextColor, METH_VARARGS, "set_item_text_color" },
	{ "get_item_text_color", (PyCFunction)UIComboboxObject_GetItemTextColor, METH_VARARGS, "get_item_text_color" },
	{ "set_dropdown_arrow_color", (PyCFunction)UIComboboxObject_SetDropdownArrowColor, METH_VARARGS, "set_dropdown_arrow_color" },
	{ "set_selected_item_color", (PyCFunction)UIComboboxObject_SetSelectedItemColor, METH_VARARGS, "set_selected_item_color" },
	{ "add_item_changed_event_handler", (PyCFunction)UIComboboxObject_AddItemChangedEventHandler, METH_VARARGS, "add_item_changed_event_handler" },
	{ NULL, NULL, 0, NULL }
};

//============ Getters ============ //
PyObject* UIComboboxObject_GetSlotSize(UIComboboxObject* self, void* closure);

//============ Setters ============ //
int UIComboboxObject_SetSlotSize(UIComboboxObject* self, PyObject* value, void* closure);

static PyGetSetDef UIComboboxObjectGettersSetters[] = {
	{ "slot_size", (getter)UIComboboxObject_GetSlotSize, (setter)UIComboboxObject_SetSlotSize, "slot_size", NULL },
	{ NULL, NULL, 0, NULL }
};

static PyTypeObject UIComboboxObject_Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"UICombobox", /*tp_name*/
	sizeof(UIComboboxObject), /*tp_basicsize*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	0, 0, 0, 0, 0, 0, 0, 
	UIComboboxObjectMethods, /*tp_methods*/
	0, /*tp_members*/
	UIComboboxObjectGettersSetters, /*tp_getset*/
	&UIViewObject_Type, /*tp_base*/
	0, 0, 0, 0, 
	(initproc)UIComboboxObject_Init, /*tp_init*/
	0, /*tp_alloc*/
	UIComboboxObject_New, /*tp_new*/
	0, /*tp_free*/
	0, /*tp_is_gc*/
};

PyTypeObject& UIComboboxObject_GetType();