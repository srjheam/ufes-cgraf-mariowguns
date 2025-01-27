MAKE = make

TARGET = trab1

OUTRELEASEPATH = src/build/release/$(TARGET)
OUTDEBUGPATH = src/build/debug/$(TARGET)

all: release

release:
	$(MAKE) -C src/
	cp -f $(OUTRELEASEPATH) ./$(TARGET)

debug:
	$(MAKE) -C src/ debug
	cp -f $(OUTDEBUGPATH) ./$(TARGET)

clean:
	$(MAKE) -C src/ clean
	rm -f $(TARGET)

valgrind:
	$(MAKE) -C src/ valgrind VALARGS="../$(VALARGS)"
	cp -f src/valgrind-out.txt ./valgrind-out.txt
