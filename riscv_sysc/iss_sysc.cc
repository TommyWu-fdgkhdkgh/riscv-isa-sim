#include "iss_sysc.h"

iss::iss()
{

}

void iss::register_insn(insn_desc_t desc)
{
}

void iss::register_base_instructions()
{
  #define DECLARE_INSN(name, match, mask) \
    insn_bits_t name##_match = (match), name##_mask = (mask); \
    bool name##_supported = true;

  #include "encoding.h"
  #undef DECLARE_INSN

  #define DECLARE_OVERLAP_INSN(name, ext) { name##_supported = isa->extension_enabled(ext); }
  #include "overlap_list.h"
  #undef DECLARE_OVERLAP_INSN

  #define DEFINE_INSN(name) \
    extern reg_t fast_rv32i_##name(processor_t*, insn_t, reg_t); \
    extern reg_t fast_rv64i_##name(processor_t*, insn_t, reg_t); \
    extern reg_t fast_rv32e_##name(processor_t*, insn_t, reg_t); \
    extern reg_t fast_rv64e_##name(processor_t*, insn_t, reg_t); \
    extern reg_t logged_rv32i_##name(processor_t*, insn_t, reg_t); \
    extern reg_t logged_rv64i_##name(processor_t*, insn_t, reg_t); \
    extern reg_t logged_rv32e_##name(processor_t*, insn_t, reg_t); \
    extern reg_t logged_rv64e_##name(processor_t*, insn_t, reg_t); \
    if (name##_supported) { \
      register_insn((insn_desc_t) { \
        name##_match, \
        name##_mask, \
        fast_rv32i_##name, \
        fast_rv64i_##name, \
        fast_rv32e_##name, \
        fast_rv64e_##name, \
        logged_rv32i_##name, \
        logged_rv64i_##name, \
        logged_rv32e_##name, \
        logged_rv64e_##name}); \
    }
  #include "insn_list.h"
  #undef DEFINE_INSN
}

iss::~iss()
{
}
