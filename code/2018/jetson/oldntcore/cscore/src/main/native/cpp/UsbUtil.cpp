/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "UsbUtil.h"

#include <fcntl.h>

#ifdef __linux__
#include <libgen.h>
#include <sys/ioctl.h>
#endif

#include <llvm/Format.h>
#include <llvm/SmallString.h>
#include <llvm/raw_ostream.h>
#include <support/raw_istream.h>

#include "Log.h"

namespace cs {

#ifdef __linux__

static llvm::StringRef GetUsbNameFromFile(int vendor, int product,
                                          llvm::SmallVectorImpl<char>& buf) {
  int fd = open("/var/lib/usbutils/usb.ids", O_RDONLY);
  if (fd < 0) return llvm::StringRef{};

  llvm::raw_svector_ostream os{buf};
  wpi::raw_fd_istream is{fd, true};

  // build vendor and product 4-char hex strings
  llvm::SmallString<16> vendorStr, productStr;
  llvm::raw_svector_ostream vendorOs{vendorStr}, productOs{productStr};
  vendorOs << llvm::format_hex_no_prefix(vendor, 4);
  productOs << llvm::format_hex_no_prefix(product, 4);

  // scan file
  llvm::SmallString<128> lineBuf;
  bool foundVendor = false;
  for (;;) {
    auto line = is.getline(lineBuf, 4096);
    if (is.has_error()) break;

    if (line.empty()) continue;

    // look for vendor at start of line
    if (line.startswith(vendorStr)) {
      foundVendor = true;
      os << line.substr(5).trim() << ' ';
      continue;
    }

    if (foundVendor) {
      // next vendor, but didn't match product?
      if (line[0] != '\t') {
        os << "Unknown";
        return os.str();
      }

      // look for product
      if (line.substr(1).startswith(productStr)) {
        os << line.substr(6).trim();
        return os.str();
      }
    }
  }

  return llvm::StringRef{};
}

llvm::StringRef GetUsbNameFromId(int vendor, int product,
                                 llvm::SmallVectorImpl<char>& buf) {
  // try reading usb.ids
  llvm::StringRef rv = GetUsbNameFromFile(vendor, product, buf);
  if (!rv.empty()) return rv;

  // Fall back to internal database
  llvm::raw_svector_ostream os{buf};
  switch (vendor) {
    case 0x046d:
      os << "Logitech, Inc. ";
      switch (product) {
        case 0x0802:
          os << "Webcam C200";
          break;
        case 0x0804:
          os << "Webcam C250";
          break;
        case 0x0805:
          os << "Webcam C300";
          break;
        case 0x0807:
          os << "Webcam B500";
          break;
        case 0x0808:
          os << "Webcam C600";
          break;
        case 0x0809:
          os << "Webcam Pro 9000";
          break;
        case 0x080a:
          os << "Portable Webcam C905";
          break;
        case 0x080f:
          os << "Webcam C120";
          break;
        case 0x0819:
          os << "Webcam C210";
          break;
        case 0x081b:
          os << "Webcam C310";
          break;
        case 0x081d:
          os << "HD Webcam C510";
          break;
        case 0x0821:
          os << "HD Webcam C910";
          break;
        case 0x0825:
          os << "Webcam C270";
          break;
        case 0x0826:
          os << "HD Webcam C525";
          break;
        case 0x0828:
          os << "HD Webcam B990";
          break;
        case 0x082b:
          os << "Webcam C170";
          break;
        case 0x082d:
          os << "HD Pro Webcam C920";
          break;
        case 0x0836:
          os << "B525 HD Webcam";
          break;
        case 0x0843:
          os << "Webcam C930e";
          break;
      }
      break;
  }

  return os.str();
}

int CheckedIoctl(int fd, unsigned long req, void* data,  // NOLINT(runtime/int)
                 const char* name, const char* file, int line, bool quiet) {
  int retval = ioctl(fd, req, data);
  if (!quiet && retval < 0) {
    llvm::SmallString<64> localfile{file};
    localfile.push_back('\0');
    ERROR("ioctl " << name << " failed at " << basename(localfile.data()) << ":"
                   << line << ": " << std::strerror(errno));
  }
  return retval;
}

#endif  // __linux__

}  // namespace cs
