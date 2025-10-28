HYPHDR = /var/cache/hyprpm/test/headersRoot/include

all:
	$(CXX) -shared -fPIC --no-gnu-unique main.cpp -o csd-titlebar-move.so -g \
	`pkg-config --cflags pixman-1 libdrm hyprland pangocairo libinput libudev wayland-server xkbcommon` \
	-I$(HYPHDR) -I/usr/include/drm -std=c++2b -O2

clean:
	rm -f csd-titlebar-move.so
