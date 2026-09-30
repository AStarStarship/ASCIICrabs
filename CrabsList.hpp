// Copyright AStarship <https://astarship.net>.
#include "CrabsList.h"
#if SEAM >= CRABS_LIST
#include "_Debug.h"
#include "_Release.h"
namespace _ {

/* Initializes a CrabList from preallocated memory.
The CrabList wraps a TList to serve as the unified Crabs machine stack.
Each stack frame is a (type, value) tuple — a single Type:Value pair.

@cl       Pointer to preallocated CrabList memory.
@total    Maximum number of Type:Value tuples.
@bytes    Total bytes available for the CrabList.
@return   Pointer to initialized CrabList, or nil on failure.
*/
template<typename ISZ, typename ISY, typename DT>
CrabList<ISZ, ISY, DT>* CrabListInit(CrabList<ISZ, ISY, DT>* cl, ISY total, ISZ bytes) {
  if (!cl || total < 1 || bytes < CrabListBytesMin<ISZ, ISY, DT>(total)) {
    return nullptr;
  }
  
  // Initialize the underlying TList with 'total' capacity
  // The TList stores Type:Value tuples directly
  TListInit<ISZ, ISY, DT>(&cl->list, total);
  cl->list.bytes = bytes;
  
  // Reset top to start of values section (after offsets and types arrays)
  cl->list.top = sizeof(TList<ISZ, ISY, DT>) + total * sizeof(ISZ) + total * sizeof(DT);
  
  D_COUT("\nCrabList initialized: total=" << total 
         << " bytes=" << bytes << " top=" << cl->list.top);
  
  return cl;
}

/* Gets the minimum bytes needed for a CrabList with the given total.
This includes the TList struct header, offsets array, and types array.
@total   Maximum number of Type:Value tuples.
@return   Minimum bytes required.
*/
template<typename ISZ, typename ISY, typename DT>
inline ISZ CrabListBytesMin(ISY total) {
  return sizeof(TList<ISZ, ISY, DT>) + total * sizeof(ISZ) + total * sizeof(DT);
}

/* Pushes a Type:Value tuple onto the CrabList stack.
This is the core stack push operation for the unified Crabs machine.
The value is stored in the TList at the aligned top position.
@cl       Pointer to the CrabList.
@type     ASCII Data Type code for the value.
@value    Pointer to the value data.
@value_bytes  Number of bytes in the value.
@return   Index of the new entry on the stack, or -1 on failure.
*/
template<typename ISZ, typename ISY, typename DT>
inline ISY CrabListPush(CrabList<ISZ, ISY, DT>* cl, DT type, void* value, ISZ value_bytes) {
  if (!cl || !value || value_bytes < 1) {
    return -1;
  }
  
  // Check capacity
  if (cl->list.map.count >= cl->list.map.total) {
    D_COUT("\nCrabListPush: stack overflow (count=" << cl->list.map.count 
           << " total=" << cl->list.map.total << ")");
    return -1;
  }
  
  // Align the top to the type's alignment mask
  ISZ aligned_top = (cl->list.top + ATypeAlignMask(type)) & ~ATypeAlignMask(type);
  
  // Check if there's enough space for the value
  if (aligned_top + value_bytes > cl->list.bytes) {
    D_COUT("\nCrabListPush: not enough space (top=" << aligned_top 
           << " + bytes=" << value_bytes << " > " << cl->list.bytes << ")");
    return -1;
  }
  
  ISY index = cl->list.map.count;
  
  // Store the offset to this value in the map
  ISZ* offsets = TListValuesMap<ISZ, ISY, DT>(&cl->list);
  offsets[index] = aligned_top;
  
  // Store the type in the types array
  DT* types = TListTypes<ISZ, ISY, DT>(&cl->list, cl->list.map.total);
  types[index] = type;
  
  // Copy the value to the values section
  CHA* values = TListValues<ISZ, ISY, DT>(&cl->list);
  CHA* value_dst = values + aligned_top;
  ArrayCopy(value_dst, value_bytes, value, value_bytes);
  
  // Update top to point after this value
  cl->list.top = aligned_top + value_bytes;
  
  // Increment count
  cl->list.map.count++;
  
  D_COUT("\nCrabListPush: index=" << index << " type=" << ATypef(type) 
         << " offset=" << aligned_top << " count=" << cl->list.map.count);
  
  return index;
}

/* Pops the top Type:Value tuple off the CrabList stack.
This is the core stack pop operation for the unified Crabs machine.
It decrements the count but does NOT clear the value data.
@cl   Pointer to the CrabList.
@return Pointer to the popped value, or nullptr if stack is empty.
*/
template<typename ISZ, typename ISY, typename DT>
inline void* CrabListPop(CrabList<ISZ, ISY, DT>* cl) {
  if (!cl || cl->list.map.count == 0) {
    D_COUT("\nCrabListPop: stack underflow (count=" << cl->list.map.count << ")");
    return nullptr;
  }
  
  // Get the index of the top element
  ISY index = cl->list.map.count - 1;
  
  // Get the offset to the value
  ISZ* offsets = TListValuesMap<ISZ, ISY, DT>(&cl->list);
  ISZ offset = offsets[index];
  
  // Get the value pointer
  CHA* values = TListValues<ISZ, ISY, DT>(&cl->list);
  void* value = values + offset;
  
  D_COUT("\nCrabListPop: index=" << index << " offset=" << offset);
  
  // Decrement count
  cl->list.map.count--;
  
  // Update top to point before the popped value
  // We need to find the previous value's end, or reset to the base
  if (cl->list.map.count > 0) {
    // Find the end of the previous value
    ISY prev_index = cl->list.map.count - 1;
    ISZ prev_offset = offsets[prev_index];
    DT* types = TListTypes<ISZ, ISY, DT>(&cl->list, cl->list.map.total);
    DT prev_type = types[prev_index];
    ISZ prev_bytes = ATypeSizeOfPOD(prev_type);
    cl->list.top = prev_offset + prev_bytes;
  } else {
    // Stack is empty, reset top to the base of values section
    cl->list.top = sizeof(TList<ISZ, ISY, DT>) + cl->list.map.total * sizeof(ISZ) 
                   + cl->list.map.total * sizeof(DT);
  }
  
  return value;
}

/* Peeks at the top Type:Value tuple without removing it.
@cl   Pointer to the CrabList.
@return TATypeValue containing the type and offset of the top element.
        Returns {0, 0} if the stack is empty.
*/
template<typename ISZ, typename ISY, typename DT>
inline TATypeValue<ISZ, DT> CrabListPeek(const CrabList<ISZ, ISY, DT>* cl) {
  TATypeValue<ISZ, DT> result{0, 0};
  
  if (!cl || cl->list.map.count == 0) {
    return result;
  }
  
  ISY index = cl->list.map.count - 1;
  DT* types = TListTypes<ISZ, ISY, DT>(cl, cl->list.map.total);
  ISZ* offsets = TListValuesMap<ISZ, ISY, DT>(const_cast<CrabList<ISZ, ISY, DT>*>(cl));
  
  result.type = types[index];
  result.value = offsets[index];
  
  return result;
}

/* Gets the Type:Value tuple at a given index.
@cl       Pointer to the CrabList.
@index    Index of the element to retrieve.
@return   TATypeValue containing the type and offset of the element.
          Returns {0, 0} if the index is out of bounds.
*/
template<typename ISZ, typename ISY, typename DT>
inline TATypeValue<ISZ, DT> CrabListGet(const CrabList<ISZ, ISY, DT>* cl, ISY index) {
  TATypeValue<ISZ, DT> result{0, 0};
  
  if (!cl || index < 0 || index >= cl->list.map.count) {
    return result;
  }
  
  ISY safe_idx = static_cast<ISY>(index);
  DT* types = TListTypes<ISZ, ISY, DT>(const_cast<CrabList<ISZ, ISY, DT>*>(cl), cl->list.map.total);
  ISZ* offsets = TListValuesMap<ISZ, ISY, DT>(const_cast<CrabList<ISZ, ISY, DT>*>(cl));
  
  result.type = types[safe_idx];
  result.value = offsets[safe_idx];
  
  return result;
}

/* Clears the CrabList stack (resets count to 0, keeps data intact).
The values section is NOT wiped — only the count is reset.
@cl   Pointer to the CrabList.
*/
template<typename ISZ, typename ISY, typename DT>
inline void CrabListClear(CrabList<ISZ, ISY, DT>* cl) {
  if (!cl) {
    return;
  }
  
  cl->list.map.count = 0;
  
  // Reset top to the base of values section
  cl->list.top = sizeof(TList<ISZ, ISY, DT>) + cl->list.map.total * sizeof(ISZ) 
                 + cl->list.map.total * sizeof(DT);
  
  D_COUT("\nCrabListClear: count reset to 0");
}

/* Returns the number of Type:Value tuples on the stack.
@cl   Pointer to the CrabList.
@return Count of elements on the stack.
*/
template<typename ISZ, typename ISY, typename DT>
inline ISY CrabListCount(const CrabList<ISZ, ISY, DT>* cl) {
  if (!cl) {
    return 0;
  }
  return cl->list.map.count;
}

/* Returns true if the CrabList is empty (count == 0).
@cl   Pointer to the CrabList.
@return true if empty, false otherwise.
*/
template<typename ISZ, typename ISY, typename DT>
inline BOL CrabListEmpty(const CrabList<ISZ, ISY, DT>* cl) {
  if (!cl) {
    return true;
  }
  return cl->list.map.count == 0;
}

/* Prints the CrabList to a printer.
@o      Reference to the printer output stream.
@cl     Pointer to the CrabList.
@return Reference to the printer after printing.
*/
template<typename Printer, typename ISZ, typename ISY, typename DT>
Printer& CrabListPrint(Printer& o, const CrabList<ISZ, ISY, DT>* cl) {
  if (!cl) {
    o << "\nCrabList: nil";
    return o;
  }
  
  o << "\n+---";
  o << "\n| CrabList (TList-wrapped Crabs stack)";
  o << "\n| bytes:  " << cl->list.bytes;
  o << "\n| total:  " << cl->list.map.total;
  o << "\n| count:  " << cl->list.map.count;
  o << "\n| top:    " << cl->list.top;
  o << "\n+---";
  
  // Print each Type:Value tuple
  for (ISY i = 0; i < cl->list.map.count; i++) {
    ISY safe_i = static_cast<ISY>(i);
    DT* types = TListTypes<ISZ, ISY, DT>(const_cast<CrabList<ISZ, ISY, DT>*>(cl), cl->list.map.total);
    ISZ* offsets = TListValuesMap<ISZ, ISY, DT>(const_cast<CrabList<ISZ, ISY, DT>*>(cl));
    CHA* values = TListValues<ISZ, ISY, DT>(const_cast<CrabList<ISZ, ISY, DT>*>(cl));
    
    DT type = types[safe_i];
    ISZ offset = offsets[safe_i];
    
    o << "\n| [" << i << "] " << ATypef(type) << " offset=" << offset;
    
    // Print the value if it's a POD type
    if (ATypeIsPOD(type)) {
      void* value_ptr = values + offset;
      o << " ";
      TPrintValue<Printer>(o, type, value_ptr);
    }
  }
  
  o << "\n+---";
  return o;
}

}  // namespace _
