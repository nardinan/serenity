INC_DIR = /usr/local/include
LIB_DIR = /usr/local/lib
DIR_NAME = serenity

all:
	cd ground; make
	cd structures; make
	cd structures/crypto; make
	cd structures/infn; make

install:
	sudo mv *.so $(LIB_DIR)
	sudo rm -rf $(INC_DIR)/$(DIR_NAME)
	sudo mkdir $(INC_DIR)/$(DIR_NAME)
	sudo mkdir $(INC_DIR)/$(DIR_NAME)/ground
	sudo mkdir $(INC_DIR)/$(DIR_NAME)/structures
	sudo mkdir $(INC_DIR)/$(DIR_NAME)/structures/crypto
	sudo mkdir $(INC_DIR)/$(DIR_NAME)/structures/infn
	sudo cp -f ground/*.h $(INC_DIR)/$(DIR_NAME)/ground
	sudo cp -f structures/*.h $(INC_DIR)/$(DIR_NAME)/structures
	sudo cp -f structures/crypto/*.h $(INC_DIR)/$(DIR_NAME)/structures/crypto
	sudo cp -f structures/infn/*.h $(INC_DIR)/$(DIR_NAME)/structures/infn

test:
	cd tests; make NUMBER=$(NUMBER) INC_DIR=$(INC_DIR) LIB_DIR=$(LIB_DIR)

clean:
	rm -f *.so

cleanall:
	cd ground; make clean
	cd structures; make clean
	cd structures/crypto; make clean
	cd structures/infn; make clean
	cd tests; make clean
	rm -f *.so

