// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_MATHCORE_HPP
#define CRABS_MATHCORE_HPP
#include <_Config.h>
#if SEAM >= CRABS_MATHCORE
#include "Stack.hpp"
#define MTX_A typename T = ISW, typename ISZ = ISC, typename ISY = ISB, \
  typename IUY = IUB
#define MTX_P T, ISZ, ISY, IUY
#define MTX TMatrix<T, ISZ, ISY, IUY>
namespace _ {

/* @defgroup AsciiMatrix
Please see the ASCII Data Specification for DRY documentation.
@link ./Spec/Data/vector_types/Matrix.md */

/* An n-dimensional matrix of homogeneous-sized plain-old-data (POD) types.
## Memory Layout

@code
    +-----------------+
    |  C-Style Array  |
    |-----------------|
    | Dimension Stack |
 ^  |-----------------|
 |  |     Header      |
0xN +-----------------+
@endcode
*/
template<MTX_A>
struct TMatrix {
  ISZ bytes,      //< Size in bytes.
      elems;      //< Total number of matrix elements.
  TStack<SCK_P> dimensions; //< Stack of dimensions.
};

/* Max number of elements that can fit in the given ISZ. */
template<MTX_A>
constexpr ISZ TMatrixElementTotal() {
  return (TISMax<ISZ>() - sizeof(TStack<SCK_P>)) / sizeof(T);
}

/* Calculates the size of the Dimensions Stack with the TMatrix header. */
template<MTX_A>
inline ISY TMatrixHeaderSize(ISY dimensions_total) {
  return sizeof(TMatrix<ISY>) + dimensions_total * sizeof(ISY);
}

// Checks if the 
template<MTX_A>
inline ISZ TMatrixArraySizeMax(ISZ dimensions_total) {
  return ~ISW(0) - TMatrixHeaderSize<ISZ>(dimensions_total);
}

/* Prints the TMatrix to the UTF. */
template<typename Printer, MTX_A>
Printer& TMatrixPrint(Printer& o, TMatrix<MTX_P>* matrix) {
  A_ASSERT(matrix);
  ISZ size = matrix->bytes, 
      dimension_size  = matrix->dimensions.total,
      dimension_count = matrix->dimensions.count;

  o << "\n\nArray: size:" << size << " dimensions:{" << dimension_count;

  ISZ *dimensions = TStackBegin<ISZ, ISZ>(&matrix->dimensions),
      *dimensions_end = dimensions + dimension_count - 1;
  ISZ element_count = *dimensions++;

  while (dimensions < dimensions_end) {
    ISZ dimension = *dimensions++;
    element_count *= dimension;
    if (dimensions == dimensions_end)
      o << dimension << '\n';
    else
      o << dimension << ", ";
  }
  o << '}';

  return o;
}

/* Returns the required size of the given array. */
template<MTX_A>
ISZ TMatrixElementCount(const ISZ* dimensions) {
  D_ASSERT(dimensions);
  ISZ dimension_count = *dimensions++, 
      element_count   = *dimensions++;
  while (--dimension_count > 0) {
    ISZ current_dimension = *dimensions++;
    if (current_dimension < 1) return -1;
    element_count *= current_dimension;
  }
  if (element_count > TMatrixArraySizeMax<MTX_P>())
    return -1;
  return element_count;
}

/* Returns the size in bytes of the given matrix dimensions.
@return -1 if the dimension_count or dimensions are < 0, >=0 otherwise. */
template<MTX_A>
ISZ TMatrixSize(const ISZ* dimensions) {
  D_ASSERT(dimensions);
  ISZ dimension_count = *dimensions++;
  D_ASSERT(dimension_count >= 0);
  if (dimension_count <= 0)
    return -1;
  ISZ element_count = *dimensions++;
  while (--dimension_count > 0) {
    ISZ dimension = *dimensions++;
    if (dimension <= 0)
      return -1;
    element_count *= dimension;
  }
  if (element_count > TMatrixArraySizeMax<T, ISZ>()) return -1;
  return sizeof(TStack<SCK_P>) + element_count * sizeof(T);
}

/* Returns the required size of the given array.
@return -1 if the dimension_count or dimensions are < 0, >=0 otherwise. */
template<MTX_A>
ISZ TMatrixSize(const ISZ* dimensions, const ISZ* delta) {
  D_ASSERT(dimensions);
  ISZ dimension_count = *dimensions++;
  if (dimension_count != *delta++) return -1;
  D_ASSERT(dimension_count >= 0);
  if (dimension_count <= 0) return 0;
  ISZ element_count = *dimensions++ + *delta++;
  while (--dimension_count > 0) {
    ISZ dimension = *dimensions++ + *delta++;
    if (dimension <= 0) return -1;
    element_count *= dimension;
  }
  if (element_count > TMatrixArraySizeMax<T, ISZ>()) return -1;
  return sizeof(TStack<SCK_P>) + element_count * sizeof(T);
}

/* Returns the required size of the given array. */
template<MTX_A>
ISZ TMatrixSize(const TMatrix<MTX_P>* matrix) {
  D_ASSERT(matrix);
  return ISZ(sizeof(TMatrix<MTX_P>)) + matrix->bytes * sizeof(T) + 
         matrix->dimensions->total * sizeof(ISZ);
}

template<MTX_A>
inline ISZ TMatrixDimensionTotal(ISZ total) {
  total += sizeof(TStack<SCK_P>) >> CBitCount<ISZ>();
  return TAlignUpArray<ISZ>(total);
}

template<MTX_A, ISZ AlignementMask = sizeof(T) - 1>
ISZ TMatrixSize(ISZ dimension_total, ISZ element_count) {
  ISZ bytes = TMatrixHeaderSize<MTX_P>(dimension_total) +
                   sizeof(T) * element_count;
  return AlignUp(bytes, AlignementMask);
}

/* Initializes an stack of n elements of the given type.
@param socket An stack of bytes large enough to fit the stack. */
template<MTX_A>
TMatrix<MTX_P>* TMatrixInit(const ISZ* dimensions) {
  A_ASSERT(dimensions);
  ISZ dimension_count = *dimensions;
  if (dimension_count < 0) return NILP;
  ISZ size = (ISZ)sizeof(TStack<SCK_P>) + dimension_count * sizeof(T);
  IUW* socket = new IUW[size >> ACPUBytesLog2];
  TStack<SCK_P>* stack = TPtr<TStack<SCK_P>>(socket);
  stack->size_array = 0;
  stack->size_stack = size;
  stack->total = dimension_count;
  stack->count = 0;
  return stack;
}

/* Gets the address of the packed array.
@param obj ASCII Array data structure.
@return Pointer to the first element in the array. */
template<MTX_A>
inline T* TMatrixStart(TMatrix<MTX_P>* obj) {
  return TPtr<T>(TStackBegin<ISZ, ISZ>(&obj->dimensions) + obj->bytes);
}

template<MTX_A>
inline TMatrix<MTX_P>* TMatrixPtr(IUW* origin) {
  return TPtr<TMatrix<MTX_P>>(origin);
}

template<MTX_A>
inline void TMatrixWriteDims(TMatrix<MTX_P>* matrix, const ISZ* dims, 
                             ISZ dim_count) {
  ISZ* cursor = &matrix->dimensions->count;
  *cursor++ = dim_count;
  while (dim_count > 0) *cursor++ = *dims++;
}

template<MTX_A>
inline TMatrix<MTX_P>* TMatrixClone(T* start_a, ISZ dim_count_a, const ISZ* dim_a,
                                  T* start_b, ISZ dim_count_b, const ISZ* dim_b) {
  
}

template<MTX_A>
inline IUW* TMatrixCopy(TMatrix<MTX_P>* destination, const TMatrix<MTX_P>* source) {
  ISZ size_destination = TMatrixSize<MTX_P>(destination),
      size_source      = TMatrixSize<MTX_P>(source);
  if (size_destination < size_source) return NILP;
  /* I'm not sure if I even want to try to copy the dimensions and array blocks
  separately because in practice the dimensions stack size will never be very 
  much.
  ArrayCopy(destination, TStackTop<T, ISZ>(destination->dimensions), 
            source     , TStackTop<T, ISZ>(source->dimensions));
  ArrayCopy(TMatrixStart<MTX_P>(destination), 
            TPtr<CHA>(destination) + SizeDestination, 
            TMatrixStart<MTX_P>(source), 
            TPtr<CHA>(source) + SizeSource); */
  ArrayCopy(destination, TPtr<CHA>(destination) + size_destination, 
            source, TPtr<CHA>(source) + size_source);
  return destination;
}

/* C++11-compatible compile-time product of variadic template values.
Replaces C++17 fold expression: (T(1) * ... * T(N)). */
template<typename ResultT, typename ValT, ValT... Ns>
struct MultiplyVals;

template<typename ResultT, typename ValT, ValT First, ValT... Rest>
struct MultiplyVals<ResultT, ValT, First, Rest...> {
  static const ResultT value =
      ResultT(First) * MultiplyVals<ResultT, ValT, Rest...>::value;
};

template<typename ResultT, typename ValT>
struct MultiplyVals<ResultT, ValT> {
  static const ResultT value = ResultT(1);
};

template<typename ISZ, typename IUY, IUY... N>
struct TDim {
  static const IUY Rank = sizeof...(N);
  static const IUY Dims[Rank];
  static const ISZ CBytes = MultiplyVals<ISZ, IUY, N...>::value;
};

template<typename ISZ, typename IUY, IUY... N>
const IUY TDim<ISZ, IUY, N...>::Dims[TDim<ISZ, IUY, N...>::Rank] = { IUY(N)... };

template<IUA... N>
struct TDimA {
  static const IUA Rank = sizeof...(N);
  static const IUA Dims[Rank];
  static const ISB CBytes = MultiplyVals<ISB, IUA, N...>::value;
};

template<IUB... N>
struct TDimB {
  static const IUB Rank = sizeof...(N);
  static const IUB Dims[Rank];
  static const ISC CBytes = MultiplyVals<ISC, IUB, N...>::value;
};

template<IUB... N>
const IUB TDimB<N...>::Dims[TDimB<N...>::Rank] = { IUB(N)... };

template<IUC... N>
struct TDimC {
  static const IUC Rank = sizeof...(N);
  static const IUC Dims[Rank];
  static const ISD CBytes = MultiplyVals<ISD, IUC, N...>::value;
};

//template<IUA... N>
//const IUA TDimA<IUA, N...>::Dims[TDimA<IUA, N...>::Rank] = { IUA(N)... };
//
//
//template<IUB... N>
//const IUB TDimB<N...>::Dims[TDimB<N...>::Rank] = { IUB(N)... };
//
//template<IUC... N>
//const IUC TDimC<N...>::Dims[TDimC<N...>::Rank] = { IUC(N)... };
//
///* Creates a immutable array of dimensions. */
//template<typename ISZ, const ISZ... N>
//inline const ISZ* TDim() {
//  static const ISZ Count = ISZ(sizeof...(N)), kList[sizeof...(N)] = {N...};
//  return &Count;
//}
//
///* Creates a immutable array of ISB dimensions. */
//template<const ISB... N>
//inline const ISB* TDimB() {
//  static const ISB Count = ISB(sizeof...(N)), kList[sizeof...(N)] = {N...};
//  return &Count;
//}
//
///* Creates a immutable array of ISC dimensions. */
//template<const ISC... N>
//inline const ISC* TDimC() {
//  static const ISC Count = ISC(sizeof...(N)), kList[sizeof...(N)] = {N...};
//  return &Count;
//}
//
///* Creates a static array of ISD dimensions. */
//template<const ISD... N>
//inline const ISD* TDimD() {
//  static const ISD Count = ISD(sizeof...(N)), kList[sizeof...(N)] = {N...};
//  return &Count;
//}

/* ============================================================================
   Machine Learning Matrix Operations (CPU-only)
============================================================================ */

/* Computes the dot product of two 1D vectors of length n.
@param a Pointer to the first vector.
@param b Pointer to the second vector.
@param n Number of elements.
@return The dot product sum(a[i]*b[i]). */
template<typename T>
T TMatrixDotProduct(const T* a, const T* b, ISW n) {
  D_ASSERT(a);
  D_ASSERT(b);
  D_ASSERT(n > 0);
  T result = T(0);
  for (ISW i = 0; i < n; ++i)
    result += a[i] * b[i];
  return result;
}

/* Adds two arrays element-wise: result[i] = a[i] + b[i].
@param result Pointer to the output array.
@param a Pointer to the first input array.
@param b Pointer to the second input array.
@param n Number of elements. */
template<typename T>
void TMatrixAdd(T* result, const T* a, const T* b, ISW n) {
  D_ASSERT(result);
  D_ASSERT(a);
  D_ASSERT(b);
  D_ASSERT(n > 0);
  for (ISW i = 0; i < n; ++i)
    result[i] = a[i] + b[i];
}

/* Subtracts two arrays element-wise: result[i] = a[i] - b[i].
@param result Pointer to the output array.
@param a Pointer to the first input array.
@param b Pointer to the second input array.
@param n Number of elements. */
template<typename T>
void TMatrixSubtract(T* result, const T* a, const T* b, ISW n) {
  D_ASSERT(result);
  D_ASSERT(a);
  D_ASSERT(b);
  D_ASSERT(n > 0);
  for (ISW i = 0; i < n; ++i)
    result[i] = a[i] - b[i];
}

/* Multiplies two arrays element-wise (Hadamard product): result[i] = a[i]*b[i].
@param result Pointer to the output array.
@param a Pointer to the first input array.
@param b Pointer to the second input array.
@param n Number of elements. */
template<typename T>
void TMatrixMultiplyElementwise(T* result, const T* a, const T* b, ISW n) {
  D_ASSERT(result);
  D_ASSERT(a);
  D_ASSERT(b);
  D_ASSERT(n > 0);
  for (ISW i = 0; i < n; ++i)
    result[i] = a[i] * b[i];
}

/* Multiplies every element of an array by a scalar: result[i] = a[i]*scalar.
@param result Pointer to the output array.
@param a Pointer to the input array.
@param scalar The scalar value.
@param n Number of elements. */
template<typename T>
void TMatrixScalarMultiply(T* result, const T* a, T scalar, ISW n) {
  D_ASSERT(result);
  D_ASSERT(a);
  D_ASSERT(n > 0);
  for (ISW i = 0; i < n; ++i)
    result[i] = a[i] * scalar;
}

/* Transposes a row-major 2D matrix.
@param result Output array of size cols*rows (transposed dimensions).
@param source Input array of size rows*cols.
@param rows Number of rows in the source matrix.
@param cols Number of columns in the source matrix. */
template<typename T>
void TMatrixTranspose(T* result, const T* source, ISW rows, ISW cols) {
  D_ASSERT(result);
  D_ASSERT(source);
  D_ASSERT(rows > 0);
  D_ASSERT(cols > 0);
  for (ISW r = 0; r < rows; ++r)
    for (ISW c = 0; c < cols; ++c)
      result[c * rows + r] = source[r * cols + c];
}

/* General matrix multiplication (GEMM): C = A * B.
A is (rows_a x cols_a), B is (cols_a x cols_b), C is (rows_a x cols_b).
All matrices are row-major.
@param c Output matrix pointer.
@param a Input matrix A pointer.
@param b Input matrix B pointer.
@param rows_a Number of rows in A (and C).
@param cols_a Number of columns in A (and rows in B).
@param cols_b Number of columns in B (and C). */
template<typename T>
void TMatrixMultiply(T* c, const T* a, const T* b,
                     ISW rows_a, ISW cols_a, ISW cols_b) {
  D_ASSERT(c);
  D_ASSERT(a);
  D_ASSERT(b);
  D_ASSERT(rows_a > 0);
  D_ASSERT(cols_a > 0);
  D_ASSERT(cols_b > 0);
  for (ISW i = 0; i < rows_a; ++i) {
    for (ISW j = 0; j < cols_b; ++j) {
      T sum = T(0);
      for (ISW k = 0; k < cols_a; ++k)
        sum += a[i * cols_a + k] * b[k * cols_b + j];
      c[i * cols_b + j] = sum;
    }
  }
}

/* Applies ReLU activation in-place: x[i] = max(0, x[i]).
@param x Pointer to the array.
@param n Number of elements. */
template<typename T>
void TMatrixReLU(T* x, ISW n) {
  D_ASSERT(x);
  D_ASSERT(n > 0);
  for (ISW i = 0; i < n; ++i)
    if (x[i] < T(0)) x[i] = T(0);
}

/* Applies the derivative of ReLU: result[i] = (x[i] > 0) ? 1 : 0.
@param result Pointer to the output gradient array.
@param x Pointer to the input array (pre-activation values).
@param n Number of elements. */
template<typename T>
void TMatrixReLUDerivative(T* result, const T* x, ISW n) {
  D_ASSERT(result);
  D_ASSERT(x);
  D_ASSERT(n > 0);
  for (ISW i = 0; i < n; ++i)
    result[i] = (x[i] > T(0)) ? T(1) : T(0);
}

/* Applies sigmoid activation in-place: x[i] = 1 / (1 + exp(-x[i])).
@param x Pointer to the array.
@param n Number of elements. */
inline void TMatrixSigmoid(FPD* x, ISW n) {
  D_ASSERT(x);
  D_ASSERT(n > 0);
  for (ISW i = 0; i < n; ++i)
    x[i] = 1.0 / (1.0 + exp(-x[i]));
}

/* Applies sigmoid activation in-place for FPC (float).
@param x Pointer to the array.
@param n Number of elements. */
inline void TMatrixSigmoid(FPC* x, ISW n) {
  D_ASSERT(x);
  D_ASSERT(n > 0);
  for (ISW i = 0; i < n; ++i)
    x[i] = 1.0f / (1.0f + expf(-x[i]));
}

/* Applies the derivative of sigmoid: result[i] = s[i] * (1 - s[i])
where s[i] is the already-computed sigmoid output.
@param result Pointer to the output gradient array.
@param sigmoid_output Pointer to the sigmoid output array.
@param n Number of elements. */
inline void TMatrixSigmoidDerivative(FPD* result, const FPD* sigmoid_output,
                                     ISW n) {
  D_ASSERT(result);
  D_ASSERT(sigmoid_output);
  D_ASSERT(n > 0);
  for (ISW i = 0; i < n; ++i)
    result[i] = sigmoid_output[i] * (1.0 - sigmoid_output[i]);
}

/* Applies the derivative of sigmoid for FPC (float).
@param result Pointer to the output gradient array.
@param sigmoid_output Pointer to the sigmoid output array.
@param n Number of elements. */
inline void TMatrixSigmoidDerivative(FPC* result, const FPC* sigmoid_output,
                                     ISW n) {
  D_ASSERT(result);
  D_ASSERT(sigmoid_output);
  D_ASSERT(n > 0);
  for (ISW i = 0; i < n; ++i)
    result[i] = sigmoid_output[i] * (1.0f - sigmoid_output[i]);
}

/* Applies softmax in-place over n elements: x[i] = exp(x[i]) / sum(exp(x[j])).
Uses the max-shift trick for numerical stability.
@param x Pointer to the array.
@param n Number of elements. */
inline void TMatrixSoftmax(FPD* x, ISW n) {
  D_ASSERT(x);
  D_ASSERT(n > 0);
  // Find max for numerical stability.
  FPD max_val = x[0];
  for (ISW i = 1; i < n; ++i)
    if (x[i] > max_val) max_val = x[i];
  FPD sum = 0.0;
  for (ISW i = 0; i < n; ++i) {
    x[i] = exp(x[i] - max_val);
    sum += x[i];
  }
  for (ISW i = 0; i < n; ++i)
    x[i] /= sum;
}

/* Applies softmax in-place for FPC (float).
@param x Pointer to the array.
@param n Number of elements. */
inline void TMatrixSoftmax(FPC* x, ISW n) {
  D_ASSERT(x);
  D_ASSERT(n > 0);
  FPC max_val = x[0];
  for (ISW i = 1; i < n; ++i)
    if (x[i] > max_val) max_val = x[i];
  FPC sum = 0.0f;
  for (ISW i = 0; i < n; ++i) {
    x[i] = expf(x[i] - max_val);
    sum += x[i];
  }
  for (ISW i = 0; i < n; ++i)
    x[i] /= sum;
}

/* Computes Mean Squared Error: (1/n) * sum((predicted[i] - target[i])^2).
@param predicted Pointer to the predicted values array.
@param target Pointer to the target values array.
@param n Number of elements.
@return The mean squared error. */
inline FPD TMatrixMSE(const FPD* predicted, const FPD* target, ISW n) {
  D_ASSERT(predicted);
  D_ASSERT(target);
  D_ASSERT(n > 0);
  FPD sum = 0.0;
  for (ISW i = 0; i < n; ++i) {
    FPD diff = predicted[i] - target[i];
    sum += diff * diff;
  }
  return sum / FPD(n);
}

/* Computes Mean Squared Error for FPC (float).
@param predicted Pointer to the predicted values array.
@param target Pointer to the target values array.
@param n Number of elements.
@return The mean squared error. */
inline FPC TMatrixMSE(const FPC* predicted, const FPC* target, ISW n) {
  D_ASSERT(predicted);
  D_ASSERT(target);
  D_ASSERT(n > 0);
  FPC sum = 0.0f;
  for (ISW i = 0; i < n; ++i) {
    FPC diff = predicted[i] - target[i];
    sum += diff * diff;
  }
  return sum / FPC(n);
}

/* Adds a bias vector to each row of a 2D matrix in-place.
matrix is (rows x cols), bias is (cols).
@param matrix Pointer to the row-major matrix.
@param bias Pointer to the bias vector.
@param rows Number of rows.
@param cols Number of columns. */
template<typename T>
void TMatrixAddBias(T* matrix, const T* bias, ISW rows, ISW cols) {
  D_ASSERT(matrix);
  D_ASSERT(bias);
  D_ASSERT(rows > 0);
  D_ASSERT(cols > 0);
  for (ISW r = 0; r < rows; ++r)
    for (ISW c = 0; c < cols; ++c)
      matrix[r * cols + c] += bias[c];
}

/* Fills an array with a constant value.
@param x Pointer to the array.
@param n Number of elements.
@param value The fill value. */
template<typename T>
void TMatrixFill(T* x, ISW n, T value) {
  D_ASSERT(x);
  D_ASSERT(n > 0);
  for (ISW i = 0; i < n; ++i)
    x[i] = value;
}

/* Computes the sum of all elements in an array.
@param x Pointer to the array.
@param n Number of elements.
@return The sum of all elements. */
template<typename T>
T TMatrixSum(const T* x, ISW n) {
  D_ASSERT(x);
  D_ASSERT(n > 0);
  T result = T(0);
  for (ISW i = 0; i < n; ++i)
    result += x[i];
  return result;
}

/* Finds the index of the maximum element (argmax).
@param x Pointer to the array.
@param n Number of elements.
@return Index of the largest element. */
template<typename T>
ISW TMatrixArgMax(const T* x, ISW n) {
  D_ASSERT(x);
  D_ASSERT(n > 0);
  ISW max_index = 0;
  T max_val = x[0];
  for (ISW i = 1; i < n; ++i) {
    if (x[i] > max_val) {
      max_val = x[i];
      max_index = i;
    }
  }
  return max_index;
}

/* A homogeneous multi-dimensional array contiguous data types. */
template<MTX_A, typename DIM = TDim<ISZ, IUY, 1>, typename BOF = Nil>
class AMatrix {

