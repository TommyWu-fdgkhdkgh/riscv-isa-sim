#ifndef _RISCV_SYSC_ISS_H
#define _RISCV_SYSC_ISS_H

#include "abstract_device.h"
#include "processor.h"
#include <vector>

// this class represents one instruction set simulator in a RISC-V machine.
class iss : public abstract_device_t
{
public:
  //iss(const isa_parser_t *isa);
  iss(const char *isa_str, const char *priv_str);
  ~iss();

private:
  // Decide this iss should support which isa-string.
  const isa_parser_t *isa; 
  std::vector<insn_desc_t> instructions;
  static const size_t OPCODE_CACHE_SIZE = 8191;
  insn_desc_t opcode_cache[OPCODE_CACHE_SIZE];

  void register_insn(insn_desc_t desc);
  void build_opcode_map();
  void register_base_instructions();
};

#endif
