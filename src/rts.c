#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <rts/rts.h>

#define RTS_MAX(lhs, rhs) (((lhs) > (rhs))? (lhs) : (rhs))

#define RTS_TYPEDEF(name, type)                     \
    struct _struct_align_##name {                   \
        char c;                                     \
        type x;                                     \
    };                                              \
    RtsType RTS_TYPE_##name = {                     \
        RTS_TYPE_TAG_##name,                        \
        offsetof(struct _struct_align_##name, x),   \
        sizeof(type),                               \
        NULL, NULL                                  \
    }                                               \

RTS_TYPEDEF(SINT, signed int);
RTS_TYPEDEF(UINT, unsigned int);
RTS_TYPEDEF(CHAR, char);
RTS_TYPEDEF(UCHAR, unsigned char);
RTS_TYPEDEF(SCHAR, signed char);
RTS_TYPEDEF(USHORT, unsigned short);
RTS_TYPEDEF(ULONG, unsigned long);
RTS_TYPEDEF(SLONG, signed long);
RTS_TYPEDEF(ULONGLONG, unsigned long long);
RTS_TYPEDEF(SLONGLONG, signed long long);
RTS_TYPEDEF(FLOAT, float);
RTS_TYPEDEF(DOUBLE, double);
RTS_TYPEDEF(LONGDOUBLE, long double);
RTS_TYPEDEF(UINT8, uint8_t);
RTS_TYPEDEF(SINT8, int8_t);
RTS_TYPEDEF(UINT16, uint16_t);
RTS_TYPEDEF(SINT16, int16_t);
RTS_TYPEDEF(UINT32, uint32_t);
RTS_TYPEDEF(SINT32, int32_t);
RTS_TYPEDEF(UINT64, uint64_t);
RTS_TYPEDEF(SINT64, int64_t);
RTS_TYPEDEF(POINTER, void *);

RtsStatus rts_type_init(RtsType *type) {
    if (type == NULL) {
        return RTS_STATUS_BAD_TYPEDEF;
    }
    bool isUnion = type->tag == RTS_TYPE_TAG_UNION;
    if (type->tag != RTS_TYPE_TAG_STRUCT && !isUnion) {
        return RTS_STATUS_OK;
    }

    RtsType **elements = type->elements;
    if (elements == NULL) {
        return RTS_STATUS_BAD_TYPEDEF;
    }

    size_t i = 0;
    size_t offset = 0;
    size_t max_align = 0;
    RtsType *element = elements[i];
    if (element == NULL) { // Empty struct
        return RTS_STATUS_BAD_TYPEDEF;
    }
    while (element != NULL) { // Align each element
        if (rts_type_init(element) != RTS_STATUS_OK) {
            return RTS_STATUS_BAD_TYPEDEF;
        }
        size_t alignment = element->alignment;
        max_align = RTS_MAX(max_align, alignment);
        if (!isUnion) {
            size_t remainder = offset % alignment;
            size_t padding = remainder ? alignment - remainder : 0;
            offset += padding;
            type->offsets[i] = offset;
            offset += element->size;
        }
        i++;
        element = elements[i];
    }

    // Apply trailing padding
    type->alignment = max_align;
    size_t remainder = offset % max_align;
    size_t padding = remainder ? max_align - remainder : 0;
    type->size = offset + padding;
    return RTS_STATUS_OK;
}

RtsStatus rts_init_from_string(RtsType **type, const char *str, size_t len) {
    if (type == NULL || str == NULL || len == 0) {
        return RTS_STATUS_BAD_TYPEDEF;
    }
    return RTS_STATUS_OK;
}
