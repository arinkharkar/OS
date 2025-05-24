#include "gdt.h"
#include "x86.h"


namespace kernel {
namespace x86 {
    GDT_full gdt_full = {0};
    GDT::GDT() {
        m_pgdt_descriptor = new GdtDescriptor64;
        m_pgdt_entries = new GdtSegment64[m_num_gdt_entries];
        // if we cannot enable the GDT, we need to hang the CPU
        if (m_pgdt_descriptor == nullptr || m_pgdt_entries == nullptr)
            PANIC("Failed to Create GDT");
    }
    result GDT::enable() {
        // the first entry needs to be a zero entry
        if (create_zero_entry(0) == result::error)
            return result::error;
        // the next 2 entries span the whole address space, the base and limit values are irrelavant here as x86_64 always has these span the 18.4 exabytes (LOL)
        if (create_entry(1, 0, UINT32_MAX, PermissionLevel::kernel_only, SegmentType::code, ReadWritePermissions::readable) == result::error)
            return result::error;
        if (create_entry(2, 0, UINT32_MAX, PermissionLevel::kernel_only, SegmentType::data, ReadWritePermissions::writeable) == result::error)
            return result::error;
        gdt_full.null = m_pgdt_entries[0];
        gdt_full.kernel_code = m_pgdt_entries[1];
        gdt_full.kernel_data = m_pgdt_entries[2];
        m_pgdt_descriptor->offset = reinterpret_cast<uint64_t>(&gdt_full);
        // the size needs to be subtracted by 1
        m_pgdt_descriptor->size = sizeof(GDT_full) - 1;
        printk("before lgdt...\r\n");
        printk("\r\ngdt entry %d: %x\r\n", 1, gdt_full.kernel_code);
        printk("\r\ngdt entry %d: %x\r\n", 2, gdt_full.kernel_data);
   //     printk("GDT entry: %u\r\n%u\r\n%u", m_pgdt_entries[0], m_pgdt_entries[1], m_pgdt_entries[2]);
        __load_gdt(m_pgdt_descriptor, &&afater_gdt);
        afater_gdt:
        printk("after lgdt...");
        return result::success;
    }

    result GDT::create_entry(int gdt_offset, size_t base, size_t limit, PermissionLevel permission_level, SegmentType segment_type, ReadWritePermissions permissions) {
        // if the offset is greater than the number of entries we initialized, error
        if (gdt_offset >= m_num_gdt_entries)
            return result::error;
        if (base > limit)
            return result::error;
        // the limit value is 20 bits, this value needs to be bitshifted by 12
        limit = limit >> 12;
        /* set the base values*/
        m_pgdt_entries[gdt_offset].base_16_bits = (uint16_t)base;
        m_pgdt_entries[gdt_offset].base_8_bits = (uint8_t)(base >> 16);
        m_pgdt_entries[gdt_offset].base_last_bits = (uint8_t)(base >> 24);
        
        /* set the limit values */
        m_pgdt_entries[gdt_offset].limit_16_bits = (uint16_t)limit;
        m_pgdt_entries[gdt_offset].limit_and_flags = (uint8_t)(limit >> 16);


        m_pgdt_entries[gdt_offset].access_byte = create_access_byte(permission_level, segment_type, permissions);
        printk("Access Byte: %x\r\n", m_pgdt_entries[gdt_offset].access_byte_raw);
        m_pgdt_entries[gdt_offset].limit_and_flags |= (0b1010 << 4);
      //  m_pgdt_entries[gdt_offset].flags = static_cast<uint8_t>(Flags::long_mode | Flags::granularity);
        
        return result::success;
    }

    EXTERN_C void after_gdt() {
        printk("here!");
        while (1) {}
    }

    result GDT::create_zero_entry(int gdt_offset) {
        if (gdt_offset >= m_num_gdt_entries)
            return result::error;
        uint32_t base = 0;
        uint32_t limit = 0;
        m_pgdt_entries[gdt_offset].base_16_bits = (uint16_t)base;
        m_pgdt_entries[gdt_offset].base_8_bits = (uint8_t)(base >> 16);
        m_pgdt_entries[gdt_offset].base_last_bits = (uint8_t)(base >> 24);

        /* set the limit values */
        m_pgdt_entries[gdt_offset].limit_16_bits = (uint16_t)limit;
        m_pgdt_entries[gdt_offset].limit_and_flags = (uint16_t)limit;


        m_pgdt_entries[gdt_offset].access_byte_raw = 0;
        m_pgdt_entries[gdt_offset].limit_and_flags = 0;
        return result::success;
    }

    AccessByte GDT::create_access_byte(PermissionLevel permission_level, SegmentType segment_type, ReadWritePermissions permissions) {
        AccessByte access_byte = { 0 };
        access_byte.accesed_bit = 0;
        access_byte.present = 1;
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

            access_byte.direction_conform_bit = 0;

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
        delete m_pgdt_descriptor;
        delete[] m_pgdt_entries;
    }


}
}