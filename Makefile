MAKE = make

TARGET = trab1

OUTRELEASEPATH = src/build/release/$(TARGET)

all: release

release:
	$(MAKE) -C src/
	cp -f $(OUTRELEASEPATH) ./$(TARGET)

clean:
	$(MAKE) -C src/ clean
	rm -f $(TARGET)
