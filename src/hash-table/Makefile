CFLAGS = -g -Wall 
SRC = ./*.cpp
TARGET = perfect-hash-table-main

.DEFAULT_GOAL := usage

usage: 
	@echo "Usage: make <target>"

main: $(SRC)
	g++ $(CFLAGS) $(SRC) -o $(TARGET)

.PHONY: clean all

all: main

clean:
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET).exe) del $(subst /,\\,$(TARGET).exe)
endif
