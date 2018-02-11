/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2017-2018. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef NTCORE_NETWORKTABLES_NETWORKTABLEENTRY_H_
#define NTCORE_NETWORKTABLES_NETWORKTABLEENTRY_H_

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

#include <llvm/StringRef.h>
#include <llvm/Twine.h>

#include "networktables/NetworkTableType.h"
#include "networktables/NetworkTableValue.h"
#include "networktables/RpcCall.h"
#include "ntcore_c.h"
#include "ntcore_cpp.h"

namespace nt {

using llvm::ArrayRef;
using llvm::StringRef;
using llvm::Twine;

class NetworkTableInstance;

/** NetworkTables Entry */
class NetworkTableEntry final {
 public:
  /**
   * Flag values (as returned by {@link #getFlags()}).
   */
  enum Flags { kPersistent = NT_PERSISTENT };

  /**
   * Construct invalid instance.
   */
  NetworkTableEntry();

  /**
   * Construct from native handle.
   * @param handle Native handle
   */
  explicit NetworkTableEntry(NT_Entry handle);

  /**
   * Determines if the native handle is valid.
   * @return True if the native handle is valid, false otherwise.
   */
  explicit operator bool() const { return m_handle != 0; }

  /**
   * Gets the native handle for the entry.
   * @return Native handle
   */
  NT_Entry GetHandle() const;

  /**
   * Gets the instance for the entry.
   * @return Instance
   */
  NetworkTableInstance GetInstance() const;

  /**
   * Determines if the entry currently exists.
   * @return True if the entry exists, false otherwise.
   */
  bool Exists() const;

  /**
   * Gets the name of the entry (the key).
   * @return the entry's name
   */
  std::string GetName() const;

  /**
   * Gets the type of the entry.
   * @return the entry's type
   */
  NetworkTableType GetType() const;

  /**
   * Returns the flags.
   * @return the flags (bitmask)
   */
  unsigned int GetFlags() const;

  /**
   * Gets the last time the entry's value was changed.
   * @return Entry last change time
   */
  uint64_t GetLastChange() const;

  /**
   * Gets combined information about the entry.
   * @return Entry information
   */
  EntryInfo GetInfo() const;

  /**
   * Gets the entry's value. If the entry does not exist, returns nullptr.
   *
   * @return the entry's value or nullptr if it does not exist.
   */
  std::shared_ptr<Value> GetValue() const;

  /**
   * Gets the entry's value as a boolean. If the entry does not exist or is of
   * different type, it will return the default value.
   *
   * @param defaultValue the value to be returned if no value is found
   * @return the entry's value or the given default value
   */
  bool GetBoolean(bool defaultValue) const;

  /**
   * Gets the entry's value as a double. If the entry does not exist or is of
   * different type, it will return the default value.
   *
   * @param defaultValue the value to be returned if no value is found
   * @return the entry's value or the given default value
   */
  double GetDouble(double defaultValue) const;

  /**
   * Gets the entry's value as a string. If the entry does not exist or is of
   * different type, it will return the default value.
   *
   * @param defaultValue the value to be returned if no value is found
   * @return the entry's value or the given default value
   */
  std::string GetString(StringRef defaultValue) const;

  /**
   * Gets the entry's value as a raw. If the entry does not exist or is of
   * different type, it will return the default value.
   *
   * @param defaultValue the value to be returned if no value is found
   * @return the entry's value or the given default value
   */
  std::string GetRaw(StringRef defaultValue) const;

  /**
   * Gets the entry's value as a boolean array. If the entry does not exist
   * or is of different type, it will return the default value.
   * @param defaultValue the value to be returned if no value is found
   * @return the entry's value or the given default value
   *
   * @note This makes a copy of the array.  If the overhead of this is a
   *       concern, use GetValue() instead.
   *
   * @note The returned array is std::vector<int> instead of std::vector<bool>
   *       because std::vector<bool> is special-cased in C++.  0 is false, any
   *       non-zero value is true.
   */
  std::vector<int> GetBooleanArray(ArrayRef<int> defaultValue) const;

