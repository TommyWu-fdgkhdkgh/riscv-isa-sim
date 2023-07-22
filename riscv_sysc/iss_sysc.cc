#include "iss_sysc.h"

iss::iss(const char *isa_str, const char *priv_str)
{
  isa = new isa_parser_t(isa_str, priv_str);
}

void iss::register_insn(insn_desc_t desc)
{
  assert(desc.fast_rv32i && desc.fast_rv64i && desc.fast_rv32e && desc.fast_rv64e &&
         desc.logged_rv32i && desc.logged_rv64i && desc.logged_rv32e && desc.logged_rv64e);

  instructions.push_back(desc);
}

void iss::build_opcode_map()
{
  struct cmp {
    bool operator()(const insn_desc_t& lhs, const insn_desc_t& rhs) {
      if (lhs.match == rhs.match)
        return lhs.mask > rhs.mask;
      return lhs.match > rhs.match;
    }
  };
  std::sort(instructions.begin(), instructions.end(), cmp());

  for (size_t i = 0; i < OPCODE_CACHE_SIZE; i++) {
    opcode_cache[i] = insn_desc_t::illegal();
  }
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
    extern reg_t fast_rv32i_##name_sysc(processor_t*, insn_t, reg_t); \
    extern reg_t fast_rv64i_##name_sysc(processor_t*, insn_t, reg_t); \
    extern reg_t fast_rv32e_##name_sysc(processor_t*, insn_t, reg_t); \
    extern reg_t fast_rv64e_##name_sysc(processor_t*, insn_t, reg_t); \
    extern reg_t logged_rv32i_##name_sysc(processor_t*, insn_t, reg_t); \
    extern reg_t logged_rv64i_##name_sysc(processor_t*, insn_t, reg_t); \
    extern reg_t logged_rv32e_##name_sysc(processor_t*, insn_t, reg_t); \
    extern reg_t logged_rv64e_##name_sysc(processor_t*, insn_t, reg_t); \
    if (name##_supported) { \
      register_insn((insn_desc_t) { \
        name##_match, \
        name##_mask, \
        fast_rv32i_##name_sysc, \
        fast_rv64i_##name_sysc, \
        fast_rv32e_##name_sysc, \
        fast_rv64e_##name_sysc, \
        logged_rv32i_##name_sysc, \
        logged_rv64i_##name_sysc, \
        logged_rv32e_##name_sysc, \
        logged_rv64e_##name_sysc}); \
    }
  #include "insn_list.h"
  #undef DEFINE_INSN

  // terminate instruction list with a catch-all
  register_insn(insn_desc_t::illegal());

  build_opcode_map();
}

iss::~iss()
{
    delete isa;
}
