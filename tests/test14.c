/*
   serenity
   Copyright (C) 2013 Andrea Nardinocchi (andrea@nardinan.it)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "../structures/structures.h"
#include "../structures/infn/infn.h"
int check_manifacturer(struct usb_dev_handle *udev, struct usb_device *dev) {
	int result = d_false;
	char buffer[256];
	if (dev->descriptor.iManufacturer) {
		usb_get_string_simple(udev, dev->descriptor.iManufacturer, buffer, 256);
		if (strncmp(buffer, "DAMPE", 5) == 0)
			result = d_true;
	}
	return result;
}

int main(int argc, char *argv[]) {
	struct o_pool *pool = f_pool_new(NULL);
	struct o_stream *out_stream;
	struct s_exception *exception;
	struct usb_bus *bus;
	struct usb_device *device;
	struct usb_dev_handle *handler;
	d_try {
		d_pool_begin(pool) {
			out_stream = d_stdout;
			usb_init();
			usb_find_busses();
			usb_find_devices();
			for (bus = usb_get_busses(); bus; bus = bus->next) {
				for (device = bus->devices; device; device = device->next) {
					handler = usb_open(device);
					if (check_manifacturer(handler, device) == d_true)
						printf("founded!\n");
					else
						printf("device skipped ...\n");
						
					usb_close(device);
				}
			}
			d_release(out_stream);
		} d_pool_end_clean;
		d_release(pool);
	} d_catch(exception) {
		d_exception_dump(stdout, exception);
	} d_endtry;
	f_memory_flush(e_log_level_ever);
	return 0;
}