  /**
   * Gets the entry's value as a double array. If the entry does not exist
   * or is of different type, it will return the default value.
   * @param defaultValue the value to be returned if no value is found
   * @return the entry's value or the given default value
   *
   * @note This makes a copy of the array.  If the overhead of this is a
   *       concern, use GetValue() instead.
   */
  std::vector<double> GetDoubleArray(ArrayRef<double> defaultValue) const;

  /**
   * Gets the entry's value as a string array. If the entry does not exist
   * or is of different type, it will return the default value.
   * @param defaultValue the value to be returned if no value is found
   * @return the entry's value or the given default value
   *
   * @note This makes a copy of the array.  If the overhead of this is a
   *       concern, use GetValue() instead.
   */
  std::vector<std::string> GetStringArray(
      ArrayRef<std::string> defaultValue) const;

  /**
   * Sets the entry's value if it does not exist.
   * @param defaultValue the default value to set
   * @return False if the entry exists with a different type
   */
  bool SetDefaultValue(std::shared_ptr<Value> value);

  /**
   * Sets the entry's value if it does not exist.
   * @param defaultValue the default value to set
   * @return False if the entry exists with a different type
   */
  bool SetDefaultBoolean(bool defaultValue);

  /**
   * Sets the entry's value if it does not exist.
   * @param defaultValue the default value to set
   * @return False if the entry exists with a different type
   */
  bool SetDefaultDouble(double defaultValue);

  /**
   * Sets the entry's value if it does not exist.
   * @param defaultValue the default value to set
   * @return False if the entry exists with a different type
   */
  bool SetDefaultString(const Twine& defaultValue);

  /**
   * Sets the entry's value if it does not exist.
   * @param defaultValue the default value to set
   * @return False if the entry exists with a different type
   */
  bool SetDefaultRaw(StringRef defaultValue);

  /**
   * Sets the entry's value if it does not exist.
   * @param defaultValue the default value to set
   * @return False if the entry exists with a different type
   */
  bool SetDefaultBooleanArray(ArrayRef<int> defaultValue);

  /**
   * Sets the entry's value if it does not exist.
   * @param defaultValue the default value to set
   * @return False if the entry exists with a different type
   */
  bool SetDefaultDoubleArray(ArrayRef<double> defaultValue);

  /**
   * Sets the entry's value if it does not exist.
   * @param defaultValue the default value to set
   * @return False if the entry exists with a different type
   */
  bool SetDefaultStringArray(ArrayRef<std::string> defaultValue);

  /**
   * Sets the entry's value.
   * @param value the value to set
   * @return False if the entry exists with a different type
   */
  bool SetValue(std::shared_ptr<Value> value);

  /**
   * Sets the entry's value.
   * @param value the value to set
   * @return False if the entry exists with a different type
   */
  bool SetBoolean(bool value);

  /**
   * Sets the entry's value.
   * @param value the value to set
   * @return False if the entry exists with a different type
   */
  bool SetDouble(double value);

  /**
   * Sets the entry's value.
   * @param value the value to set
   * @return False if the entry exists with a different type
   */
  bool SetString(const Twine& value);

  /**
   * Sets the entry's value.
   * @param value the value to set
   * @return False if the entry exists with a different type
   */
  bool SetRaw(StringRef value);

  /**
   * Sets the entry's value.
   * @param value the value to set
   * @return False if the entry exists with a different type
   */
  bool SetBooleanArray(ArrayRef<int> value);

  /**
   * Sets the entry's value.
   * @param value the value to set
   * @return False if the entry exists with a different type
   */
  bool SetDoubleArray(ArrayRef<double> value);

  /**
   * Sets the entry's value.
   * @param value the value to set
   * @return False if the entry exists with a different type
   */
  bool SetStringArray(ArrayRef<std::string> value);

