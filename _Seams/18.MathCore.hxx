// Copyright AStarship <https://astarship.net>.
#if _Seam >= _CrabsMathCore
#include "../Matrix.hpp"
#if _Seam == _CrabsMathCore
#include "../_Debug.h"
#else
#include "../_Release.h"
#endif
namespace CRTest {
using namespace ::_;

template<MTX_A>
void TestMatrixIntegers() {
  //_DLinef("\n\n+---\n| Testing AMatrix<IS" << CSizef<ISZ>() << ">\n+---");
  _DLinef("\n\n+---\n| Testing AMatrix<IS");
  // << ASizeCodef<ISZ>() << Linef(">\n+---");

  static const ISC Expected2x2x2[2][2][2] = {{{0, 1}, {2, 3}},
                                             {{4, 5}, {6, 7}}};
  AMatrix<MTX_P, TDim<ISZ, IUY, 2, 2, 2>> matrix(Expected2x2x2[0][0][0]);
  _DROutOBJ(matrix);
  ISC w = 0;
  T* array_base = matrix.Start();
  for (ISC x = 0; x < 2; ++x) {
    for (ISC y = 0; y < 2; ++y) {
      for (ISC z = 0; z < 2; ++z) {
        _AAvow(w++, Expected2x2x2[x][y][z]);
      }
    }
  }
}

/* Tests the C++11 MultiplyVals compile-time product helper. */
void TestMultiplyVals() {
  _DLinef("\n\n+---\n| Testing MultiplyVals C++11 compile-time product\n+---");

  // Single dimension.
  _AAvow(ISC(5), ISC(MultiplyVals<ISC, IUC, 5>::value));

  // Two dimensions.
  _AAvow(ISC(6), ISC(MultiplyVals<ISC, IUC, 2, 3>::value));

  // Three dimensions.
  _AAvow(ISC(24), ISC(MultiplyVals<ISC, IUC, 2, 3, 4>::value));

  // TDimC CBytes constant.
  //_AAvow(ISD(8), ISD(TDimC<2, 2, 2>::CBytes));
  //_AAvow(ISD(12), ISD(TDimC<3, 4>::CBytes));
  //_AAvow(ISD(60), ISD(TDimC<3, 4, 5>::CBytes));
}

/* Tests the dot product of two vectors. */
void TestDotProduct() {
  _DLinef("\n\n+---\n| Testing TMatrixDotProduct\n+---");

  // Integer dot product.
  {
    const ISC a[] = {1, 2, 3, 4};
    const ISC b[] = {5, 6, 7, 8};
    // 1*5 + 2*6 + 3*7 + 4*8 = 5 + 12 + 21 + 32 = 70
    ISC result = TMatrixDotProduct<ISC>(a, b, 4);
    _AAvow(ISC(70), result);
  }
  // Float dot product.
  {
    const FPC a[] = {1.0f, 2.0f, 3.0f};
    const FPC b[] = {4.0f, 5.0f, 6.0f};
    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    FPC result = TMatrixDotProduct<FPC>(a, b, 3);
    _AAvow(FPC(32.0f), result);
  }
  // Double dot product.
  {
    const FPD a[] = {0.5, 1.5, 2.5};
    const FPD b[] = {2.0, 3.0, 4.0};
    // 0.5*2 + 1.5*3 + 2.5*4 = 1 + 4.5 + 10 = 15.5
    FPD result = TMatrixDotProduct<FPD>(a, b, 3);
    _AAvow(FPD(15.5), result);
  }
}

/* Tests element-wise addition. */
void TestMatrixAdd() {
  _DLinef("\n\n+---\n| Testing TMatrixAdd\n+---");

  const FPD a[] = {1.0, 2.0, 3.0, 4.0};
  const FPD b[] = {5.0, 6.0, 7.0, 8.0};
  FPD result[4];
  TMatrixAdd<FPD>(result, a, b, 4);
  _AAvow(FPD(6.0),  result[0]);
  _AAvow(FPD(8.0),  result[1]);
  _AAvow(FPD(10.0), result[2]);
  _AAvow(FPD(12.0), result[3]);
}

/* Tests element-wise subtraction. */
void TestMatrixSubtract() {
  _DLinef("\n\n+---\n| Testing TMatrixSubtract\n+---");

  const FPD a[] = {10.0, 20.0, 30.0};
  const FPD b[] = {1.0, 2.0, 3.0};
  FPD result[3];
  TMatrixSubtract<FPD>(result, a, b, 3);
  _AAvow(FPD(9.0),  result[0]);
  _AAvow(FPD(18.0), result[1]);
  _AAvow(FPD(27.0), result[2]);
}

/* Tests element-wise multiplication (Hadamard product). */
void TestMatrixMultiplyElementwise() {
  _DLinef("\n\n+---\n| Testing TMatrixMultiplyElementwise\n+---");

  const FPD a[] = {2.0, 3.0, 4.0};
  const FPD b[] = {5.0, 6.0, 7.0};
  FPD result[3];
  TMatrixMultiplyElementwise<FPD>(result, a, b, 3);
  _AAvow(FPD(10.0), result[0]);
  _AAvow(FPD(18.0), result[1]);
  _AAvow(FPD(28.0), result[2]);
}

/* Tests scalar multiplication. */
void TestMatrixScalarMultiply() {
  _DLinef("\n\n+---\n| Testing TMatrixScalarMultiply\n+---");

  const FPD a[] = {1.0, 2.0, 3.0, 4.0};
  FPD result[4];
  TMatrixScalarMultiply<FPD>(result, a, FPD(3.0), 4);
  _AAvow(FPD(3.0),  result[0]);
  _AAvow(FPD(6.0),  result[1]);
  _AAvow(FPD(9.0),  result[2]);
  _AAvow(FPD(12.0), result[3]);
}

/* Tests matrix transpose for a 2x3 -> 3x2 matrix. */
void TestMatrixTranspose() {
  _DLinef("\n\n+---\n| Testing TMatrixTranspose\n+---");

  // 2x3 row-major matrix:
  // | 1  2  3 |
  // | 4  5  6 |
  const FPD src[] = {1.0, 2.0, 3.0,
                     4.0, 5.0, 6.0};
  FPD dst[6];
  TMatrixTranspose<FPD>(dst, src, 2, 3);
  // Expected 3x2:
  // | 1  4 |
  // | 2  5 |
  // | 3  6 |
  _AAvow(FPD(1.0), dst[0]);
  _AAvow(FPD(4.0), dst[1]);
  _AAvow(FPD(2.0), dst[2]);
  _AAvow(FPD(5.0), dst[3]);
  _AAvow(FPD(3.0), dst[4]);
  _AAvow(FPD(6.0), dst[5]);
}

/* Tests general matrix multiplication (GEMM). */
void TestMatrixMultiply() {
  _DLinef("\n\n+---\n| Testing TMatrixMultiply (GEMM)\n+---");

  // A (2x3):
  // | 1  2  3 |
  // | 4  5  6 |
  const FPD a[] = {1.0, 2.0, 3.0,
                   4.0, 5.0, 6.0};
  // B (3x2):
  // | 7   8  |
  // | 9   10 |
  // | 11  12 |
  const FPD b[] = {7.0,  8.0,
                   9.0,  10.0,
                   11.0, 12.0};
  // C = A * B  (2x2):
  // | 1*7+2*9+3*11   1*8+2*10+3*12 |   | 58   64  |
  // | 4*7+5*9+6*11   4*8+5*10+6*12 |   | 139  154 |
  FPD c[4];
  TMatrixMultiply<FPD>(c, a, b, 2, 3, 2);
  _AAvow(FPD(58.0),  c[0]);
  _AAvow(FPD(64.0),  c[1]);
  _AAvow(FPD(139.0), c[2]);
  _AAvow(FPD(154.0), c[3]);

  // Identity matrix multiplication: A * I = A.
  const FPD eye[] = {1.0, 0.0, 0.0,
                     0.0, 1.0, 0.0,
                     0.0, 0.0, 1.0};
  FPD ai[6];
  TMatrixMultiply<FPD>(ai, a, eye, 2, 3, 3);
  for (ISW i = 0; i < 6; ++i)
    _AAvow(a[i], ai[i]);
}

/* Tests ReLU activation and its derivative. */
void TestMatrixReLU() {
  _DLinef("\n\n+---\n| Testing TMatrixReLU\n+---");

  FPD x[] = {-3.0, -1.0, 0.0, 0.5, 2.0};
  TMatrixReLU<FPD>(x, 5);
  _AAvow(FPD(0.0), x[0]);
  _AAvow(FPD(0.0), x[1]);
  _AAvow(FPD(0.0), x[2]);
  _AAvow(FPD(0.5), x[3]);
  _AAvow(FPD(2.0), x[4]);

  // Test derivative.
  FPD pre[] = {-3.0, -1.0, 0.0, 0.5, 2.0};
  FPD grad[5];
  TMatrixReLUDerivative<FPD>(grad, pre, 5);
  _AAvow(FPD(0.0), grad[0]);
  _AAvow(FPD(0.0), grad[1]);
  _AAvow(FPD(0.0), grad[2]);
  _AAvow(FPD(1.0), grad[3]);
  _AAvow(FPD(1.0), grad[4]);

  // Integer ReLU.
  ISC xi[] = {-5, -1, 0, 3, 7};
  TMatrixReLU<ISC>(xi, 5);
  _AAvow(ISC(0), xi[0]);
  _AAvow(ISC(0), xi[1]);
  _AAvow(ISC(0), xi[2]);
  _AAvow(ISC(3), xi[3]);
  _AAvow(ISC(7), xi[4]);
}

/* Tests sigmoid activation and its derivative. */
void TestMatrixSigmoid() {
  _DLinef("\n\n+---\n| Testing TMatrixSigmoid\n+---");

  // sigmoid(0) should be 0.5.
  FPD x[] = {0.0};
  TMatrixSigmoid(x, 1);
  // Allow small floating-point tolerance.
  FPD diff = x[0] - 0.5;
  if (diff < 0.0) diff = -diff;
  _DAssert(diff < 1e-10);

  // sigmoid of large positive should be ~1.0.
  FPD big[] = {100.0};
  TMatrixSigmoid(big, 1);
  _DAssert(big[0] > 0.999);

  // sigmoid of large negative should be ~0.0.
  FPD small[] = {-100.0};
  TMatrixSigmoid(small, 1);
  _DAssert(small[0] < 0.001);

  // Test derivative: s*(1-s) at s=0.5 should be 0.25.
  FPD s_out[] = {0.5};
  FPD s_grad[1];
  TMatrixSigmoidDerivative(s_grad, s_out, 1);
  FPD d2 = s_grad[0] - 0.25;
  if (d2 < 0.0) d2 = -d2;
  _DAssert(d2 < 1e-10);
}

/* Tests softmax activation. */
void TestMatrixSoftmax() {
  _DLinef("\n\n+---\n| Testing TMatrixSoftmax\n+---");

  // Softmax of equal values should give uniform distribution.
  FPD uniform[] = {1.0, 1.0, 1.0};
  TMatrixSoftmax(uniform, 3);
  FPD expected_uniform = 1.0 / 3.0;
  for (ISW i = 0; i < 3; ++i) {
    FPD diff = uniform[i] - expected_uniform;
    if (diff < 0.0) diff = -diff;
    _DAssert(diff < 1e-10);
  }

  // Softmax outputs should sum to 1.0.
  FPD vals[] = {1.0, 2.0, 3.0, 4.0};
  TMatrixSoftmax(vals, 4);
  FPD sum = TMatrixSum<FPD>(vals, 4);
  FPD sum_diff = sum - 1.0;
  if (sum_diff < 0.0) sum_diff = -sum_diff;
  _DAssert(sum_diff < 1e-10);

  // Largest input should have largest output.
  _DAssert(vals[3] > vals[2]);
  _DAssert(vals[2] > vals[1]);
  _DAssert(vals[1] > vals[0]);
}

/* Tests Mean Squared Error. */
void TestMatrixMSE() {
  _DLinef("\n\n+---\n| Testing TMatrixMSE\n+---");

  // MSE of identical arrays should be 0.
  const FPD a[] = {1.0, 2.0, 3.0};
  _AAvow(FPD(0.0), TMatrixMSE(a, a, 3));

  // MSE of known values.
  const FPD predicted[] = {1.0, 2.0, 3.0};
  const FPD target[]    = {1.5, 2.5, 3.5};
  // (0.25 + 0.25 + 0.25) / 3 = 0.25
  FPD mse = TMatrixMSE(predicted, target, 3);
  FPD mse_diff = mse - 0.25;
  if (mse_diff < 0.0) mse_diff = -mse_diff;
  _DAssert(mse_diff < 1e-10);
}

/* Tests bias addition to a matrix. */
void TestMatrixAddBias() {
  _DLinef("\n\n+---\n| Testing TMatrixAddBias\n+---");

  // 2x3 matrix + 3-element bias.
  FPD m[] = {1.0, 2.0, 3.0,
             4.0, 5.0, 6.0};
  const FPD bias[] = {10.0, 20.0, 30.0};
  TMatrixAddBias<FPD>(m, bias, 2, 3);
  _AAvow(FPD(11.0), m[0]);
  _AAvow(FPD(22.0), m[1]);
  _AAvow(FPD(33.0), m[2]);
  _AAvow(FPD(14.0), m[3]);
  _AAvow(FPD(25.0), m[4]);
  _AAvow(FPD(36.0), m[5]);
}

/* Tests TMatrixFill, TMatrixSum, and TMatrixArgMax. */
void TestMatrixUtilities() {
  _DLinef("\n\n+---\n| Testing TMatrixFill, TMatrixSum, TMatrixArgMax\n+---");

  // Fill.
  FPD x[4];
  TMatrixFill<FPD>(x, 4, FPD(7.0));
  for (ISW i = 0; i < 4; ++i)
    _AAvow(FPD(7.0), x[i]);

  // Sum.
  const FPD s[] = {1.0, 2.0, 3.0, 4.0};
  _AAvow(FPD(10.0), TMatrixSum<FPD>(s, 4));

  // ArgMax.
  const FPD am[] = {0.1, 0.9, 0.3, 0.7};
  _AAvow(ISW(1), TMatrixArgMax<FPD>(am, 4));

  // ArgMax with negative values.
  const FPD am2[] = {-5.0, -1.0, -3.0};
  _AAvow(ISW(1), TMatrixArgMax<FPD>(am2, 3));
}

/* Tests a simple forward pass: Y = ReLU(X * W + b).
This verifies the operations chain together correctly for ML inference. */
void TestForwardPass() {
  _DLinef("\n\n+---\n| Testing ML Forward Pass: Y = ReLU(X*W + b)\n+---");

  // X: 1x3 input row vector.
  const FPD X[] = {1.0, 2.0, 3.0};
  // W: 3x2 weight matrix.
  const FPD W[] = {0.1, 0.4,
                   0.2, 0.5,
                   0.3, 0.6};
  // b: 1x2 bias vector.
  const FPD b[] = {0.1, -10.0};

  // Step 1: Z = X * W  (1x2)
  // Z[0] = 1*0.1 + 2*0.2 + 3*0.3 = 0.1+0.4+0.9 = 1.4
  // Z[1] = 1*0.4 + 2*0.5 + 3*0.6 = 0.4+1.0+1.8 = 3.2
  FPD Z[2];
  TMatrixMultiply<FPD>(Z, X, W, 1, 3, 2);
  FPD z0_diff = Z[0] - 1.4;
  if (z0_diff < 0.0) z0_diff = -z0_diff;
  _DAssert(z0_diff < 1e-10);

  // Step 2: Z = Z + b
  TMatrixAdd<FPD>(Z, Z, b, 2);
  // Z = {1.5, -6.8}

  // Step 3: Y = ReLU(Z)
  TMatrixReLU<FPD>(Z, 2);
  FPD y0_diff = Z[0] - 1.5;
  if (y0_diff < 0.0) y0_diff = -y0_diff;
  _DAssert(y0_diff < 1e-10);
  _AAvow(FPD(0.0), Z[1]);  //< Negative clamped to 0.
}

/* Tests a two-layer forward pass with softmax output.
Simulates: hidden = ReLU(X*W1 + b1), output = softmax(hidden*W2 + b2). */
void TestTwoLayerForwardPass() {
  _DLinef("\n\n+---\n| Testing Two-Layer Forward Pass with Softmax\n+---");

  // X: 1x2 input.
  const FPD X[] = {0.5, -0.3};

  // Layer 1: W1 (2x3), b1 (1x3).
  const FPD W1[] = {0.2, -0.1, 0.4,
                    0.3,  0.5, -0.2};
  const FPD b1[] = {0.0, 0.0, 0.0};

  // hidden = X * W1 + b1  (1x3)
  FPD hidden[3];
  TMatrixMultiply<FPD>(hidden, X, W1, 1, 2, 3);
  TMatrixAdd<FPD>(hidden, hidden, b1, 3);
  TMatrixReLU<FPD>(hidden, 3);

  // Layer 2: W2 (3x2), b2 (1x2).
  const FPD W2[] = {0.1, 0.2,
                    0.3, 0.4,
                    0.5, 0.6};
  const FPD b2[] = {0.0, 0.0};

  // output = softmax(hidden * W2 + b2)  (1x2)
  FPD output[2];
  TMatrixMultiply<FPD>(output, hidden, W2, 1, 3, 2);
  TMatrixAdd<FPD>(output, output, b2, 2);
  TMatrixSoftmax(output, 2);

  // Verify softmax properties: outputs sum to 1 and are in [0,1].
  FPD sum = TMatrixSum<FPD>(output, 2);
  FPD sum_diff = sum - 1.0;
  if (sum_diff < 0.0) sum_diff = -sum_diff;
  _DAssert(sum_diff < 1e-10);
  _DAssert(output[0] >= 0.0 && output[0] <= 1.0);
  _DAssert(output[1] >= 0.0 && output[1] <= 1.0);

  _DROut("\n  Softmax output: [" << output[0] << ", " << output[1] << "]");
}

}  //< namespace CRTest
#endif

