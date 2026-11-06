// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsRoomHPP
#define _CrabsRoomHPP
#include <_Config.h>
#if _Seam >= _CrabsRoom
#include "Interrupts.h"
#include "BOut.hpp"
#include "Wall.hpp"
#include "AType.hpp"
#include "List.hpp"
#include "Dic.hpp"
#include "Op.hpp"
#define ROM_A typename CHS = CHR, typename CHT = CHD, typename ISZ = ISR, \
  typename ISY = ISR, typename DT = DTB, typename HSH = IUN
#define ROM_P CHS, CHT, ISZ, ISY, DT, HSH
#define ROM TRoom<ROM_P>
/* @defgroup Room
Chinese Room
A state machine that receives messages through Door slots,
scans them with the Crabs interpreter, and executes the
resulting operations. The Floor is the Room's ASCII List -
one contiguous block of memory, the minimum the Room needs.
Walls provide additional heap space on top. The Room
round-robins its Doors, reading volatile data from
TBIn/TBOut slots and calling the Star reducer on the
Operand vtable.
@{ */

namespace _ {

  /* A list of Requests that can be sent from Slot<ISC, TSizeBytes> to
  Slot<ISC, TSizeBytes>.  */
  typedef enum {
    CRRequestOpenDoor = 0,
    CRRequestCloseDoor,
    CRRequestConnection,
    CRRequestDisconnect,
    CRRequestInvalid,
  } CRRequest;

  typedef enum {
    CRStateBooting = 0,
    CRStateSleeping,
    CRStateShutdown,
    CRStateInvalid
  } CRState;

  /* Returns an array of pointers to Strings that describe the program states.
   */
  const CHA** TCRStates();

  /* Returns a pointer to an array of pointers to the Request Strings. */
  const CHA** TCRRequests();

  /* Gets the response CHA corresponding to the given request. */
  const CHA* CRRequests(CRRequest r);

template<typename CHS = CHR, typename CHT = CHD>
const CHS** TCRStates() {
  static const CHS* Strings[CRStateInvalid][16] = {
    "Initializing",
    "Waking up",
    "Running",
    "Going to sleep",
    "Exiting",
    "Invalid"
  };
  return Strings;
}

template<typename CHS = CHR, typename CHT = CHD>
inline const CHS* CRStates(CRRequest r) {
  if (r < 0 || r > CRRequestInvalid) r = CRRequestInvalid;
  return TCRRequests()[r];
}

template<typename CHS = CHR, typename CHT = CHD>
inline const CHS** TCRRequests() {
  static const CHS* Strings[CRRequestInvalid][16] = {
    "Open door",
    "Close door",
    "Invalid request"
  };
  return Strings;
}

template<typename CHS = CHR, typename CHT = CHD>
const CHS* CRRequests(CRRequest r) {
  if (r < 0 || r > CRRequestInvalid) r = CRRequestInvalid;
  return TCRRequests()[r];
}

/* A Chinese Room.
An Chinese Room works the same way as in the Chinese Room thought
experiment. An Room receives a message through a slot in the door, the man
in the room reads does manual optical character recognition with a pen and
paper stack, some filing cabinets, and a library of multimaps.
  The size of the Crabs Stack is defined by the Script Spec to be a
a maximum of 2^15-1 expressions tall.
# Memory Layout
Some systems have the stack grow up from the bottom and heap grow up and
vice versa.
@code
    +-----------------------+
    |         Heap          |
    |         Wall 1        |
    |         Wall 2        |
    |          ...          |
    |         Wall N        |
    |vvvvvvvvvvvvvvvvvvvvvvv|
    |     Unused Memory     |
    |^^^^^^^^^^^^^^^^^^^^^^^|
    |     Program Stack     |
 +  |-----------------------|
 |  | Floor (Static Memory) |
0xN +-----------------------+
@endcode

# ASCII Mappings

The mapping of the layout of the Chinese Room can be seen in the following
super cute ASCII house:

@code
 _________
/_________\
[_#_]@[_#_]
@endcode

# Room Connections

There are multiple doors in a Chinese Room that lead to other Chinese Rooms.
Each door has multiple slots in lead to the same room. These slots are the
various IO ports of the system.

# Doors

There are two ways to access Doors in the CR. in the Root Scope, there are:

@code
Use Case Scenario:
Actors: Jo  - The app developer.
        Sam - The stop-user.
1.  Jo is creating an App and needs to make a Chinese Room so Jo downloads
    Script, starts his app, and makes a Room. Jo defines a Floor
    statically but is does not define any walls.
    2.  Host creates a memory stack without any heap space.
2.  Jo needs to add a UART port out to his App so he adds a TBOut with a
    slot_size of 2KB.
    3.  Host adds the Slot to the Program Stack.
4.  Jo needs to add a SPI IO device so he initializes a Slot with 256
    bytes.
    5.  Host adds the new Slot on top of the Slot.
6.  Jo is out of memory in the Floor so he creates a Ceiling of size 2KB.
    7.  Host creates a Heap Block for the Ceiling.
7.  Jo needs Interprocess communication to three threads: one SlotIn,
    one SlotOut, and a Slot of size 2KB.
    8.  Host adds the SlotIn, MirroOut, and Slot to the Ceiling.
9.  Jo wants to add a Server so Jo creates Wall_1 with 1MB space.
    10. Host creates a Wall_1 with 1MB memory.
11. Jo wants needs to distribute information to the stop-users so Jo
    creates Dictionary in Wall_1 with some keys and values.
    12. Host creates dictionary and adds keys and values.
13. Jo complies the program and launches the server.
    14. Host loads.
15. Sam launches Jo's app and connections to the Host.
    16. Host receives login attempt from Sam and opens a Door for him.
17. Sam needs to get the values Jo stored in step 11 so Same sends a
    Dictionary GET request.
@endcode
*/
template<ROM_A>
class TRoom : public Operand {
  // NONCOPYABLE (Room) //< Not sure why this isn't working?

