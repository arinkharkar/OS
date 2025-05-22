#pragma once
#include <stdint.h>
#include <stddef.h>
#include <fermion.h>
#include "x86.h"



namespace kernel {

namespace gdt {
/*
 The GDT is set up super weirdly to maintain backwards compatibility, sadly this makes our lives a bit harder
 Even more weirdly, the 'limit' value (how far from the base this gdt segment is valid) is only 20 bits long, so you must specify you want to have 4kb granularity 
 with the granularity so that the max limit * 4kb = 4.2 billion  
*/


typedef struct {
    // the CPU sets this to 1 when accessed, just set this to 1 by default
    uint8_t accesed_bit : 1;
    // if this is a code segment, whether reading from this segment is allowed (1) or not (0)
    // if this is a data segment, whether writing to this segment is allowed (1) or not (0)
    uint8_t rw_bit : 1;
    // if this is a code segment, whether the code can be executed from a lower DPL level (1) or not (0)
    // if this is a data segment, whether the segment grows up (1) or down (0) (want this to be set)
    uint8_t direction_conform_bit : 1;
    // if set, this is a code segment, if not, this is a data segment
    uint8_t executable_bit : 1;
    // if set, this represents a code or data segment, otherwise represents a TSS segment
    uint8_t descriptor_bit : 1;
    // priviledge level bits
    uint8_t dpl : 2;
    // present bit, must be set 
    uint8_t present : 1;
} __attribute__((packed)) AccessByte;


typedef struct {
    // first 16 bits of the limit
    uint16_t limit_16_bits;
    // first 16 bits of the base
    uint16_t base_16_bits;
    // next 8 bits of the base
    uint8_t  base_8_bits;
    // 8 bits for the access
    union {
    AccessByte access_byte;
    uint8_t access_byte_raw;
    };
    //first 4 bits are the last part of the limit, last 4 bits are the flags
    uint8_t limit : 4;
    uint8_t flags : 4;
    // last 8 bits of the base
    uint8_t base_last_bits;

} __attribute__((packed)) GdtSegment64;



// when calling lgdt, we pass base as the start of our gdt and limit as the size of it
typedef struct {
    // size of the GDT, in bytes minus 1
    uint16_t size;
    // the virtual address of the GDT
    uint64_t offset;
} __attribute__((packed)) GdtDescriptor64;

// the permission levels for each GDT entry, not really needed
enum class PermissionLevel {
    kernel_only,
    all
};

// the type of data stored in the gdt entry
enum class SegmentType {
    data,
    code,
    tss
};

// whether the gdt entry can be read or written to
enum class ReadWritePermissions {
    readable,
    writeable,
    none
};

enum class Flags : uint8_t {
    none         = 0,
    reserved     = 1 << 0,
    long_mode    = 1 << 1,
    size         = 1 << 2,
    granularity  = 1 << 3
};

// the or bitflag mapped to the enum class
inline Flags operator|(Flags a, Flags b) {
    return static_cast<Flags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline Flags operator&(Flags a, Flags b) {
    return static_cast<uint8_t>(a) & static_cast<uint8_t>(b) ? b : Flags::none;
}


EXTERN_C void __load_gdt(GdtDescriptor64* descriptor);

class GDT {
private:
    // the number of GDT entries we are using
    const int m_num_gdt_entries = 3;
    GdtDescriptor64* m_pgdt_descriptor;
    GdtSegment64* m_pgdt_entries;

    AccessByte create_access_byte(PermissionLevel permission_level, SegmentType segment_type, ReadWritePermissions permissions);

    // Zeroes out a gdt entry
    result create_zero_entry(int gdt_offset);

    result create_entry(int gdt_offset, size_t base, size_t limit, PermissionLevel permission_level, SegmentType segment_type, ReadWritePermissions permissions);

public:
    GDT();

    result enable();

    ~GDT();

};

}

}