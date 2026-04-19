#pragma once

#define GBE_CLASS_NO_COPY(_class)           \
_class(const _class&) = delete;             \
_class& operator=(const _class&) = delete;

#define GBE_CLASS_NO_MOVE(_class)           \
    _class(_class &&) = delete;             \
    _class &operator=(_class &&) = delete;

#define GBE_CLASS_NO_COPY_NO_MOVE(_class)   \
    GBE_CLASS_NO_COPY(_class);              \
    GBE_CLASS_NO_MOVE(_class);              

    