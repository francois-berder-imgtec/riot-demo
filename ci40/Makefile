include $(TOPDIR)/rules.mk

PKG_NAME:=demo
PKG_VERSION:=0.1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/demo
  SECTION:=libs
  CATEGORY:=Utilities
  DEPENDS:= +letmecreate
  TITLE:=Demo
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) Demo/* $(PKG_BUILD_DIR)/
	rm -Rf $(PKG_BUILD_DIR)/CMakeFiles/
	rm -f $(PKG_BUILD_DIR)/CMakeCache.txt
	rm -f $(PKG_BUILD_DIR)/Makefile
	rm -f $(PKG_BUILD_DIR)/cmake_install.cmake
endef

define Package/letmecreate/install
	mkdir -p $(1)/usr/bin/letmecreate_demo
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/demo $(1)/usr/bin/letmecreate_demo
endef

$(eval $(call BuildPackage,demo))