  AArray<IUA, ISZ, DIM::CBytes, BOF> obj_;  //< An Auto-Array object.

 public:
  /* Constructs an empty Matrix with 4 reserved dimensions and 64 elements. */
  AMatrix() : obj_(TMatrixSize<MTX_P>(4, 64)) {}

  /* Constructs an empty Matrix with 4 reserved dimensions and 64 elements. */
  AMatrix(const T* elements) : obj_(elements) {}

  /* Initializes an array of n elements of the given type. 
  @param max_elements The max number_ of elements in the array socket. */
  AMatrix(ISZ x, const T* elements = NILP) : obj_(TMatrixSize<MTX_P>(1, x)) {
    D_ASSERT(x >= 0);
  }

  /* Initializes an array of n elements of the given type.
  @param max_elements The max number_ of elements in the array socket. */
  AMatrix(ISZ x, ISZ y, const T* elements = NILP)
      : obj_(TMatrixSize<MTX_P>(2, x * y)) {
    D_ASSERT(x >= 0 && y >= 0);
  }

  /* Initializes an array of n elements of the given type.
  @param max_elements The max number_ of elements in the array socket. */
  AMatrix(ISZ x, ISZ y, ISZ z, const T* elements = NILP)
      : obj_(TMatrixSize<MTX_P>(3, x * y * z)) {
    D_ASSERT(x >= 0 && y >= 0 && z >= 0);
  }

