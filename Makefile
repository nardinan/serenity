prefix_inc = /usr/local
prefix_lib = /usr/local
include_dir = $(prefix_inc)/include
library_dir = $(prefix_lib)/lib
library_name = serenity
folders = ground structures structures/crypto structures/infn

all:
	for current_dir in $(folders); do make -C $${current_dir}; done

install:
	rm -rf $(include_dir)/$(library_name)
	mkdir $(include_dir)/$(library_name)
	for current_dir in $(folders); do \
		mkdir $(include_dir)/$(library_name)/$${current_dir} && cp -f $${current_dir}/*.h $(include_dir)/$(library_name)/$${current_dir}; \
	done
	cp *.so $(library_dir)

test:
	cd tests; make number=$(number) include_dir=$(include_dir) library_dir=$(library_dir)

clean:
	rm -f *.so
	for current_dir in $(folders); do make clean -C $${current_dir}; done
	make clean -C tests