 public:

  enum {
    FloorSize = 2048,           //< Room Floor in bytes.
    FloorSizeWords = FloorSize / sizeof(IUW) + 2,  //< +2 socket.
    WallCountMax = 5,           //< Additional borrowed walls; Floor is separate.
    FloorTupleMax = 16,         //< Fixed index capacity of the inline ASCII List.
    FloorKeysSize = 512,        //< Key Table bytes inside the Floor Dic.
  };

  /* Creates a Room with the given size.
  @param floor Buffer used to create the Wall Stack. Set to _NilP to
  enable dynamic memory.
  @param size  The room size that is bounded between the RoomBytesMin and
  RoomTotal. */
  TRoom(const CHA* room_name = "chinese_room", ISC state_count = 2) :
    state_(1),
    state_count_(state_count < 1 ? 1 : state_count),
    wall_count_(0),
    xoff_(_NilP),
    device_(_NilP),
    devices_(_NilP) {
    for (ISN i = 0; i < WallCountMax; ++i)
      walls_[i] = _NilP;
    *name_ = '\0';
    SetRoomName(room_name);
    expr_ = _NilP;
    Floor()->values.bytes = ISZ(FloorSize);
    TDicInit<DIC_P>(Floor(), ISY(FloorTupleMax), ISZ(FloorKeysSize));
  }

  TRoom(const TRoom&) = delete;
  TRoom& operator=(const TRoom&) = delete;

  /** Borrow the inline ASCII List (Dictionary layout). Tuples belong to
   * this Room and are invalidated by ClearFloor or destruction. No OS
   * allocation or I/O. This is a native in-memory representation, not
   * a portable wire format. The Floor is a TDic: a TList of values with
   * a TTable of keys at index 0 (the ASCII KV rule — keys in element 0). */
  TDic<DIC_P>* Floor() {
    return TRef<TDic<DIC_P>>(origin_);
  }
  const TDic<DIC_P>* Floor() const {
    return TRef<const TDic<DIC_P>>(origin_);
  }
  void ClearFloor() {
    TDic<DIC_P>* floor = TRef<TDic<DIC_P>>(origin_);
    TListClear<ISZ, ISY, DT>(&floor->values);
    // Re-initialize the keys Table at value index 0 after the clear.
    DTB KeysType = CTableKeysType<TBL_P>();
    ISY keys_index = TListAlloc<ISZ, ISY, DT>(&floor->values, KeysType,
        ISZ(FloorKeysSize));
    (void)keys_index;
    TBL* keys = TListValue<ISZ, ISY, DT, TBL>(&floor->values, keys_index);
    TTableInit<TBL_P>(keys, ISY(FloorTupleMax));
    TTableAppend<TBL_P>(keys, TSEmpty<CHS>());
  }

  /* RAMFactory. */
  virtual ~TRoom() {}

  /* Gets the Room state_. */
  ISC GetState() { return state_; }

  /* Gets the Room state_. */
  ISC GetStateCount() { return state_count_; }

  // Gets the room's name.
  const CHA* GetRoomName() { return name_; }

  /* Sets the Room state_. */
  virtual BOL SetRoomName(const CHA* name) {
    if (IsError(name)) {
      return false;
    }
		TSPrint<CHR, CHL>(name_, CrabsRoomNameLengthMax, name);
    return true;
  }

  /* Processes a request from another Room.
      @return Returns false upon success and true if there is an error. */
  CRRequest HandleNextRequest(CRRequest r) { return 0; }

  /* Clears the log. */
  void ClearLog() {}

  /* Processes all of the errors in the error log. */
  void ProcessLog() {}

  /* Prints the error log to a expression. */
  void PrintErrors(TBOut<ISC>* bout) {
    // ISC errorHeader[] = { 0 };
    // return a.prints (errorHeader);
  }

  /* Gets the wall_count_. */
  ISN WallCount() { return wall_count_; }