  /* Initializes an array of n elements of the given type.
  @param max_elements The max number_ of elements in the array socket. */
  AMatrix(ISZ w, ISZ x, ISZ y, ISZ z, const T* elements = NILP)
      : obj_(TMatrixSize<MTX_P>(4, w * x * y * z)) {}

  /* Initializes an array of n elements of the given type.
  @param max_elements The max number_ of elements in the array socket. */
  AMatrix(const AMatrix& other) : obj_() {
  //other.AJT_ARY()
  }

  /* Destructs nothing. */
  ~AMatrix() { 
    Delete(AJT());
  }

  /* Clones the other object; up-sizing the socket only if required. */
  inline TMatrix<MTX_P>* Resize(const ISZ* dimension_delta) {
    //TMatrixResize<T, ISZ>(AJT(), dimension_delta);
    return This();
  }

  template<typename TMTX_, typename ISZMTX_, typename ISYMTX_, typename IUYMTX_>
  TMatrix<TMTX_, ISZMTX_, ISYMTX_, IUYMTX_>* CloneDelta(const ISZ* dimensions_delta) {
    Autoject& ajt = AJT();
    TMatrix<T, ISZ, ISY, IUY>* matrix = TMatrixPtr<ISZ>(this->AJT().origin);
    ISZ* a_dimensions = &matrix->dimensions.count;
    ISZ dimension_count = *a_dimensions;
    if (IsError(dimensions_delta) || dimension_count == 0) {
      ISZ bytes = TMatrixSize<T, ISZ, ISY, IUY>(matrix);
      if (bytes < 0) return NILP;
      IUW* origin_new = ajt.ram(NILP, bytes);
      if (IsError(origin_new)) return NILP;
      IUW* origin = ajt.origin;
      ISZ bytes_copy = TMatrixSize<T, ISZ, ISY, IUY>(&matrix->dimensions.count);
      ArrayCopy(origin_new, TPtr<CHA>(origin_new) + bytes_copy,
        origin, TPtr<CHA>(origin) + bytes_copy);
      return TPtr<TMatrix<T, ISZ, ISY, IUY>>(origin_new);
    }
    ISZ dimensions_delta_count = *dimensions_delta++;
    if (dimension_count != dimensions_delta_count) return -1;
    if (dimension_count == 1) {
      return NILP;
    }
    ISZ bytes = TMatrixSize<MTX_P>(dimensions_delta);
    if (bytes < 0) return NILP;
    IUW* origin_new = ajt.ram(NILP, bytes);
    const ISZ* b_dimensions = 0,
      * a_cursor = a_dimensions,
      * b_cursor = dimensions_delta_count;
    a_dimensions += dimension_count;
    dimensions_delta += dimension_count;
    T* ary = TMatrixStart<MTX_P>(matrix);
    ISZ dimensions_new[dimension_count] = { 0 };
    ISZ* dimensions_new_ptr = dimensions_new;
    ISZ dimensions_counter = dimension_count;
    ISZ dimension_last;
    while (dimensions_counter-- > 0) {
      dimension_last = *++a_cursor + *++b_cursor;
      *dimensions_new_ptr++ = dimension_last;
    }
    while (a_cursor > a_dimensions)
      return TMatrixPtr<ISZ>(origin_new);
  }

