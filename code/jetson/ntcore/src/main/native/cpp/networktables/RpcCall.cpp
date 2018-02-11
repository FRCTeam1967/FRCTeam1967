/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2017-2018. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "networktables/RpcCall.h"

#include "networktables/NetworkTableEntry.h"

using namespace nt;

NetworkTableEntry RpcCall::GetEntry() const {
  return NetworkTableEntry{m_entry};
}