  /* Gets the given wall by index.
  @return Nil if the index is invalid. */
  TWall<ISZ, ISY>* GetWall(ISN wall_number) {
    if (wall_number < 0 || wall_number >= wall_count_) return _NilP;
    return walls_[wall_number];
  }

  /* Adds a wall to the room.
  @return The inputted new_wall pointer upon success or nil upon failure. */
  TWall<ISZ, ISY>* AddWall(TWall<ISZ, ISY>* new_wall) {
    if (new_wall == _NilP || wall_count_ >= WallCountMax) return _NilP;
    walls_[wall_count_++] = new_wall;
    return new_wall;
  }

  /* Removes the given Wall by index.
  @return False upon failure. */
  BOL RemoveWall(ISN wall_number) {
    if (wall_number < 0 || wall_number >= wall_count_) return false;
    for (ISN i = wall_number; i < wall_count_ - 1; ++i)
      walls_[i] = walls_[i + 1];
    --wall_count_;
    return true;
  }

  /* Gets the entire Room size, including dynamic memory, in bytes. */
  IUW GetBytes() {
    IUW count = FloorSize;
    for (ISN i = 0; i < wall_count_; ++i) {
      count += walls_[i]->GetSizeBytes();
    }
    return count;
  }

  /* Function run every main loop cycle to check the system status. */
  virtual void DiagnoseProblems() {
    // Check for remote crash request.
    // throw RoomCrashException ();
  }

  /* Sets up the Room. */
  virtual const Op* Init(Crabs* crabs) {
    if (crabs != _NilP) {
      // @todo We need to load a stored Room state.
      return _NilP;
    }
    return _NilP;
  }

  /* Handler for shut down event. */
  virtual void ShutDown() { _DROut("\nShutting down..."); }

  /* Handler go to sleep event. */
  virtual void Sleep() { _DROut("\nGoing to sleep..."); }

  /* Handler for wake from sleep event. */
  virtual void Wake() { _DROut("\nWaking up..."); }

  /* Handler for recovering from an external crash message. */
  virtual void Crash() { _DROut("\nRoom crash!"); }

  /* Main program loop. */
  virtual const Op* Loop() { return 0; }

  /* Returns true if the Room is on. */
  virtual BOL IsOn() { return true; }

  /* The default main function.
      Please feel free to override this with your own main function. */
  virtual ISC Main(const CHA** args, ISC args_count) {
    const Op* result = _NilP;
    _DROut("\nInitializing Chinese Room with " << args_count << " args:");
#ifdef _DThis
    for (ISC i = 0; i < args_count; ++i)
      _DROut('\n' << i << ":\"" << args[i] << '\"');
#endif
    _DROut('\n');
    while (IsOn()) {
      try {
        result = Init(_NilP);
        if (result) return 1;
        do {
          // Doors are now in the List, not embedded in the Room.
          // The Crabs interpreter pushes Door items onto the stack
          // and calls them via the List.
          result = Loop();
        } while (!result);
        ShutDown();
      }
      catch (...) {
        _DROut("\nRoom crashed!");
        return 3;
      }
    }
    return 1;
  }

  /* Handles Crabs Commands.
  @param text     Beginning of the Text socket.
  @param text_end End of the Text socket.
  @return Returns nil upon success and an error  upon failure. */
  //virtual CHA CommandNext();

  /* Crabs operations. */
  /* Query the abstract Room. Concrete applications override Star to bind
   * operation IDs and validated data to their own C++ methods. Unknown
   * operations fail explicitly; the base Room has no demo side effects. */
  virtual const Op* Star(CHC index, Crabs* crabs) override {
    static const Op This = {
      "Room", _NilP, _NilP, "An abstract Chinese Room.", ';', '}', 0, _NilP
    };
    (void)crabs;
    return index == '?' ? &This : OpError(AErrorInvalidOp);
  }

 protected:
 ISC state_count_,                 //< Number of FSM states.
     state_;                       //< Room state.
 CHR name_[CrabsRoomNameLengthMax];//< Room Name.
 TWall<ISZ, ISY>* walls_[WallCountMax];  //< 4 walls + ceiling. Floor is index 0.
 ISN wall_count_;                  //< Number of walls in use.
 Crabs* expr_;                     //< Current Crabs being executed.
                                   //< DC1: this.
 Operand* xoff_,                   //< DC2: XOFF - XOFF handling device.
        * device_,                 //< DC3: the current device control.
        * devices_;                //< Pointer to the current device control.
 IUW origin_[FloorSizeWords];      //< Room Floor socket.

  private:

  /* Sets the Room state_. */
  BOL SetState(ISC new_state) {
    if (new_state < 0) {
      return false;
    }
    if (new_state >= state_count_) {
      return false;
    }
    state_ = new_state;
    return true;
  }
};

}  //< namespace _

/* @} */  // End of Room group.
#endif
#endif  //< _CrabsRoomHPP