  /**
   * Sets the entry's value.  If the value is of different type, the type is
   * changed to match the new value.
   * @param value the value to set
   */
  void ForceSetValue(std::shared_ptr<Value> value);

  /**
   * Sets the entry's value.  If the value is of different type, the type is
   * changed to match the new value.
   * @param value the value to set
   */
  void ForceSetBoolean(bool value);

  /**
   * Sets the entry's value.  If the value is of different type, the type is
   * changed to match the new value.
   * @param value the value to set
   */
  void ForceSetDouble(double value);

  /**
   * Sets the entry's value.  If the value is of different type, the type is
   * changed to match the new value.
   * @param value the value to set
   */
  void ForceSetString(const Twine& value);

  /**
   * Sets the entry's value.  If the value is of different type, the type is
   * changed to match the new value.
   * @param value the value to set
   */
  void ForceSetRaw(StringRef value);

  /**
   * Sets the entry's value.  If the value is of different type, the type is
   * changed to match the new value.
   * @param value the value to set
   */
  void ForceSetBooleanArray(ArrayRef<int> value);

  /**
   * Sets the entry's value.  If the value is of different type, the type is
   * changed to match the new value.
   * @param value the value to set
   */
  void ForceSetDoubleArray(ArrayRef<double> value);

  /**
   * Sets the entry's value.  If the value is of different type, the type is
   * changed to match the new value.
   * @param value the value to set
   */
  void ForceSetStringArray(ArrayRef<std::string> value);

  /**
   * Sets flags.
   * @param flags the flags to set (bitmask)
   */
  void SetFlags(unsigned int flags);

  /**
   * Clears flags.
   * @param flags the flags to clear (bitmask)
   */
  void ClearFlags(unsigned int flags);

  /**
   * Make value persistent through program restarts.
   */
  void SetPersistent();

  /**
   * Stop making value persistent through program restarts.
   */
  void ClearPersistent();

  /**
   * Returns whether the value is persistent through program restarts.
   * @return True if the value is persistent.
   */
  bool IsPersistent() const;

  /**
   * Deletes the entry.
   */
  void Delete();

  /**
   * Create a callback-based RPC entry point.  Only valid to use on the server.
   * The callback function will be called when the RPC is called.
   * This function creates RPC version 0 definitions (raw data in and out).
   * @param callback  callback function
   */
  void CreateRpc(std::function<void(const RpcAnswer& answer)> callback);

  /**
   * Create a polled RPC entry point.  Only valid to use on the server.
   * The caller is responsible for calling NetworkTableInstance::PollRpc()
   * to poll for servicing incoming RPC calls.
   * This function creates RPC version 0 definitions (raw data in and out).
   */
  void CreatePolledRpc();

  /**
   * Call a RPC function.  May be used on either the client or server.
   * This function is non-blocking.  Either RpcCall::GetResult() or
   * RpcCall::CancelResult() must be called on the return value to either
   * get or ignore the result of the call.
   * @param params      parameter
   * @return RPC call object.
   */
  RpcCall CallRpc(StringRef params);

  /**
   * Add a listener for changes to this entry.
   *
   * @param callback          listener to add
   * @param flags             NotifyKind bitmask
   * @return Listener handle
   */
  NT_EntryListener AddListener(
      std::function<void(const EntryNotification& event)> callback,
      unsigned int flags) const;

  /**
   * Remove an entry listener.
   * @param entry_listener Listener handle to remove
   */
  void RemoveListener(NT_EntryListener entry_listener);

  /**
   * Equality operator.  Returns true if both instances refer to the same
   * native handle.
   */
  bool operator==(const NetworkTableEntry& oth) const {
    return m_handle == oth.m_handle;
  }

  /** Inequality operator. */
  bool operator!=(const NetworkTableEntry& oth) const {
    return !(*this == oth);
  }

 protected:
  /* Native handle */
  NT_Entry m_handle;
};

}  // namespace nt

#include "networktables/NetworkTableEntry.inl"

#endif  // NTCORE_NETWORKTABLES_NETWORKTABLEENTRY_H_
