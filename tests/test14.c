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
#include <serenity/structures/structures.h>
#include <serenity/structures/infn/infn.h>
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

char *get_argument(int argc, char *argv[], unsigned int position, char *default_value) {
	if (argc >= (position+1))
		return argv[position];
	return default_value;
}

int main(int argc, char *argv[]) {
	struct o_pool *pool = f_pool_new(NULL);
	struct o_stream *out_stream;
	struct s_exception *exception;
	struct usb_bus *bus;
	struct usb_device *device;
	struct usb_dev_handle *handler;
	struct o_trb *trb_device;
	int counter, local_counter, errors = 0, last = -1, device_num = 0;
	unsigned char trigger, dac, channel;
	float delay;
	enum e_trb_mode mode = e_trb_mode_normal;
	char *out_file;
	struct o_trb_event *event = f_trb_event_new(NULL);
	d_try {
		d_pool_begin(pool) {
			out_stream = d_stdout;
			if (argc == 1) {
				d_printf(out_stream, d_SP("format: <app> <number of events> {file name} {1-SELF TRIGGER, 2-ext trigger} {hold delay (6.4)} "
							"{1-NORMAL MODE, 2-calibration, 3-cal debug (analogic), 4-cal debug (digital)} {dac} {channel}\n"));
			} else {
				counter = atoi(get_argument(argc, argv, 1, "1024"));
				switch (atoi(get_argument(argc, argv, 3, "1"))) {
					case 1:
						trigger = 0x22;
						break;
					default:
						trigger = 0x11;
				}
				delay = atof(get_argument(argc, argv, 4, "6.4"));
				if ((delay < 3.0f) || (delay > 10.0f)) {
					d_printf(out_stream, d_SP("Warning, delay value is out of scale [3, 10]. Restoring default value (6.4)\n"));
					delay = 6.4f;
				}
				switch (atoi(get_argument(argc, argv, 5, "1"))) {
					case 1:
						mode = e_trb_mode_normal;
						break;
					case 2:
						mode = e_trb_mode_calibration;
						break;
					case 3:
						mode = e_trb_mode_calibration_debug_analogic;
						break;
					default:
						mode = e_trb_mode_calibration_debug_digital;
				}
				dac = atoi(get_argument(argc, argv, 6, "0"));
				channel = atoi(get_argument(argc, argv, 7, "0"));
				usb_init();
				usb_find_busses();
				usb_find_devices();
				for (bus = usb_get_busses(); bus; bus = bus->next)
					for (device = bus->devices; device; device = device->next, device_num++) {
						if ((handler = usb_open(device))) {
							if (check_manifacturer(handler, device) == d_true)
								if ((trb_device = f_trb_new(NULL, device, handler))) {
									printf("[DEVICE %d] -<%d events>-\n", device_num, counter);
									errors = 0;
									if ((out_file = get_argument(argc, argv, 2, NULL)))
										trb_device->m_stream(trb_device, NULL, d_SP(out_file), "w", 0777);
									trb_device->m_setup(trb_device, trigger, delay, mode, dac, channel, 1000);
									local_counter = counter;
									while (local_counter) {
										trb_device->m_event(trb_device, event, 500);
										if (event->filled) {
											local_counter--;
											if ((last >= 0) && ((event->code > 0) && (event->code-1 != last))) {
												printf("[WARNING] {counter_trigger %d, last is %d}\n", event->code,
														last);
												errors++;
											} else
												printf("[EVENT %d]\n", event->code);
											last = event->code;
										}
									}
									d_release(trb_device);
									printf("[DEVICE %d] - errors %d\n", device_num, errors);
								}
							usb_close(handler);
						}
					}
			}
			d_release(out_stream);
		} d_pool_end_clean;
		d_release(pool);
	} d_catch(exception) {
		d_exception_dump(stdout, exception);
	} d_endtry;
	d_release(event);
	f_memory_flush(e_log_level_ever);
	return 0;
}

