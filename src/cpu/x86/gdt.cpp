#include "gdt.h"

namespace kernel {
namespace gdt {

    GDT::GDT() {
        p_gdt_descriptor = new GdtDescriptor64;
        p_gdt_entries = new GdtSegment64[num_gdt_entries];
        // if we cannot enable the GDT, we need to hang the CPU
        if (p_gdt_descriptor == nullptr || p_gdt_entries == nullptr)
            PANIC("Failed to Create GDT");
    }
    result GDT::enable() {
        // the first entry needs to be a zero entry
        create_zero_entry(0);
        // the next to span the whole address space, the base and limit values are irrelavant here as x86_64 always has these span the 18.4 exabytes (LOL)
        create_entry(1, 0, UINT32_MAX, PermissionLevel::kernel_only, SegmentType::code, ReadWritePermissions::readable);
        create_entry(2, 0, UINT32_MAX, PermissionLevel::kernel_only, SegmentType::data, ReadWritePermissions::writeable);
    }

    result GDT::create_entry(int gdt_offset, size_t base, size_t limit, PermissionLevel permission_level, SegmentType segment_type, ReadWritePermissions permissions) {
        // if the offset is greater than the number of entries we initialized, error
        if (gdt_offset >= num_gdt_entries)
            return result::result_error;
        if (base > limit)
            return result::result_error;
        // the limit value is 20 bits, this value needs to be bitshifted by 12
        limit = limit >> 12;
        /* set the base values*/
        p_gdt_entries[gdt_offset].base_16_bits = (uint16_t)base;
        p_gdt_entries[gdt_offset].base_8_bits = (uint8_t)(base >> 16);
        p_gdt_entries[gdt_offset].base_last_bits = (uint8_t)(base >> 24);

        /* set the limit values */
        p_gdt_entries[gdt_offset].limit_16_bits = (uint16_t)limit;
        p_gdt_entries[gdt_offset].limit = (uint16_t)limit;


        p_gdt_entries[gdt_offset].access_byte = create_access_byte(permission_level, segment_type, permissions);
        p_gdt_entries[gdt_offset].flags = 0;
        p_gdt_entries[gdt_offset].flags = static_cast<uint8_t>(Flags::long_mode | Flags::granularity);

        return result::result_success;
    }

    AccessByte GDT::create_access_byte(PermissionLevel permission_level, SegmentType segment_type, ReadWritePermissions permissions) {
        AccessByte access_byte = { 0 };
        access_byte.accesed_bit = 1;

        if (segment_type == SegmentType::code) {
            // since this is either a code or data segment, set this
            access_byte.descriptor_bit = 1;

            if (permissions == ReadWritePermissions::readable)
                access_byte.rw_bit = 1;
            else
                access_byte.rw_bit = 0;

            if (permission_level == PermissionLevel::kernel_only) {
                access_byte.direction_conform_bit = 0;
                access_byte.dpl = static_cast<uint8_t>(DPL::kernel_mode);
            } else {
                access_byte.direction_conform_bit = 1;
                access_byte.dpl = static_cast<uint8_t>(DPL::user_mode);
            }
            // since this is a data segment, clear this
            access_byte.executable_bit = 1;
        } else if (segment_type == SegmentType::data) {
            // since this is either a code or data segment, set this
            access_byte.descriptor_bit = 1;

            if (permissions == ReadWritePermissions::writeable)
                access_byte.rw_bit = 1;
            else
                access_byte.rw_bit = 0;

            access_byte.direction_conform_bit = 1;

            if (permission_level == PermissionLevel::kernel_only) {
                access_byte.dpl = static_cast<uint8_t>(DPL::kernel_mode);
            } else {
                access_byte.dpl = static_cast<uint8_t>(DPL::user_mode);
            }

            // since this is a data segment, clear this
            access_byte.executable_bit = 0;
        } else if (segment_type == SegmentType::tss) {
            // TODO: Implement TSS

            // since this is neither a code nor data segment, unset this
            access_byte.descriptor_bit = 0;
        }

        return access_byte;
    }


    GDT::~GDT() {
        delete p_gdt_descriptor;
        delete[] p_gdt_entries;
    }


}
}