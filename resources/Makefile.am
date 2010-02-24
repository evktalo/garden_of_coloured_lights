desktopdir = @datadir@/applications
icon_scalabledir = @datadir@/icons/hicolor/scalable/apps

if HAVE_FREEDESKTOP
dist_desktop_DATA = garden.desktop
dist_icon_scalable_DATA = garden.svg

# the '-' at the beginning indicates that there is no problem if the command fails
install-data-hook:
	-update-desktop-database
uninstall-hook:
	-update-desktop-database
endif #HAVE_FREEDESKTOP

EXTRA_DIST = garden.ico