  /* Clones the other object; up-sizing the socket only if required. */
  inline TMatrix<MTX_P>* Copy(AMatrix<MTX_P>& other) {
    return NILP;// TMatrixCopy<MTX_P>(obj_.This(), other.This());
  }

  template<typename T_, typename ISZ_, typename ISY_, typename IUY_>
  inline TMatrix<T_, ISZ_, ISY_, IUY_>* Resize(const ISZ_* dimensions_delta) {
		//Autoject& ajt = AJT();
  //  auto matrix = Clone<ISZ>(dimensions_delta);
  //  if (IsError(matrix)) return matrix;
  //  ajt.origin = TPtr<IUW>(matrix);
  //  return matrix;
    return NILP;
  }

  /* Gets the number_ of dimensions. */
  inline ISZ DimensionCount() { return This()->dimensions.count; }

  /* Gets the dimensions array. */
  inline T* Dimension() { return TStackBegin<SCK_P>(&This()->dimensions); }

  /* Gets the underlying array. */
  inline T* Start() { return TMatrixStart<MTX_P>(This()); }

  /* Returns the Autoject-Array. */
  inline AArray<IUA, ISZ, DIM::CBytes, BOF>& AJT_ARY() { return obj_; }

  /* Returns the Autoject. */
  inline Autoject& AJT() { return obj_.AJT(); }

  /* Returns the Autoject::origin cased as a TMap<ISZ>. */
  inline TMatrix<MTX_P>* This() { return TPtr<TMatrix<MTX_P>>(AJT().origin);
  }

  /* Returns the Auto-Array. */
  inline AArray<IUA, ISZ, DIM::CBytes, BOF>& Array() { return obj_; }

  template<typename Printer = COut>
  inline Printer& PrintTo(Printer& o) {
    return TMatrixPrint<Printer, MTX_P>(o, This());
  }

  /* Operator= overload. */
  inline AMatrix<MTX_P>& operator=(AMatrix<MTX_P>& other) {
    //Clone(other);
    return *this;
  }
};

}  //< namespace _
#endif
#endif
