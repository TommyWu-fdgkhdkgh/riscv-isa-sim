#include "abstract_device.h"
#include "processor.h"

// this class represents one instruction set simulator in a RISC-V machine.
class iss : public abstract_device_t
{
public:
  iss();
  ~iss();

private:
  void register_insn(insn_desc_t desc);
  void register_base_instructions();
};
