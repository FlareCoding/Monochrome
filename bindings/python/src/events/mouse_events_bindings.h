#pragma once
#include <Monochrome.h>
using namespace mc;

#include <Python.h>

typedef struct _MouseButtonPressedEventObject {
    PyObject_HEAD
    MouseButtonPressedEvent native_event = MouseButtonPressedEvent({ 0, 0 }, MouseButton::None);
} MouseButtonPressedEventObject;

PyTypeObject& MouseButtonPressedEventObject_GetType();

typedef struct _MouseButtonClickedEventObject {
    PyObject_HEAD
    MouseButtonClickedEvent native_event = MouseButtonClickedEvent({ 0, 0 }, MouseButton::None);
} MouseButtonClickedEventObject;

PyTypeObject& MouseButtonClickedEventObject_GetType();

typedef struct _MouseHoverOnEventObject {
    PyObject_HEAD
    MouseHoverOnEvent native_event = MouseHoverOnEvent({ 0, 0 } , { 0, 0 }, MouseButton::None);
} MouseHoverOnEventObject;

PyTypeObject& MouseHoverOnEventObject_GetType();

typedef struct _MouseHoverOffEventObject {
    PyObject_HEAD
    MouseHoverOffEvent native_event = MouseHoverOffEvent({ 0, 0 }, { 0, 0 }, MouseButton::None);
} MouseHoverOffEventObject;

PyTypeObject& MouseHoverOffEventObject_GetType();

typedef struct _MouseMovedEventObject {
    PyObject_HEAD
    MouseMovedEvent native_event = MouseMovedEvent({ 0, 0 }, { 0, 0 }, MouseButton::None);
} MouseMovedEventObject;

PyTypeObject& MouseMovedEventObject_GetType();

typedef struct _MouseScrolledEventObject {
    PyObject_HEAD
    MouseScrolledEvent native_event = MouseScrolledEvent({ 0, 0 }, 0.0f);
} MouseScrolledEventObject;

PyTypeObject& MouseScrolledEventObject_GetType();
