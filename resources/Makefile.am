desktopdir = @datadir@/applications
icon_scalabledir = @datadir@/icons/hicolor/scalable/apps

if HAVE_FREEDESKTOP
dist_desktop_DATA = garden.desktop
dist_icon_scalable_DATA = garden.svg

install-data-hook:
	update-desktop-database
uninstall-hook:
	update-desktop-database
endif #HAVE_FREEDESKTOP

