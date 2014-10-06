//
//                 INTEL CORPORATION PROPRIETARY INFORMATION
//
//    This software is supplied under the terms of a license agreement or
//    nondisclosure agreement with Intel Corporation and may not be copied
//    or disclosed except in accordance with the terms of that agreement.
//    Copyright (C) 2014 Intel Corporation. All Rights Reserved.
//

#pragma once

#include <stdlib.h>
#include <string>
#include "ospray/common/managed.h"
#include "ospray/fileio/ObjectCatalog.h"
#include "ospray/volume/Volume.h"

//! \brief Define a function to create an instance of the InternalClass
//!  associated with ExternalName.
//!
//! \internal The function generated by this macro is used to create an
//!  instance of a concrete subtype of an abstract base class.  This
//!  macro is needed since the subclass type may not be known to OSPRay
//!  at build time.  Rather, the subclass can be defined in an external
//!  module and registered with OSPRay using this macro.
//! 
#define OSP_REGISTER_VOLUME_FILE(InternalClass, ExternalName) \
    extern "C" OSPObjectCatalog ospray_import_volume_file_##ExternalName(const std::string &filename, Volume *volume) \
        { InternalClass file(filename);  return(file.importVolume(volume)); }

namespace ospray {

    //! \brief The VolumeFile class is an abstraction for the concrete
    //!  object which is used to load volume data from a file.
    //!
    //!  The format of the volume data stored in a file on disk is unknown
    //!  to this class.  Subclasses implement loaders for specific formats,
    //!  and the actual subclass used is determined from the file extension.
    //!  This subclass must be registered in OSPRay proper, or in a loaded
    //!  module via OSP_REGISTER_VOLUME_FILE.
    //!
    class VolumeFile : public ManagedObject {
    public:

        //! Constructor.
        VolumeFile() {};

        //! Destructor.
        virtual ~VolumeFile() {};

        //! Create a VolumeFile object of the subtype given by the file extension and import the volume.
        static OSPObjectCatalog importVolume(const std::string &filename, Volume *volume);

        //! Import the volume specification and voxel data.
        virtual OSPObjectCatalog importVolume(Volume *volume) = 0;

        //! A string description of this class.
        virtual std::string toString() const { return("ospray::VolumeFile"); }

    protected:

        //! Print an error message.
        void emitMessage(const std::string &kind, const std::string &message) const
            { std::cerr << "  " + toString() + "  " + kind + ": " + message + "." << std::endl; }

        //! Error checking.
        void exitOnCondition(bool condition, const std::string &message) const
            { if (!condition) return;  emitMessage("ERROR", message);  exit(1); }

        //! Warning condition.
        void warnOnCondition(bool condition, const std::string &message) const
            { if (!condition) return;  emitMessage("WARNING", message); }

        //! Get the absolute file path.
        static std::string getFullFilePath(const std::string &filename)
            { char *fullpath = realpath(filename.c_str(), NULL);  return(fullpath != NULL ? fullpath : filename); }

    };

} // namespace ospray