namespace CRTest {
static const CHA* MathCore(const CHA* args) {
#if _Seam >= _CrabsMathCore
  _ATestBegin;
  //TestMatrixIntegers<ISA, ISA, IUA, ISA>();
  //TestMatrixIntegers<ISA, ISA, IUA, ISB>();
  //TestMatrixIntegers<ISA, ISA, IUA, ISC>();
  //TestMatrixIntegers<ISA, ISA, IUA, ISD>();
  TestMatrixIntegers<FPC, ISB, ISA, IUA>();
  //TestMatrixIntegers<ISB, ISB, IUA, ISB>();
  //TestMatrixIntegers<ISB, ISB, IUA, ISC>();
  //TestMatrixIntegers<ISB, ISB, IUA, ISD>();
  //TestMatrixIntegers<ISC, ISC, IUB, ISA>();
  //TestMatrixIntegers<ISC, ISC, IUB, ISB>();
  //TestMatrixIntegers<ISC, ISC, IUB, ISC>();
  //TestMatrixIntegers<ISC, ISC, IUB, ISD>();
  //TestMatrixIntegers<ISD, ISC, IUC, ISA>();
  //TestMatrixIntegers<ISD, ISC, IUC, ISB>();
  //TestMatrixIntegers<ISD, ISC, IUC, ISC>();
  //TestMatrixIntegers<ISD, ISC, IUC, ISD>();

  // C++11 compile-time product tests.
  TestMultiplyVals();

  // ML matrix operation tests.
  TestDotProduct();
  TestMatrixAdd();
  TestMatrixSubtract();
  TestMatrixMultiplyElementwise();
  TestMatrixScalarMultiply();
  TestMatrixTranspose();
  TestMatrixMultiply();
  TestMatrixReLU();
  TestMatrixSigmoid();
  TestMatrixSoftmax();
  TestMatrixMSE();
  TestMatrixAddBias();
  TestMatrixUtilities();

  // ML forward pass integration tests.
  TestForwardPass();
  TestTwoLayerForwardPass();
#endif
  return _NilP;
}
}  //< namespace CRTest
