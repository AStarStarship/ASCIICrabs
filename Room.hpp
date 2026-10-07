// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_ROOM_HPP
#define CRABS_ROOM_HPP
#include <_Config.h>
#if SEAM >= CRABS_ROOM
#include "Interrupts.h"
#include "BOut.hpp"
#include "Wall.hpp"
#define ROM_A typename CHS = CHR, typename CHT = CHD, typename ISZ = ISR, \
  typename ISY = ISQ, typename DT = DTB, typename HSH = IUN
#define ROM_P CHS, CHT, ISZ, ISY, DT, HSH
#define ROM TRoom<ROM_P>
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
  if (r < 0 || r > CRStateInvalid) r = CRStateInvalid;
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
2.  Jo needs to add a UART port out to his App so he adds a BOut with a
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
#ifndef CRABS_MAX_WALLS
    RoomFloorSize = 1024,
#else
    FloorSize = CRABS_MAX_WALLS,
#undef ROOM_FLOOR_SIZE
#endif
    FloorSizeWords = FloorSize / sizeof(IUW) + 2,  //< +2 socket.
  };

  /* Creates a Room with the given size.
  @param floor Boofer used to create the Wall Stack. Set to NILP to
  enable dynamic memory.
  @param size  The room size that is bounded between the RoomBytesMin and
  RoomTotal. */
  TRoom(const CHA* room_name = "chinese_room", ISC state_count = 2) :
    state_(1),
    state_count_(state_count < 1 ? 1 : state_count),
    this_(NILP),
    xoff_(NILP),
    device_(NILP),
    devices_(NILP) {
    *name_ = '\0';
    SetRoomName(room_name);
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
  void PrintErrors(BOut* bout) {
    // ISC errorHeader[] = { 0 };
    // return a.prints (errorHeader);
  }

  /* Gets the wall_count_. */
  ISN WallCount() { return walls_.origin ? TStackGet<ISN, ISC, ISB>(TPtr<TStack<ISN, ISC, ISB>>(walls_.origin), 0) : 0; }

  /* Gets the given wall by index.
  @return Nil if the index is invalid  */
  TWall<ISZ, ISY>* GetWall(ISN wall_number) {
    if (wall_number < 0 || !walls_.origin) return NILP;
    if (wall_number >= TStackGet<ISN, ISC, ISB>(TPtr<TStack<ISN, ISC, ISB>>(walls_.origin), 0)) return NILP;
    return TStackGet<TWall<ISZ, ISY>*, ISC, ISB>(TPtr<TStack<ISN, ISC, ISB>>(walls_.origin), wall_number);
  }

  /* Ads a wall to the room.
  @return The inputted new_wall pointer upon success or nil upon failure. */
  TWall<ISZ, ISY>* AddWall(TWall<ISZ, ISY>* new_wall) {
    if (new_wall == NILP || !walls_.origin) return NILP;
    if (TStackGet<ISN, ISC, ISB>(TPtr<TStack<ISN, ISC, ISB>>(walls_.origin), 0) >= TStackGet<ISN, ISC, ISB>(TPtr<TStack<ISN, ISC, ISB>>(walls_.origin), 1)) return NILP;
    TStackInsert<TWall<ISZ, ISY>*, ISC, ISB>(TPtr<TStack<ISN, ISC, ISB>>(walls_.origin), new_wall);
    return new_wall;
  }

  /* Removes the given Wall by index.
  @return False upon failure.  */
  BOL RemoveWall(ISN wall_number) {
    return TStackRemove<TWall<ISZ, ISY>*, ISC, ISB>(TPtr<TStack<ISN, ISC, ISB>>(walls_.origin), wall_number);
  }

  /* Gets the entire Room size, including dynamic memory, in bytes. */
  IUW GetBytes() {
    IUW count = FloorSize;
    for (ISN i = 0; i < WallCount(); ++i) {
      count += TStackGet<TWall<ISZ, ISY>*, ISC, ISB>(
          TPtr<TStack<ISN, ISC, ISB>>(walls_.origin), i)->GetSizeBytes();
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
    if (crabs != NILP) {
      // @todo We need to load a stored Room state.
      return NILP;
    }
    return NILP;
  }

  /* Handler for shut down event. */
  virtual void ShutDown() { D_COUT("\nShutting down..."); }

  /* Handler go to sleep event. */
  virtual void Sleep() { D_COUT("\nGoing to sleep..."); }

  /* Handler for wake from sleep event. */
  virtual void Wake() { D_COUT("\nWaking up..."); }

  /* Handler for recovering from an external crash message. */
  virtual void Crash() { D_COUT("\nRoom crash!"); }

  /* Main program loop. */
  virtual const Op* Loop() { return 0; }

  /* Returns true if the Room is on. */
  virtual BOL IsOn() { return true; }

  /* The default main function.
      Please feel free to override this with your own main function. */
  virtual ISC Main(const CHA** args, ISC args_count) {
    const Op* result = NILP;
    D_COUT("\nInitializing Chinese Room with " << args_count << " args:");
#ifdef D_THIS
    for (ISC i = 0; i < args_count; ++i)
      D_COUT('\n' << i << ":\"" << args[i] << '\"');
#endif
    D_COUT('\n');
    while (IsOn()) {
      try {
        result = Init(NILP);
        if (result) return 1;
        do {
          this_->ExecAll();
          result = Loop();
        } while (!result);
        ShutDown();
      }
      catch (...) {
        D_COUT("\nRoom crashed!");
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
  virtual const Op* Star(CHC index, Crabs* crabs) {
  static const Op This = {
      "Room", OpFirst('A'), OpLast('A'), "A Chinese Room.", ';', '}', 0
  };

  switch (index) {
    case '?': {
      return 0;// ExprQuery(crabs, This);
      case 'a': {
        static const Op This = {
          "FunctionA",
          OpFirst('A'),
          OpLast('A'),
          "Description of function a.",
          ';',
          '}',
          0
        };
        return OpPush(this, crabs);
      }
    }
  }
  return 0;
}

 protected:
  ISC state_count_,                 //< Number of FSM states.
      state_;                       //< Room state.
  CHR name_[CrabsRoomNameLengthMax];//< Room Name.
  Autoject walls_;                  //< Walls in the Room.
  Crabs* expr_;                     //< Current Crabs being executed.
                                    //< DC1: this.
  TDoor<ISZ>* this_;                //< DC2: The Door to this room.
  Operand* xoff_,                   //< DC3: XOFF - XOFF handling device.
         * device_,                 //< DC4: the current device control.
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
#endif
#endif
