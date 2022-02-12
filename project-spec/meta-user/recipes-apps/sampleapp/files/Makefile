APP = sampleapp

# Add any other object files to this list below
APP_OBJS = sampleapp.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) -o $@ $(APP_OBJS) $(LDFLAGS) $(LDLIBS) -lsample
clean:
	rm -f $(APP) *.o

