#pragma once
#include "vfs_device.h"
#include "vfs_virtual_directory.h"

#include <memory>

namespace vfs
{

class VirtualDevice : public Device, public std::enable_shared_from_this<VirtualDevice>
{
public:
   VirtualDevice(std::string rootDeviceName = {});
   ~VirtualDevice() override = default;

   Result<std::shared_ptr<Device>>
   getLinkDevice(const User &user, const Path &path) override;

   Error
   makeFolder(const User &user, const Path &path) override;

   Error
   makeFolders(const User &user, const Path &path) override;

   Error
   mountDevice(const User &user, const Path &path,
               std::shared_ptr<Device> device) override;

   Error
   mountOverlayDevice(const User &user, OverlayPriority priority,
                      const Path &path,
                      std::shared_ptr<Device> device) override;

   Error
   unmountDevice(const User &user, const Path &path) override;

   Error
   unmountOverlayDevice(const User &user, OverlayPriority priority,
                        const Path &path) override;

   Result<DirectoryIterator>
   openDirectory(const User &user, const Path &path) override;

   Result<std::unique_ptr<FileHandle>>
   openFile(const User &user, const Path &path,
            FileHandle::Mode mode) override;

   Error
   remove(const User &user, const Path &path) override;

   Error
   rename(const User &user, const Path &src, const Path &dst) override;

   Error
   setGroup(const User &user, const Path &path, GroupId group) override;

   Error
   setOwner(const User &user, const Path &path, OwnerId owner) override;

   Error
   setPermissions(const User &user, const Path &path, Permissions mode) override;

   Result<Status>
   status(const User &user, const Path &path) override;

private:
   std::pair<std::shared_ptr<VirtualNode>, Path>
   findDeepest(const User &user, Path path, int parentLevel = 0);

private:
   std::string mRootDeviceName;
   std::shared_ptr<VirtualDirectory> mRoot;
};

} // namespace vfs
