#ifndef NXDK_PGRAPH_TESTS_2D_PRIMITIVE_TESTS_H
#define NXDK_PGRAPH_TESTS_2D_PRIMITIVE_TESTS_H

#include <pbkit/pbkit.h>

#include "test_suite.h"

class TestHost;

class TwoDPrimitiveTests : public TestSuite {
 public:
  struct BlitTest {
    uint32_t blit_operation;
    uint32_t buffer_color_format;
    uint32_t beta;
  };

 public:
  TwoDPrimitiveTests(TestHost& host, std::string output_dir);

  std::string Name() override { return "2D Primitive"; }
  void Initialize() override;

 private:
  void Test(const BlitTest& test);

  static std::string MakeTestName(const BlitTest& test);

  struct s_CtxDma solid_lin_ctx_ {};
  struct s_CtxDma surface_destination_ctx_ {};
};

#endif  // NXDK_PGRAPH_TESTS_2D_PRIMITIVE_TESTS_H
